#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <tepla/ec.h>

void peks_jr_keygen(const EC_PAIRING p, mpz_t x, mpz_t y, mpz_t x0, mpz_t y0, mpz_t x1, mpz_t y1, mpz_t x2, mpz_t y2, EC_POINT G1, EC_POINT G2, EC_POINT G1alpha, Element z, EC_POINT u1, EC_POINT h1, EC_POINT w1){
    // EC_PAIRING p;
    // pairing_init(p, "ECBN254a");

	gmp_randstate_t state;
	gmp_randinit_default(state);
	gmp_randseed_ui(state, (unsigned long)time(NULL));
	
    mpz_t order;
    mpz_init(order);
    mpz_set(order, *pairing_get_order(p));

    mpz_t alpha;
    mpz_init(alpha);

    //Zp上の値をランダムに選択
    mpz_urandomm(x, state, order);  
    mpz_urandomm(y, state, order);  
    mpz_urandomm(x0, state, order);  
    mpz_urandomm(y0, state, order);  
    mpz_urandomm(x1, state, order);  
    mpz_urandomm(y1, state, order);  
    mpz_urandomm(x2, state, order);  
    mpz_urandomm(y2, state, order);  
    mpz_urandomm(alpha, state, order);  

	point_random(G1);
	point_random(G2);

    Element tmpVal;
    element_init(tmpVal, p->g3);
   	pairing_map(tmpVal, G1, G2, p);

    mpz_t tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8;
    mpz_init(tmp1);
    mpz_init(tmp2);
    mpz_init(tmp3);
    mpz_init(tmp4);
    mpz_init(tmp5);
    mpz_init(tmp6);
    mpz_init(tmp7);
    mpz_init(tmp8);

    mpz_mul(tmp1,x0,alpha);
    mpz_add(tmp2,tmp1,y0); //x0*alpha+y0

    mpz_mul(tmp3,x1,alpha);
    mpz_add(tmp4,tmp3,y1); //x1*alpha+y1

    mpz_mul(tmp5,x2,alpha);
    mpz_add(tmp6,tmp5,y2); //x2*alpha+y2

    mpz_mul(tmp7,x,alpha);
    mpz_add(tmp8,tmp7,y); //x*alpha+y

    element_pow(z, tmpVal, tmp2);   //z=e(G1,G2)^(x0*alpha+y0)
	point_mul(u1, tmp4, G1);        //u1=G1^(x1*alpha+y1)
	point_mul(h1, tmp6, G1);        //h1=G1^(x2*alpha+y2)
	point_mul(w1, tmp8, G1);        //w1=G1^(x*alpha+y)

    point_mul(G1alpha, alpha, G1);    

}

void peks_jr_peks(const EC_PAIRING p, const mpz_t I, const EC_POINT G1, const EC_POINT G1alpha, const Element z, const EC_POINT u1, const EC_POINT h1, const EC_POINT w1, mpz_t tag, Element Cm, EC_POINT Cx, EC_POINT Cy, EC_POINT C){
    // EC_PAIRING p;
    // pairing_init(p, "ECBN254a");

	gmp_randstate_t state;
	gmp_randinit_default(state);
	gmp_randseed_ui(state, (unsigned long)time(NULL));
	
    mpz_t order;
    mpz_init(order);
    mpz_set(order, *pairing_get_order(p));

    //Zp上の値をランダムに選択
    mpz_t t, gamma1, gamma2;
    mpz_init(t);
    mpz_init(gamma1);
    mpz_init(gamma2);
    mpz_urandomm(t, state, order);  
    mpz_urandomm(gamma1, state, order);  
    mpz_urandomm(gamma2, state, order);  

    mpz_t tmp1;
    mpz_init(tmp1);
    mpz_mul(tmp1, gamma1,I);
    mpz_add(tag, tmp1, gamma2); //tag = gamma1*I+gamma2

    // Element tmpE1;
    // element_init(tmpE1, p->g3);
    // element_pow(tmpE1, z, t); //z^t
    // element_mul(Cm, m, tmpE1); //Cm = m*z^t
    element_pow(Cm, z, t); //z^t (peksではibeのm=1としている)

    point_mul(Cx, t, G1alpha); //Cx = (g1^alpha)^t
    point_mul(Cy, t, G1); //Cy = g1^t

    EC_POINT tmpP1, tmpP2, tmpP3, tmpP4;
    point_init(tmpP1, p->g1);
    point_init(tmpP2, p->g1);
    point_init(tmpP3, p->g1);
    point_init(tmpP4, p->g1);

    point_mul(tmpP1, I, u1); //u1^I
    point_mul(tmpP2, tag, w1); //w1^tag
    point_add(tmpP3, tmpP1, h1);
    point_add(tmpP4, tmpP3, tmpP2); //(u1^I)*h1*(w1^tag)
    point_mul(C, t, tmpP4); //((u1^I)*h1*(w1^tag))^t
  
}

