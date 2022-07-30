#include "ECoderReader.h"
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <px4_platform_common/log.h>
#define ECODER_BITRATE 2500000

static uint8_t commands[] {
	0x02, //Read single turn position
	0x8A, ///Read multi turn position
	0x92, //Read precision
	0x1A //Read single multi precision
};

uint8_t ECoderReader::CRC8X1[CRC_TAB_SIZE];
uint8_t CRC_C(uint8_t *CRCbuf, uint8_t* CRC_8X1,uint8_t Length);
void CRC_8X1_TAB_Creat(uint8_t *CRC_8X1);

ECoderReader::ECoderReader(int reader_id, const char *device):
	_reader_id(reader_id) {
	if (device) {
		strncpy(_device, device, sizeof(_device) - 1);
		_device[sizeof(_device) - 1] = '\0';
	}
	CRC_8X1_TAB_Creat(CRC8X1);
}

ECoderReader::~ECoderReader() {
	close(_rcs_fd);
}

int ECoderReader::init() {
	_rcs_fd = open(_device, O_RDWR | O_NONBLOCK);
	struct termios t;
	tcgetattr(_rcs_fd, &t);
	t.c_cflag &= ~(CSIZE | PARENB | CSTOPB | CRTSCTS);
	t.c_cflag |= (CS8);
	cfsetspeed(&t, ECODER_BITRATE);
	tcsetattr(_rcs_fd, TCSANOW, &t);

	int termios_state = tcsetattr(_rcs_fd, TCSANOW, &t);
	if (termios_state < 0) {
		PX4_ERR("ECoder %d open device %s for ecoder with 2.5Mbps failed", _reader_id, _device);
		return PX4_ERROR;
	} else {
		PX4_INFO("ECoder %d succ opened device %s for ecoder with 2.5Mbps", _reader_id, _device);
		return PX4_OK;
	}
}

void ECoderReader::ask() {
	::write(_rcs_fd, &commands[3], 1);
}

int ECoderReader::read_once(uint32_t & total_bytes) {
	// const hrt_abstime cycle_timestamp = hrt_absolute_time();
	int newBytes = 0;
	newBytes = ::read(_rcs_fd, &_serial_buf[0], ECODER_BUFFER_SIZE);
	if (newBytes > 0) {
		total_bytes += newBytes;
		if (newBytes == 10) {
			//Currently it jumps the second zero byte.
			dataBuf[0] = _serial_buf[0];
			dataBuf[1] = 0;
			memcpy(dataBuf+2, _serial_buf+1, 9);
		} else {
			return newBytes;
		}
		int32_t single_Turn = dataBuf[2] | dataBuf[3] << 8
			| dataBuf[4] << 16;
		float resolution = (float) (1<<dataBuf[5]);
		float abs_angle = (float)single_Turn/resolution*M_TWOPI_F;
		// PX4_INFO_RAW("New bytes %d:", newBytes);
		// for (int i = 0; i<newBytes; i ++) {
		// 	PX4_INFO_RAW("%d:%x ", i, _serial_buf[i]);
		// }
		// PX4_INFO_RAW("\n");
		uint8_t crc = CRC_C(dataBuf, CRC8X1, ECODER_RES_FRAME_LEN);
		PX4_INFO("ECoder %d, single_Turn :%d resolution %.1f Abs Angle: %.1f CRC %d", _reader_id,
			single_Turn, (double)resolution, (double) abs_angle*M_RAD_TO_DEG, crc);
		if (crc == 0) {
			return PX4_OK;
		}
	}
	return PX4_ERROR;
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
