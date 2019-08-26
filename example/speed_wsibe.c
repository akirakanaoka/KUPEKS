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

    int l = 128;
    int i;

    int loop=100;
    int ll;
    long total = 0;

    //ws_gen
	gmp_randstate_t state;
	gmp_randinit_default(state);
	gmp_randseed_ui(state, (unsigned long)time(NULL));
	
    mpz_t order;
    mpz_init(order);
    mpz_set(order, *pairing_get_order(p));

    mpz_t I;
    mpz_init(I);
    mpz_urandomm(I, state, order);  

    mpz_t x0, y0, x2, y2, x3, y3, alpha;
    mpz_t x1[l+1];
    mpz_t y1[l+1];

    EC_POINT G1, G2;
    Element z;
    EC_POINT u1[l+1];
    EC_POINT w1, h1;

    EC_POINT G1alpha, G2x2, G2y2, G2x3, G2y3;
    EC_POINT G2x1[l+1];
    EC_POINT G2y1[l+1];

    mpz_t B;

    EC_POINT R[l];
    EC_POINT D1, D1p, D2, D2p, D3;
    EC_POINT K[l];
    EC_POINT Kp[l];

    for(ll=0;ll<loop;ll++){
        mpz_init(x0);
        mpz_init(y0);
        mpz_init(x2);
        mpz_init(y2);
        mpz_init(x3);
        mpz_init(y3);

        for(i=0;i<l+1;i++){
            mpz_init(x1[i]);
            mpz_init(y1[i]);
        }

        mpz_init(alpha);

        point_init(G1, p->g1);
        point_init(G2, p->g2);

        element_init(z, p->g3);


        for(i=0;i<l+1;i++){
            point_init(u1[i], p->g1);
        }
        point_init(w1, p->g1);
        point_init(h1, p->g1);
        
        point_init(G1alpha, p->g1);
        point_init(G2x2, p->g2);
        point_init(G2y2, p->g2);
        point_init(G2x3, p->g2);
        point_init(G2y3, p->g2);


        for(i=0;i<l+1;i++){
            point_init(G2x1[i], p->g2);
            point_init(G2y1[i], p->g2);
        }

        //mpz_t B, EC_POINT *R, EC_POINT D1, EC_POINT D1p, EC_POINT D2, EC_POINT D2p, EC_POINT D3, EC_POINT *K, EC_POINT *Kp)

        mpz_init(B);


        point_init(D1, p->g2);
        point_init(D1p, p->g2);
        point_init(D2, p->g2);
        point_init(D2p, p->g2);
        point_init(D3, p->g2);

        for(i=0;i<l;i++){
            point_init(R[i], p->g2);
            point_init(K[i], p->g2);
            point_init(Kp[i], p->g2);
        }

        start = clock();
        ws_gen(p,l,x0,y0,x1,y1,x2,y2,x3,y3,alpha,G1,G2,z,u1,u1,h1,G1alpha,G2x1,G2y1,G2x2,G2y2,G2x3,G2y3,I,B,R,D1,D1p,D2,D2p,D3,K,Kp);
        end=clock();
        total+=(end-start);
    }
    extime = (double)(total*1000.0)/(CLOCKS_PER_SEC*loop);
    printf("exec ws_gen: %lf (%ld)\n", extime, total);


    //ws_deltagen
    EC_POINT d1h;
    EC_POINT d1ph;
    EC_POINT d2h;
    EC_POINT d2ph;
    EC_POINT d3h;
    EC_POINT kh[l-1];
    EC_POINT kph[l-1];


    total=0;
    for(ll=0;ll<loop;ll++){
    point_init(d1h,p->g2);
    point_init(d1ph,p->g2);
    point_init(d2h,p->g2);
    point_init(d2ph,p->g2);
    point_init(d3h,p->g2);
    for(i=0;i<l-1;i++){
        point_init(kh[i], p->g2);
        point_init(kph[i], p->g2);
    }

        start = clock();
    ws_deltagen(p,l,1,I,G2,G2x1,G2x2,G2x3,G2y1,G2y2,G2y3,D1,D1p,D2,D2p,D3,K,Kp,d1h,d1ph,d2h,d2ph,d3h,kh,kph);
        end=clock();
        total+=(end-start);
    }
    extime = (double)(total*1000.0)/(CLOCKS_PER_SEC*loop);
    printf("exec deltagen: %lf (%ld)\n", extime, total);

    //ws_upd
    EC_POINT nD1;
    EC_POINT nD1p;
    EC_POINT nD2;
    EC_POINT nD2p;
    EC_POINT nD3;
    EC_POINT nK[l];
    EC_POINT nKp[l];

    total=0;
    for(ll=0;ll<loop;ll++){
    point_init(nD1,p->g2);
    point_init(nD1p,p->g2);
    point_init(nD2,p->g2);
    point_init(nD2p,p->g2);
    point_init(nD3,p->g2);
    for(i=0;i<l;i++){
        point_init(nK[i], p->g2);
        point_init(nKp[i], p->g2);
    }

        start = clock();
    ws_upd(1,R[1],d1h,d1ph,d2h,d2ph,d3h,kh,kph,nD1,nD1p,nD2,nD2p,nD3,nK,nKp);
        end=clock();
        total+=(end-start);
    }
    extime = (double)(total*1000.0)/(CLOCKS_PER_SEC*loop);
    printf("exec upd: %lf (%ld)\n", extime, total);

    //ws_enc
    Element C0;
    EC_POINT C1;
    EC_POINT C2;
    EC_POINT C3;
    mpz_t tag;

    total=0;
    for(ll=0;ll<loop;ll++){
    element_init(C0, p->g3);
    point_init(C1, p->g1);
    point_init(C2, p->g1);
    point_init(C3, p->g1);
    mpz_init(tag);

    Element m;
    element_init(m, p->g3);
    element_random(m);

        start = clock();
    ws_enc(p,l,m,z,G1,G1alpha,u1,w1,h1,I,C0,C1,C2,C3,tag);
        end=clock();
        total+=(end-start);
    }
    extime = (double)(total*1000.0)/(CLOCKS_PER_SEC*loop);
    printf("exec enc: %lf (%ld)\n", extime, total);

    //ws_dec
    Element mp;
    total=0;
    for(ll=0;ll<loop;ll++){
    element_init(mp,p->g3);

        start = clock();
    ws_dec(p,C0,C1,C2,C3,tag,D1,D1p,D2,D2p,D3,mp);
        end=clock();
        total+=(end-start);
    }
    extime = (double)(total*1000.0)/(CLOCKS_PER_SEC*loop);
    printf("exec enc: %lf (%ld)\n", extime, total);

}