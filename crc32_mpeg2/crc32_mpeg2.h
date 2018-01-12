#ifndef _CRC32_MPEG2_
#define _CRC32_MPEG2_
#include <stdint.h>
#include <stdlib.h>

unsigned char reverse_one(unsigned char input);
uint32_t reverse_32(uint32_t input);
uint32_t crc_32_mpeg2( const unsigned char *input_str, size_t num_bytes );

#if !defined(CRC_START_32)
#define CRC_START_32 0xFFFFFFFFul
#endif

#endif
