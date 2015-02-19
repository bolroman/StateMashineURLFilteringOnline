/*
Copyright (c) 2007. Victor M. Alvarez [plusvic@gmail.com].

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef HASH_H_
#define HASH_H_

#define ROTATE_INT32(x, shift) (((x) << (shift)) | ((x) >> (32 - (shift))))

#ifdef WIN32
#define inline __inline
#endif

typedef struct {
	unsigned int byte_to_int32[256];
	unsigned int tabHash1[256 * 16];
	unsigned int tabHash2[256 * 16];
	unsigned int tabHash3[256 * 16];
	unsigned int seed;
} RollingHashTool;

void init_hash(RollingHashTool *obj);

#define HASH_WORD_ORIG(hash_object_ptr, buff, len, result)									\
	do {																				\
		int i;																			\
		unsigned int res = (hash_object_ptr)->seed;										\
		unsigned char *buffer = (unsigned char *)(buff);								\
																						\
		for (i = (len) - 1; i > 0; i--)													\
		{																				\
			res ^= ROTATE_INT32((hash_object_ptr)->byte_to_int32[*buffer], i);			\
			buffer++;																	\
		}																				\
																						\
		res ^= (hash_object_ptr)->byte_to_int32[*buffer];								\
		result = res;																	\
	} while (0)

#define HASH_WORD_ORIG1(hash_object_ptr, buff, len, result)									\
	do {																				\
		int i;																			\
		unsigned int res = (hash_object_ptr)->seed;										\
		unsigned char *buffer = (unsigned char *)(buff);								\
		int j; \
																						\
		for (i = 0; i < len; i+=16)													\
		{																				\
			j = i - len + 1;			\
			res ^= ROTATE_INT32((hash_object_ptr)->byte_to_int32[buffer[i]], j++);			\
			res ^= ROTATE_INT32((hash_object_ptr)->byte_to_int32[buffer[i + 1]], j++);			\
			res ^= ROTATE_INT32((hash_object_ptr)->byte_to_int32[buffer[i + 2]], j++);			\
			res ^= ROTATE_INT32((hash_object_ptr)->byte_to_int32[buffer[i + 3]], j++);			\
			res ^= ROTATE_INT32((hash_object_ptr)->byte_to_int32[buffer[i + 4]], j++);			\
			res ^= ROTATE_INT32((hash_object_ptr)->byte_to_int32[buffer[i + 5]], j++);			\
			res ^= ROTATE_INT32((hash_object_ptr)->byte_to_int32[buffer[i + 6]], j++);			\
			res ^= ROTATE_INT32((hash_object_ptr)->byte_to_int32[buffer[i + 7]], j++);			\
			res ^= ROTATE_INT32((hash_object_ptr)->byte_to_int32[buffer[i + 8]], j++);			\
			res ^= ROTATE_INT32((hash_object_ptr)->byte_to_int32[buffer[i + 9]], j++);			\
			res ^= ROTATE_INT32((hash_object_ptr)->byte_to_int32[buffer[i + 10]], j++);			\
			res ^= ROTATE_INT32((hash_object_ptr)->byte_to_int32[buffer[i + 11]], j++);			\
			res ^= ROTATE_INT32((hash_object_ptr)->byte_to_int32[buffer[i + 12]], j++);			\
			res ^= ROTATE_INT32((hash_object_ptr)->byte_to_int32[buffer[i + 13]], j++);			\
			res ^= ROTATE_INT32((hash_object_ptr)->byte_to_int32[buffer[i + 14]], j++);			\
			res ^= ROTATE_INT32((hash_object_ptr)->byte_to_int32[buffer[i + 15]], j);			\
		}																				\
																						\
		result = res;																	\
	} while (0)


#define REHASH_WORD_ORIG(hash_object_ptr, hash, newc, oldc, len, result)											\
	result = ROTATE_INT32((hash), 1) ^ ROTATE_INT32((hash_object_ptr)->byte_to_int32[(unsigned char)oldc], len) ^ (hash_object_ptr)->byte_to_int32[(unsigned char)newc];

#define HASH_WORD_SIMPLE(hash_object_ptr, buff, len, result) \
	do { \
		int i; \
		unsigned char *buffer = (unsigned char *)(buff); \
		result = 0; \
		for (i = 0; i < len; i+=16) { \
			result += (hash_object_ptr)->byte_to_int32[(unsigned char)(buffer[i])]; \
			result += (hash_object_ptr)->byte_to_int32[(unsigned char)(buffer[i + 1])]; \
			result += (hash_object_ptr)->byte_to_int32[(unsigned char)(buffer[i + 2])]; \
			result += (hash_object_ptr)->byte_to_int32[(unsigned char)(buffer[i + 3])]; \
			result += (hash_object_ptr)->byte_to_int32[(unsigned char)(buffer[i + 4])]; \
			result += (hash_object_ptr)->byte_to_int32[(unsigned char)(buffer[i + 5])]; \
			result += (hash_object_ptr)->byte_to_int32[(unsigned char)(buffer[i + 6])]; \
			result += (hash_object_ptr)->byte_to_int32[(unsigned char)(buffer[i + 7])]; \
			result += (hash_object_ptr)->byte_to_int32[(unsigned char)(buffer[i + 8])]; \
			result += (hash_object_ptr)->byte_to_int32[(unsigned char)(buffer[i + 9])]; \
			result += (hash_object_ptr)->byte_to_int32[(unsigned char)(buffer[i + 10])]; \
			result += (hash_object_ptr)->byte_to_int32[(unsigned char)(buffer[i + 11])]; \
			result += (hash_object_ptr)->byte_to_int32[(unsigned char)(buffer[i + 12])]; \
			result += (hash_object_ptr)->byte_to_int32[(unsigned char)(buffer[i + 13])]; \
			result += (hash_object_ptr)->byte_to_int32[(unsigned char)(buffer[i + 14])]; \
			result += (hash_object_ptr)->byte_to_int32[(unsigned char)(buffer[i + 15])]; \
		} \
	} while (0)

#define REHASH_WORD_SIMPLE(hash_object_ptr, hash, newc, oldc, len, result) \
	result = hash - (hash_object_ptr)->byte_to_int32[(unsigned char)oldc] + (hash_object_ptr)->byte_to_int32[(unsigned char)newc];
#define HASH_WORD_NOROLL_TAB_16(tab_hash_tbl, buff, result) \
	{ \
		(result) =  (tab_hash_tbl)[(unsigned char)(buff)[0]] ^ \
					(tab_hash_tbl)[((unsigned char)(buff)[1]) + 256] ^ \
					(tab_hash_tbl)[((unsigned char)(buff)[2]) + 256 * 2] ^ \
					(tab_hash_tbl)[((unsigned char)(buff)[3]) + 256 * 3] ^ \
					(tab_hash_tbl)[((unsigned char)(buff)[4]) + 256 * 4] ^ \
					(tab_hash_tbl)[((unsigned char)(buff)[5]) + 256 * 5] ^ \
					(tab_hash_tbl)[((unsigned char)(buff)[6]) + 256 * 6] ^ \
					(tab_hash_tbl)[((unsigned char)(buff)[7]) + 256 * 7] ^ \
					(tab_hash_tbl)[((unsigned char)(buff)[8]) + 256 * 8] ^ \
					(tab_hash_tbl)[((unsigned char)(buff)[9]) + 256 * 9] ^ \
					(tab_hash_tbl)[((unsigned char)(buff)[10]) + 256 * 10] ^ \
					(tab_hash_tbl)[((unsigned char)(buff)[11]) + 256 * 11] ^ \
					(tab_hash_tbl)[((unsigned char)(buff)[12]) + 256 * 12] ^ \
					(tab_hash_tbl)[((unsigned char)(buff)[13]) + 256 * 13] ^ \
					(tab_hash_tbl)[((unsigned char)(buff)[14]) + 256 * 14] ^ \
					(tab_hash_tbl)[((unsigned char)(buff)[15]) + 256 * 15]; \
	}

#define MASK_4LSB 0x0Fl

#define LEFT_ROTATE(word, width, bits, mask) \
	((((word) << bits) | ((word) >> ((width) - (bits)))) & (mask))

/***************************************************************************************
 * THE FOLLOWING CODE WAS AUTOMATICALLY GENERATED USING A RANDOM HASH FUNCTION CREATOR *
 ***************************************************************************************/

