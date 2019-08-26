#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <tepla/ec.h>

void ke_std_setup(const EC_PAIRING p, EC_POINT P, EC_POINT ek1, mpz_t dk1, mpz_t x_1, mpz_t y_1, mpz_t x0_1, mpz_t y0_1, mpz_t x1_1, mpz_t y1_1, mpz_t x2_1, mpz_t y2_1, EC_POINT G1_1, EC_POINT G2_1, EC_POINT G1alpha_1, Element z_1, EC_POINT u1_1, EC_POINT h1_1, EC_POINT w1_1);
void ke_std_upd(const EC_PAIRING p, const EC_POINT P, mpz_t dk1, EC_POINT ek_ipp, mpz_t dk_ipp, mpz_t x_ipp, mpz_t y_ipp, mpz_t x0_ipp, mpz_t y0_ipp, mpz_t x1_ipp, mpz_t y1_ipp, mpz_t x2_ipp, mpz_t y2_ipp, EC_POINT G1_ipp, EC_POINT G2_ipp, EC_POINT G1alpha_ipp, Element z_ipp, EC_POINT u1_ipp, EC_POINT h1_ipp, EC_POINT w1_ipp, mpz_t rk_ipp);

void ke_std_setup(const EC_PAIRING p, EC_POINT P, EC_POINT ek1, mpz_t dk1, mpz_t x_1, mpz_t y_1, mpz_t x0_1, mpz_t y0_1, mpz_t x1_1, mpz_t y1_1, mpz_t x2_1, mpz_t y2_1, EC_POINT G1_1, EC_POINT G2_1, EC_POINT G1alpha_1, Element z_1, EC_POINT u1_1, EC_POINT h1_1, EC_POINT w1_1){

    //ElGamal Setup
    eg_setup(P);

    //ElGamal KeyGen
    eg_keygen(p,dk1,P,ek1);

    //PEKS KeyGen
    peks_jr_keygen(p,x_1,y_1,x0_1,y0_1,x1_1,y1_1,x2_1,y2_1,G1_1,G2_1,G1alpha_1,z_1,u1_1,h1_1,w1_1);

}

void ke_std_upd(const EC_PAIRING p, const EC_POINT P, mpz_t dk1, EC_POINT ek_ipp, mpz_t dk_ipp, mpz_t x_ipp, mpz_t y_ipp, mpz_t x0_ipp, mpz_t y0_ipp, mpz_t x1_ipp, mpz_t y1_ipp, mpz_t x2_ipp, mpz_t y2_ipp, EC_POINT G1_ipp, EC_POINT G2_ipp, EC_POINT G1alpha_ipp, Element z_ipp, EC_POINT u1_ipp, EC_POINT h1_ipp, EC_POINT w1_ipp, mpz_t rk_ipp){
    //論文的には、時刻iの鍵を入力して、時刻i+1の鍵出力と、rki+1の出力
    //ElGamalでもPEKSでも、鍵ペア（のセット）を前の情報とは関係なく生成して出力

    //Elgamal Keygen
    eg_keygen(p,dk_ipp,P,ek_ipp);

    //PEKS KeyGen
    peks_jr_keygen(p,x_ipp,y_ipp,x0_ipp,y0_ipp,x1_ipp,y1_ipp,x2_ipp,y2_ipp,G1_ipp,G2_ipp,G1alpha_ipp,z_ipp,u1_ipp,h1_ipp,w1_ipp);

    //rk_i->i++
    rk_ipp = dk1;

}

void ke_std_enc(const EC_PAIRING p, const EC_POINT P_keyword, const EC_POINT P, const EC_POINT eki, EC_POINT cti1, EC_POINT cti2, const EC_POINT G1_i, const EC_POINT G1alpha_i, const Element z_i, const EC_POINT u1_i, const EC_POINT h1_i, const EC_POINT w1_i, mpz_t tag_i, Element Cm_i, EC_POINT Cx_i, EC_POINT Cy_i, EC_POINT C_i){

    //Elgamal Encryption
    eg_encrypt(p,P,eki,P_keyword,cti1,cti2);

    //PEKS Encryption
    mpz_t keyword;
    mpz_init(keyword);
    char str_p[256];
    point_get_str(str_p,P_keyword);
    char *ret;
    ret = strstr(str_p,",");
    int pos = ret-str_p;
    char str_pa[256];
    strncpy(str_pa,str_p+1,pos-1);
    str_pa[pos-1]='\0';
    mpz_set_str(keyword,str_pa,16);
    
    peks_jr_peks(p,keyword, G1_i, G1alpha_i, z_i,u1_i,h1_i,w1_i,tag_i,Cm_i, Cx_i, Cy_i, C_i);

}

