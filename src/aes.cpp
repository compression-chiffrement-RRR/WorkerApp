#include <cstring>

#include "aes.h"

#ifdef AES_DEBUG

#include <iostream>
using namespace std;

#endif

/*
    Rijndael cipher S-box substitution table.
*/
static const uint8_t SBox[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76, 
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0, 
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15, 
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75, 
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84, 
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf, 
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8, 
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2, 
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 
};

/* Rijndael cipher reverse S-box substitution table. */
static const uint8_t RSBox[256] = {
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d 
};

AES::AES (AESKeySize keySize){

    this->keySize = keySize;

    // Define Nr and Nk according to the key size.
    switch (keySize){
        
        case AESKeySize::AES_128:
            this->Nk = 4;
            this->Nr = 10;
            break;
        case AESKeySize::AES_192:
            this->Nk = 6;
            this->Nr = 12;
            break;
        case AESKeySize::AES_256:
            this->Nk = 8;
            this->Nr = 14;
            break;
    };

    // Allocate data for the round keys which are going to be produced during the key expansion step.
    // We need one round key allocation per round + 1 for the original key that is added before the rounds start.
    this->RoundKeys = new uint8_t[16 * (this->Nr + 1)];

};

AES::~AES (){
    delete [] this->RoundKeys;
};

/* 
    Rcon is a round constant used within the key expansion step.

    The array starts from 01 00 00 00, the value of the first byte is multiplied by 2 in GF(2^8) for each index of the array.
                                                                                             
*/

void AES::Cipher (uint8_t state[Nb][Nl]){

    uint8_t roundNumber;

    this->AddRoundKey(state, 0);

    for (roundNumber = 1; roundNumber < this->Nr - 1; roundNumber++){
        this->SubBytes(state);
        this->ShiftRows(state);
        this->MixColumns(state);
        this->AddRoundKey(state, roundNumber);
    }

    this->SubBytes(state);
    this->ShiftRows(state);
    this->AddRoundKey(state, roundNumber);
}

static const uint8_t Rcon [] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36 };

/*
    The key expansion step is required in order to have one unique key for each round.

    It will expand the original key into a longer key, its size will be (Nr + 1) * 4 words.
*/

void AES::ExpandKey (uint8_t *key){
    
    // number of round keys that we need.
    uint8_t roundKeysNeeded = (this->Nr + 1);
    uint8_t tmp[4];

    // for each word in our needed round keys
    for (uint8_t i = 0; i < Nb * roundKeysNeeded; i++){

        // First round key is the actual encryption key.
        if (i < this->Nk){

            // Copy each word from the 
            memcpy(this->RoundKeys + (i * 4), key + (i * 4), 4);

            continue;
        }

        memcpy(tmp, this->RoundKeys + ((i - 1) * 4), 4);

        if (i % this->Nk == 0){

            this->RotWord(tmp);
            this->SubWord(tmp);

            // we XOR only the first byte cause Rcon constant has 3 null bytes after its most high order byte.
            // Only this high order byte is multiplied by 2 in GF(2^8) for each step of Rcon.
            tmp[0] ^= Rcon[i/this->Nk];

            this->RoundKeys[(i * 4)]      = tmp[0];
            this->RoundKeys[(i * 4) + 1]  = tmp[1];
            this->RoundKeys[(i * 4) + 2]  = tmp[2];
            this->RoundKeys[(i * 4) + 3]  = tmp[3];
        }

        else if (this->keySize == AESKeySize::AES_256 && i % this->Nk == 4){

            this->SubWord(tmp);

            this->RoundKeys[(i * 4)]      = tmp[0];
            this->RoundKeys[(i * 4) + 1]  = tmp[1];
            this->RoundKeys[(i * 4) + 2]  = tmp[2];
            this->RoundKeys[(i * 4) + 3]  = tmp[3];
        }

        memcpy(tmp, this->RoundKeys + ((i - this->Nk) * 4), 4);

        this->RoundKeys[(i * 4)]      ^= tmp[0];
        this->RoundKeys[(i * 4) + 1]  ^= tmp[1];
        this->RoundKeys[(i * 4) + 2]  ^= tmp[2];
        this->RoundKeys[(i * 4) + 3]  ^= tmp[3];
    }
}

