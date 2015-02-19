/* Copyright (c) 2013 the authors listed at the following URL, and/or
the authors of referenced articles or incorporated external code:
http://en.literateprograms.org/Bloom_filter_(C)?action=history&offset=20111019004917

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Retrieved from: http://en.literateprograms.org/Bloom_filter_(C)?oldid=17386
*/

#ifndef __BLOOM_H__
#define __BLOOM_H__

#include<stdlib.h>
#include<limits.h>
#include "../Common/Flags.h"
#include "hash.h"

typedef unsigned int (*hashfunc_t)(char *, int);
typedef struct {
	size_t asize;
	unsigned char *a;
	size_t nfuncs;
	hashfunc_t *funcs;
} BLOOM;

//BLOOM *bloom_create(size_t size, size_t nfuncs, ...);
BLOOM *fast_bloom_create(size_t size);
//int bloom_destroy(BLOOM *bloom);
int fast_bloom_destroy(BLOOM *bloom);
//int bloom_add(BLOOM *bloom, char *s, int len);
int fast_bloom_add(BLOOM *bloom, RollingHashTool *hasher, char *s, int len);
int fast_bloom_add_dh(BLOOM *bloom, RollingHashTool *hasher, char *s, int len);
//int bloom_check(BLOOM *bloom, char *s, int len);
double fast_bloom_get_ratio(BLOOM *bloom);
void fast_bloom_print_hist(BLOOM *bloom, int bins);

unsigned int sax_hash(char *key, int len);
unsigned int sdbm_hash(char *key, int len);

#define GETBIT(a, n) (a[n/CHAR_BIT] & (1<<(n%CHAR_BIT)))

#ifdef BLOOM_THIRD_FUNC
#define FAST_BLOOM_CHECK(bloom, hasher, s, len, hash1, result) 							\
	{ 																	\
		/*unsigned long hash;*/											\
		/*HASH_WORD_NOROLL_16(NULL, s, len, hash);*/					\
		/*hash = sax_hash(s, len);	*/ \
		if (!GETBIT((bloom)->a, hash1 % (bloom)->asize)) { 				\
			result = 0; 												\
		} else { 														\
			unsigned long hash;											\
			HASH_WORD_NOROLL_16_2(hasher, s, len, hash);	 				\
			/*hash = sdbm_hash(s, len);*/	\
			if (!GETBIT((bloom)->a, hash % (bloom)->asize)) { 			\
				HASH_WORD_NOROLL_16_3(hasher, s, len, hash);	 				\
				if (!GETBIT((bloom)->a, hash % (bloom)->asize)) { 			\
					result = 0; 											\
				} else {													\
					result = 1;												\
				} 															\
			} else {													\
				result = 1;												\
			} 															\
		}																\
	}

#define FAST_BLOOM_CHECK_DH(bloom, hasher, s, len, hash1, result) 							\
	{ 																	\
		if (!GETBIT((bloom)->a, hash1 % (bloom)->asize)) { 				\
			result = 0; 												\
		} else { 														\
			unsigned long hash;											\
			HASH_WORD_NOROLL_16_2(hasher, s, len, hash);	 				\
			/*hash = sdbm_hash(s, len);*/	\
			if (!GETBIT((bloom)->a, (hash1 + hash) % (bloom)->asize)) { 			\
				if (!GETBIT((bloom)->a, (hash1 + hash + hash) % (bloom)->asize)) { 			\
					result = 0; 											\
				} else {													\
					result = 1;												\
				} 											\
			} else {													\
				result = 1;												\
			} 															\
		}																\
	}

#else

#define FAST_BLOOM_CHECK(bloom, hasher, s, len, hash1, result) 							\
	{ 																	\
		/*unsigned long hash;*/											\
		/*HASH_WORD_NOROLL_16(NULL, s, len, hash);*/					\
		/*hash = sax_hash(s, len);	*/ \
		if (!GETBIT((bloom)->a, hash1 % (bloom)->asize)) { 				\
			result = 0; 												\
		} else { 														\
			unsigned long hash;											\
			HASH_WORD_NOROLL_16_2(hasher, s, len, hash);	 				\
			/*hash = sdbm_hash(s, len);*/	\
			if (!GETBIT((bloom)->a, hash % (bloom)->asize)) { 			\
				result = 0; 											\
			} else {													\
				result = 1;												\
			} 															\
		}																\
	}

#define FAST_BLOOM_CHECK_DH(bloom, hasher, s, len, hash1, result) 							\
	{ 																	\
		if (!GETBIT((bloom)->a, hash1 % (bloom)->asize)) { 				\
			result = 0; 												\
		} else { 														\
			unsigned long hash;											\
			HASH_WORD_NOROLL_16_2(hasher, s, len, hash);	 				\
			/*hash = sdbm_hash(s, len);*/	\
			if (!GETBIT((bloom)->a, (hash1 + hash) % (bloom)->asize)) { 			\
				result = 0; 											\
			} else {													\
				result = 1;												\
			} 															\
		}																\
	}

#endif

#endif
