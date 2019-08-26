#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <tepla/ec.h>

void ke_rom_setup(const EC_PAIRING p, EC_POINT P, EC_POINT G, EC_POINT ek1, mpz_t dk1, EC_POINT mpk1, mpz_t msk1);
void ke_rom_upd(const EC_PAIRING p, const EC_POINT P, const EC_POINT G, mpz_t dk1,  EC_POINT ek_ipp, mpz_t dk_ipp, EC_POINT mpk_ipp, mpz_t msk_ipp, mpz_t rk_ipp);
void ke_rom_enc(const EC_PAIRING p, EC_POINT P_keyword, const EC_POINT P, const EC_POINT G, const EC_POINT eki, const EC_POINT mpki, EC_POINT cti1, EC_POINT cti2, EC_POINT ci1, Element ci2);
void ke_rom_reenc(const EC_PAIRING p, const int i, const int j, const int k, const EC_POINT P, const EC_POINT G, const EC_POINT ek_ipp, const EC_POINT mpk_ipp, const mpz_t dk_ipp, const EC_POINT cti1, const EC_POINT cti2, const EC_POINT ci1, const Element ci2, EC_POINT cti1r, EC_POINT cti2r, EC_POINT ci1r, Element ci2r);
void ke_rom_trapdoor(const EC_PAIRING p, const mpz_t mski, const EC_POINT P_keyword, EC_POINT Twi);
int ke_rom_test(const EC_PAIRING p, const int i, const int j, const int k, const EC_POINT Twi, const EC_POINT ci1, const Element ci2);

void ke_rom_setup(const EC_PAIRING p, EC_POINT P, EC_POINT G, EC_POINT ek1, mpz_t dk1, EC_POINT mpk1, mpz_t msk1){

    //ElGamal Setup
    eg_setup(P);

    //ElGamal KeyGen
    eg_keygen(p,dk1,P,ek1);

    //PEKS KeyGen
    peks_bf_keygen(p,msk1, G, mpk1);

    //添え字の1は返していないが、暗黙的にsetup時には1となるよ、というのは関数を使う側で対処

}

void ke_rom_upd(const EC_PAIRING p, const EC_POINT P, const EC_POINT G, mpz_t dk1,  EC_POINT ek_ipp, mpz_t dk_ipp, EC_POINT mpk_ipp, mpz_t msk_ipp, mpz_t rk_ipp){
    //Elgamal Keygen
    eg_keygen(p, dk_ipp,P,ek_ipp);

    //PEKS KeyGen
    peks_bf_keygen(p, msk_ipp, G, mpk_ipp);

    //rk_i->i++
    rk_ipp = dk1;

}

void ke_rom_enc(const EC_PAIRING p, EC_POINT P_keyword, const EC_POINT P, const EC_POINT G, const EC_POINT eki, const EC_POINT mpki, EC_POINT cti1, EC_POINT cti2, EC_POINT ci1, Element ci2){
    //H(w)で実装。ACISP版とは違う。入力時点でEC_POINT化されている

    //Elgamal Encryption
    eg_encrypt(p,P,eki,P_keyword,cti1,cti2);

    //PEKS Encryption
    peks_bf_peks_forECPoint(p,P_keyword, G, mpki, ci1, ci2);

    //添え字のiは独立して使ってはいないが、暗黙的にiの鍵を使うよ、というのは関数を使う側で対処
    //cの右上添え字0は返していないが、暗黙的にenc時には0となるよ、というのは関数を使う側で対処

}

void ke_rom_reenc(const EC_PAIRING p, const int i, const int j, const int k, 
const EC_POINT P, const EC_POINT G, const EC_POINT ek_ipp, const EC_POINT mpk_ipp, const mpz_t dk_ipp, 
const EC_POINT cti1, const EC_POINT cti2, const EC_POINT ci1, const Element ci2,
EC_POINT cti1r, EC_POINT cti2r, EC_POINT ci1r, Element ci2r
){

    if(i!=(j+k)){
        //このときは何を返そうかな？
        point_set_infinity(cti1r);
        point_set_infinity(cti2r);
        point_set_infinity(ci1r);
        element_set_zero(ci2r);
        
    }
    else{
        //ElGamalでDecryption
        EC_POINT What;
        point_init(What, p->g1);
        eg_decrypt(p,cti1, cti2, dk_ipp, What);

        //ke_rom_encs
        //void ke_rom_enc(const char *keyword, const EC_POINT P, const EC_POINT G, const EC_POINT eki, const EC_POINT mpki, EC_POINT cti1, EC_POINT cti2, EC_POINT ci1, EC_POINT ci2)
        ke_rom_enc(p,What, P, G, ek_ipp, mpk_ipp, cti1r, cti2r, ci1r, ci2r);
    }


    //cの右上添え字k+1は返していないが、暗黙的にreenc時にはk+1となるよ、というのは関数を使う側で対処

}

void ke_rom_trapdoor(const EC_PAIRING p, const mpz_t mski, const EC_POINT P_keyword, EC_POINT Twi){
    //PEKSでTrapdoor作成
    peks_bf_trapdoor_forECPoint(P_keyword, Twi, mski);
}

int ke_rom_test(const EC_PAIRING p, const int i, const int j, const int k, const EC_POINT Twi, const EC_POINT ci1, const Element ci2){
    if(i!=(j+k)){
        return 0;
    }
    else{
        //int peks_bf_test(const EC_POINT Tw, const EC_POINT C1, const Element C2)
        return peks_bf_test(p,Twi, ci1, ci2);
    }

}