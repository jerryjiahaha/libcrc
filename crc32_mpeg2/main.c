#include <stdint.h>
#include <stdio.h>

#include "crc32_mpeg2.h"

int main() {
	uint8_t input[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
	uint32_t crc = crc_32_mpeg2(input, 8);
	printf("%#x\n", crc);
	return 0;
}
