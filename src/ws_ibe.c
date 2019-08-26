#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include <tepla/ec.h>

void ws_gen(const EC_PAIRING p, int const l, mpz_t x0, mpz_t y0, mpz_t *x1, mpz_t *y1, mpz_t x2, mpz_t y2, mpz_t x3, mpz_t y3, mpz_t alpha, 
            EC_POINT G1, EC_POINT G2, Element z, EC_POINT *u1, EC_POINT w1, EC_POINT h1,
            EC_POINT G1alpha, EC_POINT *G2x1, EC_POINT *G2y1, EC_POINT G2x2, EC_POINT G2y2, EC_POINT G2x3, EC_POINT G2y3,
            const mpz_t I, mpz_t B, EC_POINT *R, EC_POINT D1, EC_POINT D1p, EC_POINT D2, EC_POINT D2p, EC_POINT D3, EC_POINT *K, EC_POINT *Kp){


    gmp_randstate_t state;
	gmp_randinit_default(state);
	gmp_randseed_ui(state, (unsigned long)time(NULL));
	
    mpz_t order;
    mpz_init(order);
    mpz_set(order, *pairing_get_order(p));

    //PGen ---------------
    mpz_urandomm(x0, state, order);
    mpz_urandomm(y0, state, order);
    mpz_urandomm(x2, state, order);
    mpz_urandomm(y2, state, order);
    mpz_urandomm(x3, state, order);
    mpz_urandomm(y3, state, order);
    mpz_urandomm(alpha, state, order);

    int i;
    for(i=0;i<l+1;i++){ 
        mpz_urandomm(x1[i], state, order);
        mpz_urandomm(y1[i], state, order);        
    }

	point_random(G1);
	point_random(G2);

    Element tmpVal;
    element_init(tmpVal, p->g3);
   	pairing_map(tmpVal, G1, G2, p);

    mpz_t tmp1,tmp2,tmp3,tmp4,tmp5,tmp6,tmp7,tmp8;

    mpz_init(tmp1);
    mpz_init(tmp2);
    mpz_init(tmp3);
    mpz_init(tmp4);
    mpz_init(tmp5);
    mpz_init(tmp6);
    mpz_init(tmp7);
    mpz_init(tmp8);

    mpz_mul(tmp1, x0,alpha);
    mpz_sub(tmp2, y0, tmp1); //-x0*alpha+y0;
    mpz_mul(tmp3, x2,alpha);
    mpz_sub(tmp4, y2, tmp1); //-x2*alpha+y2;
    mpz_mul(tmp5, x3,alpha);
    mpz_sub(tmp6, y3, tmp1); //-x3*alpha+y3;

    element_pow(z, tmpVal, tmp2);   //z=e(G1,G2)^(-x0*alpha+y0)
	point_mul(w1, tmp4, G1);        //h1=G1^(-x2*alpha+y2)
	point_mul(h1, tmp6, G1);        //w1=G1^(-x3*alpha+y3)


    for(i=0;i<l+1;i++){
        mpz_init(tmp7);
        mpz_init(tmp8);
        mpz_mul(tmp7, x1[i],alpha);
        mpz_sub(tmp8, y1[i], tmp7); //-x1j*alpha+y1j;
        point_mul(u1[i], tmp8, G1); //u1j=G1^(-x1j*alpha+y1j)
    }

    point_mul(G1alpha, alpha, G1);    
    for(i=0;i<l+1;i++){
        point_mul(G2x1[i],x1[i],G2);
        point_mul(G2y1[i],y1[i],G2);
    }
    point_mul(G2x2,x2,G2);
    point_mul(G2y2,y2,G2);
    point_mul(G2x3,x3,G2);
    point_mul(G2y3,y3,G2);

    //Gen ---------------

    mpz_t beta[l];
    for(i=0;i<l;i++){
        mpz_init(beta[i]);
        mpz_urandomm(beta[i], state, order);        
    }

    mpz_t r;
    mpz_init(r);
    mpz_urandomm(r, state, order);

    mpz_t negr;
    mpz_init(negr);
    mpz_neg(negr,r);


    for(i=0;i<l;i++){
        mpz_add(B,B,beta[i]);
    }

    for(i=0;i<l;i++){
        mpz_t negb;
        mpz_init(negb);
        mpz_neg(negb,beta[i]);
        point_mul(R[i],negb,G2);
    }

    point_mul(D1,r,G2y2);
    point_mul(D2,negr,G2x2);

    EC_POINT tmpP1, tmpP2, tmpP3, tmpP4;
    point_init(tmpP1,p->g2);
    point_init(tmpP2,p->g2);
    point_init(tmpP3,p->g2);
    point_init(tmpP4,p->g2);
    point_mul(tmpP1, y0, G2); //g2^y0;
    point_mul(tmpP2, I, G2y1[l]);    //(g2^y1l)^I
    point_add(tmpP3, tmpP2, G2y3); //(g2^y1l)^I * g2^y3
    point_mul(tmpP4, r, tmpP3); //((g2^y1l)^I * g2^y3)^r
    point_add(D1p, tmpP1, tmpP4); //g2^y0 * ((g2^y1l)^I * g2^y3)^r

    EC_POINT tmpP5, tmpP6, tmpP7, tmpP8;

    point_init(tmpP5,p->g2);
    point_init(tmpP6,p->g2);
    point_init(tmpP7,p->g2);
    point_init(tmpP8,p->g2);
    mpz_t negx0;
    mpz_init(negx0);
    mpz_neg(negx0, x0);
    point_mul(tmpP5, negx0, G2); //g2^-x0;
    point_mul(tmpP6, I, G2x1[l]);    //(g2^x1l)^I
    point_add(tmpP7, tmpP6, G2x3); //(g2^x1l)^I * g2^x3
    point_mul(tmpP8, negr, tmpP7); //((g2^x1l)^I * g2^x3)^-r
    point_add(D2p, tmpP5, tmpP8); //g2^-x0 * ((g2^x1l)^I * g2^x3)^-r

    mpz_t rb;
    mpz_init(rb);
    mpz_add(rb, r, B);
    point_mul(D3, rb, G2);   

    for(i=0;i<l;i++){
        point_mul(K[i], r, G2y1[i]);
        point_mul(Kp[i], negr, G2x1[i]);
    }

}