// Apply S-Box substitution to a single word (4 bytes), to be used inside the ExpandKey method.
void AES::SubWord (uint8_t *word){
    
    for (uint8_t i = 0; i < Nb; i++){
        word[i] = SBox[word[i]];
    }
}

// Simple circular rotation to the left of each byte in a word.
void AES::RotWord(uint8_t *word){
    
    uint8_t tmp = word[0];

    word[0] = word[1];
    word[1] = word[2];
    word[2] = word[3];
    word[3] = tmp;
}

/* The AddRoundKey step is a simple XOR operation between the state and the corresponding part of the RoundKey for the current round. */
void AES::AddRoundKey (uint8_t state[Nb][Nl], uint8_t roundNumber){

    for (uint8_t i = 0; i < Nb; i++){

        for (uint8_t y = 0; y < Nl; y++){

            state[i][y] ^= this->RoundKeys[(roundNumber * Nb * Nl) + (i * Nb) + y];
        }
    }
}


/* 
    The SubBytes and InvSubBytes methods are quite simple. 
    It's just a substitution made by a simple lookup in the S-Box (SBox) or the Reverse S-Box (RSBox).  
*/

void AES::SubBytes (uint8_t state[Nb][Nl]){

    for (uint8_t i = 0; i < Nb; i ++){
        for (uint8_t y = 0; i < Nl; i ++){
            state[i][y] = SBox[state[i][y]];
        }
    }
};

void AES::InvSubBytes (uint8_t state[Nb][Nl]){

    for (uint8_t i = 0; i < Nb; i ++){
        for (uint8_t y = 0; i < Nl; i ++){
            state[i][y] = RSBox[state[i][y]];
        }
    }
};

/* 
    The ShiftRows method works like this:

    The first row of the state is not affected.
    1 left circular bytes shifts is applied to the second row.
    2 left circular bytes shifts are applied to the third row.
    3 left circular bytes shifts are applied to the fourth row.

    In other terms, each in each row's bytes are left shifted with an offset equal to the row number (starting from 0 with first row, which is not shifted at all).

    To illustrate (first number is column number, second number is row number) :

    00 10 20 30
    01 11 21 31
    02 12 22 32
    03 13 23 33

    becomes...

    00 10 20 30
    11 21 31 01
    22 32 02 12
    33 03 13 23

 */

void AES::ShiftRows (uint8_t state[Nb][Nl]){

    uint8_t tmp;

    // apply 1 left shift to second row.
    tmp = state[0][1];
    state[0][1] = state[1][1];
    state[1][1] = state[2][1];
    state[2][1] = state[3][1];
    state[3][1] = tmp;

    // apply 2 left shifts to third row.
    tmp = state[0][2];
    state[0][2] = state[2][2];
    state[2][2] = tmp;

    tmp = state[1][2];
    state[1][2] = state[3][2];
    state[3][2] = tmp;

    // apply 3 left shifts to fourth row.
    tmp = state[0][3];
    state[0][3] = state[3][3];
    state[3][3] = state[2][3];
    state[2][3] = state[1][3];
    state[1][3] = tmp;
}

/* To invert the ShiftRows step, we invert the rotations (so we right-shift instead of left-shift). */
void AES::InvShiftRows (uint8_t state[Nb][Nl]){
    
    uint8_t tmp;

    // apply 1 right shift to second row.  
    tmp = state[3][1];
    state[3][1] = state[2][1];
    state[2][1] = state[1][1];
    state[1][1] = state[0][1];
    state[0][1] = tmp;

    // apply 2 right shifts to third row.
    tmp = state[0][2];
    state[0][2] = state[2][2];
    state[2][2] = tmp;

    tmp = state[1][2];
    state[1][2] = state[3][2];
    state[3][2] = tmp;

    // apply 3 right shifts to fourth row.
    tmp = state[0][3];
    state[0][3] = state[1][3];
    state[1][3] = state[2][3];
    state[2][3] = state[3][3];
    state[3][3] = tmp;
}

