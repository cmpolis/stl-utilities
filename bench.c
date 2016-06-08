// Chris Polis
// bench.c - Functions for testing and benchmarking stl_io and stl_util

#include <string.h>
#include <stdlib.h>

#include "stl_util.h"
#include "stl_io.h"

void writeBinTestCube(char *filename, int count) {
  FILE *out = fopen(filename, "w");
  writeHeaderBin(out, 12 * count);
  
  float root[3] = { 0.0, 0.0, 0.0 };
  stl_tri *tris = malloc(sizeof(stl_tri) * 12 * count);
  int ndx = 0;
  for(ndx = 0; ndx < count; ndx++) 
    createRectPrism(&tris[12 * ndx], root, 3.0, 2.5, 4.0);
  
  writeTriArrayBin(out, 12 * count, &tris[0]);
  free(tris);
  fclose(out);
}

void writeAsciiTestCube(char *filename, int count) {
  FILE *out = fopen(filename, "w");
  writeHeaderAscii(out);

  float root[3] = { 0.0, 0.0, 0.0 };
  stl_tri *tris = malloc(sizeof(stl_tri) * 12 * count);
  int ndx = 0;
  for(ndx = 0; ndx < count; ndx++)
    createRectPrism(&tris[12 * ndx], root, 3.0, 2.5, 4.0);

  writeTriArrayASCII(out, 12 * count, &tris[12 * ndx]);
  free(tris);
  fclose(out);
}

int main(int argc, char *argv[]) {
  writeBinTestCube("testcube.stl", 1);
  return 0;
}
