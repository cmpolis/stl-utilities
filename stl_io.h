// Chris Polis
// stl_io.h - tools for input and output from STL files

#include <stdio.h>
#include <stdint.h>
#include "stl_util.h"

typedef enum stl_mode_en {
  BINARY, 
  ASCII
} stl_mode;

//////////////////////////////////////////////////////
// Input
//////////////////////////////////////////////////////

// Get STL type
stl_mode getFileMode(FILE *in);

// Read bin header -> triCount
uint32_t readBinaryHeader(FILE *in);

// Read ASCII header
void readASCIIHeader(FILE *in);

// Read tri from binary
void readTriBin(FILE *in, stl_tri *tri);

// Read tri from ASCII
int readTriASCII(FILE *in, stl_tri *tri);

//////////////////////////////////////////////////////
// Output
//////////////////////////////////////////////////////

// Write binary header
void writeHeaderBin(FILE *out, int triCount);

// Set tri count in binary header
void setTriCount(FILE *out, int triCount);

// Write ascii header
void writeHeaderAscii(FILE *out);

// Write ascii footer
void writeFooterAscii(FILE *out);

// Write single tri in binary
void writeTriBin(FILE *out, stl_tri *tri);

// Write tri array in binary
void writeTriArrayBin(FILE *out, int triCount, stl_tri *tris);

// Write single tri in ASCII
void writeTriASCII(FILE *out, stl_tri *tri);

// Write tri array in ASCII
void writeTriArrayASCII(FILE *out, int triCount, stl_tri *tris);

