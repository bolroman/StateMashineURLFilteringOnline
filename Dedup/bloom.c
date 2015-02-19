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

#include<stdarg.h>
#include<stdio.h>

#include"bloom.h"

#define SETBIT(a, n) (a[n/CHAR_BIT] |= (1<<(n%CHAR_BIT)))

unsigned int sax_hash(char *key, int len)
{
	unsigned int h=0;
	int i;

	for (i = 0; i < len; i++) {
		h ^= (h << 5) + (h >> 2) + key[i];
	}

	return h;
}

unsigned int sdbm_hash(char *key, int len)
{
	unsigned int h=0;
	int i;

	for (i = 0; i < len; i++) {
		h = key[i] + (h << 6) + (h << 16) - h;
	}

	return h;
}

BLOOM *bloom_create(size_t size, size_t nfuncs, ...)
{
	BLOOM *bloom;
	va_list l;
	int n;
	
	if(!(bloom=(BLOOM*)malloc(sizeof(BLOOM)))) return NULL;
	if(!(bloom->a=(unsigned char *)calloc((size+CHAR_BIT-1)/CHAR_BIT, sizeof(char)))) {
		free(bloom);
		return NULL;
	}
	if(!(bloom->funcs=(hashfunc_t*)malloc(nfuncs*sizeof(hashfunc_t)))) {
		free(bloom->a);
		free(bloom);
		return NULL;
	}

	va_start(l, nfuncs);
	for(n=0; n<nfuncs; ++n) {
		bloom->funcs[n]=va_arg(l, hashfunc_t);
	}
	va_end(l);

	bloom->nfuncs=nfuncs;
	bloom->asize=size;

	return bloom;
}

BLOOM *fast_bloom_create(size_t size) {
	BLOOM *bloom;

	if(!(bloom=(BLOOM*)malloc(sizeof(BLOOM)))) return NULL;
	if(!(bloom->a=(unsigned char *)calloc((size+CHAR_BIT-1)/CHAR_BIT, sizeof(char)))) {
		free(bloom);
		return NULL;
	}

	bloom->asize=size;

	return bloom;
}

int bloom_destroy(BLOOM *bloom)
{
	free(bloom->a);
	free(bloom->funcs);
	free(bloom);

	return 0;
}

int fast_bloom_destroy(BLOOM *bloom)
{
	free(bloom->a);
	free(bloom);

	return 0;
}

int bloom_add(BLOOM *bloom, char *s, int len)
{
	size_t n;

	for(n=0; n<bloom->nfuncs; ++n) {
		SETBIT(bloom->a, bloom->funcs[n](s, len)%bloom->asize);
	}

	return 0;
}

int fast_bloom_add(BLOOM *bloom, RollingHashTool *hasher, char *s, int len)
{
	unsigned long hash;

	HASH_WORD_NOROLL_16(hasher, s, len, hash);
	//hash = sax_hash(s, len);
	SETBIT(bloom->a, hash%bloom->asize);
	HASH_WORD_NOROLL_16_2(hasher, s, len, hash);
	//hash = sdbm_hash(s, len);
	SETBIT(bloom->a, hash%bloom->asize);
	/**/
#ifdef BLOOM_THIRD_FUNC
	HASH_WORD_NOROLL_16_3(hasher, s, len, hash);
	//hash = sdbm_hash(s, len);
	SETBIT(bloom->a, hash%bloom->asize);
	/**/
#endif

	return 0;
}

int fast_bloom_add_dh(BLOOM *bloom, RollingHashTool *hasher, char *s, int len)
{
	unsigned long x, y;

	HASH_WORD_NOROLL_16(hasher, s, len, x);
	HASH_WORD_NOROLL_16_2(hasher, s, len, y);

	SETBIT(bloom->a, x % bloom->asize);
	SETBIT(bloom->a, (x + y) % bloom->asize);

#ifdef BLOOM_THIRD_FUNC
	SETBIT(bloom->a, (x + y + y) % bloom->asize);
#endif

	return 0;
}

double fast_bloom_get_ratio(BLOOM *bloom) {
	int ones;
	int i;

	ones = 0;

	for (i = 0; i < bloom->asize; i++) {
		if (GETBIT(bloom->a, i)) {
			ones++;
		}
	}

	return ((double)ones) / bloom->asize;
}

void fast_bloom_print_hist(BLOOM *bloom, int bins) {
	int per_bin, bin;
	int i;
	int hist;

	per_bin = (int)(bloom->asize / (double)bins);
	printf("[ ");
	for (bin = 0; bin < bins; bin++) {
		hist = 0;
		for (i = 0; i < per_bin; i++) {
			if (GETBIT(bloom->a, per_bin * bin + i)) {
				hist++;
			}
		}
		printf("%d", hist);
		if (bin != bins - 1)
			printf(", ");
	}
	printf(" ]\n");
}

int bloom_check(BLOOM *bloom, char *s, int len)
{
	size_t n;

	for(n=0; n<bloom->nfuncs; ++n) {
		if(!(GETBIT(bloom->a, bloom->funcs[n](s, len)%bloom->asize))) return 0;
	}

	return 1;
}