#define HASH_WORD_16_4LSBS_1(buff, result) \
	(result) = LEFT_ROTATE(((unsigned long)((buff)[15]) & MASK_4LSB), 4, 3, MASK_4LSB) | \
			(LEFT_ROTATE(((unsigned long)((buff)[3]) & MASK_4LSB), 4, 1, MASK_4LSB) << 4) | \
			(((unsigned long)((buff)[10]) & MASK_4LSB) << 8) | \
			(LEFT_ROTATE(((unsigned long)((buff)[0]) & MASK_4LSB), 4, 3, MASK_4LSB) << 12) | \
			(((unsigned long)((buff)[5]) & MASK_4LSB) << 16) | \
			(((unsigned long)((buff)[8]) & MASK_4LSB) << 20) | \
			(((unsigned long)((buff)[12]) & MASK_4LSB) << 24) | \
			(((unsigned long)((buff)[7]) & MASK_4LSB) << 28) | \
			(LEFT_ROTATE(((unsigned long)((buff)[14]) & MASK_4LSB), 4, 1, MASK_4LSB) << 32) | \
			(LEFT_ROTATE(((unsigned long)((buff)[9]) & MASK_4LSB), 4, 1, MASK_4LSB) << 36) | \
			(LEFT_ROTATE(((unsigned long)((buff)[13]) & MASK_4LSB), 4, 1, MASK_4LSB) << 40) | \
			(LEFT_ROTATE(((unsigned long)((buff)[4]) & MASK_4LSB), 4, 2, MASK_4LSB) << 44) | \
			(LEFT_ROTATE(((unsigned long)((buff)[1]) & MASK_4LSB), 4, 2, MASK_4LSB) << 48) | \
			(LEFT_ROTATE(((unsigned long)((buff)[6]) & MASK_4LSB), 4, 3, MASK_4LSB) << 52) | \
			(LEFT_ROTATE(((unsigned long)((buff)[2]) & MASK_4LSB), 4, 3, MASK_4LSB) << 56) | \
			(LEFT_ROTATE(((unsigned long)((buff)[11]) & MASK_4LSB), 4, 1, MASK_4LSB) << 60);

