#include "gentab32.h"
#include "crc32_mpeg2.h"

unsigned char reverse_one(unsigned char input) {
	unsigned char output = 0;
	for (int i = 0; i < 8; i++) {
		output |= ( (input & 0x01) << (8-1-i) );
		input >>= 1;
	}
	return output;
}

uint32_t reverse_32(uint32_t input) {
	uint8_t *pin = (uint8_t *)&input;
	uint32_t output = 0;
	uint8_t *pout = (uint8_t *)&output;
	for (int i = 0; i < 4; i++) {
		pout[i] = reverse_one(pin[4-1-i]);
	}
	return output;
}


/*
 * uint32_t crc_32_mpeg2( const unsigned char *input_str, size_t num_bytes );
 *
 * The function crc_32_mpeg2() calculates in one pass the common 32 bit CRC (mpeg2) value for
 * a byte string that is passed to the function together with a parameter
 * indicating the length.
 */
uint32_t crc_32_mpeg2( const unsigned char *input_str, size_t num_bytes ) {
	uint32_t crc;
	const unsigned char *ptr;
	size_t a;

	crc = CRC_START_32;

//	unsigned char* new_input = reverse(input_str, num_bytes);

	ptr = input_str;

	if ( ptr != NULL ) for (a=0; a<num_bytes; a++) {

		crc = (crc >> 8) ^ crc_tab32[ (crc ^ (uint32_t) reverse_one(*ptr++)) & 0x000000FFul ];
	}

//	free(new_input);

	return reverse_32(crc); // if use crc_tab32 not crc_tab32_mpeg2
	// return crc;
} /* crc32_mpeg2 */


