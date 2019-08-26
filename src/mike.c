#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <tepla/ec.h>

void mike_init(const EC_PAIRING p,
    mpz_t x0, mpz_t x1, mpz_t x2, mpz_t x3, mpz_t x4,
    mpz_t y0, mpz_t y1, mpz_t y2, mpz_t y3, mpz_t y4,
    mpz_t b0, mpz_t b1, mpz_t b2, mpz_t b3, mpz_t b4,
    mpz_t bp0, mpz_t bp1, mpz_t bp2, mpz_t bp3, mpz_t bp4,
    mpz_t alpha,
    Element z, EC_POINT G1, EC_POINT G1alpha, EC_POINT G2, EC_POINT u1, EC_POINT w1, EC_POINT h1, EC_POINT v1, 
    EC_POINT Bx0, EC_POINT Bx1, EC_POINT Bx2, EC_POINT Bx3, EC_POINT Bx4,  
    EC_POINT By0, EC_POINT By1, EC_POINT By2, EC_POINT By3, EC_POINT By4
){
    gmp_randstate_t state;
	gmp_randinit_default(state);
	gmp_randseed_ui(state, (unsigned long)time(NULL));
	
    mpz_t order;
    mpz_init(order);
    mpz_set(order, *pairing_get_order(p));

    mpz_urandomm(x0, state, order);
    mpz_urandomm(x1, state, order);
    mpz_urandomm(x2, state, order);
    mpz_urandomm(x3, state, order);
    mpz_urandomm(x4, state, order);

    mpz_urandomm(y0, state, order);
    mpz_urandomm(y1, state, order);
    mpz_urandomm(y2, state, order);
    mpz_urandomm(y3, state, order);
    mpz_urandomm(y4, state, order);

    mpz_urandomm(b0, state, order);
    mpz_urandomm(b1, state, order);
    mpz_urandomm(b2, state, order);
    mpz_urandomm(b3, state, order);
    mpz_urandomm(b4, state, order);

    mpz_urandomm(bp0, state, order);
    mpz_urandomm(bp1, state, order);
    mpz_urandomm(bp2, state, order);
    mpz_urandomm(bp3, state, order);
    mpz_urandomm(bp4, state, order);

    mpz_urandomm(alpha, state, order);
    
	point_random(G1);
	point_random(G2);
    point_mul(G1alpha, alpha,G1);

    Element tmpVal;
    element_init(tmpVal, p->g3);
   	pairing_map(tmpVal, G1, G2, p);

    mpz_t tmp1,tmp2,tmp3,tmp4,tmp5,tmp6,tmp7,tmp8,tmp9,tmp10;
    mpz_init(tmp1);
    mpz_init(tmp2);
    mpz_init(tmp3);
    mpz_init(tmp4);
    mpz_init(tmp5);
    mpz_init(tmp6);
    mpz_init(tmp7);
    mpz_init(tmp8);
    mpz_init(tmp9);
    mpz_init(tmp10);

    mpz_mul(tmp1, x0,alpha);
    mpz_sub(tmp2, tmp1, y0); //x0*alpha-y0;
    mpz_mul(tmp3, x1,alpha);
    mpz_sub(tmp4, tmp1, y1); //x1*alpha-y1;
    mpz_mul(tmp5, x2,alpha);
    mpz_sub(tmp6, tmp1, y2); //x2*alpha-y2;
    mpz_mul(tmp7, x3,alpha);
    mpz_sub(tmp8, tmp1, y3); //x3*alpha-y3;
    mpz_mul(tmp9, x4,alpha);
    mpz_sub(tmp10, tmp1, y4); //x4*alpha-y4;

    element_pow(z, tmpVal,tmp2);
    point_mul(u1, tmp4, G1);
	point_mul(w1, tmp6, G1);
	point_mul(h1, tmp8, G1);
	point_mul(v1, tmp10, G1);

    mpz_init(tmp1);
    mpz_init(tmp2);
    mpz_init(tmp3);
    mpz_init(tmp4);
    mpz_init(tmp5);
    mpz_add(tmp1, x0,b0);
    mpz_add(tmp2, x1,b1);
    mpz_add(tmp3, x2,b2);
    mpz_add(tmp4, x3,b3);
    mpz_add(tmp5, x4,b4);

    point_mul(Bx0, tmp1, G2);
    point_mul(Bx1, tmp2, G2);
    point_mul(Bx2, tmp3, G2);
    point_mul(Bx3, tmp4, G2);
    point_mul(Bx4, tmp5, G2);

    mpz_init(tmp1);
    mpz_init(tmp2);
    mpz_init(tmp3);
    mpz_init(tmp4);
    mpz_init(tmp5);
    mpz_init(tmp6);
    mpz_init(tmp7);
    mpz_init(tmp8);
    mpz_init(tmp9);
    mpz_init(tmp10);
    mpz_add(tmp1, y0,bp0);
    mpz_add(tmp2, y1,bp1);
    mpz_add(tmp3, y2,bp2);
    mpz_add(tmp4, y3,bp3);
    mpz_add(tmp5, y4,bp4);
    mpz_neg(tmp6,tmp1);
    mpz_neg(tmp7,tmp2);
    mpz_neg(tmp8,tmp3);
    mpz_neg(tmp9,tmp4);
    mpz_neg(tmp10,tmp5);

    point_mul(By0, tmp6, G2);
    point_mul(By1, tmp7, G2);
    point_mul(By2, tmp8, G2);
    point_mul(By3, tmp9, G2);
    point_mul(By4, tmp10, G2);

}

