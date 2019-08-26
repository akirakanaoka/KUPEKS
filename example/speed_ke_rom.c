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
    EC_POINT P,G,ek1,mpk1;
    mpz_t dk1,msk1;
    total =0;
    for(aaaa=0;aaaa<loop;aaaa++){
        point_init(P, p->g1);
        point_init(G, p->g2);
        point_init(ek1, p->g1);
        mpz_init(dk1);
        point_init(mpk1, p->g2);
        mpz_init(msk1);

        start = clock();
        ke_rom_setup(p,P,G,ek1,dk1,mpk1,msk1);
        end=clock();
        total+=(end-start);
    }
    extime = (double)(total*1000.0)/(CLOCKS_PER_SEC*loop);
    printf("exec setup: %lf (%ld)\n", extime, total);

    //upd
    EC_POINT ek2,mpk2;
    mpz_t dk2,msk2,rk2;
    total =0;
    for(aaaa=0;aaaa<loop;aaaa++){
        point_init(ek2, p->g1);
        mpz_init(dk2);
        point_init(mpk2, p->g2);
        mpz_init(msk2);
        mpz_init(rk2);

        start = clock();
        ke_rom_upd(p,P,G,dk1,ek2,dk2,mpk2,msk2,rk2);
        end=clock();
        total+=(end-start);
    }
    extime = (double)(total*1000.0)/(CLOCKS_PER_SEC*loop);
    printf("exec upd: %lf (%ld)\n", extime, total);

    //enc
    EC_POINT cti1_10, cti2_10, ci1_10;
    Element ci2_10;

    EC_POINT W1;
    total =0;
    for(aaaa=0;aaaa<loop;aaaa++){
        point_init(cti1_10, p->g1);
        point_init(cti2_10, p->g1);
        point_init(ci1_10, p->g2);
        element_init(ci2_10, p->g3);

        point_init(W1, p->g1);
        point_map_to_point(W1, "test", sizeof("test"), 128);	
        start = clock();
        ke_rom_enc(p,W1, P, G, ek1, mpk1, cti1_10, cti2_10, ci1_10, ci2_10);
        end=clock();
        total+=(end-start);
    }
    extime = (double)(total*1000.0)/(CLOCKS_PER_SEC*loop);
    printf("exec enc: %lf (%ld)\n", extime, total);

    //reenc
    EC_POINT cti1_11, cti2_11, ci1_11;
    Element ci2_11;
    total =0;
    for(aaaa=0;aaaa<loop;aaaa++){
        point_init(cti1_11, p->g1);
        point_init(cti2_11, p->g1);
        point_init(ci1_11, p->g2);
        element_init(ci2_11, p->g3);

        start = clock();
        ke_rom_reenc(p,i,j,k,P,G,ek2,mpk2,dk2,cti1_10,cti2_10,ci1_10,ci2_10,cti1_11,cti2_11,ci1_11,ci2_11);
        end=clock();
        total+=(end-start);
    }
    extime = (double)(total*1000.0)/(CLOCKS_PER_SEC*loop);
    printf("exec reenc: %lf (%ld)\n", extime, total);

    //trapdoor
    EC_POINT Tw1;
    total =0;
    for(aaaa=0;aaaa<loop;aaaa++){
        point_init(Tw1, p->g1);

        start = clock();
        ke_rom_trapdoor(p,msk1, W1, Tw1);
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
        int result1 = ke_rom_test(p,i,j,k,Tw1,ci1_10,ci2_10);
        end=clock();
        total+=(end-start);
    }
    extime = (double)(total*1000.0)/(CLOCKS_PER_SEC*loop);
    printf("exec test: %lf (%ld)\n", extime, total);

}