#pragma once

// AES DEBUG UTILITIES
#ifdef AES_DEBUG

using namespace std;

#include <iostream>
#include <stdint.h>
#include "aes.h"

void DisplayWord(uint8_t *word);
void DisplayState(uint8_t state[Nb][Nl]);


#define AES_DEBUG_MSG(msg) cout << msg << endl;

#define AES_DEBUG_WORD(msg, word) { \
    cout << msg << ": 0x"; \
    DisplayWord(word); \
    cout << endl; \
}

#define AES_DEBUG_STATE(msg, state) { \
    cout << msg << ": ";\
    DisplayState(state);\
    cout << endl; \
}

#else

#define AES_DEBUG_MSG(msg)
#define AES_DEBUG_WORD(word, debug)
#define AES_DEBUG_STATE(msg, word) 

#endif

#ifdef DEBUG

using namespace std;

#include <iostream>
#include <stdint.h>

void DisplayBuffer(uint8_t *buffer, size_t size);

#define DEBUG_BUFFER(msg, buffer, size) { \
    cout << msg << ": " << endl; \
    DisplayBuffer(buffer, size); \
    cout << endl; \
}

#define DEBUG_MSG(msg) cout << msg << endl;

#else

#define DEBUG_MSG(msg) 
#define DEBUG_BUFFER(msg, buffer, size)

#endif
