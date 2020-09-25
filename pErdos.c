/*
pErdos.c
compute Erdos multiplication persistence for numbers x=dddddd...dddd (n times d), and for next x with increasing n.
display and store best persistences found.
usage: pErdos digit nb [pmin]
ex: pErdos 9 149497
pmin is used to display only results with p(x)>=pmin
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <time.h>

#include "gmp.h"

char *str;
int persistErdosGMP(mpz_t *v){
	int i, occ[10];
	mpz_t a;
	mpz_init(a);
	mpz_set(a, *v);
	for(i=1;;i++){
		mpz_get_str(str, 10, a);	// gmp_sprintf(str, "%Zd", a);
		int len=strlen(str);
		if (len<=1){
			mpz_clear(a);
			return i;
		}
		for(int j=0;j<10;j++) occ[j]=0;
		for(int j=len-1;j>=0;j--) occ[str[j]-'0']++;
		mpz_clear(a);	// prevent memory leak...

		mpz_init_set_ui(a, 1);
		mpz_init(k);
		for(int j=2;j<10;j++){
			if (!occ[j]) continue;
			mpz_t k;
			mpz_init(k);
			mpz_ui_pow_ui(k, j, occ[j]);	// k = j ^ occ[j]
			mpz_mul(a, a, k);		// a *= k
			mpz_clear(k);
		}
	}
	mpz_clear(a);
	return i;
}
int main(int argc, char *argv[]){
	char fSave[512];
	if (argc<3){
		fprintf(stderr,"usage:%s digit n\n", argv[0]);
		return 1;
	}
	int digit, n, nDeb, nmax=0, pmin=0;
	sscanf(argv[1], "%d", &digit);
	sscanf(argv[2], "%d", &n);
	if (argc>3) sscanf(argv[3], "%d", &nmax);
	if (argc>4) sscanf(argv[4], "%d", &pmin);
	printf("compute Erdos multiplication persistence for numbers x=%d%d%d...%d%d%d", digit, digit, digit, digit, digit, digit);
	if (nmax==0) printf(" (n>=%d digits)", n); else printf(" (%d<=n<%d)", n, nmax);
	if (pmin!=0) printf(" with pE(x)>=%d", pmin);
	printf("\n");
	nDeb=n;
	sprintf(fSave, "pErdos_%d_%d.txt", digit, nDeb);

	printf("alloc...");
	str=(char*)malloc((n+1000000)*sizeof(char));	// get space for 1M digits more
	if (str==(char*)NULL){ fprintf(stderr, "Can't alloc memory...\n"); exit(1); }
	printf("done.\n");

	printf("compute first...");
	mpz_t k, keep;
	mpz_init_set_ui(k, digit);
	mpz_init(keep);
	// gmp_printf("k=%Zd\n", k);
	// keep=digit^n
	mpz_pow_ui(keep, k, n);
	// gmp_printf("keep=%Zd\n", keep);
	printf("done.\n");

	int best[64];
	for(int i=0;i<64;i++) best[i]=0;

	for(;nmax==0 || n<nmax;n++){
		printf("%d..%d%c", nDeb, n, 13);
		clock_t t1=clock();
		int p=persistErdosGMP(&keep);
		clock_t t2=clock();
		if (best[p]==0){
			best[p]=n;
			if (p>=pmin){
				char buf[512];
				sprintf(buf, "p(%d^%d)=%d (in %.3lf s)",
					digit, n, p, (double)(((double)t2-(double)t1)/(double)CLOCKS_PER_SEC) );
				printf("%s            \n", buf);
				FILE *f=fopen(fSave, "a");
				fprintf(f, "%s\n", buf);
				fclose(f);
			}
		}
		// keep *= digit
		mpz_mul_ui(keep, keep, digit);
	}
	printf("Finito.\n");
	exit (0);
}
