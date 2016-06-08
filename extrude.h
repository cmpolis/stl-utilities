// Chris Polis
// extrude.h - A tool for converting 2D images into 3D objects
//
// Usage: $ extrude [input file (.png)] [output (.stl)] [options]
// Options: 
//    --binary | --ascii                 STL output in binary or ASCII format
//    --extrude | cut | semicut |overlay Extrusion yype
//    --width [#]                        STL object width
//    --height [#]                       STL object height
//    --depth [#]                        Extrusion depth 
//    --base [#]                         Base depth 
//    --addto [filename]                 Add to existing STL 
//    --invert                           Invert black/white on 2D img 

void parseArgs(int argc, char *argv[]);
int copyTemplate(FILE *out, char *filename);
void parsePNG(FILE *png, char *data, int size);
void parseHMP(FILE *hmp, char *data, int size, int iWidth);
void invertData(char *data, int size);
int simpleExtrude(FILE *out, char *data);
int complexExtrude(stl_tri *tris, char *data);
int addBase(FILE *out);
