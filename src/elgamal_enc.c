#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <tepla/ec.h>

void eg_setup(EC_POINT P){

    //G1上の点Pをランダムに選択
    //point_init(P, p->g1);
    point_random(P);
    
}

void eg_keygen(const EC_PAIRING p, mpz_t a, const EC_POINT P, EC_POINT KA){
    gmp_randstate_t s;
    gmp_randinit_default(s);
	gmp_randseed_ui(s, (unsigned long)time(NULL));

    mpz_t order;
    mpz_init(order);
    mpz_set(order, *pairing_get_order(p));

    //mpz_init(a);
    mpz_urandomm(a, s, order);  //Zp上の値aをランダムに選択

    //point_init(KA, p->g1);
    point_mul(KA, a, P); //公開鍵KA＝aPを計算

}

void eg_encrypt(const EC_PAIRING p, const EC_POINT P, const EC_POINT KA, const EC_POINT M, EC_POINT C1, EC_POINT C2){

    gmp_randstate_t s;
    gmp_randinit_default(s);
	gmp_randseed_ui(s, (unsigned long)time(NULL));

    mpz_t order;
    mpz_init(order);
    mpz_set(order, *pairing_get_order(p));

    mpz_t r;
    mpz_init(r);
    mpz_urandomm(r, s, order);  //Zp上の値rをランダムに選択

    //point_init(C1, p->g1);
    point_mul(C1, r, P); //暗号文C1＝rPを計算

    EC_POINT tmpC;
    point_init(tmpC, p->g1);
    point_mul(tmpC, r, KA);
    point_add(C2, M, tmpC); //暗号文C2＝M+rKAを計算

}

void eg_decrypt(const EC_PAIRING p, const EC_POINT C1, const EC_POINT C2, const mpz_t a, EC_POINT M){

    EC_POINT tmpC;
    point_init(tmpC, p->g1);
    point_mul(tmpC, a, C1);

    point_sub(M, C2, tmpC);

}

