#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <tepla/ec.h>

int main(void){
    clock_t start, end;
    long total;
    double extime;
    int len;
    EC_PAIRING p;
    pairing_init(p, "ECBN254a");

    //SetUp
    EC_POINT P;
    point_init(P, p->g1);
    eg_setup(P);

    printf("Clocks:%ld\n",CLOCKS_PER_SEC);


    int loop = 100;
    int i;
    total =0;
    EC_POINT KA;
    mpz_t a;
    for(i=0;i<loop;i++){
        //KeyGen
        point_init(KA, p->g1);
        mpz_init(a);

        start = clock();
        eg_keygen(p,a,P,KA);
        end=clock();
        total+=(end-start);
    }
    extime = (double)(total*1000.0)/(CLOCKS_PER_SEC*loop);
    printf("exec KeyGen: %lf (%ld)\n", extime, total);

    total = 0;
    EC_POINT M, C1, C2;
    for(i=0;i<loop;i++){
        //Encrypt
        point_init(M, p->g1);
        point_init(C1, p->g1);
        point_init(C2, p->g1);
        point_random(M);

        start = clock();
        eg_encrypt(p, P, KA, M, C1, C2);
        end=clock();

        total+=(end-start);
    }
    extime = (double)(total*1000.0)/(CLOCKS_PER_SEC*loop);
    printf("exec Encrypt: %lf (%ld)\n", extime, total);


    EC_POINT Mhat;
    total = 0;
    for(i=0;i<loop;i++){
        //Decrypt
        point_init(Mhat, p->g1);

        start = clock();
        eg_decrypt(p, C1, C2, a, Mhat);
        end=clock();
        total+=(end-start);

        total+=(end-start);
    }
    extime = (double)(total*1000.0)/(CLOCKS_PER_SEC*loop);
    printf("exec Decrypt: %lf (%ld)\n", extime, total);

}