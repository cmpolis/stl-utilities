// Chris Polis
// extrude.c - A tool for converting 2D images into 3D objects
//
// Usage: $ extrude [input file (.png)] [width(px)] [height(px)] [output (.stl)] [options]
// Options: 
//    --binary | --ascii                 STL output in binary or ASCII format
//    --extrude | cut | sunken | relief  Extrusion type
//    --width [#]                        STL object width
//    --height [#]                       STL object height
//    --depth [#]                        Extrusion depth 
//    --base [#]                         Base depth 
//    --addto [filename]                 Add to existing STL 
//    --invert                           Invert black/white on 2D img 
//    --flip                             Flip image horizontally
//
// Examples:
//  - generate iPhone 4 case:
//  $ make clean && make extrude && time ./extrude ../cp_xlarge.hmp 2200 3200 testExt.stl --depth 1.5 --width 54.2 --height 78.8 --ascii --addto ../iphonev5.stl
// 

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>

#include "stl_util.h"
#include "stl_io.h"

#define TRI_ALLOC_SIZE 20000
// Access 1D array as a 2D array
#define DATA(r, c) (data[(r)*pxWidth + (c)])


// Defaults
stl_mode       output_mode                    = BINARY;
extrusion_mode extrude_mode                   = EXTRUDE;
int            invert                         = 0;
int            flip                           = 0;
float          depth                          = 10.0;
float          width                          = 10.0;
float          height                         = 10.0;
float          base                           = 0.0;
float          xScale                         = 1.0;
float          yScale                         = 1.0;
float          zScale                         = 1.0;
char           *addTo                         = NULL;

// Options
static const char *optString = "yzecsrw:d:h:b:a:i:";
static const struct option longOpts[] = {
    { "binary",  no_argument,       NULL, 'B' },
    { "ascii",   no_argument,       NULL, 'A' },
    { "extrude", no_argument,       NULL, 'e' },
    { "cut",     no_argument,       NULL, 'c' },
    { "sunken",  no_argument,       NULL, 's' },
    { "relief",  no_argument,       NULL, 'r' },
    { "width",   required_argument, NULL, 'w' },
    { "depth",   required_argument, NULL, 'd' },
    { "height",  required_argument, NULL, 'h' },
    { "base",    required_argument, NULL, 'b' },
    { "addto",   required_argument, NULL, 'a' },
    { "invert",  no_argument,       NULL, 'i' },
    { "flip",    no_argument,       NULL, 'f' },
    { NULL,      no_argument,       NULL, 0 }
};

void parseArgs(int argc, char *argv[]) {
  int longIndex;
  int opt = getopt_long( argc, argv, optString, longOpts, &longIndex );
  while( opt != -1 ) {
    switch( opt ) {
      case 'B': output_mode = BINARY; break;
      case 'A': output_mode = ASCII;  break;
      case 'e': extrude_mode = EXTRUDE; break;
      case 'c': extrude_mode = CUT; break;
      case 's': extrude_mode = SUNKEN; break;
      case 'r': extrude_mode = RELIEF; break;
      case 'w': width = atof(optarg); break;
      case 'd': depth = atof(optarg); break;
      case 'h': height = atof(optarg); break;
      case 'b': base = atof(optarg); break;
      case 'f': flip = 1; break;
      case 'a':
         addTo = malloc(sizeof(char) * (strlen(optarg) + 1));
        strcpy(addTo, optarg);
        break;
      case 'i': invert = 1; break;
      default: break;
    }        
    opt = getopt_long( argc, argv, optString, longOpts, &longIndex );
  }
}

void printState(char *dest, char *source, int iWidth, int iHeight) {
  printf("********** EXTRUDING **********\n"); 
  printf("source (png or hmp): %s (%dx%d)\n", source, iWidth, iHeight);
  printf("invert source      : %s\n", invert ? "true" : "false");
  printf("template (stl)     : %s\n", addTo ? addTo : "none");
  printf("dest (stl)         : %s (%s)\n", dest, (output_mode == ASCII ? "ASCII" : "Binary"));
  printf("extrusion type     : %s\n", extrusionModeString(extrude_mode));
  printf("output dimensions  : %f x %f x %f(+ %f base)\n", width, height, depth, base);
  printf("dimension scaling  : x: %f y: %f z: %f\n", xScale, yScale, zScale);
}