void peks_jr_trapdoor(const EC_PAIRING p, const EC_POINT G2, const mpz_t x, const mpz_t x0, const mpz_t x1, const mpz_t x2, const mpz_t y, const mpz_t y0, const mpz_t y1, const mpz_t y2, const mpz_t I, EC_POINT D, EC_POINT Dx, EC_POINT Dxp, EC_POINT Dy, EC_POINT Dyp){
    // EC_PAIRING p;
    // pairing_init(p, "ECBN254a");

	gmp_randstate_t state;
	gmp_randinit_default(state);
	gmp_randseed_ui(state, (unsigned long)time(NULL));
	
    mpz_t order;
    mpz_init(order);
    mpz_set(order, *pairing_get_order(p));

    //Zp上の値をランダムに選択
    mpz_t r;
    mpz_init(r);
    mpz_urandomm(r, state, order);  


    mpz_t tmp1, tmp2;
    mpz_init(tmp1);
    mpz_init(tmp2);

    mpz_mul(tmp1, x, r);
    mpz_mul(tmp2, y, r);

    point_mul(D, r, G2);
    point_mul(Dx, tmp1, G2);
    point_mul(Dy, tmp2, G2);

    EC_POINT tmpP1, tmpP2, tmpP3, tmpP4, tmpP5, tmpP6, tmpP7, tmpP8, tmpP9, tmpP10, tmpP11, tmpP12;
    point_init(tmpP1, p->g2);
    point_init(tmpP2, p->g2);
    point_init(tmpP3, p->g2);
    point_init(tmpP4, p->g2);
    point_init(tmpP5, p->g2);
    point_init(tmpP6, p->g2);
    point_init(tmpP7, p->g2);
    point_init(tmpP8, p->g2);
    point_init(tmpP9, p->g2);
    point_init(tmpP10, p->g2);
    point_init(tmpP11, p->g2);
    point_init(tmpP12, p->g2);

    point_mul(tmpP1, x0, G2); //g2^x0
    point_mul(tmpP2, x1, G2); //g2^x1
    point_mul(tmpP3, I, tmpP2); //(g2^x1)^I
    point_mul(tmpP4, x2, G2); //g2^x2
    point_add(tmpP5, tmpP3, tmpP4);//((g2^x1)^I)*(g2^x2)
    point_mul(tmpP6, r, tmpP5); //(((g2^x1)^I)*(g2^x2))^r
    point_add(Dxp, tmpP1, tmpP6);

    point_mul(tmpP7, y0, G2); //g2^y0
    point_mul(tmpP8, y1, G2); //g2^y1
    point_mul(tmpP9, I, tmpP8); //(g2^y1)^I
    point_mul(tmpP10, y2, G2); //g2^y2
    point_add(tmpP11, tmpP9, tmpP10);//((g2^y1)^I)*(g2^y2)
    point_mul(tmpP12, r, tmpP11); //(((g2^y1)^I)*(g2^y2))^r
    point_add(Dyp, tmpP7, tmpP12);


}


int peks_jr_test(const EC_PAIRING p, const EC_POINT D, const EC_POINT Dx, const EC_POINT Dxp, const EC_POINT Dy, const EC_POINT Dyp, const mpz_t tag, const Element Cm, const EC_POINT Cx, const EC_POINT Cy, const EC_POINT C){
    // EC_PAIRING p;
    // pairing_init(p, "ECBN254a");

    
    //分子を求める
    //　e(C,D)を求める
    Element tmpE1;
    element_init(tmpE1, p->g3);
   	pairing_map(tmpE1, C, D, p);

    //　Cm*e(C,D)を求める
    Element nume;
    element_init(nume, p->g3);
    element_mul(nume,Cm,tmpE1);

    //分子を求める
    //  Dx^tagを求める
    EC_POINT tmpP1;
    point_init(tmpP1,p->g2);
    point_mul(tmpP1,tag,Dx);

    //  Dx^tag*Dxpを求める
    EC_POINT tmpP2;
    point_init(tmpP2,p->g2);
    point_add(tmpP2,tmpP1,Dxp);

    //  e(Cx,Dx^tag*Dxp)を求める
    Element tmpE2;
    element_init(tmpE2, p->g3);
   	pairing_map(tmpE2, Cx, tmpP2, p);

    //  Dy^tagを求める
    EC_POINT tmpP3;
    point_init(tmpP3,p->g2);
    point_mul(tmpP3,tag,Dy);

    //  Dy^tag*Dypを求める
    EC_POINT tmpP4;
    point_init(tmpP4,p->g2);
    point_add(tmpP4,tmpP3,Dyp);

    //  e(Cy,Dy^tag*Dyp)を求める
    Element tmpE3;
    element_init(tmpE3, p->g3);
   	pairing_map(tmpE3, Cy, tmpP4, p);

    //  e(Cx,Dx^tag*Dxp)*e(Cy,Dy^tag*Dyp)を求める
    Element denomi;
    element_init(denomi, p->g3);
    element_mul(denomi,tmpE2,tmpE3);

    // //分母のinvを求める
    // Element invdenomi;
    // element_init(invdenomi, p->g3);
    // element_inv(invdenomi,denomi);

    // //分母と分子invをかけてmpを得る
    // element_mul(mp,nume,invdenomi);    

    //peksの場合、jr_ibeのm=1としているから、分母と分子が一致すればTest成功
    if(element_cmp(nume, denomi)==0)
        return 1;//Yes
    else
        return 0;//No
}