void mike_updgen(const EC_PAIRING p, const int t, const EC_POINT G2,
    const EC_POINT Bx0, const EC_POINT Bx1, const EC_POINT Bx2, const EC_POINT Bx3, const EC_POINT Bx4,  
    const EC_POINT By0, const EC_POINT By1, const EC_POINT By2, const EC_POINT By3, const EC_POINT By4,
    EC_POINT D1, EC_POINT D2, 
    EC_POINT Dx1, EC_POINT Dx2, EC_POINT Dxp1, EC_POINT Dxp2, EC_POINT Dxpp1, EC_POINT Dxpp2, 
    EC_POINT Dy1, EC_POINT Dy2, EC_POINT Dyp1, EC_POINT Dyp2, EC_POINT Dypp1, EC_POINT Dypp2
){

    gmp_randstate_t state;
	gmp_randinit_default(state);
	gmp_randseed_ui(state, (unsigned long)time(NULL));
	
    mpz_t order;
    mpz_init(order);
    mpz_set(order, *pairing_get_order(p));

    mpz_t tmpt;
    mpz_init(tmpt);
    mpz_set_si(tmpt, t);

    mpz_t s1, s2;
    mpz_init(s1);
    mpz_init(s2);
    mpz_urandomm(s1, state, order);
    mpz_urandomm(s2, state, order);

    //D1, D2
    point_mul(D1, s1, G2);
    point_mul(D2, s2, G2);

    //Dx1, Dx2
    point_mul(Dx1, s1, Bx2);
    point_mul(Dx2, s2, Bx2);

    //Dxp1,
    EC_POINT tmpP1, tmpP2, tmpP3;
    point_init(tmpP1, p->g2);
    point_init(tmpP2, p->g2);
    point_init(tmpP3, p->g2);

    point_mul(tmpP1,tmpt,Bx1);
    point_add(tmpP2,tmpP1,Bx3);
    point_mul(tmpP3,s1,tmpP2);
    point_add(Dxp1,Bx0,tmpP3);


    //Dxp2
    point_mul(Dxp2,s2,tmpP2);

    //Dxpp1, Dxpp2
    point_mul(Dxpp1, s1, Bx4);
    point_mul(Dxpp2, s2, Bx4);

    //Dy1, Dy2
    point_mul(Dy1, s1, By2);
    point_mul(Dy2, s2, By2);

    //Dyp1
    point_init(tmpP1, p->g2);
    point_init(tmpP2, p->g2);
    point_init(tmpP3, p->g2);

    point_mul(tmpP1,tmpt,By1);
    point_add(tmpP2,tmpP1,By3);
    point_mul(tmpP3,s1,tmpP2);
    point_add(Dyp1,By0,tmpP3);

    //Dyp2
    point_mul(Dyp2,s2,tmpP2);

    //Dypp1, Dypp2
    point_mul(Dypp1, s1, By4);
    point_mul(Dypp2, s2, By4);

}

