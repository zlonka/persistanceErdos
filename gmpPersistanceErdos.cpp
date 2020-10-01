// gmpPersistanceErdos.cpp : multiplicative persistence (Erdos mode) computation of numbers ddd...ddd (d n times)
// usage: gmpPersistanceErdos.exe d n [nmax] [pmin]
// ex: gmpPersistanceErdos.exe 9 7008258 7008300 18
// for all numbers N=9999...99999 with between 7008258 and 7008300 digits,  displays persistenceErdos(N) if it's >= 18
// needs https://github.com/ShiftMediaProject/gmp/releases/download/6.2.1-1/libgmp_6.2.1-1_msvc16.zip

#pragma warning (disable : 4146)

#include <iostream>
#include <time.h>

#include "C:\Users\m\Documents\dev\cpp\libgmp_6.2.1-1_msvc16\include\gmp.h"

char* str;
int persistErdosGMP(mpz_t *keep) {
	int i, occ[10];
	mpz_t a;
	mpz_init(a);
	mpz_set(a, *keep);
	for (i = 1;; i++) {
		mpz_get_str(str, 10, a);	// gmp_sprintf(str, "%Zd", a);
		// fprintf(stderr, "i=%d str=%s\n", i, str);
		int len = strlen(str);
		if (len <= 1) {
			mpz_clear(a);
			return i;
		}
		for (int j = 0; j < 10; j++) occ[j] = 0;
		for (int j = len - 1; j >= 0; j--) occ[str[j] - '0']++;
		mpz_clear(a);	// !!!! prevent memory leak :)

		mpz_init_set_ui(a, 1);
		for (int j = 2; j < 10; j++) {
			if (!occ[j]) continue;
			mpz_t k;
			mpz_init(k);
			mpz_set_ui(k, 1);
			mpz_ui_pow_ui(k, j, occ[j]);
			mpz_mul(a, a, k);
			mpz_clear(k);
		}
	}
	mpz_clear(a);
	return i;
}
int main(int argc, char* argv[]) {
	char fSave[512];
	if (argc < 3) {
		fprintf(stderr, "usage:%s digit n\n", argv[0]);
		return 1;
	}
	int digit, n, nDeb, nmax = 0, pmin = 0;
	sscanf_s(argv[1], "%d", &digit);
	sscanf_s(argv[2], "%d", &n);
	if (argc > 3) sscanf_s(argv[3], "%d", &nmax);
	if (argc > 4) sscanf_s(argv[4], "%d", &pmin);
	printf("compute Erdos multiplication persistence for numbers x=%d%d%d...%d%d%d", digit, digit, digit, digit, digit, digit);
	if (nmax == 0) printf(" (n>=%d digits)", n); else printf(" (%d<=n<%d)", n, nmax);
	if (pmin != 0) printf(" with pE(x)>=%d", pmin);
	printf("\n");
	nDeb = n;
	sprintf_s(fSave, "pErdos_%d_%d.txt", digit, nDeb);

	printf("alloc %.1lf Mb...", ((double)n*0.000001) );
	str = (char*)malloc((n + 1000000) * sizeof(char));	// get space for n + 1M
	if (str==(char*)NULL){ fprintf(stderr, "Can't alloc memory...\n"); exit(1); }
	printf("done.\n");

	printf("compute first...");
	mpz_t keep;
	mpz_init(keep);
	// gmp_printf("k=%Zd\n", k);
	// keep=digit^n
	mpz_ui_pow_ui(keep, digit, n);
	// gmp_printf("keep=%Zd\n", keep);
	printf("done.\n");

	int best[64];
	for (int i = 0; i < 64; i++) best[i] = 0;

	for (; nmax == 0 || n < nmax; n++) {
		printf("%d..%d%c", nDeb, n, 13);
		clock_t t1 = clock();
		int p = persistErdosGMP(&keep);
		clock_t t2 = clock();
		if (best[p] == 0) {
			best[p] = n;
			if (p >= pmin) {
				char buf[512];
				sprintf_s(buf, "p(%d^%d)=%d (in %.3lf s)",
					digit, n, p, (double)(((double)t2 - (double)t1) / (double)CLOCKS_PER_SEC));
				printf("%s            \n", buf);
				FILE* f;
				int ok = fopen_s(&f, fSave, "a");
				fprintf(f, "%s\n", buf);
				fclose(f);
			}
		}
		// keep *= digit
		mpz_mul_ui(keep, keep, digit);
	}
	printf("Finito.\n");
	free(str);

	exit(0);
}
