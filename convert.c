// Chris Polis
// convert.c - A tool to convert STL files between ASCII and binary encoding
// 
// Usage: $ extrude [input (.stl)] [output (.stl)]

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>

#include "stl_util.h"
#include "stl_io.h"

int main(int argc, char *argv[]) {

  if(argc != 3) {
    printf("Usage: $ convert [input (.stl)] [output (.stl)]\n");
    return 1;
  }

  int ndx;
  stl_tri tempTri;
  uint32_t triCount = 0;
  FILE *infile = fopen(argv[1], "r");
  FILE *outfile = fopen(argv[2], "w");

  // Check if it is binary or ascii
  if(getFileMode(infile) == ASCII) {
    printf("Detected ASCII input, converting to binary...\n");

    readASCIIHeader(infile);
    writeHeaderBin(outfile, 0);
    while(readTriASCII(infile, &tempTri)) {
      writeTriBin(outfile, &tempTri);
      triCount++;
    }
    setTriCount(outfile, triCount);

  } else {
    printf("Detected BINARY input, converting to ascii...\n");

    triCount = readBinaryHeader(infile);
    writeHeaderAscii(outfile);
    for(ndx = 0; ndx < triCount; ndx++) {
      readTriBin(infile, &tempTri);
      writeTriASCII(outfile, &tempTri);
    }
    writeFooterAscii(outfile);
  }

  fclose(infile);
  fclose(outfile);

  return 0;
}
