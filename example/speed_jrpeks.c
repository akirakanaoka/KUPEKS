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

	gmp_randstate_t state;
	gmp_randinit_default(state);
	gmp_randseed_ui(state, (unsigned long)time(NULL));
	
    mpz_t order;
    mpz_init(order);
    mpz_set(order, *pairing_get_order(p));


    int loop = 100;
    int i;
    total =0;

    //keygen
    total =0;
    mpz_t x,y,x0,y0,x1,y1,x2,y2;
    EC_POINT G1,G2,G1alpha;
    EC_POINT u1, h1, w1;
    Element z;
    for(i=0;i<loop;i++){
        mpz_init(x);
        mpz_init(y);
        mpz_init(x0);
        mpz_init(y0);
        mpz_init(x1);
        mpz_init(y1);
        mpz_init(x2);
        mpz_init(y2);

        point_init(G1, p->g1);
        point_init(G2, p->g2);
        point_init(G1alpha, p->g1);

        element_init(z, p->g3);

        point_init(u1, p->g1);
        point_init(h1, p->g1);
        point_init(w1, p->g1);

        start = clock();
        peks_jr_keygen(p,x,y,x0,y0,x1,y1,x2,y2,G1,G2,G1alpha,z,u1,h1,w1);    

        end=clock();
        total+=(end-start);
    }
    extime = (double)(total*1000.0)/(CLOCKS_PER_SEC*loop);
    printf("exec keygen: %lf (%ld)\n", extime, total);

    //peks
    total =0;
    mpz_t I;
    mpz_t tag;
    Element Cm;
    EC_POINT Cx, Cy, C;
    for(i=0;i<loop;i++){
        mpz_init(I);
        mpz_urandomm(I, state, order);  

        mpz_init(tag);
        element_init(Cm, p->g3);
        point_init(Cx, p->g1);
        point_init(Cy, p->g1);
        point_init(C, p->g1);

        start = clock();
        peks_jr_peks(p,I,G1,G1alpha,z,u1,h1,w1,tag,Cm,Cx,Cy,C);
        end=clock();
        total+=(end-start);
    }
    extime = (double)(total*1000.0)/(CLOCKS_PER_SEC*loop);
    printf("exec peks: %lf (%ld)\n", extime, total);

    //trapdoor
    mpz_t I_d;
    EC_POINT D, Dx, Dxp, Dy, Dyp;
    total =0;
    for(i=0;i<loop;i++){
        mpz_init(I_d);
        mpz_urandomm(I_d, state, order);  
        point_init(D, p->g2);
        point_init(Dx, p->g2);
        point_init(Dxp, p->g2);
        point_init(Dy, p->g2);
        point_init(Dyp, p->g2);

        start = clock();
        peks_jr_trapdoor(p,G2,x,x0,x1,x2,y,y0,y1,y2,I,D,Dx,Dxp,Dy,Dyp);
        end=clock();
        total+=(end-start);
    }
    extime = (double)(total*1000.0)/(CLOCKS_PER_SEC*loop);
    printf("exec trapdoor: %lf (%ld)\n", extime, total);

    //test
    total =0;
    for(i=0;i<loop;i++){

        start = clock();
        int result1 = peks_jr_test(p,D,Dx,Dxp,Dy,Dyp,tag,Cm,Cx,Cy,C);
        end=clock();
        total+=(end-start);
    }
    extime = (double)(total*1000.0)/(CLOCKS_PER_SEC*loop);
    printf("exec test: %lf (%ld)\n", extime, total);


}