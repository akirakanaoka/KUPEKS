#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <tepla/ec.h>

int main(void){
    clock_t start, end;
    long total;
    double extime;
    int len;
    EC_PAIRING p;
    pairing_init(p, "ECBN254a");


    int loop = 100;
    int i;
    total =0;


    //Keygen
    mpz_t alpha;
    mpz_init(alpha);
    EC_POINT G, A_pub;
    total =0;
    for(i=0;i<loop;i++){
        point_init(G, p->g2);
        point_init(A_pub, p->g2);

        start = clock();
        peks_bf_keygen(p, alpha, G, A_pub);
        end=clock();
        total+=(end-start);
    }
    extime = (double)(total*1000.0)/(CLOCKS_PER_SEC*loop);
    printf("exec KeyGen: %lf (%ld)\n", extime, total);

    //peks
    EC_POINT C1;
    Element C2;

    total =0;
    for(i=0;i<loop;i++){
        point_init(C1, p->g2);
        element_init(C2, p->g3);

        start = clock();
        peks_bf_peks(p, "test", G, A_pub, C1, C2);    
        end=clock();
        total+=(end-start);
    }
    extime = (double)(total*1000.0)/(CLOCKS_PER_SEC*loop);
    printf("exec peks: %lf (%ld)\n", extime, total);

    //trapdoor
    EC_POINT Tw1;
    total =0;

    for(i=0;i<loop;i++){
        point_init(Tw1, p->g1);

        start = clock();
        peks_bf_trapdoor(p, "test", Tw1, alpha);
        end=clock();
        total+=(end-start);
    }
    extime = (double)(total*1000.0)/(CLOCKS_PER_SEC*loop);
    printf("exec trapdoor: %lf (%ld)\n", extime, total);

    //test
    total =0;
    for(i=0;i<loop;i++){

        start = clock();
        int result1 = peks_bf_test(p, Tw1, C1, C2);
        end=clock();
        total+=(end-start);
    }
    extime = (double)(total*1000.0)/(CLOCKS_PER_SEC*loop);
    printf("exec test: %lf (%ld)\n", extime, total);

    //peks for ec_point
    EC_POINT W1;
	point_init(W1, p->g1);
	point_map_to_point(W1, "test", sizeof("test"), 128);	

    EC_POINT C1r;
    Element C2r;
    total =0;

    for(i=0;i<loop;i++){
        point_init(C1r, p->g2);
        element_init(C2r, p->g3);

        start = clock();
        peks_bf_peks_forECPoint(p, W1, G, A_pub, C1r, C2r);    
        end=clock();
        total+=(end-start);
    }
    extime = (double)(total*1000.0)/(CLOCKS_PER_SEC*loop);
    printf("exec peks_forECPoint: %lf (%ld)\n", extime, total);

    //trapdoor for EC_POINT
    EC_POINT Tw1r;
    total =0;
    for(i=0;i<loop;i++){
        point_init(Tw1r, p->g1);

        start = clock();
        peks_bf_trapdoor_forECPoint(W1, Tw1r, alpha);
        end=clock();
        total+=(end-start);
    }
    extime = (double)(total*1000.0)/(CLOCKS_PER_SEC*loop);
    printf("exec trapdoor_forECPoint: %lf (%ld)\n", extime, total);


}