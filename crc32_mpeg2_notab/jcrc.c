#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#ifdef _WIN32
#include <winsock.h>
#else
#include <arpa/inet.h>
#endif

#include "jcrc.h"

void reverse(uint32_t input) {
	uint32_t init = 0;
	for (int i = 0; i < 32; i++) {
		init |= ((input & 0x00000001ul) << (32-1-i));
		input >>= 1;
	}
	printf("%lx\n", init);
}

uint8_t reverse_byte(uint8_t input) {
	uint8_t init = 0;
	for (int i = 0; i < 8; i++) {
		init |= ((input & 0x01) << (8-1-i));
		input >>= 1;
	}
	return init;
}

void shift_left(uint8_t *stream, size_t len) {
	// s[0] s[1] .... s[len-1]
	uint8_t first_bit = 0x0;
	uint8_t new_first_bit;
	for (int i = len - 1; i >= 0; i--) {
		new_first_bit = (stream[i] & BYTE_FIRST_ONE); // 0b_asdf_hjkl -> 0b_a000_0000
		stream[i] <<= 1; // 0b_qwer_tyui -> 0b_wery_yui0
		stream[i] ^= first_bit; // 0b_wery_yuia
		first_bit = new_first_bit >> 7; // 0b_a000_0000 -> 0b_0000_000a
	}
}

void xor_32(uint32_t xor, uint8_t *stream, size_t len) {
	// NOTE stream length must >= 4
	// s[0]_s[1]_s[2]_s[3] ^= xor
	// x[3]_x[2]_x[1]_x[0]
	uint8_t *px = (uint8_t *)&xor;
	for (int i = 0; i < 4; i++) {
		stream[i] ^= px[4-1-i];
	}
}

uint32_t crc_32_mpeg2(uint8_t *stream, size_t len) {
	uint8_t *stream_padded = calloc(len + 4, 1);
	memcpy(stream_padded, stream, len);
	size_t bit_len = 8 * len;

	// init, xor 0xFFFFFFFF
	// *((uint32_t *)stream_padded) ^= CRC_START_32;
	xor_32(CRC_START_32, stream_padded, len);

	for (int i = 0; i < bit_len; i++) {
		// loop
		if (stream_padded[0] & BYTE_FIRST_ONE) {
			shift_left(stream_padded, len + 4);
			xor_32(CRC_POLY_32_MPEG2, stream_padded, len);
		}
		else {
			shift_left(stream_padded, len + 4);
		}
	}
	for (int j = 0; j < 4; j++) {
		printf("%#4x ", stream_padded[j]);
	}
	printf("\n");
	uint32_t crc = *((uint32_t *)stream_padded);
	free(stream_padded);
	return ntohl(crc);
}

int crc32() {
	uint32_t crc = 0x12000000ul ^ 0xFFFFFFFFul;
	for (int i = 0; i < 32; i++) {
		if (crc & (0x1ul<<31)) {
//			printf("shift %d\n", i);
			crc = (crc << 1) ^ CRC_POLY_32_MPEG2;
		}
		else {
//			printf("next %d\n", i);
			crc = (crc << 1);
		}
		printf("%lx\n", crc);
	}
//	printf("%lx\n", crc);
	return 0;
}

#define W32(t) ( *( (uint32_t *)t ) )

#ifdef TEST_JCRC
int main() {
	crc32();
	printf("====\n");
	uint8_t input[] = {0x12, 0x34, 0x56, 0x78, \
			   0x1a, 0x2b, 0x3c, 0x4d};
//	uint8_t input[] = {0x78, 0x56, 0x34, 0x12};
	uint32_t crc = crc_32_mpeg2(input, 8);
	printf("CRC: %#010X\n", crc);

	printf("====CHECK=SHIFT_LEFT===\n");
	uint8_t t1[4] = {0x01, 0x02, 0x03, 0x04}; // 0x04030201
	shift_left(t1, 4);
	assert(W32(t1) == 0x08060402); // t1: 0x08060402

	printf("===CHECK=XOR32===\n");
	xor_32(CRC_POLY_32_MPEG2, t1, 4);
	assert(W32(t1) == (0x08060402^0xB71DC104));

	printf("===CHECK2===\n");
	uint8_t t2[] = {0xF1, 0xE2, 0xD3, 0xC4}; // 0xC4D3E2F1
	shift_left(t2, 4);
	printf("%#x\n", W32(t2));
	assert(W32(t2) == 0x88A7C5E3);
	xor_32(CRC_POLY_32_MPEG2, t2, 4);
	assert(W32(t2) == (0x88A7C5E3 ^ 0xB71DC104));

	printf("====CHECK_STEP_BY_STEP===\n");
	uint8_t t3[] = {0x12, 0x00, 0x00, 0x00, 0x00};
	xor_32(CRC_START_32, t3, 5); // {0xED, 0xFF, 0xFF, 0xFF, 0x00}
	printf("%#x\n", W32(t3));
	assert(W32(t3) == 0xFFFFFFED);

	// 1st
	assert(t3[0] & BYTE_FIRST_ONE);
	shift_left(t3, 5); // {0xDB, 0xFF, 0xFF, 0xFE, 0x00}
	printf("%#x\n", W32(t3));
	assert(W32(t3) == 0xFEFFFFDB);
	xor_32(CRC_POLY_32_MPEG2, t3, 5); // {0xDF, 0x3E, 0xE2, 0x49, 0x00}
	assert(W32(t3) == 0x49E23EDF);

	// 2nd
	assert(t3[0] & BYTE_FIRST_ONE);
	shift_left(t3, 5); // {0xBE, 0x7D, 0xC4, 0x92, 0x00}
	assert(W32(t3) == 0x92C47DBE);
	xor_32(CRC_POLY_32_MPEG2, t3, 5); // {0xBA, 0xBC, 0xD9, 0x25, 0x00}
	assert(W32(t3) == 0x25D9BCBA);

	// 3rd
	assert(t3[0] & BYTE_FIRST_ONE);
	shift_left(t3, 5); // shift
	assert(W32(t3) == 0x4ab27975);
	xor_32(CRC_POLY_32_MPEG2, t3, 5); // xor
	assert(W32(t3) == 0xfdafb871);

	// 4th
	assert(!(t3[0] & BYTE_FIRST_ONE));

	printf("TO BE CONTINUED\n");

	return 0;
}
#endif
