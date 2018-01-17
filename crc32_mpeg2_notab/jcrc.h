#ifndef J_CRC_
#define J_CRC_

#include <stdint.h>
#include <stdlib.h>

#define CRC_POLY_32_MPEG2	0x04C11DB7ul
#define CRC_START_32		0xFFFFFFFFul
#define BYTE_FIRST_ONE (0x01 << 7)


/**
 * Calculate crc32-mpeg2 of buffer
 * input: 8-byte stream with len
 * output: crc32-mpeg2 value
 */
uint32_t crc_32_mpeg2(uint8_t *stream, size_t len);

/**
 * Reverse bit of uint32_t
 */
void reverse(uint32_t input);

/**
 * Reverse bit of uint8_t
 */
uint8_t reverse_byte(uint8_t input);

/**
 * Shift stream to the left
 * stream is s[0],s[1],...,s[len-1] format
 * Low memory address is one the left
 * (However in each byte LSB is one the right)
 * stream will be modified
 */
void shift_left(uint8_t *stream, size_t len);

/**
 * XOR stream with a uint32_t value
 * only the left will xor
 * stream will be modified, used internally
 */
void xor_32(uint32_t xor, uint8_t *stream, size_t len);

#endif
