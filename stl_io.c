// Chris Polis
// stl_io.h - tools for input and output from STL files

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "stl_io.h"

//////////////////////////////////////////////////////
// Input
//////////////////////////////////////////////////////

// Get STL type
stl_mode getFileMode(FILE *in) {
  char firstChar;
  stl_mode mode;

  fseek(in, 0L, SEEK_SET);
  fread(&firstChar, 1, 1, in);
  mode = firstChar == 's' ? ASCII : BINARY;
  fseek(in, 0L, SEEK_SET);
  return mode;
}

// Read bin header -> triCount
uint32_t readBinaryHeader(FILE *in) {
  uint32_t triCount;

  fseek(in, 80L, SEEK_SET);
  fread(&triCount, 4, 1, in);
  return triCount;
}

// Read ASCII header
void readASCIIHeader(FILE *in) {
  char buffer[256];

  fseek(in, 0L, SEEK_SET);
  fgets(buffer, 256, in);
}

// Read tri from binary
void readTriBin(FILE *in, stl_tri *tri) {
  fread(&tri->normal[0], 4, 1, in);
  fread(&tri->normal[1], 4, 1, in);
  fread(&tri->normal[2], 4, 1, in);

  fread(&tri->vertexA[0], 4, 1, in);
  fread(&tri->vertexA[1], 4, 1, in);
  fread(&tri->vertexA[2], 4, 1, in);

  fread(&tri->vertexB[0], 4, 1, in);
  fread(&tri->vertexB[1], 4, 1, in);
  fread(&tri->vertexB[2], 4, 1, in);

  fread(&tri->vertexC[0], 4, 1, in);
  fread(&tri->vertexC[1], 4, 1, in);
  fread(&tri->vertexC[2], 4, 1, in);
  fseek(in, 2L, SEEK_CUR);
}

// Read tri from ASCII
int readTriASCII(FILE *in, stl_tri *tri) {
  char buffer[256];
  int dataSize;

  dataSize = fscanf(in, "%*[ \n\t]facet normal %E %E %E", &tri->normal[0], 
                                                          &tri->normal[1],
                                                          &tri->normal[2]);
  if(dataSize != 3)
    return 0;

  fgets(buffer, 256, in); // clear line
  fgets(buffer, 256, in); // 'outer loop'
  fscanf(in, "%*[ \n\t]vertex %E %E %E", &tri->vertexA[0], 
                                         &tri->vertexA[1],
                                         &tri->vertexA[2]);
  fgets(buffer, 256, in); //clear line
  fscanf(in, "%*[ \n\t]vertex %E %E %E", &tri->vertexB[0], 
                                         &tri->vertexB[1],
                                         &tri->vertexB[2]);
  fgets(buffer, 256, in); // clear line
  fscanf(in, "%*[ \n\t]vertex %E %E %E", &tri->vertexC[0], 
                                         &tri->vertexC[1],
                                         &tri->vertexC[2]);
  fgets(buffer, 256, in); // clear line
  fgets(buffer, 256, in); // 'endloop'
  fgets(buffer, 256, in); // 'endfacet'
  return 1;
}

//////////////////////////////////////////////////////
// Output
//////////////////////////////////////////////////////

// Write binary header
void writeHeaderBin(FILE *out, int triCount) {
  char filler[80];
  uint32_t count = triCount;

  memset(filler, 'z', 80);
  fwrite(filler, 80, 1, out);
  fwrite(&count, 4, 1, out);
}

// Set tri count in binary header
void setTriCount(FILE *out, int triCount) {
  uint32_t count = triCount;

  fseek(out, 80L, SEEK_SET);
  fwrite(&count, 4, 1, out);
}

// Write ascii header
void writeHeaderAscii(FILE *out) {
  fprintf(out, "solid\n");
}

// Write ascii footer
void writeFooterAscii(FILE *out) {
  fprintf(out, "endsolid\n");
}

// Write single tri in binary
void writeTriBin(FILE *out, stl_tri *tri) {
  char *filler = "zz";

  fwrite(&tri->normal[0], 4, 1, out);
  fwrite(&tri->normal[1], 4, 1, out);
  fwrite(&tri->normal[2], 4, 1, out);

  fwrite(&tri->vertexA[0], 4, 1, out);
  fwrite(&tri->vertexA[1], 4, 1, out);
  fwrite(&tri->vertexA[2], 4, 1, out);

  fwrite(&tri->vertexB[0], 4, 1, out);
  fwrite(&tri->vertexB[1], 4, 1, out);
  fwrite(&tri->vertexB[2], 4, 1, out);

  fwrite(&tri->vertexC[0], 4, 1, out);
  fwrite(&tri->vertexC[1], 4, 1, out);
  fwrite(&tri->vertexC[2], 4, 1, out);
  fwrite(filler, 2, 1, out);
}

// Write tri array in binary
// UNTESTED
void writeTriArrayBin(FILE *out, int triCount, stl_tri *tris) {
  int ndx;
  for(ndx = 0; ndx < triCount; ndx++)
    writeTriBin(out, &tris[ndx]);  
}

// Write single tri in ASCII
void writeTriASCII(FILE *out, stl_tri *tri) {
  fprintf(out, "  facet normal %E %E %E\n"
               "    outer loop\n"
               "      vertex %E %E %E\n"
               "      vertex %E %E %E\n"
               "      vertex %E %E %E\n"
               "    endloop\n"
               "  endfacet\n", 
               tri->normal[0],  tri->normal[1],  tri->normal[2],
               tri->vertexA[0], tri->vertexA[1], tri->vertexA[2],
               tri->vertexB[0], tri->vertexB[1], tri->vertexB[2],
               tri->vertexC[0], tri->vertexC[1], tri->vertexC[2]);
}

// Write tri array in ASCII
// UNTESTED
void writeTriArrayASCII(FILE *out, int triCount, stl_tri *tris) {
  int ndx;
  for(ndx = 0; ndx < triCount; ndx++)
    writeTriASCII(out, &tris[ndx]);  
}
