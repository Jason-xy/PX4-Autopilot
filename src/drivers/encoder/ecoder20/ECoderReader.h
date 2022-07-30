
#pragma once

#include <float.h>
#include <stdint.h>

#define ECODER_BUFFER_SIZE 64
#define ECODER_WRITE_SIZE 16
#define CRC_TAB_SIZE 256
#define ECODER_RES_FRAME_LEN 11

class ECoderReader {
	int		_rcs_fd{-1};
	char		_device[20] {};
	uint8_t _serial_buf[ECODER_BUFFER_SIZE] {};
	uint8_t _serial_write_buf[ECODER_WRITE_SIZE] {};
	int _reader_id;
	static uint8_t CRC8X1[CRC_TAB_SIZE];
	uint8_t dataBuf[16] {0};
public:
	ECoderReader(int reader_id, const char *device);
	virtual ~ECoderReader();
	int read_once(uint32_t & new_bytes);
	void ask();
	int init();
};