//void ke_rom_reenc(const int i, const int j, const int k, 
// const EC_POINT P, const EC_POINT G, const EC_POINT ek_ipp, const EC_POINT mpk_ipp, const mpz_t dk_ipp, 
// const EC_POINT cti1, const EC_POINT cti2, const EC_POINT ci1, const Element ci2,
// EC_POINT cti1r, EC_POINT cti2r, EC_POINT ci1r, Element ci2r
void ke_std_reenc(const EC_PAIRING p, const int i, const int j, const int k,
const EC_POINT P, const EC_POINT ek_ipp, const mpz_t rk_ipp,
const EC_POINT cti1, const EC_POINT cti2,
const EC_POINT G1_ipp, const EC_POINT G1alpha_ipp, const Element z_ipp, const EC_POINT u1_ipp, const EC_POINT h1_ipp, const EC_POINT w1_ipp, 
EC_POINT cti1r, EC_POINT cti2r,
mpz_t tag_ipp, Element Cm_ipp, EC_POINT Cx_ipp, EC_POINT Cy_ipp, EC_POINT C_ipp
){
    //i+1の公開鍵群とrki+1が入力される
    //jとkの暗号文が入力される
    //rki+1はdkiなので、i=j+kなら、jとkの暗号文からwが復活できる
    //i+1の公開鍵群でw（h(w))を暗号化してjとk+1の暗号文が出力される

    // EC_PAIRING p;
    // pairing_init(p, "ECBN254a");    

    if(i!=(j+k)){
        //このときは何を返そうかな？
        point_set_infinity(cti1r);
        point_set_infinity(cti2r);
        mpz_init(tag_ipp);
        element_set_zero(Cm_ipp);
        point_set_infinity(Cx_ipp);
        point_set_infinity(Cy_ipp);
        point_set_infinity(C_ipp);
        
    }
    else{
        //ElGamalでDecryption
        EC_POINT What;
        point_init(What, p->g1);
        eg_decrypt(p,cti1, cti2, rk_ipp, What);

        //ke_rom_encs
        //void ke_rom_enc(const char *keyword, const EC_POINT P, const EC_POINT G, const EC_POINT eki, const EC_POINT mpki, EC_POINT cti1, EC_POINT cti2, EC_POINT ci1, EC_POINT ci2)
        // ke_rom_enc(What, P, G, ek_ipp, mpk_ipp, cti1r, cti2r, ci1r, ci2r);
        //void ke_std_enc(const EC_POINT P_keyword, const EC_POINT P, const EC_POINT eki, EC_POINT cti1, EC_POINT cti2, 
        //const EC_POINT G1_i, const EC_POINT G1alpha_i, const Element z_i, const EC_POINT u1_i, const EC_POINT h1_i, const EC_POINT w1_i, mpz_t tag_i, Element Cm_i, EC_POINT Cx_i, EC_POINT Cy_i, EC_POINT C_i)
        ke_std_enc(p,What,P,ek_ipp,cti1r,cti2r,G1_ipp,G1alpha_ipp,z_ipp,u1_ipp,h1_ipp,w1_ipp,tag_ipp,Cm_ipp,Cx_ipp,Cy_ipp,C_ipp);
    }

}


void ke_std_trapdoor(const EC_PAIRING p, const EC_POINT P_keyword, const EC_POINT G2_i, const mpz_t x_i, const mpz_t x0_i, const mpz_t x1_i, const mpz_t x2_i, const mpz_t y_i, const mpz_t y0_i, const mpz_t y1_i, const mpz_t y2_i, EC_POINT D_i, EC_POINT Dx_i, EC_POINT Dxp_i, EC_POINT Dy_i, EC_POINT Dyp_i){
    //PEKSでTrapdoor作成
    // peks_bf_trapdoor_forECPoint(P_keyword, Twi, mski);
    //void peks_jr_trapdoor(const EC_POINT G2, const mpz_t x, const mpz_t x0, const mpz_t x1, const mpz_t x2, const mpz_t y, const mpz_t y0, const mpz_t y1, const mpz_t y2, const mpz_t I, EC_POINT D, EC_POINT Dx, EC_POINT Dxp, EC_POINT Dy, EC_POINT Dyp)
    //PEKS Encryption

    mpz_t keyword;
    mpz_init(keyword);
    char str_p[256];
    point_get_str(str_p,P_keyword);
    char *ret;
    ret = strstr(str_p,",");
    int pos = ret-str_p;
    char str_pa[256];
    strncpy(str_pa,str_p+1,pos-1);
    str_pa[pos-1]='\0';
    mpz_set_str(keyword,str_pa,16);

    peks_jr_trapdoor(p,G2_i,x_i,x0_i,x1_i,x2_i,y_i,y0_i,y1_i,y2_i,keyword,D_i,Dx_i,Dxp_i,Dy_i,Dyp_i);

}


int ke_std_test(const EC_PAIRING p, const int i, const int j, const int k, const EC_POINT D_i, const EC_POINT Dx_i, const EC_POINT Dxp_i, const EC_POINT Dy_i, const EC_POINT Dyp_i, const mpz_t tag_i, const Element Cm_i, const EC_POINT Cx_i, const EC_POINT Cy_i, const EC_POINT C_i){
    if(i!=(j+k)){
        return 0;
    }
    else{
        //int peks_jr_test(const EC_POINT D, const EC_POINT Dx, const EC_POINT Dxp, const EC_POINT Dy, const EC_POINT Dyp, const mpz_t tag, const Element Cm, const EC_POINT Cx, const EC_POINT Cy, const EC_POINT C)
        return peks_jr_test(p,D_i,Dx_i,Dxp_i,Dy_i,Dyp_i,tag_i,Cm_i,Cx_i,Cy_i,C_i);
    }
    
}

