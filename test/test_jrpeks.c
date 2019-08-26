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

	gmp_randstate_t state;
	gmp_randinit_default(state);
	gmp_randseed_ui(state, (unsigned long)time(NULL));
	
    mpz_t order;
    mpz_init(order);
    mpz_set(order, *pairing_get_order(p));

    //keygen
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

    peks_jr_keygen(p,x,y,x0,y0,x1,y1,x2,y2,G1,G2,G1alpha,z,u1,h1,w1);    

    //jr_peks
    mpz_t I;
    mpz_init(I);
    mpz_urandomm(I, state, order);  

    mpz_t tag;
    Element Cm;
    EC_POINT Cx, Cy, C;
    mpz_init(tag);
    element_init(Cm, p->g3);
    point_init(Cx, p->g1);
    point_init(Cy, p->g1);
    point_init(C, p->g1);

    peks_jr_peks(p,I,G1,G1alpha,z,u1,h1,w1,tag,Cm,Cx,Cy,C);

    //jr_trapdoor
    // void peks_jr_trapdoor(const EC_POINT G2, const mpz_t x, const mpz_t x0, const mpz_t x1, const mpz_t x2, const mpz_t y, const mpz_t y0, const mpz_t y1, const mpz_t y2, const mpz_t I, EC_POINT D, EC_POINT Dx, EC_POINT Dxp, EC_POINT Dy, EC_POINT Dyp)
    mpz_t I_d;
    mpz_init(I_d);
    mpz_urandomm(I_d, state, order);  

    EC_POINT D, Dx, Dxp, Dy, Dyp;
    point_init(D, p->g2);
    point_init(Dx, p->g2);
    point_init(Dxp, p->g2);
    point_init(Dy, p->g2);
    point_init(Dyp, p->g2);
    
    peks_jr_trapdoor(p,G2,x,x0,x1,x2,y,y0,y1,y2,I,D,Dx,Dxp,Dy,Dyp);

    EC_POINT D_d, Dx_d, Dxp_d, Dy_d, Dyp_d;
    point_init(D_d, p->g2);
    point_init(Dx_d, p->g2);
    point_init(Dxp_d, p->g2);
    point_init(Dy_d, p->g2);
    point_init(Dyp_d, p->g2);

    peks_jr_trapdoor(p,G2,x,x0,x1,x2,y,y0,y1,y2,I_d,D_d,Dx_d,Dxp_d,Dy_d,Dyp_d);

    //jr_test
    //int peks_jr_test(const EC_POINT D, const EC_POINT Dx, const EC_POINT Dxp, const EC_POINT Dy, const EC_POINT Dyp, const mpz_t tag, const Element Cm, const EC_POINT Cx, const EC_POINT Cy, const EC_POINT C){
    int result1 = peks_jr_test(p,D,Dx,Dxp,Dy,Dyp,tag,Cm,Cx,Cy,C);
    int result2 = peks_jr_test(p,D_d,Dx_d,Dxp_d,Dy_d,Dyp_d,tag,Cm,Cx,Cy,C);

    printf("%d\n", result1);
    printf("%d\n", result2);
}