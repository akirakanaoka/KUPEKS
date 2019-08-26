#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <tepla/ec.h>

int main(void){
    clock_t start, end;
    double extime;

    EC_PAIRING p;
    pairing_init(p, "ECBN254a");


    //SetUp
    EC_POINT P;
    point_init(P, p->g1);


    start = clock();
    eg_setup(P);
    end=clock();
    extime = (double)((end-start)/1000)/CLOCKS_PER_SEC;
    printf("exec SetUp: %lf\n", extime);

    int len = point_get_str_length(P);
    char *s1 = (char *)malloc(sizeof(char)*len);
    point_get_str(s1,P);
    printf("%s\n",s1);


    //KeyGen
    EC_POINT KA;
    mpz_t a;
    point_init(KA, p->g1);
    mpz_init(a);

    start = clock();
    eg_keygen(p,a,P,KA);
    end=clock();
    extime = (double)((end-start)/1000)/CLOCKS_PER_SEC;
    printf("exec KeyGen: %lf\n", extime);

    len = point_get_str_length(KA);
    char *s2 = (char *)malloc(sizeof(char)*len);
    point_get_str(s2,KA);
    printf("%s\n",s2);


    //Encrypt
    EC_POINT M, C1, C2;
    point_init(M, p->g1);
    point_init(C1, p->g1);
    point_init(C2, p->g1);
    point_random(M);

    len = point_get_str_length(M);
    char *s5 = (char *)malloc(sizeof(char)*len);
    point_get_str(s5,M);
    printf("-- Plain Text: M --\n");
    printf("%s\n",s5);
    printf("-------------------\n");


    start = clock();
    eg_encrypt(p,P, KA, M, C1, C2);
    end=clock();
    extime = (double)((end-start)/1000)/CLOCKS_PER_SEC;
    printf("exec Encrypt: %lf\n", extime);

    len = point_get_str_length(C1);
    char *s3 = (char *)malloc(sizeof(char)*len);
    point_get_str(s3,C1);
    printf("%s\n",s3);
    char *s4 = (char *)malloc(sizeof(char)*len);
    point_get_str(s4,C2);
    printf("%s\n",s4);

    //Decrypt
    //void eg_decrypt(const EC_POINT C1, const EC_POINT C2, const mpz_t a, EC_POINT M)
    EC_POINT Mhat;
    point_init(Mhat, p->g1);

    start = clock();
    eg_decrypt(p,C1, C2, a, Mhat);
    end=clock();
    extime = (double)((end-start)/1000)/CLOCKS_PER_SEC;
    printf("exec Decrypt: %lf\n", extime);

    len = point_get_str_length(Mhat);
    char *s6 = (char *)malloc(sizeof(char)*len);
    point_get_str(s6,Mhat);
    printf("%s\n",s6);


    printf("-- Plain     Text: M    --\n");
    printf("%s\n",s5);
    printf("--------------------------\n");
    printf("-- Decrypted Text: Mhat --\n");
    printf("%s\n",s6);
    printf("--------------------------\n");


}