#define HASH_WORD_16_4LSBS_2(buff, result) \
	(result) = ((unsigned long)((buff)[15]) & MASK_4LSB) | \
			(((unsigned long)((buff)[5]) & MASK_4LSB) << 4) | \
			(LEFT_ROTATE(((unsigned long)((buff)[14]) & MASK_4LSB), 4, 3, MASK_4LSB) << 8) | \
			(LEFT_ROTATE(((unsigned long)((buff)[3]) & MASK_4LSB), 4, 1, MASK_4LSB) << 12) | \
			(LEFT_ROTATE(((unsigned long)((buff)[8]) & MASK_4LSB), 4, 1, MASK_4LSB) << 16) | \
			(LEFT_ROTATE(((unsigned long)((buff)[4]) & MASK_4LSB), 4, 2, MASK_4LSB) << 20) | \
			(LEFT_ROTATE(((unsigned long)((buff)[12]) & MASK_4LSB), 4, 3, MASK_4LSB) << 24) | \
			(LEFT_ROTATE(((unsigned long)((buff)[10]) & MASK_4LSB), 4, 2, MASK_4LSB) << 28) | \
			(LEFT_ROTATE(((unsigned long)((buff)[6]) & MASK_4LSB), 4, 1, MASK_4LSB) << 32) | \
			(LEFT_ROTATE(((unsigned long)((buff)[1]) & MASK_4LSB), 4, 1, MASK_4LSB) << 36) | \
			(LEFT_ROTATE(((unsigned long)((buff)[11]) & MASK_4LSB), 4, 3, MASK_4LSB) << 40) | \
			(LEFT_ROTATE(((unsigned long)((buff)[2]) & MASK_4LSB), 4, 2, MASK_4LSB) << 44) | \
			(((unsigned long)((buff)[13]) & MASK_4LSB) << 48) | \
			(LEFT_ROTATE(((unsigned long)((buff)[7]) & MASK_4LSB), 4, 2, MASK_4LSB) << 52) | \
			(LEFT_ROTATE(((unsigned long)((buff)[9]) & MASK_4LSB), 4, 1, MASK_4LSB) << 56) | \
			(((unsigned long)((buff)[0]) & MASK_4LSB) << 60);

