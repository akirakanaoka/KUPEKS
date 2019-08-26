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

    //SetUp → i=1鍵取得
    EC_POINT P,G,ek1,mpk1;
    mpz_t dk1,msk1;
    point_init(P, p->g1);
    point_init(G, p->g2);
    point_init(ek1, p->g1);
    mpz_init(dk1);
    point_init(mpk1, p->g2);
    mpz_init(msk1);

    printf("OK 01\n");

    ke_rom_setup(p,P,G,ek1,dk1,mpk1,msk1);

    printf("OK 02\n");

    //i=1でEnc c-w-1-0取得
    //void ke_rom_enc(EC_POINT P_keyword, const EC_POINT P, const EC_POINT G, const EC_POINT eki, const EC_POINT mpki, EC_POINT cti1, EC_POINT cti2, EC_POINT ci1, Element ci2);
    EC_POINT cti1_10, cti2_10, ci1_10;
    Element ci2_10;
    point_init(cti1_10, p->g1);
    point_init(cti2_10, p->g1);
    point_init(ci1_10, p->g2);
    element_init(ci2_10, p->g3);

    EC_POINT W1;
	point_init(W1, p->g1);
	point_map_to_point(W1, "test", sizeof("test"), 128);	

    printf("OK 03\n");
    ke_rom_enc(p,W1, P, G, ek1, mpk1, cti1_10, cti2_10, ci1_10, ci2_10);
    printf("OK 04\n");

    //i=1でTrapdoor
    //void ke_rom_trapdoor(const mpz_t mski, const EC_POINT P_keyword, EC_POINT Twi);
    EC_POINT Tw1;
    point_init(Tw1, p->g1);
    printf("OK 05\n");
    ke_rom_trapdoor(p,msk1, W1, Tw1);
    printf("OK 06\n");

    //i=1でc-w-1-0をTest（うまくいくはず）
    //int ke_rom_test(const int i, const int j, const int k, const EC_POINT Twi, const EC_POINT ci1, const Element ci2);
    int i=1;
    int j=1;
    int k=0;

    printf("OK 07\n");
    int result1 = ke_rom_test(p,i,j,k,Tw1,ci1_10,ci2_10);
    printf("OK 08\n");
    printf("result1: %d\n", result1);

    //i=1でUpd → i=2鍵取得
    //void ke_rom_upd(const EC_POINT P, const EC_POINT G, const mpz_t dk1,  EC_POINT ek_ipp, mpz_t dk_ipp, EC_POINT mpk_ipp, mpz_t msk_ipp, mpz_t rk_ipp);
    EC_POINT ek2,mpk2;
    mpz_t dk2,msk2,rk2;
    point_init(ek2, p->g1);
    mpz_init(dk2);
    point_init(mpk2, p->g2);
    mpz_init(msk2);
    mpz_init(rk2);

    ke_rom_upd(p,P,G,dk1,ek2,dk2,mpk2,msk2,rk2);

    //i=1でc-w-1-0をReEnc -> c-w-1-1を取得
    //void ke_rom_reenc(const int i, const int j, const int k, const EC_POINT P, const EC_POINT G, 
    //  const EC_POINT ek_ipp, const EC_POINT mpk_ipp, const mpz_t dk_ipp, 
    //  const EC_POINT cti1, const EC_POINT cti2, const EC_POINT ci1, const Element ci2, 
    //  EC_POINT cti1r, EC_POINT cti2r, EC_POINT ci1r, Element ci2r);
    EC_POINT cti1_11, cti2_11, ci1_11;
    Element ci2_11;
    point_init(cti1_11, p->g1);
    point_init(cti2_11, p->g1);
    point_init(ci1_11, p->g2);
    element_init(ci2_11, p->g3);

    ke_rom_reenc(p,i,j,k,P,G,ek2,mpk2,dk2,cti1_10,cti2_10,ci1_10,ci2_10,cti1_11,cti2_11,ci1_11,ci2_11);

    //i=2でEnc → c-w-2-0取得
    i=2; j=2; k=0;

    EC_POINT cti1_20, cti2_20, ci1_20;
    Element ci2_20;
    point_init(cti1_20, p->g1);
    point_init(cti2_20, p->g1);
    point_init(ci1_20, p->g2);
    element_init(ci2_20, p->g3);

    ke_rom_enc(p,W1, P, G, ek2, mpk2, cti1_20, cti2_20, ci1_20, ci2_20);

    //i=2でTrapdoor
    //void ke_rom_trapdoor(const mpz_t mski, const EC_POINT P_keyword, EC_POINT Twi);
    EC_POINT Tw2;
    point_init(Tw2, p->g1);
    ke_rom_trapdoor(p,msk2, W1, Tw2);

    //i=1でc-w-1-1をTest（うまくいかないはず）
    i=1;j=1;k=1;
    int result2 = ke_rom_test(p,i,j,k,Tw1,ci1_11,ci2_11);
    printf("result2: %d\n", result2);

    //i=1でc-w-2-0をTest（うまくいかないはず）
    i=1;j=2;k=0;
    int result3 = ke_rom_test(p,i,j,k,Tw1,ci1_20,ci2_20);
    printf("result3: %d\n", result2);

    //i=2でc-w-1-1をTest（うまくいくはず）
    i=2;j=1;k=1;
    int result4 = ke_rom_test(p,i,j,k,Tw2,ci1_11,ci2_11);
    printf("result4: %d\n", result4);

    //i=2でc-w-2-0をTest（うまくいくはず）
    i=2;j=2;k=0;
    int result5 = ke_rom_test(p,i,j,k,Tw2,ci1_20,ci2_20);
    printf("result5: %d\n", result5);

    //i=2でUpd → i=3鍵取得
    //void ke_rom_upd(const EC_POINT P, const EC_POINT G, const mpz_t dk1,  EC_POINT ek_ipp, mpz_t dk_ipp, EC_POINT mpk_ipp, mpz_t msk_ipp, mpz_t rk_ipp);
    EC_POINT ek3,mpk3;
    mpz_t dk3,msk3,rk3;
    point_init(ek3, p->g1);
    mpz_init(dk3);
    point_init(mpk3, p->g2);
    mpz_init(msk3);
    mpz_init(rk3);

    ke_rom_upd(p,P,G,dk2,ek3,dk3,mpk3,msk3,rk3);

    //i=2でc-w-1-0をReEnc （取得できないはず）
    i=2;j=1;k=0;
    EC_POINT cti1_xx, cti2_xx, ci1_xx;
    Element ci2_xx;
    point_init(cti1_xx, p->g1);
    point_init(cti2_xx, p->g1);
    point_init(ci1_xx, p->g2);
    element_init(ci2_xx, p->g3);

    ke_rom_reenc(p,i,j,k,P,G,ek3,mpk3,dk3,cti1_10,cti2_10,ci1_10,ci2_10,cti1_xx,cti2_xx,ci1_xx,ci2_xx);

    int cti1flag = point_is_infinity(cti1_xx);
    int cti2flag = point_is_infinity(cti2_xx);
    int ci1flag = point_is_infinity(ci1_xx);
    int ci2flag = element_is_zero(ci2_xx);

    printf("result6: %d, %d, %d, %d\n", cti1flag, cti2flag, ci1flag, ci2flag);


    //i=2でc-w-1-1をReEnc → c-w-1-2を取得
    i=2;j=1;k=1;
    EC_POINT cti1_12, cti2_12, ci1_12;
    Element ci2_12;
    point_init(cti1_12, p->g1);
    point_init(cti2_12, p->g1);
    point_init(ci1_12, p->g2);
    element_init(ci2_12, p->g3);

    ke_rom_reenc(p,i,j,k,P,G,ek3,mpk3,dk3,cti1_11,cti2_11,ci1_11,ci2_11,cti1_12,cti2_12,ci1_12,ci2_12);
    
    cti1flag = point_is_infinity(cti1_12);
    cti2flag = point_is_infinity(cti2_12);
    ci1flag = point_is_infinity(ci1_12);
    ci2flag = element_is_zero(ci2_12);

    printf("result7: %d, %d, %d, %d\n", cti1flag, cti2flag, ci1flag, ci2flag);



    //i=2でc-w-2-0をReEnc → c-w-2-1を取得
    i=2;j=2;k=0;
    EC_POINT cti1_21, cti2_21, ci1_21;
    Element ci2_21;
    point_init(cti1_21, p->g1);
    point_init(cti2_21, p->g1);
    point_init(ci1_21, p->g2);
    element_init(ci2_21, p->g3);

    ke_rom_reenc(p,i,j,k,P,G,ek3,mpk3,dk3,cti1_20,cti2_20,ci1_20,ci2_20,cti1_21,cti2_21,ci1_21,ci2_21);
    
    cti1flag = point_is_infinity(cti1_21);
    cti2flag = point_is_infinity(cti2_21);
    ci1flag = point_is_infinity(ci1_21);
    ci2flag = element_is_zero(ci2_21);

    printf("result8: %d, %d, %d, %d\n", cti1flag, cti2flag, ci1flag, ci2flag);



    //i=3でEnc → c-w-3-0を取得
    i=3; j=3; k=0;

    EC_POINT cti1_30, cti2_30, ci1_30;
    Element ci2_30;
    point_init(cti1_30, p->g1);
    point_init(cti2_30, p->g1);
    point_init(ci1_30, p->g2);
    element_init(ci2_30, p->g3);

    ke_rom_enc(p,W1, P, G, ek3, mpk3, cti1_30, cti2_30, ci1_30, ci2_30);


    //i=3でTrapdoor
    EC_POINT Tw3;
    point_init(Tw3, p->g1);
    ke_rom_trapdoor(p,msk3, W1, Tw3);

    //i=1でc-w-1-2をTest（うまくいかないはず）
    i=1;j=1;k=2;
    int result9 = ke_rom_test(p,i,j,k,Tw1,ci1_12,ci2_12);
    printf("result9: %d\n", result9);

    //i=1でc-w-2-1をTest（うまくいかないはず）
    i=1;j=2;k=1;
    int result10 = ke_rom_test(p,i,j,k,Tw1,ci1_21,ci2_21);
    printf("result10: %d\n", result10);

    //i=1でc-w-3-0をTest（うまくいかないはず）
    i=1;j=3;k=0;
    int result11 = ke_rom_test(p,i,j,k,Tw1,ci1_30,ci2_30);
    printf("result11: %d\n", result11);

    //i=2でc-w-1-2をTest（うまくいかないはず）
    i=2;j=1;k=2;
    int result12 = ke_rom_test(p,i,j,k,Tw2,ci1_12,ci2_12);
    printf("result12: %d\n", result12);

    //i=2でc-w-2-1をTest（うまくいかないはず）
    i=2;j=2;k=1;
    int result13 = ke_rom_test(p,i,j,k,Tw2,ci1_21,ci2_21);
    printf("result13: %d\n", result13);

    //i=2でc-w-3-0をTest（うまくいかないはず）
    i=2;j=3;k=0;
    int result14 = ke_rom_test(p,i,j,k,Tw2,ci1_30,ci2_30);
    printf("result14: %d\n", result14);

    //i=3でc-w-1-2をTest（うまくいくはず）
    i=3;j=1;k=2;
    int result15 = ke_rom_test(p,i,j,k,Tw3,ci1_12,ci2_12);
    printf("result15: %d\n", result15);

    //i=3でc-w-2-1をTest（うまくいくはず）
    i=3;j=2;k=1;
    int result16 = ke_rom_test(p,i,j,k,Tw3,ci1_21,ci2_21);
    printf("result16: %d\n", result16);

    //i=3でc-w-3-0をTest（うまくいくはず）
    i=3;j=3;k=0;
    int result17 = ke_rom_test(p,i,j,k,Tw3,ci1_30,ci2_30);
    printf("result17: %d\n", result17);



}