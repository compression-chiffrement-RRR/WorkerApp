#include "debug.h"

#ifdef AES_DEBUG

using namespace std;

#include <iostream>
#include "aes.h"

void DisplayWord(uint8_t *word){
    for (int i = 0; i < 4; i ++)
        printf("%02x", word[i]);
};

void DisplayState(uint8_t state[Nb][Nl]){
    for (int i = 0; i < 4; i ++){
        cout << endl;
        for (int y = 0; y < 4; y ++){
            printf("%02x ", state[y][i]);
        }
    }
};

#endif

#ifdef DEBUG

void DisplayBuffer(uint8_t *block, size_t length){
    for (size_t i = 0; i < length; i ++){
        if (i != 0 && i % 16  == 0)
            cout << endl;
        printf("0x%02x ", block[i]);
    }
    cout << endl;
};

#endif