void ws_deltagen(const EC_PAIRING p, const int l, const int i, const int int_time, const mpz_t I,
    const EC_POINT G2, const EC_POINT *G2x1, const EC_POINT G2x2, const EC_POINT G2x3, const EC_POINT *G2y1, const EC_POINT G2y2, const EC_POINT G2y3, 
    const EC_POINT D1, const EC_POINT D1p, const EC_POINT D2, const EC_POINT D2p, const EC_POINT D3, const EC_POINT *K, const EC_POINT *Kp,
    EC_POINT d1h, EC_POINT d1ph, EC_POINT d2h, EC_POINT d2ph, EC_POINT d3h, EC_POINT *kh, EC_POINT *kph){


    gmp_randstate_t state;
	gmp_randinit_default(state);
	gmp_randseed_ui(state, (unsigned long)time(NULL));
	
    mpz_t order;
    mpz_init(order);
    mpz_set(order, *pairing_get_order(p));

    mpz_t rh,negrh;
    mpz_init(rh);
    mpz_init(negrh);
    mpz_urandomm(rh, state, order);
    mpz_neg(negrh,rh);

    // d1h
    EC_POINT tmpP1, tmpP2, tmpP3, tmpP4;
    point_init(tmpP1,p->g2);
    point_init(tmpP2,p->g2);
    point_init(tmpP3,p->g2);
    point_init(tmpP4,p->g2);    

    point_mul(tmpP1, rh, G2y2);//(g2^y2)^rhat
    point_add(d1h, D1, tmpP1); //D1*((g2^y2)^rhat)

    // d1ph
    int j;
    EC_POINT tmpYP[l-i+1];
    for(j=0;j<l-i+1;j++){
        point_init(tmpYP[j],p->g2);   
    }
    int count=0;
    for(j=i-1;j<l;j++){  //d1hatの計算の右側のΠの計算部分
        EC_POINT tmpXP1, tmpXP2;
        mpz_t tmpj;
        point_init(tmpXP1,p->g2);
        point_init(tmpXP2,p->g2);
        mpz_init(tmpj);
        int timej = (int)ceil(int_time/pow(2,j));
        mpz_set_si(tmpj, timej); // tj=Tj(time)=Ceiling(time/2^j);
        point_mul(tmpXP1, tmpj,G2y1[j]);
        // point_add(tmpXP2, tmpXP1,G2y3);
        if(count==0){
            point_set(tmpYP[count],tmpXP1);
        }
        else{
            point_add(tmpYP[count],tmpYP[count-1],tmpXP1);
        }
        count++;
    }
    point_init(tmpP1, p->g2);
    point_init(tmpP2, p->g2);
    point_init(tmpP3, p->g2);
    point_init(tmpP4, p->g2);
    point_mul(tmpP1, I, G2y1[l]);  //(g2^y1l)^I
    point_add(tmpP2, tmpP1, tmpYP[count-1]);//d1hatp計算の一番右側部分の中身
    point_add(tmpP3, tmpP2, G2y3);
    point_mul(tmpP4, rh, tmpP3); //d1hatp計算の一番右側部分

    mpz_t tmpi;
    mpz_init(tmpi);
    int timei = (int)ceil(int_time/pow(2,i-1));
    mpz_set_si(tmpi, timei);
    point_init(tmpP4,p->g2);    
    point_mul(tmpP4, tmpi, K[i-1]);//Ki-1^ti-1
    EC_POINT tmpP5;
    point_init(tmpP5,p->g2);
    point_add(tmpP5, D1p, tmpP4); //D1p*((Ki-1)^ti-1)
    point_add(d1ph,tmpP5,tmpP3);


    // d2h
    point_init(tmpP1,p->g2);
    point_init(tmpP2,p->g2);
    point_init(tmpP3,p->g2);
    point_init(tmpP4,p->g2);    

    point_mul(tmpP1, negrh, G2x2);//(g2^x2)^-rhat
    point_add(d2h, D2, tmpP1); //D1*((g2^y2)^rhat)

    // d2ph
    for(j=0;j<l-i+1;j++){
        point_init(tmpYP[j],p->g2);   
    }
    count=0;
    for(j=i-1;j<l;j++){  //d2hatpの計算の右側のΠの計算部分
        EC_POINT tmpXP1, tmpXP2;
        mpz_t tmpj;
        point_init(tmpXP1,p->g2);
        point_init(tmpXP2,p->g2);
        mpz_init(tmpj);
        int timej = (int)ceil(int_time/pow(2,j));
        mpz_set_si(tmpj, timej); // tj=Tj(time)=Ceiling(time/2^j);
        point_mul(tmpXP1, tmpj,G2x1[j]);
        // point_add(tmpXP2, tmpXP1,G2x3);
        if(count==0){
            point_set(tmpYP[count],tmpXP2);
        }
        else{
            point_add(tmpYP[count],tmpYP[count-1],tmpXP2);
        }
        count++;
    }
    point_init(tmpP1, p->g2);
    point_init(tmpP2, p->g2);
    point_init(tmpP3, p->g2);
    point_init(tmpP4, p->g2);
    point_mul(tmpP1, I, G2x1[l]);  //(g2^x1l)^I
    point_add(tmpP2, tmpP1, tmpYP[count-1]);//d2hatp計算の一番右側部分の中身
    point_add(tmpP3, tmpP2,G2x3);
    point_mul(tmpP4, negrh, tmpP3); //d2hatp計算の一番右側部分
    
    point_init(tmpP4,p->g2);    
    point_init(tmpP5,p->g2);
    point_mul(tmpP4, tmpi, Kp[i-1]);//Kpi-1^ti-1
    point_add(tmpP5, D2p, tmpP4); //D2p*((Kpi-1)^ti-1)
    point_add(d2ph,tmpP5,tmpP3);

    // d3h
    point_init(tmpP1,p->g2);

    point_mul(tmpP1, rh, G2);//g2^rhat
    point_add(d3h, D3, tmpP1); //D3*(g2^rhat)

    // kh[i]
    for(j=0;j<i-1;j++){
        point_init(tmpP1,p->g2);
        point_mul(tmpP1,rh,G2y1[j]);
        point_add(kh[j],K[j],tmpP1);
    }

    // kph[i];
    for(j=0;j<i-1;j++){
        point_init(tmpP1,p->g2);
        point_mul(tmpP1,negrh,G2x1[j]);
        point_add(kph[j],Kp[j],tmpP1);
    }

}

