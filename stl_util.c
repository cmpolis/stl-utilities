// Chris Polis
// stl_util.c - library for creating and editing STL files 

#include <string.h>
#include "stl_util.h"

// Leg of 45/45/90 tri with c=1
# define LEG45 0.70710678118 //= sqrt(2.0)/2

//////////////////////////////////////////////////////
// Face/solid construction
//////////////////////////////////////////////////////
void createRectPrism(stl_tri *tris, float *root, float x, float y, float z) {
  float vertex_OOO[] = {root[0]  , root[1]  , root[2]  }; // (0, 0, 0)
  float vertex_XOO[] = {root[0]+x, root[1]  , root[2]  }; // (x, 0, 0)
  float vertex_OYO[] = {root[0]  , root[1]+y, root[2]  }; // (0, y, 0)
  float vertex_OOZ[] = {root[0]  , root[1]  , root[2]+z}; // (0, 0, z)
  float vertex_XYO[] = {root[0]+x, root[1]+y, root[2]  }; // (x, y, 0)
  float vertex_XOZ[] = {root[0]+x, root[1]  , root[2]+z}; // (x, 0, z)
  float vertex_OYZ[] = {root[0]  , root[1]+y, root[2]+z}; // (0, y, z)
  float vertex_XYZ[] = {root[0]+x, root[1]+y, root[2]+z}; // (x, y, z)
  
  createXYFace(&tris[0],  vertex_OOO, vertex_XYO, V_NZ); // bottom
  createXYFace(&tris[2],  vertex_OOZ, vertex_XYZ, V_PZ); // top
  createYZFace(&tris[4],  vertex_OOO, vertex_OYZ, V_NX); // left
  createYZFace(&tris[6],  vertex_XOO, vertex_XYZ, V_PX); // right
  createXZFace(&tris[8],  vertex_OOO, vertex_XOZ, V_NY); // front
  createXZFace(&tris[10], vertex_OYO, vertex_XYZ, V_PY); // back
}


// SKIP
stl_tri* createSphere(float *center, float r, int granularity);
stl_tri* createCylinder(float *center, float r, float h, int granularity);

//
// A--(c)
// | / |
//(d)-(b)
void createXYFace(stl_tri *tris, float *vertexA, float *vertexB, float *normal) {
  float vertexC[3] = { vertexA[0], vertexB[1], vertexA[2] };
  float vertexD[3] = { vertexB[0], vertexA[1], vertexA[2] };
  if(normal[2] != V_PZ[2]) { // bottom 
    tris[0] = (stl_tri) {
      {vertexA[0], vertexA[1], vertexA[2]},
      {vertexC[0], vertexC[1], vertexC[2]},
      {vertexD[0], vertexD[1], vertexD[2]},
      {normal[0], normal[1], normal[2]} };
    tris[1] = (stl_tri) {
      {vertexB[0], vertexB[1], vertexB[2]},
      {vertexD[0], vertexD[1], vertexD[2]},
      {vertexC[0], vertexC[1], vertexC[2]},
      {normal[0], normal[1], normal[2]} };

  } else {            // top
    tris[0] = (stl_tri) {
      {vertexA[0], vertexA[1], vertexA[2]},
      {vertexD[0], vertexD[1], vertexD[2]},
      {vertexC[0], vertexC[1], vertexC[2]},
      {normal[0], normal[1], normal[2]} };
    tris[1] = (stl_tri) {
      {vertexB[0], vertexB[1], vertexB[2]},
      {vertexC[0], vertexC[1], vertexC[2]},
      {vertexD[0], vertexD[1], vertexD[2]},
      {normal[0], normal[1], normal[2]} };
  }
}