// copy STL data from template into output file, return tri count
int copyTemplate(FILE *out, char *filename) {
  FILE *template = fopen(filename, "r");
  int triCount, ndx;
  stl_tri tempTri;

  if(!filename)
    return 0;

  if(getFileMode(template) == ASCII) {
    readASCIIHeader(template);
    while(readTriASCII(template, &tempTri)) {
      if(output_mode == ASCII)
        writeTriASCII(out, &tempTri);
      else
        writeTriBin(out, &tempTri);
      triCount++;
    }

  } else {
    triCount = readBinaryHeader(template);
    for(ndx = 0; ndx < triCount; ndx++) {
      readTriBin(template, &tempTri);
      if(output_mode == ASCII)
        writeTriASCII(out, &tempTri);
      else
        writeTriBin(out, &tempTri);
    }
  }
  fclose(template);
  return triCount;
}

void parsePNG(FILE *png, char *data, int size, int invert) {

}

void parseHMP(FILE *hmp, char *data, int size, int invert, int iWidth) {
  int ndx;
  fread(data, 1, size, hmp);

  for(ndx = 0; ndx < size; ndx++)
    data[ndx] = (data[ndx] == '0') ? 0 : 1;

  if(flip) {
    printf("flipping...\n");
    char *newData = (char*)malloc(sizeof(char) * size);
    for(ndx = 0; ndx < size; ndx++) {
      //int revNdx = (iWidth - (ndx % iWidth) - 1) + (iWidth * (ndx / iWidth);
      newData[ndx] = data[(iWidth - (ndx % iWidth) - 1) + (iWidth * (ndx / iWidth))];
    }
    data = newData;
  }
}

void invertData(char *data, int size) {
  int ndx;
  for(ndx = 0; ndx < size; ndx++)
    data[ndx] = (data[ndx] == 0) ? 1 : 0;
}

int addBase(FILE *out) {
  if(base == 0.0f)
    return 0;

  stl_tri tris[12];
  float root[3] = { 0.0f, 0.0f, 0.0f };
  createRectPrism(&tris, root, width, height, base);
  if(output_mode == BINARY)
    writeTriArrayBin(out, 12, &tris);
  else
    writeTriArrayASCII(out, 12, &tris);
  return 4;
}

void writeFace(FILE *out, stl_tri *tris) {
  if(output_mode == BINARY) {
    writeTriBin(out, &tris[0]);
    writeTriBin(out, &tris[1]);
  } else {
    writeTriASCII(out, &tris[0]);
    writeTriASCII(out, &tris[1]);
  }
}

void writeYZFace(FILE *out, stl_tri *tris, int col, int startRow, int endRow, float *normal) {
  float tempA[3] = { col * xScale, startRow * yScale, base };
  float tempB[3] = { col * xScale, endRow * yScale, base + zScale };
                
  createYZFace(tris, tempA, tempB, normal);
  writeFace(out, tris);
}
void writeXZFace(FILE *out, stl_tri *tris, int row, int startCol, int endCol, float *normal) {
  float tempA[3] = { startCol * xScale, row * yScale, base };
  float tempB[3] = { endCol * xScale, row * yScale, base + zScale };
                
  createXZFace(tris, tempA, tempB, normal);
  writeFace(out, tris);
}
void writeXYFace(FILE *out, stl_tri *tris, int startCol, int endCol, int startRow, int endRow, float z, float *normal) {
  float tempA[3] = { startCol * xScale, startRow * yScale, base + z };
  float tempB[3] = { endCol * xScale, endRow * yScale, base + z };
                
  createXYFace(tris, tempA, tempB, normal);
  writeFace(out, tris);
}


