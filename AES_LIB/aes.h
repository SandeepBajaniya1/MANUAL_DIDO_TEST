#ifndef _AES_H_
#define _AES_H_

//#include "stdint.h"
#include "r_cg_userdefine.h"


// #define the macros below to 1/0 to enable/disable the mode of operation.
//
// CBC enables AES128 encryption in CBC-mode of operation and handles 0-padding.
// ECB enables the basic ECB 16-byte block algorithm. Both can be enabled simultaneously.

// The #ifndef-guard allows it to be configured before #include'ing or at compile time.
#ifndef CBC
  #define CBC 1
#endif

#ifndef ECB
  #define ECB 1
#endif



#if defined(ECB) && ECB

void AES128_ECB_encrypt(BYTE* input, const BYTE* key, BYTE *output);
void AES128_ECB_decrypt(BYTE* input, const BYTE* key, BYTE *output);

#endif // #if defined(ECB) && ECB


#if defined(CBC) && CBC

void AES128_CBC_encrypt_buffer(BYTE* output, BYTE* input, DWORD length, const BYTE* key, const BYTE* iv);
void AES128_CBC_decrypt_buffer(BYTE* output, BYTE* input, DWORD length, const BYTE* key, const BYTE* iv);

#endif // #if defined(CBC) && CBC



#endif //_AES_H_
