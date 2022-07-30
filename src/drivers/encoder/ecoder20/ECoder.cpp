/****************************************************************************
 *
 *   Copyright (c) 2012-2021 PX4 Development Team. All rights reserved.
 *
 * Redistributn and use in source and binary forms, with or without
 * modificatn, are permitted provided that the following conditns
 * are met:
 *
 * 1. Redistributns of source code must retain the above copyright
 *    notice, this list of conditns and the following disclaimer.
 * 2. Redistributns in binary form must reproduce the above copyright
 *    notice, this list of conditns and the following disclaimer in
 *    the documentatn and/or other materials provided with the
 *    distributn.
 * 3. Neither the name PX4 nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

#include "ECoder.hpp"
#include <termios.h>
#include <fcntl.h>

static uint8_t commands[] {
	0x02, //Read single turn position
	0x8A, ///Read multi turn position
	0x92, //Read precision
	0x1A //Read single multi precision
};
// static char charCmd_Read_Single_Turn[]  { 0x02 };    //Read single turn positn
// // static char charCmd_Read_Multi_Turn[]  { 0x8A };     //Read multi turn positn
// // static char charCmd_Read_Res[]  { 0x92 };            //Read precision
// // static char charCmd_Read_All[]  { 0x1A };            //Read single multi precision
// // static char charCmd_Set_Single_To_Zero[]  { 0xC2 };  //single turn clear
// // static char charCmd_Set_Multi_To_Zero[]  { 0x62 };   //Multi turn clear


#define ECODER_BITRATE 2500000
using namespace time_literals;

uint8_t CRC_C(uint8_t *CRCbuf, uint8_t* CRC_8X1,uint8_t Length);
void CRC_8X1_TAB_Creat(uint8_t *CRC_8X1);


ECoder::ECoder(const char *device) :
	ModuleParams(nullptr),
	ScheduledWorkItem(MODULE_NAME, px4::serial_port_to_wq(device)),
	_cycle_perf(perf_alloc(PC_ELAPSED, MODULE_NAME": cycle time")),
	_publish_interval_perf(perf_alloc(PC_INTERVAL, MODULE_NAME": publish interval"))
{
	if (device) {
		strncpy(_device, device, sizeof(_device) - 1);
		_device[sizeof(_device) - 1] = '\0';
	}
}

ECoder::~ECoder()
{
	perf_free(_cycle_perf);
	perf_free(_publish_interval_perf);
}

int
ECoder::init()
{
	_rcs_fd = open(_device, O_RDWR | O_NONBLOCK);
	struct termios t;
	tcgetattr(_rcs_fd, &t);
	t.c_cflag &= ~(CSIZE | PARENB | CSTOPB | CRTSCTS);
	t.c_cflag |= (CS8);
	cfsetspeed(&t, ECODER_BITRATE);
	tcsetattr(_rcs_fd, TCSANOW, &t);

	int termios_state = tcsetattr(_rcs_fd, TCSANOW, &t);
	if (termios_state < 0) {
		PX4_ERR("Opening device %s for ecoder with 2.5Mbps failed", _device);
		return -1;
	} else {
		PX4_INFO("Succ opened device %s for ecoder with 2.5Mbps", _device);
		return 0;
	}
}

int
ECoder::task_spawn(int argc, char *argv[])
{
	bool error_flag = false;

	int myoptind = 1;
	int ch;
	const char *myoptarg = nullptr;
	const char *device = nullptr;

	while ((ch = px4_getopt(argc, argv, "d:", &myoptind, &myoptarg)) != EOF) {
		switch (ch) {
		case 'd':
			device = myoptarg;
			break;

		case '?':
			error_flag = true;
			break;

		default:
			PX4_WARN("unrecognized flag");
			error_flag = true;
			break;
		}
	}

	if (error_flag) {
		return -1;
	}

	if (device == nullptr) {
		PX4_ERR("valid device required");
		return PX4_ERROR;
	}

	ECoder *instance = new ECoder(device);

	if (instance == nullptr) {
		PX4_ERR("alloc failed");
		return PX4_ERROR;
	}

	_object.store(instance);
	_task_id = task_id_is_work_queue;

	instance->ScheduleOnInterval(_current_update_interval);

	return PX4_OK;
}

void ECoder::Run()
{
	if (should_exit()) {
		exit_and_cleanup();
		return;
	}

	if (!_initialized) {
		if (init() == PX4_OK) {
			_initialized = true;
		}
	} else {

		perf_begin(_cycle_perf);

		// Check if parameters have changed
		if (_parameter_update_sub.updated()) {
			// clear update
			parameter_update_s param_update;
			_parameter_update_sub.copy(&param_update);

			updateParams();
		}
		::write(_rcs_fd, &commands[3], 1);
		// const hrt_abstime cycle_timestamp = hrt_absolute_time();
		int newBytes = 0;
		usleep(50);
		newBytes = ::read(_rcs_fd, &_serial_buf[0], ECODER_BUFFER_SIZE);
		if (newBytes > 0) {
			int start_byte=0;
			if (newBytes == 10) {
				//Currently it jumps the second zero byte.
				start_byte = 0;
			} else {
				return;
			}
			int32_t single_Turn = _serial_buf[start_byte + 1] | _serial_buf[start_byte + 2] << 8
				| _serial_buf[start_byte + 3] << 16;
			float resolution = (float) (1<<_serial_buf[start_byte + 4]);
			float abs_angle = (float)single_Turn/resolution*M_TWOPI_F;
			// PX4_INFO_RAW("New bytes %d:", newBytes);
			// for (int i = 0; i<newBytes; i ++) {
			// 	PX4_INFO_RAW("%d:%x ", i, _serial_buf[i]);
			// }
			// PX4_INFO_RAW("\n");
			PX4_INFO("single_Turn :%d resolution %.1f Abs Angle: %.1f", single_Turn, (double)resolution,
				(double) abs_angle*M_RAD_TO_DEG);
		}
		if (newBytes > 0) {
			_bytes_rx += newBytes;
		}
		count_send += 1;
	}
}

int ECoder::custom_command(int argc, char *argv[])
{
	return 0;
}

int ECoder::print_status()
{
	PX4_INFO("Max update rate: %u Hz", 1000000 / _current_update_interval);

	if (_device[0] != '\0') {
		PX4_INFO("UART device: %s", _device);
		PX4_INFO("UART RX bytes: %"  PRIu32, _bytes_rx);
	}

	perf_print_counter(_cycle_perf);
	perf_print_counter(_publish_interval_perf);
	return 0;
}

int
ECoder::print_usage(const char *reason)
{
	if (reason) {
		PX4_WARN("%s\n", reason);
	}

	PRINT_MODULE_DESCRIPTION(
		R"DESCR_STR(
### Descriptn
Encoder
)DESCR_STR");

	PRINT_MODULE_USAGE_NAME("ecoder", "driver");
	PRINT_MODULE_USAGE_COMMAND("start");
	PRINT_MODULE_USAGE_PARAM_STRING('d', "/dev/ttyS3", "<file:dev>", "RC device", true);
	PRINT_MODULE_USAGE_DEFAULT_COMMANDS();

	return 0;
}

extern "C" __EXPORT int ecoder_main(int argc, char *argv[])
{
	return ECoder::main(argc, argv);
}

uint8_t CRC_C(uint8_t *CRCbuf, uint8_t* CRC_8X1,uint8_t Length)
{
	uint8_t CRCResult=0;
	uint8_t CRCLength=0;
	while(CRCLength<Length)
	{
		CRCResult ^= CRCbuf[CRCLength];
		CRCResult = (CRCResult&0x00ff);
		CRCLength++;
		CRCResult = CRC_8X1[CRCResult];
	}
	return CRCResult;
}

void CRC_8X1_TAB_Creat(uint8_t *CRC_8X1)
{
	uint16_t i,j;
	uint8_t CRCResult;
	for(j = 0;j < 256;j++)
	{
		CRCResult = j;
		for(i = 0;i < 8;i++)
		{
			if(CRCResult & 0x80)
		{
			CRCResult = (CRCResult << 1) ^ 0x01; //0x01--x^8+1
		}
		else
		{
			CRCResult <<= 1;
		}
		}
		CRC_8X1[j] = (CRCResult&0x00ff);
	}
}