void mike_mkupd(const EC_PAIRING p, const int t, const EC_POINT G2,
    const mpz_t b0, const mpz_t b1, const mpz_t b2, const mpz_t b3, const mpz_t b4, 
    const mpz_t bp0, const mpz_t bp1, const mpz_t bp2, const mpz_t bp3, const mpz_t bp4, 
    const EC_POINT D1, const EC_POINT D2, 
    const EC_POINT Dx1, const EC_POINT Dx2, const EC_POINT Dxp1, const EC_POINT Dxp2, const EC_POINT Dxpp1, const EC_POINT Dxpp2, 
    const EC_POINT Dy1, const EC_POINT Dy2, const EC_POINT Dyp1, const EC_POINT Dyp2, const EC_POINT Dypp1, const EC_POINT Dypp2,
    EC_POINT LD1, EC_POINT LD2, 
    EC_POINT LDx1, EC_POINT LDx2, EC_POINT LDxp1, EC_POINT LDxp2, EC_POINT LDxpp1, EC_POINT LDxpp2,
    EC_POINT LDy1, EC_POINT LDy2, EC_POINT LDyp1, EC_POINT LDyp2, EC_POINT LDypp1, EC_POINT LDypp2
){
    gmp_randstate_t state;
	gmp_randinit_default(state);
	gmp_randseed_ui(state, (unsigned long)time(NULL));
	
    mpz_t order;
    mpz_init(order);
    mpz_set(order, *pairing_get_order(p));

    mpz_t tmpt;
    mpz_init(tmpt);
    mpz_set_si(tmpt, t);


    //LD1, LD2
    point_set(LD1, D1);
    point_set(LD2, D2);

    //LDx1, LDx2
    EC_POINT tmpP1, tmpP2;
    point_init(tmpP1, p->g2);
    point_init(tmpP2, p->g2);
    mpz_t tmp1;
    mpz_init(tmp1);
    mpz_neg(tmp1,b2);
    point_mul(tmpP1,tmp1,D1);//D1^-b2
    point_add(LDx1,Dx1,tmpP1);//Dx1 * D1^-b2
    point_mul(tmpP2,tmp1,D2);//D2^-b2
    point_add(LDx2,Dx2,tmpP2);//Dx2 * D2^-b2

    //LDxp1
    mpz_t tmp2,tmp3,tmp4;
    mpz_init(tmp1);
    mpz_init(tmp2);
    mpz_init(tmp3);
    mpz_init(tmp4);
    mpz_neg(tmp1,b0);//-b0
    mpz_neg(tmp2,b3);
    mpz_mul(tmp3,b1,tmpt);//b1*T
    mpz_sub(tmp4,tmp2,tmp3);//-b3-b1*T
    point_init(tmpP1, p->g2);
    point_init(tmpP2, p->g2);
    point_mul(tmpP1,tmp1,G2);//g2^-b0
    point_mul(tmpP2,tmp4,D1);//D1^(-b3-b1*T)
    EC_POINT tmpP3;
    point_init(tmpP3, p->g2);
    point_add(tmpP3,Dxp1,tmpP1);
    point_add(LDxp1,tmpP3,tmpP2);

    //LDxp2
    point_init(tmpP1, p->g2);
    point_mul(tmpP1,tmp4,D2);//D2^(-b3-b1*T)
    point_add(LDxp2,Dxp2,tmpP1);

    //LDxpp1, LDxpp2
    mpz_init(tmp1);
    mpz_neg(tmp1,b4);//-b4
    point_init(tmpP1, p->g2);
    point_init(tmpP2, p->g2);
    point_mul(tmpP1,tmp1,D1);//D1^-b4
    point_add(LDxpp1,Dxpp1,tmpP1);
    point_mul(tmpP2,tmp1,D2);//D2^-b4
    point_add(LDxpp2,Dxpp2,tmpP2);

    //LDy1, LDy2
    point_init(tmpP1, p->g2);
    point_init(tmpP2, p->g2);
    point_mul(tmpP1,bp2,D1);//D1^bp2
    point_add(LDy1,Dy1,tmpP1);//Dy1 * D1^bp2
    point_mul(tmpP2,bp2,D2);//D2^bp2
    point_add(LDy2,Dy2,tmpP2);//Dx2 * D2^bp2

    //LDyp1
    mpz_init(tmp3);
    mpz_init(tmp4);
    mpz_mul(tmp3,bp1,tmpt);//bp1*T
    mpz_add(tmp4,bp3,tmp3);//bp3+bp1*T
    point_init(tmpP1, p->g2);
    point_init(tmpP2, p->g2);
    point_mul(tmpP1,bp0,G2);//g2^bp0
    point_mul(tmpP2,tmp4,D1);//D1^(bp3+bp1*T)
    point_init(tmpP3, p->g2);
    point_add(tmpP3,Dyp1,tmpP1);
    point_add(LDyp1,tmpP3,tmpP2);

    //LDyp2
    point_init(tmpP1, p->g2);
    point_mul(tmpP1,tmp4,D2);//D2^(bp3+bp1*T)
    point_add(LDyp2,Dyp2,tmpP1);

    //LDypp1, LDypp2
    point_init(tmpP1, p->g2);
    point_init(tmpP2, p->g2);
    point_mul(tmpP1,bp4,D1);//D1^bp4
    point_add(LDypp1,Dypp1,tmpP1);
    point_mul(tmpP2,bp4,D2);//D2^bp4
    point_add(LDypp2,Dypp2,tmpP2);



}