/* 
    Multiplication by X in the Galois field GF(2^8), i.e multiply by 2. 

    This is done by left shifting (each monomial is multiplied by X, so for e.g 1 becomes X, X becomes X^2 ,X^2 becomes X^3...etc) in the first place.

    Then we might need to reduce the result, if the input number had the most significant bit set, otherwise we go out of the field by having an X^7 monomial transformed to X^8.

    The reduction could be done by applying the modulo operation to the result with any irreducible polynomial of a highest degree of 8 (with a X^8 monomial as the highest).
    
    In Rijndael, this irreducible polynomial is set as a constant one, its value is : 
    
    m(x) = X^8 + X^4 + X^3 + x +1

    m(x) = 0x11b in hexadecimal representation.

    m(x) = 0x100011011

    In the XTime method, we simply check if the input number's most significant bit (X^7 monomial) is set by doing 7 right rotations followed by a logical AND against 0b00000001 ((number >> 7) & 1). If it is set, it will return 1, otherwise 0.
    
    Then the return value of this expression (1 or 0) is multiplied by m(x), the irreducible polynomial that we're using.
    
    Then we XOR the resulting value to our shifted byte to apply modulo m(x). 
    
    If (((number >> 7) & 1) * 0x1b) was equal to 0, then the modulo isn't actually applied, cause there is no need for reduction.

    0x11b is written as 0x1b cause we don't really need to apply XOR for the last overflowed byte (X^8). It already has been striped while shifting.

    The condition to apply reduction by m(x) or not is done that way because we want to avoid timing attacks (the method will always execute the same instructions in a fixed time).

    See 4.2.1 in the NIST specification for more details about what is going on here.
*/

uint8_t AES::XTime (uint8_t number){
    return (
        
        // multiply by 2
        (number << 1) ^ 
        
        // reduce with mod m(x) if necessary
        (((number >> 7) & 0x01) * 0x1b));
};

/* 
    Galois multiplication in GF(2^8) by any constant (up to 15 in decimal)

    We can multiply in the Galois field GF(2^8) by any number by doing consecutive multiplications by X and optionnaly a final xor with our initial number.

    Multiplication by X is defined in the XTime operation

    Let's checkout some examples.

    if we want to multiply 0x80 by 0x03:

        (0x80) . (0x03)
    =   (0x80) . (0x02 ^ 0x01)
    =   (0x80 . 0x01) ^ (xtime(0x80))
    =   (0x80) ^ (0b10000000 << 1 modulo m(x))  ------> high bit is set so we need to apply reduction in the xtime operation.
    =   (0x80) ^ (0b00000000 ^ 0x1B)
    =   (0x80) ^ (0x1B)
    =   (0x10000000) ^ (0x00011011)
    =   (0x10011011)
    =   (0x9B)

    then if we want to multiply 0x80 by 0x05

        (0x80) . (0x05)
    =   (0x80) . (0x04 ^ 0x01)
    =   (0x80) . 0x04) ^ (0x80 . 0x01)
    
    and (0x80 . 0x04) can be converted to (0x80 . 0x02 . 0x02) = xtime(xtime(0x80))

    so then we have:

    =   (xtime(0x1B)) ^ (0x80)
    =   (0b00011011 << 1) ^ (0x80)  ------> highest bit is not set so we don't need to apply reduction in the xtime operation.
    =   (0b00110110) ^ (0x80)
    =   (0b00110110) ^ (0x10000000)
    =   (0b10110110)
    =   (0xB6)

    We can do the same for any multiplication by any constant.

    In the GMultiply method, the multiplier will only go up to 15 in decimal (0b00001111 in binary or 0x0E in hex), so the maximum repetition of the xtime operation is six times.

    See 4.2.1 in the NIST specification for more details about what is going on here.
*/

