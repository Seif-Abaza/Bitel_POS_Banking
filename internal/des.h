#ifndef __DES__H
#define __DES__H

#include "types.h"

#define	DES_MODE_ECB		0	// Ёлектронна€ кодова€ книга ( Electronic Code Book )
#define	DES_MODE_CBC		1	// ÷епочный режим ( CBC - Clipher Block Chaining )

typedef struct
{
	ULONG esk[32];     /* DES encryption subkeys */
	ULONG dsk[32];     /* DES decryption subkeys */
}
des_context;

typedef struct
{
	ULONG esk[96];     /* Triple-DES encryption subkeys */
	ULONG dsk[96];     /* Triple-DES decryption subkeys */
}
des3_context;

int  des_set_key( des_context *ctx, BYTE key[8] );
void des_encrypt( des_context *ctx, BYTE input[8], BYTE output[8] );
void des_decrypt( des_context *ctx, BYTE input[8], BYTE output[8] );

int  des3_set_2keys( des3_context *ctx, BYTE key1[8], BYTE key2[8] );
int  des3_set_3keys( des3_context *ctx, BYTE key1[8], BYTE key2[8], BYTE key3[8] );

void des3_encrypt( des3_context *ctx, BYTE input[8], BYTE output[8] );
void des3_decrypt( des3_context *ctx, BYTE input[8], BYTE output[8] );



// dataLen       the len of the input data.
//
// mode:
//               DES_MODE_ECB - Electronic Code Book.
//               DES_MODE_CBC - Clipher Block Chaining.
//
// pInitVector - init vector (8 bytes) for mode = DES_MODE_CBC, NULL in other case.
void des_encrypt_data( des_context *ctx, BYTE *pInputBuffer, BYTE *pOutputBuffer, ULONG dataLen, BYTE mode, BYTE *pInitVector );
// dataLen       the len of the input data.
//
// mode:
//               DES_MODE_ECB - Electronic Code Book.
//               DES_MODE_CBC - Clipher Block Chaining.
//
// pInitVector - init vector (8 bytes) for mode = DES_MODE_CBC, NULL in other case.
void des_decrypt_data( des_context *ctx, BYTE *pInputBuffer, BYTE *pOutputBuffer, ULONG dataLen, BYTE mode, BYTE *pInitVector );

// dataLen       the len of the input data.
//
// mode:
//               DES_MODE_ECB - Electronic Code Book.
//               DES_MODE_CBC - Clipher Block Chaining.
//
// pInitVector - init vector (8 bytes) for mode = DES_MODE_CBC, NULL in other case.
void des3_encrypt_data( des3_context *ctx, BYTE *pInputBuffer, BYTE *pOutputBuffer, ULONG dataLen, BYTE mode, BYTE *pInitVector );
// dataLen       the len of the input data.
//
// mode:
//               DES_MODE_ECB - Electronic Code Book.
//               DES_MODE_CBC - Clipher Block Chaining.
//
// pInitVector - init vector (8 bytes) for mode = DES_MODE_CBC, NULL in other case.
void des3_decrypt_data( des3_context *ctx, BYTE *pInputBuffer, BYTE *pOutputBuffer, ULONG dataLen, BYTE mode, BYTE *pInitVector );

// блоками шифруем
ULONG des3_encrypt_data_block( des3_context *ctx, BYTE *pInputBuffer, BYTE *pOutputBuffer, ULONG dataLen, ULONG max_block_len, BYTE mode, BYTE *pInitVector, BYTE *pContext );

// блоками расшифровываем
ULONG des3_decrypt_data_block( des3_context *ctx, BYTE *pInputBuffer, BYTE *pOutputBuffer, ULONG dataLen, ULONG max_block_len, BYTE mode, BYTE *pInitVector, BYTE *pContext );


#endif /* des.h */
