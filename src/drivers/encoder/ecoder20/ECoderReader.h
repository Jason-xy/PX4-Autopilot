
#pragma once

#include <float.h>
#include <stdint.h>
#include <px4_platform_common/px4_work_queue/ScheduledWorkItem.hpp>

#define ECODER_BUFFER_SIZE 64
#define ECODER_WRITE_SIZE 16
#define CRC_TAB_SIZE 256
#define ECODER_RES_FRAME_LEN 11

class ECoderReader : public px4::ScheduledWorkItem {
	int		_rcs_fd{-1};
	char		_device[20] {};
	uint8_t _serial_buf[ECODER_BUFFER_SIZE] {};
	uint8_t _serial_write_buf[ECODER_WRITE_SIZE] {};
	int _reader_id;
	static uint8_t CRC8X1[CRC_TAB_SIZE];
	uint8_t dataBuf[16] {0};
	void Run() override;
	perf_counter_t	_cycle_perf{0};
	bool _initialized{false};
public:
	uint32_t _bytes_rx {0};
	uint8_t ecoder_ok = 0;
	float real_time_angle = 0;
	ECoderReader(const char * module_name, int reader_id, const char *device);
	virtual ~ECoderReader();
	int read_once(uint32_t & new_bytes);
	void ask();
	int init();
	void print_status();
};