void createXZFace(stl_tri *tris, float *vertexA, float *vertexB, float *normal) {
  float vertexC[3] = { vertexA[0], vertexA[1], vertexB[2] };
  float vertexD[3] = { vertexB[0], vertexA[1], vertexA[2] };
  if(normal[1] == V_PY[1]) { // back
    tris[0] = (stl_tri) {
      {vertexA[0], vertexA[1], vertexA[2]},
      {vertexC[0], vertexC[1], vertexC[2]},
      {vertexD[0], vertexD[1], vertexD[2]},
      {normal[0], normal[1], normal[2]} };
    tris[1] = (stl_tri) {
      {vertexB[0], vertexB[1], vertexB[2]},
      {vertexD[0], vertexD[1], vertexD[2]},
      {vertexC[0], vertexC[1], vertexC[2]},
      {normal[0], normal[1], normal[2]} };

  } else {            // front
    tris[0] = (stl_tri) {
      {vertexA[0], vertexA[1], vertexA[2]},
      {vertexD[0], vertexD[1], vertexD[2]},
      {vertexC[0], vertexC[1], vertexC[2]},
      {normal[0], normal[1], normal[2]} };
    tris[1] = (stl_tri) {
      {vertexB[0], vertexB[1], vertexB[2]},
      {vertexC[0], vertexC[1], vertexC[2]},
      {vertexD[0], vertexD[1], vertexD[2]},
      {normal[0], normal[1], normal[2]} };
  }
}

void createYZFace(stl_tri *tris, float *vertexA, float *vertexB, float *normal) {
  float vertexC[3] = { vertexA[0], vertexA[1], vertexB[2] };
  float vertexD[3] = { vertexA[0], vertexB[1], vertexA[2] };
  if(normal[0] != V_PX[0]) { // right
    tris[0] = (stl_tri) {
      {vertexA[0], vertexA[1], vertexA[2]},
      {vertexC[0], vertexC[1], vertexC[2]},
      {vertexD[0], vertexD[1], vertexD[2]},
      {normal[0], normal[1], normal[2]} };
    tris[1] = (stl_tri) {
      {vertexB[0], vertexB[1], vertexB[2]},
      {vertexD[0], vertexD[1], vertexD[2]},
      {vertexC[0], vertexC[1], vertexC[2]},
      {normal[0], normal[1], normal[2]} };

  } else {            // left
    tris[0] = (stl_tri) {
      {vertexA[0], vertexA[1], vertexA[2]},
      {vertexD[0], vertexD[1], vertexD[2]},
      {vertexC[0], vertexC[1], vertexC[2]},
      {normal[0], normal[1], normal[2]} };
    tris[1] = (stl_tri) {
      {vertexB[0], vertexB[1], vertexB[2]},
      {vertexC[0], vertexC[1], vertexC[2]},
      {vertexD[0], vertexD[1], vertexD[2]},
      {normal[0], normal[1], normal[2]} };
  }
}

//////////////////////////////////////////////////////
// 3D solid operations
//////////////////////////////////////////////////////

void translateTri(stl_tri *tri, float x, float y, float z) {
  tri->vertexA[0] += x;
  tri->vertexA[1] += y;
  tri->vertexA[2] += z;
  
  tri->vertexB[0] += x;
  tri->vertexB[1] += y;
  tri->vertexB[2] += z;

  tri->vertexC[0] += x;
  tri->vertexC[1] += y;
  tri->vertexC[2] += z;
}

// Scale (float)
//void scaleSolid(float scale, int triCount, stl_tri *tris);

// Rotate (theta, phi)
//void rotateSolid(float theta, float phi, int triCount, stl_tri *tris);

// Move (x, y, z)
//void moveSolid(float x, float y, float z, int triCount, stl_tri *tris);

// Get Bounding Box -> xMin, xMax, yMin, yMax, zMin, zMax
//bounding_box getBoundingBox(int triCount, stl_tri *tris);

// SKIP: Get Volume -> units^3


///////////////////////////////////////////////////////
// Misc
///////////////////////////////////////////////////////
char *extrusionModeString(extrusion_mode mode) {
  if(mode == EXTRUDE)
    return "Extrude";
  else if(mode == CUT)
    return "Cut";
  else if(mode == SUNKEN)
    return "Sunken";
  else if(mode == RELIEF)
    return "Relief";
  else
    return "Unknown extrusion mode";
}
