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

    int loop=100;
    int ll;
    long total = 0;


    //init
    mpz_t x0;
    mpz_t x1;
    mpz_t x2;
    mpz_t x3;
    mpz_t x4;

    mpz_t y0;
    mpz_t y1;
    mpz_t y2;
    mpz_t y3;
    mpz_t y4;

    mpz_t b0;
    mpz_t b1;
    mpz_t b2;
    mpz_t b3;
    mpz_t b4;

    mpz_t bp0;
    mpz_t bp1;
    mpz_t bp2;
    mpz_t bp3;
    mpz_t bp4;

    mpz_t alpha;

    Element z;
    EC_POINT G1;
    EC_POINT G1alpha;
    EC_POINT G2;
    EC_POINT u1;
    EC_POINT w1;
    EC_POINT h1;
    EC_POINT v1;

    EC_POINT Bx0;
    EC_POINT Bx1;
    EC_POINT Bx2;
    EC_POINT Bx3;
    EC_POINT Bx4;
    
    EC_POINT By0;
    EC_POINT By1;
    EC_POINT By2;
    EC_POINT By3;
    EC_POINT By4;

    total=0;
    for(ll=0;ll<loop;ll++){


    mpz_init(x0);
    mpz_init(x1);
    mpz_init(x2);
    mpz_init(x3);
    mpz_init(x4);

    mpz_init(y0);
    mpz_init(y1);
    mpz_init(y2);
    mpz_init(y3);
    mpz_init(y4);

    mpz_init(b0);
    mpz_init(b1);
    mpz_init(b2);
    mpz_init(b3);
    mpz_init(b4);

    mpz_init(bp0);
    mpz_init(bp1);
    mpz_init(bp2);
    mpz_init(bp3);
    mpz_init(bp4);

    mpz_init(alpha);

    element_init(z, p->g3);

    point_init(G1, p->g1);
    point_init(G1alpha, p->g1);
    point_init(G2, p->g2);

    point_init(u1, p->g1);
    point_init(w1, p->g1);
    point_init(h1, p->g1);
    point_init(v1, p->g1);

    point_init(Bx0, p->g2);
    point_init(Bx1, p->g2);
    point_init(Bx2, p->g2);
    point_init(Bx3, p->g2);
    point_init(Bx4, p->g2);
    
    point_init(By0, p->g2);
    point_init(By1, p->g2);
    point_init(By2, p->g2);
    point_init(By3, p->g2);
    point_init(By4, p->g2);

        start = clock();
    mike_init(p,x0,x1,x2,x3,x4,y0,y1,y2,y3,y4,b0,b1,b2,b3,b4,bp0,bp1,bp2,bp3,bp4,alpha,z,G1,G1alpha,G2,u1,w1,h1,v1,Bx0,Bx1,Bx2,Bx3,Bx4,By0,By1,By2,By3,By4);
        end=clock();
        total+=(end-start);
    }
    extime = (double)(total*1000.0)/(CLOCKS_PER_SEC*loop);
    printf("exec init: %lf (%ld)\n", extime, total);

    //UpdGen
    EC_POINT D1;
    EC_POINT D2;
    EC_POINT Dx1;
    EC_POINT Dx2;
    EC_POINT Dxp1;
    EC_POINT Dxp2;
    EC_POINT Dxpp1;
    EC_POINT Dxpp2;
    EC_POINT Dy1;
    EC_POINT Dy2;
    EC_POINT Dyp1;
    EC_POINT Dyp2;
    EC_POINT Dypp1;
    EC_POINT Dypp2;

    total=0;
    for(ll=0;ll<loop;ll++){

    point_init(D1, p->g2);
    point_init(D2, p->g2);
    point_init(Dx1, p->g2);
    point_init(Dx2, p->g2);
    point_init(Dxp1, p->g2);
    point_init(Dxp2, p->g2);
    point_init(Dxpp1, p->g2);
    point_init(Dxpp2, p->g2);
    point_init(Dy1, p->g2);
    point_init(Dy2, p->g2);
    point_init(Dyp1, p->g2);
    point_init(Dyp2, p->g2);
    point_init(Dypp1, p->g2);
    point_init(Dypp2, p->g2);

        start = clock();
    mike_updgen(p,1,G2,Bx0,Bx1,Bx2,Bx3,Bx4,By0,By1,By2,By3,By4,D1,D2,Dx1,Dx2,Dxp1,Dxp2,Dxpp1,Dxpp2,Dy1,Dy2,Dyp1,Dyp2,Dypp1,Dypp2);
        end=clock();
        total+=(end-start);
    }
    extime = (double)(total*1000.0)/(CLOCKS_PER_SEC*loop);
    printf("exec updgen: %lf (%ld)\n", extime, total);


    //mkupd
    EC_POINT LD1;
    EC_POINT LD2;
    EC_POINT LDx1;
    EC_POINT LDx2;
    EC_POINT LDxp1;
    EC_POINT LDxp2;
    EC_POINT LDxpp1;
    EC_POINT LDxpp2;
    EC_POINT LDy1;
    EC_POINT LDy2;
    EC_POINT LDyp1;
    EC_POINT LDyp2;
    EC_POINT LDypp1;
    EC_POINT LDypp2;
    
    total=0;
    for(ll=0;ll<loop;ll++){
    point_init(LD1, p->g2);
    point_init(LD2, p->g2);
    point_init(LDx1, p->g2);
    point_init(LDx2, p->g2);
    point_init(LDxp1, p->g2);
    point_init(LDxp2, p->g2);
    point_init(LDxpp1, p->g2);
    point_init(LDxpp2, p->g2);
    point_init(LDy1, p->g2);
    point_init(LDy2, p->g2);
    point_init(LDyp1, p->g2);
    point_init(LDyp2, p->g2);
    point_init(LDypp1, p->g2);
    point_init(LDypp2, p->g2);

        start = clock();
    mike_mkupd(p,1,G2,b0,b1,b2,b3,b4,bp0,bp1,bp2,bp3,bp4,D1,D2,Dx1,Dx2,Dxp1,Dxp2,Dxpp1,Dxpp2,Dy1,Dy2,Dyp1,Dyp2,Dypp1,Dypp2,LD1,LD2,LDx1,LDx2,LDxp1,LDxp2,LDxpp1,LDxpp2,LDy1,LDy2,LDyp1,LDyp2,LDypp1,LDypp2);
        end=clock();
        total+=(end-start);
    }
    extime = (double)(total*1000.0)/(CLOCKS_PER_SEC*loop);
    printf("exec mkupd: %lf (%ld)\n", extime, total);


    //kg
    EC_POINT K;
    EC_POINT Kx;
    EC_POINT Kpx;
    EC_POINT Ky;
    EC_POINT Kpy;

	gmp_randstate_t state;
	gmp_randinit_default(state);
	gmp_randseed_ui(state, (unsigned long)time(NULL));
	
    mpz_t order;
    mpz_init(order);
    mpz_set(order, *pairing_get_order(p));

    mpz_t I;

    total=0;
    for(ll=0;ll<loop;ll++){
    point_init(K, p->g2);
    point_init(Kx, p->g2);
    point_init(Kpx, p->g2);
    point_init(Ky, p->g2);
    point_init(Kpy, p->g2);
    mpz_init(I);
    mpz_urandomm(I, state, order);  

        start = clock();
    mike_kg(p,I,D1,D2,Dx1,Dx2,Dxp1,Dxp2,Dxpp1,Dxpp2,Dy1,Dy2,Dyp1,Dyp2,Dypp1,Dypp2,K,Kx,Kpx,Ky,Kpy);
        end=clock();
        total+=(end-start);
    }
    extime = (double)(total*1000.0)/(CLOCKS_PER_SEC*loop);
    printf("exec kg: %lf (%ld)\n", extime, total);


    //IBEnc
    Element CM;
    EC_POINT Cx;
    EC_POINT Cy;
    EC_POINT C;
    mpz_t tag;

    total=0;
    for(ll=0;ll<loop;ll++){
    element_init(CM, p->g3);
    point_init(Cx,p->g1);
    point_init(Cy,p->g1);
    point_init(C,p->g1);
    mpz_init(tag);

    Element m;
    element_init(m, p->g3);
    element_random(m);

        start = clock();
    mike_ibenc(p,I,1,m,z,G1,G1alpha,u1,w1,h1,v1,CM,Cx,Cy,C,tag);
        end=clock();
        total+=(end-start);
    }
    extime = (double)(total*1000.0)/(CLOCKS_PER_SEC*loop);
    printf("exec ibenc: %lf (%ld)\n", extime, total);


    //IBDec
    Element mp;
    total=0;
    for(ll=0;ll<loop;ll++){
    element_init(mp,p->g3);
        start = clock();
    mike_ibdec(p,K,Kx,Kpx,Ky,Kpy,CM,Cx,Cy,C,tag,mp);
        end=clock();
        total+=(end-start);
    }
    extime = (double)(total*1000.0)/(CLOCKS_PER_SEC*loop);
    printf("exec ibdec: %lf (%ld)\n", extime, total);


}