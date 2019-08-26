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

    //KeyGen
    //void peks_bf_keygen(mpz_t alpha, EC_POINT G, EC_POINT A_pub);
    mpz_t alpha;
    mpz_init(alpha);
    EC_POINT G, A_pub;
    point_init(G, p->g2);
    point_init(A_pub, p->g2);

    peks_bf_keygen(p, alpha, G, A_pub);

    //PEKS
    //void peks_bf_peks(const char *keyword, const EC_POINT G, const EC_POINT A_pub, EC_POINT C1, Element C2);
    EC_POINT C1;
    Element C2;
    point_init(C1, p->g2);
    element_init(C2, p->g3);

    peks_bf_peks(p, "test", G, A_pub, C1, C2);    

    //Trapdoor
    //void peks_bf_trapdoor(const char *keyword, EC_POINT Tw, const mpz_t alpha);
    EC_POINT Tw1, Tw2;
    point_init(Tw1, p->g1);
    point_init(Tw2, p->g1);

    peks_bf_trapdoor(p, "test", Tw1, alpha);
    peks_bf_trapdoor(p, "kanaoka", Tw2, alpha);

    //Test
    //int peks_bf_test(const EC_POINT Tw, const EC_POINT C1, const Element C2);
    int result1 = peks_bf_test(p, Tw1, C1, C2);
    int result2 = peks_bf_test(p, Tw2, C1, C2);

    printf("%d\n", result1);
    printf("%d\n", result2);

    printf("==============\n");
    //PEKS for EC_POINT
    EC_POINT W1, W2;
	point_init(W1, p->g1);
	point_init(W2, p->g1);
	point_map_to_point(W1, "test", sizeof("test"), 128);	
	point_map_to_point(W2, "kanaoka", sizeof("kanaoka"), 128);	

    EC_POINT C1r;
    Element C2r;
    point_init(C1r, p->g2);
    element_init(C2r, p->g3);

    peks_bf_peks_forECPoint(p, W1, G, A_pub, C1r, C2r);    

    //Trapdoor for EC_POINT
    EC_POINT Tw1r, Tw2r;
    point_init(Tw1r, p->g1);
    point_init(Tw2r, p->g1);

    peks_bf_trapdoor_forECPoint(W1, Tw1r, alpha);
    peks_bf_trapdoor_forECPoint(W2, Tw2r, alpha);

    //Test
    int result3 = peks_bf_test(p, Tw1r, C1r, C2r);
    int result4 = peks_bf_test(p, Tw2r, C1r, C2r);
    printf("%d\n", result3);
    printf("%d\n", result4);

}