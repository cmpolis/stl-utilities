// Chris Polis
// stl_util.h - library for creating and editing STL files

#ifndef __include_stl_util
#define __include_stl_util

#include <sys/types.h>
#include <math.h>

// Leg of 45/45/90 tri with c=1
# define LEG45 0.70710678118 //= sqrt(2.0)/2

typedef enum corner_type_en {
  PXPY = 0,
  PXNY = 1,
  NXPY = 2,
  NXNY = 3
} corner_type;
#define CORNER_PX(type) (type <= 1)
#define CORNER_PY(type) (type % 2 == 0)

typedef enum corner_concavity_en {
  CXCY = 0,
  CXSY = 1, // i.e. Concave X, Straight Y
  SXCY = 2,
  SXSY = 3
} corner_concavity; 
#define CONCAVE_X(concavity) (concavity <= 1)
#define CONCAVE_Y(concavity) (concavity % 2 == 0)

// X,Y,Z unit vectors
static float V_PX[3] = { 1.0, 0.0, 0.0 };
static float V_NX[3] = { -1.0, 0.0, 0.0 };
static float V_PY[3] = { 0.0, 1.0, 0.0 };
static float V_NY[3] = { 0.0, -1.0, 0.0 };
static float V_PZ[3] = { 0.0, 0.0, 1.0 };
static float V_NZ[3] = { 0.0, 0.0, -1.0 };

// 45deg vectors
static float V_PXPY[3] = { LEG45, LEG45, 0.0 };
static float V_PXNY[3] = { LEG45, LEG45 * -1, 0.0 };
static float V_NXPY[3] = { LEG45 * -1, LEG45, 0.0 };
static float V_NXNY[3] = { LEG45 * -1, LEG45 * -1, 0.0 };


typedef enum extrusion_mode_en {
  EXTRUDE,
  CUT,
  SUNKEN,
  RELIEF
} extrusion_mode;

typedef struct vertex_st {
  float x, y, z;
} vertex;

typedef struct vector_st {
  float x, y, z;
} vector;

typedef struct stl_tri_st {
  float vertexA[3];
  float vertexB[3];
  float vertexC[3];
  float normal[3];
} stl_tri;

typedef struct bounding_box_st {
  float minX, maxX, minY, maxY, minZ, maxZ;
} bounding_box;


//////////////////////////////////////////////////////
// Face/solid construction
//////////////////////////////////////////////////////
void createRectPrism(stl_tri *tris, float *root, float x, float y, float z);
stl_tri* createSphere(float *center, float r, int granularity);
stl_tri* createCylinder(float *center, float r, float h, int granularity);

void createXYFace(stl_tri *tris, float *vertexA, float *vertexB, float *normal);
void createXZFace(stl_tri *tris, float *vertexA, float *vertexB, float *normal);
void createYZFace(stl_tri *tris, float *vertexA, float *vertexB, float *normal);

//////////////////////////////////////////////////////
// 3D solid operations
//////////////////////////////////////////////////////

// Move all points in a tri by x, y, z
void translateTri(stl_tri *tri, float x, float y, float z);

// Scale (float)
void scaleSolid(float scale, int triCount, stl_tri *tris);

// Rotate (theta, phi)
void rotateSolid(float theta, float phi, int triCount, stl_tri *tris);

// Move (x, y, z)
void moveSolid(float x, float y, float z, int triCount, stl_tri *tris);

// Get Bounding Box -> xMin, xMax, yMin, yMax, zMin, zMax
bounding_box getBoundingBox(int triCount, stl_tri *tris);

// SKIP: Get Volume -> units^3


//////////////////////////////////////////////
// Misc
//////////////////////////////////////////////
char *extrusionModeString(extrusion_mode mode);

#endif