#define HASH_WORD_16_4LSBS_3(buff, result) \
	(result) = LEFT_ROTATE(((unsigned long)((buff)[13]) & MASK_4LSB), 4, 3, MASK_4LSB) | \
			(LEFT_ROTATE(((unsigned long)((buff)[12]) & MASK_4LSB), 4, 3, MASK_4LSB) << 4) | \
			(LEFT_ROTATE(((unsigned long)((buff)[8]) & MASK_4LSB), 4, 1, MASK_4LSB) << 8) | \
			(LEFT_ROTATE(((unsigned long)((buff)[15]) & MASK_4LSB), 4, 3, MASK_4LSB) << 12) | \
			(LEFT_ROTATE(((unsigned long)((buff)[0]) & MASK_4LSB), 4, 3, MASK_4LSB) << 16) | \
			(LEFT_ROTATE(((unsigned long)((buff)[9]) & MASK_4LSB), 4, 3, MASK_4LSB) << 20) | \
			(((unsigned long)((buff)[4]) & MASK_4LSB) << 24) | \
			(LEFT_ROTATE(((unsigned long)((buff)[7]) & MASK_4LSB), 4, 2, MASK_4LSB) << 28) | \
			(LEFT_ROTATE(((unsigned long)((buff)[14]) & MASK_4LSB), 4, 3, MASK_4LSB) << 32) | \
			(((unsigned long)((buff)[6]) & MASK_4LSB) << 36) | \
			(((unsigned long)((buff)[11]) & MASK_4LSB) << 40) | \
			(((unsigned long)((buff)[10]) & MASK_4LSB) << 44) | \
			(LEFT_ROTATE(((unsigned long)((buff)[3]) & MASK_4LSB), 4, 2, MASK_4LSB) << 48) | \
			(((unsigned long)((buff)[5]) & MASK_4LSB) << 52) | \
			(LEFT_ROTATE(((unsigned long)((buff)[2]) & MASK_4LSB), 4, 3, MASK_4LSB) << 56) | \
			(((unsigned long)((buff)[1]) & MASK_4LSB) << 60);

/***************************************************************************************
 *   THE CODE ABOVE WAS AUTOMATICALLY GENERATED USING A RANDOM HASH FUNCTION CREATOR   *
 ***************************************************************************************/

#define HASH_WORD_NOROLL_16(hash_object_ptr, buff, len, result) \
		HASH_WORD_16_4LSBS_1(buff, result)

#define HASH_WORD_NOROLL_16_2(hash_object_ptr, buff, len, result) \
		HASH_WORD_16_4LSBS_2(buff, result)

#define HASH_WORD_NOROLL_16_3(hash_object_ptr, buff, len, result) \
		HASH_WORD_16_4LSBS_3(buff, result)

#define HASH_WORD HASH_WORD_ORIG
#define REHASH_WORD REHASH_WORD_ORIG

#endif /* HASH_H_ */
