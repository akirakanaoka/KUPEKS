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
| elgamal_enc.c | ElGamal Encryption |
| bf-ibe.c | Boneh-Franklin Identity-Based Encryption ([*1](#01))|
| bf-peks.c | PEKS based on Boneh-Franklin IBE |
| jr-ibe.c | Jutla-Roy Identity-Based Encryption| |
| jr-peks.c | PEKS based on Jutla-Roy IBE |
| ws_ibe.c | Watanabe-Shikata Key-Insulated Encryption |
| mike.c | Key-Insulated Identiby-Based Encryption for Master Keys (MIKE) |
| ke_rom.c | KU-PEKS in the Key-evolution (KE) Model (Random Oracle Model) | 
| ke_std.c | KU-PEKS in the Key-evolution (KE) Model (Standard Model) | 
| ki_std.c | KU-PEKS in the Key-insulation (KI) Model|
| ki_util.c | Utility Functions for KUPEKS in the KI Model |

## Examples
(TBD)

## Benchmarks
(TBD)

## References
1. aaaa {#01}

## License

The MIT License. Please see [License File](LICENSE.md) for more information.
