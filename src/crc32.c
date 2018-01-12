/*
 * Library: libcrc
 * File:    src/crc32.c
 * Author:  Lammert Bies
 *
 * This file is licensed under the MIT License as stated below
 *
 * Copyright (c) 1999-2016 Lammert Bies
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Description
 * -----------
 * The source file src/crc32.c contains the routines which are needed to
 * calculate a 32 bit CRC value of a sequence of bytes.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "checksum.h"

/*
 * Include the lookup table for the CRC 32 calculation
 */

#include "../tab/gentab32.inc"
#include "../tab/gentab32_mpeg2.inc"

/*
 * uint32_t crc_32( const unsigned char *input_str, size_t num_bytes );
 *
 * The function crc_32() calculates in one pass the common 32 bit CRC value for
 * a byte string that is passed to the function together with a parameter
 * indicating the length.
 */

uint32_t crc_32( const unsigned char *input_str, size_t num_bytes ) {

	uint32_t crc;
	const unsigned char *ptr;
	size_t a;

	crc = CRC_START_32;
	ptr = input_str;

	if ( ptr != NULL ) for (a=0; a<num_bytes; a++) {

		crc = (crc >> 8) ^ crc_tab32[ (crc ^ (uint32_t) *ptr++) & 0x000000FFul ];
	}

	return (crc ^ 0xFFFFFFFFul);

}  /* crc_32 */

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
unsigned char* reverse(const unsigned char *input_str, size_t num_bytes ) {
	unsigned char *ret = (unsigned char *)calloc(num_bytes, sizeof(unsigned char));
	for (size_t i = 0; i < num_bytes; i++) {
		// ret[i] = reverse_one(input_str[i]);
		ret[i] = reverse_one(input_str[num_bytes-1-i]);
	}
	return ret;
}
*/


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

/*
 * uint32_t update_crc_32( uint32_t crc, unsigned char c );
 *
 * The function update_crc_32() calculates a new CRC-32 value based on the
 * previous value of the CRC and the next byte of the data to be checked.
 */

uint32_t update_crc_32( uint32_t crc, unsigned char c ) {

	return (crc >> 8) ^ crc_tab32[ (crc ^ (uint32_t) c) & 0x000000FFul ];

}  /* update_crc_32 */
