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

    //jr_init
    mpz_t x,y,x0,y0,x1,y1,x2,y2;
    mpz_init(x);
    mpz_init(y);
    mpz_init(x0);
    mpz_init(y0);
    mpz_init(x1);
    mpz_init(y1);
    mpz_init(x2);
    mpz_init(y2);

    EC_POINT G1,G2,G1alpha;
    EC_POINT u1, h1, w1;
    Element z;

    point_init(G1, p->g1);
    point_init(G2, p->g2);
    point_init(G1alpha, p->g1);

    element_init(z, p->g3);

    point_init(u1, p->g1);
    point_init(h1, p->g1);
    point_init(w1, p->g1);



    jr_init(p,x,y,x0,y0,x1,y1,x2,y2,G1,G2,G1alpha,z,u1,h1,w1);

    //jr_ext
	gmp_randstate_t state;
	gmp_randinit_default(state);
	gmp_randseed_ui(state, (unsigned long)time(NULL));
	
    mpz_t order;
    mpz_init(order);
    mpz_set(order, *pairing_get_order(p));

    mpz_t I;
    mpz_init(I);
    mpz_urandomm(I, state, order);  

    EC_POINT D, Dx, Dxp, Dy, Dyp;
    point_init(D, p->g2);
    point_init(Dx, p->g2);
    point_init(Dxp, p->g2);
    point_init(Dy, p->g2);
    point_init(Dyp, p->g2);

    jr_ext(p,G2,x,x0,x1,x2,y,y0,y1,y2,I,D,Dx,Dxp,Dy,Dyp);


    //jr_enc
    //void jr_enc(const mpz_t I, const Element m, const EC_POINT G1, const EC_POINT G1alpha, const Element z, const EC_POINT u1, const EC_POINT h1, const EC_POINT w1, 
    //mpz_t tag, Element Cm, EC_POINT Cx, EC_POINT Cy, EC_POINT C)
    //m,tag,Cm,Cx,Cy,C準備
    Element m;
    element_init(m, p->g3);
    element_random(m);

    mpz_t tag;
    Element Cm;
    EC_POINT Cx, Cy, C;
    mpz_init(tag);
    element_init(Cm, p->g3);
    point_init(Cx, p->g1);
    point_init(Cy, p->g1);
    point_init(C, p->g1);

    jr_enc(p,I,m,G1,G1alpha,z,u1,h1,w1,tag,Cm,Cx,Cy,C);

    //jr_dec
    //void jr_dec(const EC_POINT D, const EC_POINT Dx, const EC_POINT Dxp, const EC_POINT Dy, const EC_POINT Dyp, const mpz_t tag, const Element Cm, const EC_POINT Cx, const EC_POINT Cy, const EC_POINT C, Element mp);
    Element mp;
    element_init(mp, p->g3);
    jr_dec(p,D,Dx,Dxp,Dy,Dyp,tag,Cm,Cx,Cy,C,mp);
    //ToDo
    //enc_decのテスト
    //その後は、プログラムを別にしてjr_ibeをベースにそれをPEKS化する

    element_print(m);
    element_print(mp);

    int result = element_cmp(m,mp);
    printf("result: %d\n",result); //0が出れば復号成功

}