#include <stdio.h>
#include <stdint.h>

#include "checksum.h"

void testcrc32() {
	printf("==========\ntestcrc32\n");
//	uint8_t input[] = {0x31, 0xFF, 0xFF, 0xFF};
//	uint8_t input[] = {0x41, 0x42, 0x43, 0x44};
//	uint8_t input[] = {'a', 'a', 'a', 'a'};
//	uint8_t input[] = {0x00, 0x00, 0x00, 0x00};
	uint8_t input[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
//	uint8_t input[] = {0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01};
	size_t len = 8;
	uint32_t crc32 = crc_32_mpeg2(input, len);
	printf("crc32-mpeg2: %#x\n", crc32);
	for (int i = 0;i<len;i++) {
		printf("%#x ", ((uint8_t*)&crc32)[i]);
	}
	printf("\n====\n");

	printf("crc32:  %#x\n", crc_32(input, len));
	uint32_t init = 0xFFFFFFFF;
	uint32_t crc = init;
	for (int j = 0; j < len; j++) {
		crc = update_crc_32(crc, input[j]);
	}
	printf("%#x\n", crc);
	printf("~%#x\n", ~crc);
}

int main(int argc, char *argv[]) {
	uint8_t input[2] = {0x01, 0x02};
	uint16_t init = CRC_START_CCITT_FFFF;
	
	uint16_t crc = crc_modbus(input, 2);
	uint8_t *crcb = (uint8_t *)&crc;

	printf("%x %x\n", crcb[0], crcb[1]);
	printf("crc: %x\n", crc);

	crc = update_crc_16(CRC_START_CCITT_FFFF, input[0]);
	crc = update_crc_16(crc, input[1]);

	printf("crc: %x\n", crc);
	testcrc32();

	return 0;
}
