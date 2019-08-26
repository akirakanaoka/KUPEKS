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

    //init
    mpz_t x,y,x0,y0,x1,y1,x2,y2;

    EC_POINT G1,G2,G1alpha;
    EC_POINT u1, h1, w1;
    Element z;



    total =0;
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
        jr_init(p,x,y,x0,y0,x1,y1,x2,y2,G1,G2,G1alpha,z,u1,h1,w1);
        end=clock();
        total+=(end-start);
    }
    extime = (double)(total*1000.0)/(CLOCKS_PER_SEC*loop);
    printf("exec init: %lf (%ld)\n", extime, total);

    //ext
    EC_POINT D, Dx, Dxp, Dy, Dyp;
    mpz_t I;
    mpz_t order;
	gmp_randstate_t state;

    total =0;
    for(i=0;i<loop;i++){

        gmp_randinit_default(state);
        gmp_randseed_ui(state, (unsigned long)time(NULL));

        mpz_init(order);
        mpz_set(order, *pairing_get_order(p));


        mpz_init(I);
        mpz_urandomm(I, state, order);  

        point_init(D, p->g2);
        point_init(Dx, p->g2);
        point_init(Dxp, p->g2);
        point_init(Dy, p->g2);
        point_init(Dyp, p->g2);

        start = clock();
        jr_ext(p,G2,x,x0,x1,x2,y,y0,y1,y2,I,D,Dx,Dxp,Dy,Dyp);
        end=clock();
        total+=(end-start);
    }
    extime = (double)(total*1000.0)/(CLOCKS_PER_SEC*loop);
    printf("exec Ext: %lf (%ld)\n", extime, total);

    //enc
    total =0;
    Element m;
    mpz_t tag;
    Element Cm;
    EC_POINT Cx, Cy, C;
    for(i=0;i<loop;i++){
        element_init(m, p->g3);
        element_random(m);

        mpz_init(tag);
        element_init(Cm, p->g3);
        point_init(Cx, p->g1);
        point_init(Cy, p->g1);
        point_init(C, p->g1);
        start = clock();
        jr_enc(p,I,m,G1,G1alpha,z,u1,h1,w1,tag,Cm,Cx,Cy,C);
        end=clock();
        total+=(end-start);
    }
    extime = (double)(total*1000.0)/(CLOCKS_PER_SEC*loop);
    printf("exec Enc: %lf (%ld)\n", extime, total);

    //dec
    Element mp;
    total =0;
    for(i=0;i<loop;i++){
        element_init(mp, p->g3);

        start = clock();
        jr_dec(p,D,Dx,Dxp,Dy,Dyp,tag,Cm,Cx,Cy,C,mp);
        end=clock();
        total+=(end-start);
    }
    extime = (double)(total*1000.0)/(CLOCKS_PER_SEC*loop);
    printf("exec Decrypt: %lf (%ld)\n", extime, total);

}