void ws_upd(const int i, const EC_POINT Ri,
    const EC_POINT d1h, const EC_POINT d1ph, const EC_POINT d2h, const EC_POINT d2ph, const EC_POINT d3h, const EC_POINT *kh, const EC_POINT *kph, 
    EC_POINT nD1, EC_POINT nD1p, EC_POINT nD2, EC_POINT nD2p, EC_POINT nD3, EC_POINT *nK, EC_POINT *nKp){

    //nD1
    point_set(nD1,d1h);
    //nD1p
    point_set(nD1p,d1ph);
    //nD2
    point_set(nD2,d2h);
    //nD2p
    point_set(nD2p,d2ph);
    //nD3
    point_add(nD3,d3h,Ri);

    //nK[j]
    int j;
    for(j=0;j<i;j++){
        point_set(nK[j],kh[j]);
    }
    //nKp[j]
    for(j=0;j<i;j++){
        point_set(nKp[j],kph[j]);
    }

}

void ws_enc(const EC_PAIRING p, const int l, const int int_time, const Element M, const Element z, const EC_POINT G1, const EC_POINT G1alpha, const EC_POINT *u1, const EC_POINT w1, const EC_POINT h1, const mpz_t I,
    Element C0, EC_POINT C1, EC_POINT C2, EC_POINT C3, mpz_t tag 
){

    gmp_randstate_t state;
	gmp_randinit_default(state);
	gmp_randseed_ui(state, (unsigned long)time(NULL));
	
    mpz_t order;
    mpz_init(order);
    mpz_set(order, *pairing_get_order(p));

    
    mpz_t s;
    mpz_init(s);
    mpz_urandomm(s, state, order);
    mpz_urandomm(tag, state, order);

    //C0
    Element tmpE1;
    element_init(tmpE1, p->g3);
    element_pow(tmpE1, z, s); //z^s
    element_init(C0, p->g3);
    element_mul(C0, M, tmpE1); //M * z^s

    //C1
    point_mul(C1, s, G1);

    //C2
    point_mul(C2, s, G1alpha);

    //C3
    //Πの部分
    int j;
    int count=0;
    EC_POINT tmpYP[l];
    for(j=0;j<l;j++){
        point_init(tmpYP[j],p->g1);   
    }
    for(j=0;j<l;j++){  
        EC_POINT tmpXP1;
        mpz_t tmpj;
        point_init(tmpXP1,p->g1);
        mpz_init(tmpj);
        int timej = (int)ceil(int_time/pow(2,j));
        mpz_set_si(tmpj, timej); // tj=Tj(time)=Ceiling(time/2^j);
        point_mul(tmpXP1, tmpj,u1[j]);
        if(count==0){
            point_set(tmpYP[count],tmpXP1);
        }
        else{
            point_add(tmpYP[count],tmpYP[count-1],tmpXP1);
        }
        count++;
    }
    //u1l^I
    EC_POINT tmpP1;
    point_init(tmpP1, p->g1);
    point_mul(tmpP1, I, u1[l]);

    //w1^tag
    EC_POINT tmpP2;
    point_init(tmpP2, p->g1);
    point_mul(tmpP2, tag, w1);
    //4つの部分を足す
    EC_POINT tmpP3,tmpP4,tmpP5;
    point_init(tmpP3, p->g1);
    point_init(tmpP4, p->g1);
    point_init(tmpP5, p->g1);
    point_add(tmpP3,tmpYP[count-1],tmpP1);
    point_add(tmpP4,tmpP3,tmpP2);
    point_add(tmpP5,tmpP4,h1);

    //sを掛ける
    point_mul(C3, s, tmpP5);

}