// Extrude heightmap(data), return # of triangles
int simpleExtrude(FILE *out, char *data, int pxWidth, int pxHeight) {
  int ndx, zCol, zRow, colNdx, rowNdx, startRow, endRow, startCol, endCol;
  int triCount = 0;
  stl_tri *tempTris = malloc(sizeof(stl_tri) * 2);

  // Check Y borders -> generate YZ faces
  for(colNdx = 0; colNdx < (pxWidth - 1); colNdx++) {
    rowNdx = 0;
  
    while(rowNdx < pxHeight) {
      if(DATA(rowNdx, colNdx) && !DATA(rowNdx, colNdx+1)) { //1|0
        startRow = rowNdx;
        endRow = rowNdx++;
        while(DATA(rowNdx, colNdx) && !DATA(rowNdx, colNdx+1))
          endRow = rowNdx++;
        
        writeYZFace(out, tempTris, (colNdx+1), startRow, endRow+1, V_PX);
        triCount += 2;

      } else if(!DATA(rowNdx, colNdx) && DATA(rowNdx, colNdx+1)) { //0|1
        startRow = rowNdx;
        endRow = rowNdx++;
        while(!DATA(rowNdx, colNdx) && DATA(rowNdx, colNdx+1)) //0|1
          endRow = rowNdx++;
        
        writeYZFace(out, tempTris, (colNdx+1), startRow, endRow+1, V_NX);
        triCount += 2;
      }
      rowNdx++;
    }
  }

  // Check X borders -> generate XZ faces
  for(rowNdx = 0; rowNdx < (pxHeight - 1); rowNdx++) {
    colNdx = 0;
  
    while(colNdx < pxWidth) {
      if(DATA(rowNdx, colNdx) && !DATA(rowNdx+1, colNdx)) { //1/0
        startCol = colNdx;
        endCol = colNdx++;
        while(DATA(rowNdx, colNdx) && !DATA(rowNdx+1, colNdx)) {
          endCol = colNdx++;
        }
        writeXZFace(out, tempTris, rowNdx+1, startCol, endCol+1, V_PY);
        triCount += 2;

      } else if(!DATA(rowNdx, colNdx) && DATA(rowNdx+1, colNdx)) { //0/1
        startCol = colNdx;
        endCol = colNdx++;
        while(!DATA(rowNdx, colNdx) && DATA(rowNdx+1, colNdx)) { //0/1
          endCol = colNdx++;
        }
        writeXZFace(out, tempTris, rowNdx+1, startCol, endCol+1, V_NY);
        triCount += 2;
      }
      colNdx++;

    }
  }

  // Check top/bottom -> generate XY faces
  for(rowNdx = 0; rowNdx < pxHeight; rowNdx++) {
    colNdx = 0;
    while(colNdx < pxWidth) {
      if(DATA(rowNdx,colNdx) == 1) {
        startCol = colNdx;
        endCol = colNdx;
        startRow = rowNdx;
        endRow = rowNdx;

        // try to extend in x direction
        colNdx++;
        while(colNdx < pxWidth && DATA(rowNdx, colNdx))
          endCol = colNdx++;

        // try to extend in y direction
        while((endRow + 1) < pxHeight) {
          int allOnes = 1;
          for(ndx = startCol; ndx <= endCol; ndx++) {
            if(!DATA((endRow + 1), ndx)) {
              allOnes = 0;
              break;
            }
          }
          if(allOnes)
            endRow++;
          else
            break;
        }

        // zero out
        for(zCol = startCol; zCol <= endCol; zCol++)
          for(zRow = startRow; zRow <= endRow; zRow++)
            DATA(zRow, zCol) = 0;

        writeXYFace(out, tempTris, startCol, endCol+1, startRow, endRow+1, 0.0f, V_NZ);
        writeXYFace(out, tempTris, startCol, endCol+1, startRow, endRow+1, zScale, V_PZ);
        triCount += 4;
      }
      colNdx++;
    }
  }

  free(tempTris);
  return triCount;
}

int complexExtrude(stl_tri *tris, char *data) {
  return 0;
}

int main(int argc, char *argv[]) {
  if(argc < 4) {
    printf("Usage: $ extrude [input file (.png)] [width(px)] [height(px)] [output (.stl)] [options]\n");
    return 1;
  }

  char *dest = argv[4];
  char *source = argv[1];
  int imgWidth = atoi(argv[2]);
  int imgHeight = atoi(argv[3]); 
  int triCount = 0;
  int pxCount; //, extTriCount;
  char *data;
  
  // Open files
  FILE *in = fopen(source, "r");
  FILE *out = fopen(dest, "w");

  // Parse arguments
  parseArgs(argc, argv);
  pxCount = imgWidth * imgHeight;
  width = (width == 10.0) ? imgWidth : width;
  height = (height == 10.0) ? imgHeight : height;
  zScale = depth;
  xScale = width / imgWidth;
  yScale = height / imgHeight;
  printState(dest, source, imgWidth, imgHeight);
  
  // Write header
  if(output_mode == ASCII)
    writeHeaderAscii(out);
  else
    writeHeaderBin(out, 0);

  // Copy in template
  triCount = copyTemplate(out, addTo);
  
  // Parse (.hmp or .png)
  data = malloc(sizeof(char) * pxCount);
  if(strstr(source, ".hmp"))
    parseHMP(in, data, pxCount, invert, imgWidth);
  else
    parsePNG(in, data, pxCount, invert);

  // Invert if necessary
  if(invert)
    invertData(data, pxCount);

  // Simple or complex extrude
  triCount += simpleExtrude(out, data, imgWidth, imgHeight);

  // Add Base
  triCount += addBase(out);

  // Add ascii header or set tri count for binary
  if(output_mode == ASCII)
    writeFooterAscii(out);
  else
    setTriCount(out, triCount);

  // Free and close
  fclose(in);
  fclose(out);
  free(data);

  return 0;
}
