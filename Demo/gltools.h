﻿#ifndef GLTOOLS_H
#define GLTOOLS_H

#include <QOpenGLFunctions_3_3_Compatibility>

// There is a static block allocated for loading shaders to
// prevent heap fragmentation
#define MAX_SHADER_LENGTH   8192

#include <cstdio>
#include <cmath>
#include "math3d.h"
#include "glbatch.h"
#include "gltrianglebatch.h"

///////////////////////////////////////////////////////
// Macros for big/little endian happiness
// These are intentionally written to be easy to understand what they
// are doing... no flames please on the inefficiency of these.
#ifdef __BIG_ENDIAN__
///////////////////////////////////////////////////////////
// This function says, "this pointer is a little endian value"
// If the value must be changed it is... otherwise, this
// function is defined away below (on Intel systems for example)
inline void LITTLE_ENDIAN_WORD(void *pWord)
{
    unsigned char *pBytes = (unsigned char *)pWord;
    unsigned char temp;

    temp = pBytes[0];
    pBytes[0] = pBytes[1];
    pBytes[1] = temp;
}

///////////////////////////////////////////////////////////
// This function says, "this pointer is a little endian value"
// If the value must be changed it is... otherwise, this
// function is defined away below (on Intel systems for example)
inline void LITTLE_ENDIAN_DWORD(void *pWord)
{
    unsigned char *pBytes = (unsigned char *)pWord;
    unsigned char temp;

    // Swap outer bytes
    temp = pBytes[3];
    pBytes[3] = pBytes[0];
    pBytes[0] = temp;

    // Swap inner bytes
    temp = pBytes[1];
    pBytes[1] = pBytes[2];
    pBytes[2] = temp;
}
#else

// Define them away on little endian systems
#define LITTLE_ENDIAN_WORD
#define LITTLE_ENDIAN_DWORD
#endif

class GLTools : protected QOpenGLFunctions_3_3_Compatibility
{
public:
    GLTools();

///////////////////////////////////////////////////////////////////////////////
//         THE LIBRARY....
///////////////////////////////////////////////////////////////////////////////
    // Get the OpenGL version
    static void gltGetOpenGLVersion(GLint &nMajor, GLint &nMinor);

    // Check to see if an exension is supported
    static int gltIsExtSupported(const char *szExtension);

    // Set working directoyr to /Resources on the Mac
    static void gltSetWorkingDirectory(const char *szArgv);

    ///////////////////////////////////////////////////////////////////////////////
    static GLbyte* gltReadBMPBits(const char *szFileName, int *nWidth, int *nHeight);

    /////////////////////////////////////////////////////////////////////////////////////
    // Load a .TGA file
    static GLbyte *gltReadTGABits(const char *szFileName, GLint *iWidth, GLint *iHeight, GLint *iComponents, GLenum *eFormat);

    // Capture the frame buffer and write it as a .tga
    // Does not work on the iPhone
    #ifndef OPENGL_ES
    static GLint gltGrabScreenTGA(const char *szFileName);
    #endif


    // Make Objects
    static void gltMakeTorus(GLTriangleBatch& torusBatch, GLfloat majorRadius, GLfloat minorRadius, GLint numMajor, GLint numMinor);
    static void gltMakeSphere(GLTriangleBatch& sphereBatch, GLfloat fRadius, GLint iSlices, GLint iStacks);
    static void gltMakeDisk(GLTriangleBatch& diskBatch, GLfloat innerRadius, GLfloat outerRadius, GLint nSlices, GLint nStacks);
    static void gltMakeCylinder(GLTriangleBatch& cylinderBatch, GLfloat baseRadius, GLfloat topRadius, GLfloat fLength, GLint numSlices, GLint numStacks);
    static void gltMakeCube(GLBatch& cubeBatch, GLfloat fRadius);

    // Shader loading support
    static void	gltLoadShaderSrc(const char *szShaderSrc, GLuint shader);
    static bool	gltLoadShaderFile(const char *szFile, GLuint shader);

    static GLuint	gltLoadShaderPair(const char *szVertexProg, const char *szFragmentProg);
    static GLuint   gltLoadShaderPairWithAttributes(const char *szVertexProg, const char *szFragmentProg, ...);

    static GLuint gltLoadShaderPairSrc(const char *szVertexSrc, const char *szFragmentSrc);
    static GLuint gltLoadShaderPairSrcWithAttributes(const char *szVertexSrc, const char *szFragmentSrc, ...);

    static bool gltCheckErrors(GLuint progName = 0);
    static void gltGenerateOrtho2DMat(GLuint width, GLuint height, M3DMatrix44f &orthoMatrix, GLBatch &screenQuad);

};

///////////////////////////////////////////////////////////////////////////////
//         THE LIBRARY....
///////////////////////////////////////////////////////////////////////////////

// Get the OpenGL version
void gltGetOpenGLVersion(GLint &nMajor, GLint &nMinor);

// Check to see if an exension is supported
int gltIsExtSupported(const char *szExtension);

// Set working directoyr to /Resources on the Mac
void gltSetWorkingDirectory(const char *szArgv);

///////////////////////////////////////////////////////////////////////////////
GLbyte* gltReadBMPBits(const char *szFileName, int *nWidth, int *nHeight);

/////////////////////////////////////////////////////////////////////////////////////
// Load a .TGA file
GLbyte *gltReadTGABits(const char *szFileName, GLint *iWidth, GLint *iHeight, GLint *iComponents, GLenum *eFormat);

// Capture the frame buffer and write it as a .tga
// Does not work on the iPhone
#ifndef OPENGL_ES
GLint gltGrabScreenTGA(const char *szFileName);
#endif


// Make Objects
void gltMakeTorus(GLTriangleBatch& torusBatch, GLfloat majorRadius, GLfloat minorRadius, GLint numMajor, GLint numMinor);
void gltMakeSphere(GLTriangleBatch& sphereBatch, GLfloat fRadius, GLint iSlices, GLint iStacks);
void gltMakeDisk(GLTriangleBatch& diskBatch, GLfloat innerRadius, GLfloat outerRadius, GLint nSlices, GLint nStacks);
void gltMakeCylinder(GLTriangleBatch& cylinderBatch, GLfloat baseRadius, GLfloat topRadius, GLfloat fLength, GLint numSlices, GLint numStacks);
void gltMakeCube(GLBatch& cubeBatch, GLfloat fRadius);

// Shader loading support
void	gltLoadShaderSrc(const char *szShaderSrc, GLuint shader);
bool	gltLoadShaderFile(const char *szFile, GLuint shader);

GLuint	gltLoadShaderPair(const char *szVertexProg, const char *szFragmentProg);
//GLuint   gltLoadShaderPairWithAttributes(const char *szVertexProg, const char *szFragmentProg, ...);

GLuint gltLoadShaderPairSrc(const char *szVertexSrc, const char *szFragmentSrc);
//GLuint gltLoadShaderPairSrcWithAttributes(const char *szVertexProg, const char *szFragmentProg, ...);

bool gltCheckErrors(GLuint progName = 0);
void gltGenerateOrtho2DMat(GLuint width, GLuint height, M3DMatrix44f &orthoMatrix, GLBatch &screenQuad);

#endif // GLTOOLS_H