void ws_dec(const EC_PAIRING p, const int t0, const int int_time, const Element C0, const EC_POINT C1, const EC_POINT C2, const EC_POINT C3, const mpz_t tag,
    const EC_POINT D1, const EC_POINT D1p, const EC_POINT D2, const EC_POINT D2p, const EC_POINT D3, 
    Element mp
){

    if(int_time!=t0)return;

    //分子
    Element tmpE1;
    element_init(tmpE1, p->g3);
   	pairing_map(tmpE1, C3, D3, p);

    Element nume;
    element_init(nume, p->g3);
    element_mul(nume,C0,tmpE1);

    //分母
    EC_POINT tmpP1, tmpP2, tmpP3, tmpP4;
    point_init(tmpP1,p->g2);
    point_init(tmpP2,p->g2);
    point_init(tmpP3,p->g2);
    point_init(tmpP4,p->g2);
    point_mul(tmpP1,tag,D1);
    point_add(tmpP2,tmpP1,D1p);
    point_mul(tmpP3,tag,D2);
    point_add(tmpP4,tmpP3,D2p);

    Element tmpE2, tmpE3;
    element_init(tmpE2, p->g3);
    element_init(tmpE3, p->g3);
   	pairing_map(tmpE2, C1, tmpP2, p);
   	pairing_map(tmpE3, C2, tmpP4, p);

    Element denomi;
    element_init(denomi, p->g3);
    element_mul(denomi,tmpE2,tmpE3);

    //分母の逆数を求める
    Element invdenomi;
    element_init(invdenomi, p->g3);
    element_inv(invdenomi,denomi);

    //平文戻す
    element_mul(mp,nume,invdenomi);

}