void mike_kg(const EC_PAIRING p, const mpz_t I,
    const EC_POINT D1, const EC_POINT D2, 
    const EC_POINT Dx1, const EC_POINT Dx2, const EC_POINT Dxp1, const EC_POINT Dxp2, const EC_POINT Dxpp1, const EC_POINT Dxpp2, 
    const EC_POINT Dy1, const EC_POINT Dy2, const EC_POINT Dyp1, const EC_POINT Dyp2, const EC_POINT Dypp1, const EC_POINT Dypp2,
    EC_POINT K, EC_POINT Kx, EC_POINT Kpx, EC_POINT Ky, EC_POINT Kpy
){
    gmp_randstate_t state;
	gmp_randinit_default(state);
	gmp_randseed_ui(state, (unsigned long)time(NULL));
	
    mpz_t order;
    mpz_init(order);
    mpz_set(order, *pairing_get_order(p));

    mpz_t r;
    mpz_init(r);
    mpz_urandomm(r, state, order);

    //K
    EC_POINT tmpP1;
    point_init(tmpP1, p->g2);
    point_mul(tmpP1, r, D2);//D2^r
    point_add(K, D1, tmpP1);//D1 * D2^r

    //Kx
    point_init(tmpP1, p->g2);
    point_mul(tmpP1, r, Dx2);//Dx2^r
    point_add(Kx, Dx1, tmpP1);//Dx1 * Dx2^r

    //Kpx
    EC_POINT tmpP2,tmpP3,tmpP4;
    point_init(tmpP1, p->g2);
    point_init(tmpP2, p->g2);
    point_init(tmpP3, p->g2);
    point_init(tmpP4, p->g2);
    point_mul(tmpP1, I, Dxpp1);//Dxpp1^I
    point_add(tmpP2, Dxp2,tmpP1);//Dxp2 * Dxpp1^I
    point_mul(tmpP3, r, tmpP2); //(Dxp2 * Dxpp1^I)^r
    point_add(tmpP4, Dxp1,tmpP1);//Dxp1 * Dxpp1^I
    point_add(Kpx,tmpP4,tmpP3);//Dxp1 * Dxpp1^I * (Dxp2 * Dxpp1^I)^r

    //Ky
    point_init(tmpP1, p->g2);
    point_mul(tmpP1, r, Dy2);//Dy2^r
    point_add(Ky, Dy1, tmpP1);//Dy1 * Dy2^r

    //Kpy
    point_init(tmpP1, p->g2);
    point_init(tmpP2, p->g2);
    point_init(tmpP3, p->g2);
    point_init(tmpP4, p->g2);
    point_mul(tmpP1, I, Dypp1);//Dypp1^I
    point_add(tmpP2, Dyp2,tmpP1);//Dyp2 * Dypp1^I
    point_mul(tmpP3, r, tmpP2); //(Dyp2 * Dypp1^I)^r
    point_add(tmpP4, Dyp1,tmpP1);//Dyp1 * Dypp1^I
    point_add(Kpy,tmpP4,tmpP3);//Dyp1 * Dypp1^I * (Dyp2 * Dypp1^I)^r

}

