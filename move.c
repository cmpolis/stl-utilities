// Chris Polis
// move.c - A tool for moving an STL model from the command line
//
// Usage: $ move [in] [out] [x] [y] [z]

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>

#include "stl_util.h"
#include "stl_io.h"

int main(int argc, char *argv[]) {

  if(argc != 6) {
    printf("Usage: $ move [x] [y] [z]\n");
    return 1;
  }

  FILE *infile = fopen(argv[1], "r");
  FILE *outfile = fopen(argv[2], "w");
  float x = atof(argv[3]);
  float y = atof(argv[4]);
  float z = atof(argv[5]);
  int ndx, triCount = 0;
  stl_tri tempTri;

  if(getFileMode(infile) == ASCII) {
    readASCIIHeader(infile);
    writeHeaderAscii(outfile);
    while(readTriASCII(infile, &tempTri)) {
      translateTri(&tempTri, x, y, z);
      writeTriASCII(outfile, &tempTri);
    }

  } else {
    triCount = readBinaryHeader(infile);
    writeHeaderBin(outfile, triCount);
    for(ndx = 0; ndx < triCount; ndx++) {
      readTriBin(infile, &tempTri);
      translateTri(&tempTri, x, y, z);
      writeTriBin(outfile, &tempTri);
    }

  }

  fclose(infile);
  fclose(outfile);

  return 0;
}
