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
    EC_POINT P, ek1;
    mpz_t dk1;

    point_init(P, p->g1);
    point_init(ek1, p->g1);
    mpz_init(dk1);


    mpz_t x_1,y_1,x0_1,y0_1,x1_1,y1_1,x2_1,y2_1;
    mpz_init(x_1);
    mpz_init(y_1);
    mpz_init(x0_1);
    mpz_init(y0_1);
    mpz_init(x1_1);
    mpz_init(y1_1);
    mpz_init(x2_1);
    mpz_init(y2_1);

    EC_POINT G1_1,G2_1,G1alpha_1;
    EC_POINT u1_1, h1_1, w1_1;
    Element z_1;

    point_init(G1_1, p->g1);
    point_init(G2_1, p->g2);
    point_init(G1alpha_1, p->g1);

    element_init(z_1, p->g3);

    point_init(u1_1, p->g1);
    point_init(h1_1, p->g1);
    point_init(w1_1, p->g1);

    ke_std_setup(p,P,ek1,dk1,x_1,y_1,x0_1,y0_1,x1_1,y1_1,x2_1,y2_1,G1_1,G2_1,G1alpha_1,z_1,u1_1,h1_1,w1_1);

    //Encテスト
    //void ke_std_enc(const EC_POINT P_keyword, const EC_POINT P, const EC_POINT eki, EC_POINT cti1, EC_POINT cti2, 
    //const EC_POINT G1_i, const EC_POINT G1alpha_i, const Element z_i, const EC_POINT u1_i, const EC_POINT h1_i, const EC_POINT w1_i, 
    //mpz_t tag_i, Element Cm_i, EC_POINT Cx_i, EC_POINT Cy_i, EC_POINT C_i);
    EC_POINT W1;
	point_init(W1, p->g1);
	point_map_to_point(W1, "test", sizeof("test"), 128);	

    EC_POINT ct1_10, ct2_10;
    point_init(ct1_10, p->g1);
    point_init(ct2_10, p->g1);

    mpz_t tag_10;
    Element Cm_10;
    EC_POINT Cx_10, Cy_10, C_10;
    mpz_init(tag_10);
    element_init(Cm_10, p->g3);
    point_init(Cx_10, p->g1);
    point_init(Cy_10, p->g1);
    point_init(C_10, p->g1);

    ke_std_enc(p,W1,P,ek1,ct1_10,ct2_10,G1_1,G1alpha_1,z_1,u1_1,h1_1,w1_1,tag_10,Cm_10,Cx_10,Cy_10,C_10);

    //i=1でTrapdoor
    //void ke_std_trapdoor(const EC_POINT P_keyword, const EC_POINT G2_i, const mpz_t x_i, const mpz_t x0_i, const mpz_t x1_i, const mpz_t x2_i, const mpz_t y_i, const mpz_t y0_i, const mpz_t y1_i, const mpz_t y2_i, 
    //EC_POINT D_i, EC_POINT Dx_i, EC_POINT Dxp_i, EC_POINT Dy_i, EC_POINT Dyp_i)
    EC_POINT D_1, Dx_1, Dxp_1, Dy_1, Dyp_1;
    point_init(D_1, p->g2);
    point_init(Dx_1, p->g2);
    point_init(Dxp_1, p->g2);
    point_init(Dy_1, p->g2);
    point_init(Dyp_1, p->g2);

    ke_std_trapdoor(p,W1,G2_1,x_1,x0_1,x1_1,x2_1,y_1,y0_1,y1_1,y2_1,D_1,Dx_1,Dxp_1,Dy_1,Dyp_1);

    //i=1でc-w-1-0をTest（うまくいくはず）　＜ここA＞
    //void ke_std_test(const int i, const int j, const int k, 
    //const EC_POINT D_i, const EC_POINT Dx_i, const EC_POINT Dxp_i, const EC_POINT Dy_i, const EC_POINT Dyp_i, 
    //const mpz_t tag_i, const Element Cm_i, const EC_POINT Cx_i, const EC_POINT Cy_i, const EC_POINT C_i)
    int i=1;
    int j=1;
    int k=0;

    int result1 = ke_std_test(p,i,j,k,D_1,Dx_1,Dxp_1,Dy_1,Dyp_1,tag_10,Cm_10,Cx_10,Cy_10,C_10);
    printf("result1: %d\n", result1);

    //ここまでうまくいった

    //i=1でUpd → i=2鍵取得
    //void ke_std_upd(const EC_POINT P, mpz_t dk1, EC_POINT ek_ipp, mpz_t dk_ipp, mpz_t x_ipp, mpz_t y_ipp, mpz_t x0_ipp, mpz_t y0_ipp, mpz_t x1_ipp, mpz_t y1_ipp, mpz_t x2_ipp, mpz_t y2_ipp, EC_POINT G1_ipp, EC_POINT G2_ipp, EC_POINT G1alpha_ipp, Element z_ipp, EC_POINT u1_ipp, EC_POINT h1_ipp, EC_POINT w1_ipp, mpz_t rk_ipp)
    EC_POINT ek2;
    point_init(ek2, p->g1);
    mpz_t dk2,rk2;
    mpz_init(dk2);
    mpz_init(rk2);

    mpz_t x_2,y_2,x0_2,y0_2,x1_2,y1_2,x2_2,y2_2;
    mpz_init(x_2);
    mpz_init(y_2);
    mpz_init(x0_2);
    mpz_init(y0_2);
    mpz_init(x1_2);
    mpz_init(y1_2);
    mpz_init(x2_2);
    mpz_init(y2_2);

    EC_POINT G1_2,G2_2,G1alpha_2;
    EC_POINT u1_2, h1_2, w1_2;
    Element z_2;

    point_init(G1_2, p->g1);
    point_init(G2_2, p->g2);
    point_init(G1alpha_2, p->g1);

    element_init(z_2, p->g3);

    point_init(u1_2, p->g1);
    point_init(h1_2, p->g1);
    point_init(w1_2, p->g1);


    ke_std_upd(p,P, dk1,ek2,dk2,x_2,y_2,x0_2,x0_2,x1_2,y1_2,x2_2,y2_2,G1_2,G2_2,G1alpha_2,z_2,u1_2,h1_2,w1_2,rk2);

    //i=1でc-w-1-0をReEnc -> c-w-1-1を取得

    EC_POINT ct1_11, ct2_11;
    point_init(ct1_11, p->g1);
    point_init(ct2_11, p->g1);

    mpz_t tag_11;
    Element Cm_11;
    EC_POINT Cx_11, Cy_11, C_11;
    mpz_init(tag_11);
    element_init(Cm_11, p->g3);
    point_init(Cx_11, p->g1);
    point_init(Cy_11, p->g1);
    point_init(C_11, p->g1);
    ke_std_reenc(p,i,j,k,P,ek2,rk2,ct1_10,ct2_10,G1_2,G1alpha_2,z_2,u1_2,h1_2,w1_2,ct1_11,ct2_11,tag_11,Cm_11,Cx_11,Cy_11,C_11);

    //i=2でEnc → c-w-2-0取得
    EC_POINT ct1_20, ct2_20;
    point_init(ct1_20, p->g1);
    point_init(ct2_20, p->g1);

    mpz_t tag_20;
    Element Cm_20;
    EC_POINT Cx_20, Cy_20, C_20;
    mpz_init(tag_20);
    element_init(Cm_20, p->g3);
    point_init(Cx_20, p->g1);
    point_init(Cy_20, p->g1);
    point_init(C_20, p->g1);

    ke_std_enc(p,W1,P,ek2,ct1_20,ct2_20,G1_2,G1alpha_2,z_2,u1_2,h1_2,w1_2,tag_20,Cm_20,Cx_20,Cy_20,C_20);



    //i=2でTrapdoor
    EC_POINT D_2, Dx_2, Dxp_2, Dy_2, Dyp_2;
    point_init(D_2, p->g2);
    point_init(Dx_2, p->g2);
    point_init(Dxp_2, p->g2);
    point_init(Dy_2, p->g2);
    point_init(Dyp_2, p->g2);

    ke_std_trapdoor(p,W1,G2_2,x_2,x0_2,x1_2,x2_2,y_2,y0_2,y1_2,y2_2,D_2,Dx_2,Dxp_2,Dy_2,Dyp_2);

    //i=1でc-w-1-1をTest（うまくいかないはず）
    i=1;
    j=1;
    k=1;

    int result2 = ke_std_test(p,i,j,k,D_1,Dx_1,Dxp_1,Dy_1,Dyp_1,tag_11,Cm_11,Cx_11,Cy_11,C_11);
    printf("result2: %d\n", result2);

    //i=1でc-w-2-0をTest（うまくいかないはず）
    i=1;
    j=2;
    k=0;

    int result3 = ke_std_test(p,i,j,k,D_1,Dx_1,Dxp_1,Dy_1,Dyp_1,tag_20,Cm_20,Cx_20,Cy_20,C_20);
    printf("result3: %d\n", result3);

    //i=2でc-w-1-1をTest（うまくいくはず）
    i=2;
    j=1;
    k=1;

    int result4 = ke_std_test(p,i,j,k,D_2,Dx_2,Dxp_2,Dy_2,Dyp_2,tag_11,Cm_11,Cx_11,Cy_11,C_11);
    printf("result4: %d\n", result4);

    //i=2でc-w-2-0をTest（うまくいくはず）
    i=2;
    j=2;
    k=0;

    int result5 = ke_std_test(p,i,j,k,D_2,Dx_2,Dxp_2,Dy_2,Dyp_2,tag_20,Cm_20,Cx_20,Cy_20,C_20);
    printf("result5: %d\n", result5);

    //i=2でUpd → i=3鍵取得
    //i=2でc-w-1-0をReEnc （取得できないはず）
    //i=2でc-w-1-1をReEnc → c-w-1-2を取得
    //i=2でc-w-2-0をReEnc → c-w-2-1を取得
    //i=3でEnc → c-w-3-0を取得
    //i=3でTrapdoor
    //i=1でc-w-1-2をTest（うまくいかないはず）
    //i=1でc-w-2-1をTest（うまくいかないはず）
    //i=1でc-w-3-0をTest（うまくいかないはず）
    //i=2でc-w-1-2をTest（うまくいかないはず）
    //i=2でc-w-2-1をTest（うまくいかないはず）
    //i=2でc-w-3-0をTest（うまくいかないはず）
    //i=3でc-w-1-2をTest（うまくいくはず）
    //i=3でc-w-2-1をTest（うまくいくはず）
    //i=3でc-w-3-0をTest（うまくいくはず）


}