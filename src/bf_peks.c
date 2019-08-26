#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>


#include <tepla/ec.h>

void peks_bf_keygen(const EC_PAIRING p, mpz_t alpha, EC_POINT G, EC_POINT A_pub){
	
	gmp_randstate_t state;
	gmp_randinit_default(state);
	gmp_randseed_ui(state, (unsigned long)time(NULL));
	
    mpz_t order;
    mpz_init(order);
    mpz_set(order, *pairing_get_order(p));

    mpz_urandomm(alpha, state, order);  //Zp上の値alphaをランダムに選択
	point_random(G);
	point_mul(A_pub, alpha, G);

}

void peks_bf_peks(const EC_PAIRING p, const char *keyword, const EC_POINT G, const EC_POINT A_pub, EC_POINT C1, Element C2){

    EC_POINT q_id;
	point_init(q_id, p->g1);

    //q_ID = H1(keyword);
	point_map_to_point(q_id, keyword, sizeof(keyword), 128);	

    //Random r
	gmp_randstate_t state;
	gmp_randinit_default(state);
	gmp_randseed_ui(state, (unsigned long)time(NULL));
	
    mpz_t order;
    mpz_init(order);
    mpz_set(order, *pairing_get_order(p));

    mpz_t r;
    mpz_init(r);
    mpz_urandomm(r, state, order);  //Zp上の値alphaをランダムに選択


    //C1 = rG
	point_mul(C1, r, G);

    //t = e(q_ID, r*A_pub)
    //C2 = H2(t) 
    EC_POINT tmpPoint;
    point_init(tmpPoint, p->g2);
    point_mul(tmpPoint, r, A_pub);
	pairing_map(C2, q_id, tmpPoint, p);
    

}

void peks_bf_peks_forECPoint(const EC_PAIRING p, const EC_POINT P_keyword, const EC_POINT G, const EC_POINT A_pub, EC_POINT C1, Element C2){

    //Random r
	gmp_randstate_t state;
	gmp_randinit_default(state);
	gmp_randseed_ui(state, (unsigned long)time(NULL));
	
    mpz_t order;
    mpz_init(order);
    mpz_set(order, *pairing_get_order(p));

    mpz_t r;
    mpz_init(r);
    mpz_urandomm(r, state, order);  //Zp上の値alphaをランダムに選択


    //C1 = rG
	point_mul(C1, r, G);

    //t = e(q_ID, r*A_pub)
    //C2 = H2(t) 
    EC_POINT tmpPoint;
    point_init(tmpPoint, p->g2);
    point_mul(tmpPoint, r, A_pub);
	pairing_map(C2, P_keyword, tmpPoint, p);

}


void peks_bf_trapdoor(const EC_PAIRING p, const char *keyword, EC_POINT Tw, const mpz_t alpha){

    EC_POINT q_id;
	point_init(q_id, p->g1);

    //q_ID = H1(keyword);
	point_map_to_point(q_id, keyword, sizeof(keyword), 128);	
	point_mul(Tw, alpha, q_id);
}

void peks_bf_trapdoor_forECPoint(const EC_POINT P_keyword, EC_POINT Tw, const mpz_t alpha){
	point_mul(Tw, alpha, P_keyword);
}


int peks_bf_test(const EC_PAIRING p, const EC_POINT Tw, const EC_POINT C1, const Element C2){

    Element tmpVal;
    element_init(tmpVal, p->g3);
   	pairing_map(tmpVal, Tw, C1, p);

    if(element_cmp(tmpVal, C2)==0)
        return 1;//Yes
    else
        return 0;//No

}