uint8_t AES::GMultiply (uint8_t number, uint8_t multiplier){
    
    // all add operations are done following by mod 2 in GF(2^8), this can be simplified to XOR
    return (
        // if first bit is set in multiplier, add number to result
        ((multiplier & 1) * number) ^
        
        // if second bit is set in multiplier, add xtime(number) to result
        ((multiplier >> 1 & 1) * this->XTime(number)) ^
        
        // if third bit is set in multiplier, add xtime(xtime(number)) to result
        ((multiplier >> 2 & 1) * this->XTime(this->XTime(number))) ^
        
        // if fourth bit is set in multiplier, add xtime(xtime(xtime(number))) to result
        ((multiplier >> 3 & 1) * this->XTime(this->XTime(this->XTime(number))))
    );
};

/* 
    The MixColumn is a matrix multiplication in the Galois field GF(2^8) between the state and a fixed matrix.

    out0  out4  out8  out12      2 3 1 1     in0  in4  in8  in12
    out1  out5  out9  out13   =  1 2 3 1  .  in1  in5  in9  in13
    out2  out6  out10 out14      1 1 2 3     in2  in6  in10 in14
    out3  out7  out11 out15      3 1 1 2     in3  in7  in11 in15

    (fixed matrix values are in decimal)

    See more at: https://en.wikipedia.org/wiki/Rijndael_MixColumns

*/

void AES::MixColumns (uint8_t state[Nb][Nl]){

    uint8_t row1, row2, row3, row4;
    
    for (uint8_t i = 0; i < Nb; i ++){

        row1 = state[i][0];
        row2 = state[i][1];
        row3 = state[i][2];
        row4 = state[i][3];
        
        state[i][0] = this->GMultiply(row1, 2) ^ this->GMultiply(row2, 3) ^ this->GMultiply(row3, 1) ^ this->GMultiply(row4, 1);
        state[i][1] = this->GMultiply(row1, 1) ^ this->GMultiply(row2, 2) ^ this->GMultiply(row3, 3) ^ this->GMultiply(row4, 1);
        state[i][2] = this->GMultiply(row1, 1) ^ this->GMultiply(row2, 1) ^ this->GMultiply(row3, 2) ^ this->GMultiply(row4, 3);
        state[i][3] = this->GMultiply(row1, 3) ^ this->GMultiply(row2, 1) ^ this->GMultiply(row3, 1) ^ this->GMultiply(row4, 2);
    }
};


/*  
    The InvMixColumn is also a matrix multiplication in the Galois field GF(2^8) between the state and another fixed matrix.

    out0  out4  out8  out12      14 11 13  9     in0  in4  in8  in12
    out1  out5  out9  out13   =  09 14 11 13  .  in1  in5  in9  in13
    out2  out6  out10 out14      13 09 14 11     in2  in6  in10 in14
    out3  out7  out11 out15      11 13 09 14     in3  in7  in11 in15

    (fixed matrix values are in decimal)

*/

void AES::InvMixColumns (uint8_t state[Nb][Nl]){

    uint8_t row1, row2, row3, row4;
    
    for (uint8_t i = 0; i < Nb; i++){

        row1 = state[i][0];
        row2 = state[i][1];
        row3 = state[i][2];
        row4 = state[i][3];
        
        state[i][0] = this->GMultiply(row1, 14) ^ this->GMultiply(row2, 11) ^ this->GMultiply(row3, 13) ^ this->GMultiply(row4,  9);
        state[i][1] = this->GMultiply(row1,  9) ^ this->GMultiply(row2, 14) ^ this->GMultiply(row3, 11) ^ this->GMultiply(row4, 13);
        state[i][2] = this->GMultiply(row1, 13) ^ this->GMultiply(row2,  9) ^ this->GMultiply(row3, 14) ^ this->GMultiply(row4, 11);
        state[i][3] = this->GMultiply(row1, 11) ^ this->GMultiply(row2, 13) ^ this->GMultiply(row3,  9) ^ this->GMultiply(row4, 14);
    }
};