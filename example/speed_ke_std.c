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
    int aaaa;
    total =0;

    int i=1;
    int j=1;
    int k=0;

    //setup
    EC_POINT P, ek1;
    mpz_t dk1;
    mpz_t x_1,y_1,x0_1,y0_1,x1_1,y1_1,x2_1,y2_1;

    EC_POINT G1_1,G2_1,G1alpha_1;
    EC_POINT u1_1, h1_1, w1_1;
    Element z_1;

    total =0;
    for(aaaa=0;aaaa<loop;aaaa++){

        point_init(P, p->g1);
        point_init(ek1, p->g1);
        mpz_init(dk1);

        mpz_init(x_1);
        mpz_init(y_1);
        mpz_init(x0_1);
        mpz_init(y0_1);
        mpz_init(x1_1);
        mpz_init(y1_1);
        mpz_init(x2_1);
        mpz_init(y2_1);

        point_init(G1_1, p->g1);
        point_init(G2_1, p->g2);
        point_init(G1alpha_1, p->g1);

        element_init(z_1, p->g3);

        point_init(u1_1, p->g1);
        point_init(h1_1, p->g1);
        point_init(w1_1, p->g1);

        start = clock();
        ke_std_setup(p,P,ek1,dk1,x_1,y_1,x0_1,y0_1,x1_1,y1_1,x2_1,y2_1,G1_1,G2_1,G1alpha_1,z_1,u1_1,h1_1,w1_1);
        end=clock();
        total+=(end-start);
    }
    extime = (double)(total*1000.0)/(CLOCKS_PER_SEC*loop);
    printf("exec setup: %lf (%ld)\n", extime, total);

    //upd
    EC_POINT ek2;
    mpz_t dk2,rk2;
    mpz_t x_2,y_2,x0_2,y0_2,x1_2,y1_2,x2_2,y2_2;
    EC_POINT G1_2,G2_2,G1alpha_2;
    EC_POINT u1_2, h1_2, w1_2;
    Element z_2;

    total =0;
    for(aaaa=0;aaaa<loop;aaaa++){
        point_init(ek2, p->g1);
        mpz_init(dk2);
        mpz_init(rk2);

        mpz_init(x_2);
        mpz_init(y_2);
        mpz_init(x0_2);
        mpz_init(y0_2);
        mpz_init(x1_2);
        mpz_init(y1_2);
        mpz_init(x2_2);
        mpz_init(y2_2);

        point_init(G1_2, p->g1);
        point_init(G2_2, p->g2);
        point_init(G1alpha_2, p->g1);

        element_init(z_2, p->g3);

        point_init(u1_2, p->g1);
        point_init(h1_2, p->g1);
        point_init(w1_2, p->g1);

        i=1; j=1; k=0;
        start = clock();
        ke_std_upd(p,P, dk1,ek2,dk2,x_2,y_2,x0_2,x0_2,x1_2,y1_2,x2_2,y2_2,G1_2,G2_2,G1alpha_2,z_2,u1_2,h1_2,w1_2,rk2);
        end=clock();
        total+=(end-start);
    }
    extime = (double)(total*1000.0)/(CLOCKS_PER_SEC*loop);
    printf("exec update: %lf (%ld)\n", extime, total);

    //enc
    total =0;
    EC_POINT W1;
    EC_POINT ct1_10, ct2_10;
    mpz_t tag_10;
    Element Cm_10;
    EC_POINT Cx_10, Cy_10, C_10;
    for(aaaa=0;aaaa<loop;aaaa++){
        point_init(W1, p->g1);
        point_map_to_point(W1, "test", sizeof("test"), 128);	

        point_init(ct1_10, p->g1);
        point_init(ct2_10, p->g1);

        mpz_init(tag_10);
        element_init(Cm_10, p->g3);
        point_init(Cx_10, p->g1);
        point_init(Cy_10, p->g1);
        point_init(C_10, p->g1);

        start = clock();
        ke_std_enc(p,W1,P,ek1,ct1_10,ct2_10,G1_1,G1alpha_1,z_1,u1_1,h1_1,w1_1,tag_10,Cm_10,Cx_10,Cy_10,C_10);
        end=clock();
        total+=(end-start);
    }
    extime = (double)(total*1000.0)/(CLOCKS_PER_SEC*loop);
    printf("exec enc: %lf (%ld)\n", extime, total);

    //reenc
    EC_POINT ct1_11, ct2_11;
    mpz_t tag_11;
    Element Cm_11;
    EC_POINT Cx_11, Cy_11, C_11;
    total =0;
    for(aaaa=0;aaaa<loop;aaaa++){
        point_init(ct1_11, p->g1);
        point_init(ct2_11, p->g1);

        mpz_init(tag_11);
        element_init(Cm_11, p->g3);
        point_init(Cx_11, p->g1);
        point_init(Cy_11, p->g1);
        point_init(C_11, p->g1);
        i=1; j=1; k=0;
        start = clock();
        ke_std_reenc(p,i,j,k,P,ek2,rk2,ct1_10,ct2_10,G1_2,G1alpha_2,z_2,u1_2,h1_2,w1_2,ct1_11,ct2_11,tag_11,Cm_11,Cx_11,Cy_11,C_11);
        end=clock();
        total+=(end-start);
    }
    extime = (double)(total*1000.0)/(CLOCKS_PER_SEC*loop);
    printf("exec reenc: %lf (%ld)\n", extime, total);

    //trapdoor
    EC_POINT D_1, Dx_1, Dxp_1, Dy_1, Dyp_1;
    total =0;
    for(aaaa=0;aaaa<loop;aaaa++){
        point_init(D_1, p->g2);
        point_init(Dx_1, p->g2);
        point_init(Dxp_1, p->g2);
        point_init(Dy_1, p->g2);
        point_init(Dyp_1, p->g2);

        start = clock();
        ke_std_trapdoor(p,W1,G2_1,x_1,x0_1,x1_1,x2_1,y_1,y0_1,y1_1,y2_1,D_1,Dx_1,Dxp_1,Dy_1,Dyp_1);
        end=clock();
        total+=(end-start);
    }
    extime = (double)(total*1000.0)/(CLOCKS_PER_SEC*loop);
    printf("exec trapdoor: %lf (%ld)\n", extime, total);
    
    //test    
    total =0;
    for(aaaa=0;aaaa<loop;aaaa++){
        i=1; j=1; k=0;
        start = clock();
        int result1 = ke_std_test(p,i,j,k,D_1,Dx_1,Dxp_1,Dy_1,Dyp_1,tag_10,Cm_10,Cx_10,Cy_10,C_10);
        end=clock();
        total+=(end-start);
    }
    extime = (double)(total*1000.0)/(CLOCKS_PER_SEC*loop);
    printf("exec test: %lf (%ld)\n", extime, total);

}