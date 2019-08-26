# KUPEKS
C implementation of "Key-updatable Public-key Encryption with Keyword Search"

## Getting Started 

### Requirements
- OpenSSL ( https://www.openssl.org/source/ )
- GM ( https://gmplib.org/ )
- TEPLA ( https://github.com/TEPLA/tepla-library )

### Confirmed Platforms
(TBD) 

### Test
(TBD)

## List of Components

| File | Comments |
|:---|:---|
| elgamal_enc.c | ElGamal Encryption ([*1](#01))|
| bf-ibe.c | Boneh-Franklin Identity-Based Encryption ([*2](#02))|
| bf-peks.c | PEKS based on Boneh-Franklin IBE  ([*3](#03))|
| jr-ibe.c | Jutla-Roy Identity-Based Encryption  ([*4](#04))|
| jr-peks.c | PEKS based on Jutla-Roy IBE |
| ws_ibe.c | Watanabe-Shikata Key-Insulated Encryption  ([*5](#05))|
| mike.c | Key-Insulated Identiby-Based Encryption for Master Keys (MIKE)  ([*6](#06))|
| ke_rom.c | KU-PEKS in the Key-evolution (KE) Model (Random Oracle Model)  ([*6](#06))| 
| ke_std.c | KU-PEKS in the Key-evolution (KE) Model (Standard Model)  ([*6](#06))| 
| ki_std.c | KU-PEKS in the Key-insulation (KI) Model  ([*6](#06))|
| ki_util.c | Utility Functions for KUPEKS in the KI Model |

## Examples
(TBD)

## Benchmarks
(TBD)

## References
1. ElGamal, T.: A public key cryptosystem and a signature scheme based on discrete logarithms. In: Blakley, G., Chaum, D. (eds.) Advances in Cryptology - CRYPTO'84. vol. 196, pp. 10-18. Springer Berlin Heidelberg (1985) <a name="01">
1. Boneh, D., Franklin, M.K.: Identity-based encryption from the weil pairing. In: Advances in Cryptology - CRYPTO'01. pp. 213-229. Springer-Verlag (2001) <a name="02">
1. Boneh, D., Crescenzo, G.D., Ostrovsky, R., Persiano, G.: Public key encryption with keyword search. In: Advances in Cryptology - EUROCRYPT 2004. pp. 506-522 (2004) <a name="03">
1. Jutla, C.S., Roy, A.: Shorter quasi-adaptive NIZK proofs for linear subspaces. In: Sako, K., Sarkar, P. (eds.) Advances in Cryptology - ASIACRYPT 2013. LNCS, vol.8269, pp. 1-20. Springer Berlin Heidelberg (2013)<a name="04">
1. Watanabe, Y., Shikata, J.: Identity-based hierarchical key-insulated encryption without random oracles. In: Cheng, C.M., Chung, K.M., Persiano, G., Yang, B.Y. (eds.) PKC 2016, Part I. LNCS, vol. 9614, pp. 255-279. Springer Berlin Heidelberg, Berlin, Heidelberg (2016)<a name="05">
1. Anada, H., Kanaoka, A., Matsuzaki, N., Watanabe, Y.: Key-updatable public-key encryption with keyword search (Or: How to realize PEKS with efficient key updates for IoT environments) in International Journal of Information Security, 2019 <a name="06">

## License

The MIT License. Please see [License File](LICENSE.md) for more information.
