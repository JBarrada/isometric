#ifndef smallfontH
#define smallfontH

#ifndef SMfgTypes
#define SMfgTypes

#include <stdint.h>

#define b2b(b7,b6,b5,b4,b3,b2,b1,b0) ((uint8_t)((b7)*128u + (b6)*64u + (b5)*32u + (b4)*16u + (b3)*8u + (b2)*4u + (b1)*2u + (b0)))

typedef uint8_t TCDATA;
typedef TCDATA* TCLISTP;

#endif

extern TCLISTP smallfont[256];

#endif