void mike_ibenc(const EC_PAIRING p, const mpz_t I, const int t, const Element M, 
    const Element z, const EC_POINT G1, const EC_POINT G1alpha, const EC_POINT u1, const EC_POINT w1, const EC_POINT h1, const EC_POINT v1, 
    Element CM, EC_POINT Cx, EC_POINT Cy, EC_POINT C, mpz_t tag
){

    gmp_randstate_t state;
	gmp_randinit_default(state);
	gmp_randseed_ui(state, (unsigned long)time(NULL));
	
    mpz_t order;
    mpz_init(order);
    mpz_set(order, *pairing_get_order(p));

    mpz_t tr;
    mpz_init(tr);
    mpz_urandomm(tr, state, order);
    mpz_urandomm(tag, state, order);

    mpz_t tmpt;
    mpz_init(tmpt);
    mpz_set_si(tmpt, t);

    //CM
    Element tmpE1;
    element_init(tmpE1, p->g3);
    element_pow(tmpE1, z, tr); //z^t
    element_mul(CM, M, tmpE1); //M * z^t
    
    //Cx
    point_mul(Cx, tr, G1alpha);

    //Cy
    point_mul(Cy,tr, G1);

    //C   
    EC_POINT tmpP1,tmpP2,tmpP3,tmpP4,tmpP5,tmpP6;
    point_init(tmpP1,p->g1);
    point_init(tmpP2,p->g1);
    point_init(tmpP3,p->g1);
    point_init(tmpP4,p->g1);
    point_init(tmpP5,p->g1);
    point_init(tmpP6,p->g1);
    point_mul(tmpP1,tmpt,u1);
    point_mul(tmpP2,tag,w1);
    point_mul(tmpP3,I,v1);
    point_add(tmpP4,tmpP1,tmpP2);
    point_add(tmpP5,tmpP4,h1);
    point_add(tmpP6,tmpP5,tmpP3);
    point_mul(C,tr,tmpP6);



}

void mike_ibdec(const EC_PAIRING p,
    const EC_POINT K, const EC_POINT Kx, const EC_POINT Kpx, const EC_POINT Ky, const EC_POINT Kpy,
    const Element CM, const EC_POINT Cx, const EC_POINT Cy, const EC_POINT C, const mpz_t tag,
    Element mp
){
    //分子
    Element tmpE1;
    element_init(tmpE1, p->g3);
   	pairing_map(tmpE1, C, K, p);

    Element nume;
    element_init(nume, p->g3);
    element_mul(nume,CM,tmpE1);

    //分母
    EC_POINT tmpP1, tmpP2, tmpP3, tmpP4;
    point_init(tmpP1,p->g2);
    point_init(tmpP2,p->g2);
    point_init(tmpP3,p->g2);
    point_init(tmpP4,p->g2);
    point_mul(tmpP1,tag,Kx);
    point_add(tmpP2,tmpP1,Kpx);
    point_mul(tmpP3,tag,Ky);
    point_add(tmpP4,tmpP3,Kpy);

    Element tmpE2, tmpE3;
    element_init(tmpE2, p->g3);
    element_init(tmpE3, p->g3);
   	pairing_map(tmpE2, Cx, tmpP2, p);
   	pairing_map(tmpE3, Cy, tmpP4, p);

    Element denomi;
    element_init(denomi, p->g3);
    element_mul(denomi,tmpE2,tmpE3);

    //分母の逆数を求める
    Element invdenomi;
    element_init(invdenomi, p->g3);
    element_inv(invdenomi,denomi);

    //平文戻す
    element_mul(mp,nume,invdenomi);        
}