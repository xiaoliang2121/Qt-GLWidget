#include "glbatch.h"

GLBatch::GLBatch():
    nNumTextureUnits(0), nNumVerts(0), pVerts(NULL), pNormals(NULL), pColors(NULL), pTexCoords(NULL), uiVertexArray(0),
        uiNormalArray(0), uiColorArray(0), vertexArrayObject(0), bBatchDone(false), nVertsBuilding(0), uiTextureCoordArray(NULL)
{

}

GLBatch::~GLBatch()
{
    // Vertex buffer objects
    if(uiVertexArray != 0)
        glDeleteBuffers(1, &uiVertexArray);

    if(uiNormalArray != 0)
        glDeleteBuffers(1, &uiNormalArray);

    if(uiColorArray != 0)
        glDeleteBuffers(1, &uiColorArray);

    for(unsigned int i = 0; i < nNumTextureUnits; i++)
        glDeleteBuffers(1, &uiTextureCoordArray[i]);

    #ifndef OPENGL_ES
    glDeleteVertexArrays(1, &vertexArrayObject);
    #endif

    delete [] uiTextureCoordArray;
    delete [] pTexCoords;
}

/**
 * @brief GLBatch::Begin
 * Start the primitive batch.
 * @param primitive
 * @param nVerts
 * @param nTextureUnits
 */
void GLBatch::Begin(GLenum primitive, GLuint nVerts, GLuint nTextureUnits)
{
    primitiveType = primitive;
    nNumVerts = nVerts;

    if(nTextureUnits > 4)   // Limit to four texture units
        nTextureUnits = 4;

    nNumTextureUnits = nTextureUnits;

    if(nNumTextureUnits != 0) {
        uiTextureCoordArray = new GLuint[nNumTextureUnits];

        // An array of pointers to texture coordinate arrays
        pTexCoords = new M3DVector2f*[nNumTextureUnits];
        for(unsigned int i = 0; i < nNumTextureUnits; i++) {
            uiTextureCoordArray[i] = 0;
            pTexCoords[i] = NULL;
            }
        }

    // Vertex Array object for this Array
    #ifndef OPENGL_ES
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);
#endif
}

/**
 * @brief GLBatch::End
 * Bind everything up in a little package
 */
void GLBatch::End()
{
#ifndef OPENGL_ES
    // Check to see if items have been added one at a time
    if(pVerts != NULL) {
        glBindBuffer(GL_ARRAY_BUFFER, uiVertexArray);
        glUnmapBuffer(GL_ARRAY_BUFFER);
        pVerts = NULL;
        }

    if(pColors != NULL) {
        glBindBuffer(GL_ARRAY_BUFFER, uiColorArray);
        glUnmapBuffer(GL_ARRAY_BUFFER);
        pColors = NULL;
        }

    if(pNormals != NULL) {
        glBindBuffer(GL_ARRAY_BUFFER, uiNormalArray);
        glUnmapBuffer(GL_ARRAY_BUFFER);
        pNormals = NULL;
        }

    for(unsigned int i = 0; i < nNumTextureUnits; i++)
        if(pTexCoords[i] != NULL) {
            glBindBuffer(GL_ARRAY_BUFFER, uiTextureCoordArray[i]);
            glUnmapBuffer(GL_ARRAY_BUFFER);
            pTexCoords[i] = NULL;
            }

    // Set up the vertex array object
    glBindVertexArray(vertexArrayObject);
#endif

    if(uiVertexArray !=0) {
        glEnableVertexAttribArray(GLT_ATTRIBUTE_VERTEX);
        glBindBuffer(GL_ARRAY_BUFFER, uiVertexArray);
        glVertexAttribPointer(GLT_ATTRIBUTE_VERTEX, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }

    if(uiColorArray != 0) {
        glEnableVertexAttribArray(GLT_ATTRIBUTE_COLOR);
        glBindBuffer(GL_ARRAY_BUFFER, uiColorArray);
        glVertexAttribPointer(GLT_ATTRIBUTE_COLOR, 4, GL_FLOAT, GL_FALSE, 0, 0);
        }

    if(uiNormalArray != 0) {
        glEnableVertexAttribArray(GLT_ATTRIBUTE_NORMAL);
        glBindBuffer(GL_ARRAY_BUFFER, uiNormalArray);
        glVertexAttribPointer(GLT_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, 0);
        }

    // How many texture units
    for(unsigned int i = 0; i < nNumTextureUnits; i++)
        if(uiTextureCoordArray[i] != 0) {
            glEnableVertexAttribArray(GLT_ATTRIBUTE_TEXTURE0 + i),
            glBindBuffer(GL_ARRAY_BUFFER, uiTextureCoordArray[i]);
            glVertexAttribPointer(GLT_ATTRIBUTE_TEXTURE0 + i, 2, GL_FLOAT, GL_FALSE, 0, 0);
            }

    bBatchDone = true;
    #ifndef OPENGL_ES
    glBindVertexArray(0);
    #endif
}

/**
 * @brief GLBatch::CopyVertexData3f
 * Block Copy in vertex data
 * @param vVerts
 */
void GLBatch::CopyVertexData3f(M3DVector3f *vVerts)
{
    // First time, create the buffer object, allocate the space
    if(uiVertexArray == 0) {
        glGenBuffers(1, &uiVertexArray);
        glBindBuffer(GL_ARRAY_BUFFER, uiVertexArray);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * nNumVerts, vVerts, GL_DYNAMIC_DRAW);
        }
    else	{ // Just bind to existing object
        glBindBuffer(GL_ARRAY_BUFFER, uiVertexArray);

        // Copy the data in
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 3 * nNumVerts, vVerts);
        pVerts = NULL;
    }
}

/**
 * @brief GLBatch::CopyNormalDataf
 * Block copy in normal data
 * @param vNorms
 */
void GLBatch::CopyNormalDataf(M3DVector3f *vNorms)
{
    // First time, create the buffer object, allocate the space
    if(uiNormalArray == 0) {
        glGenBuffers(1, &uiNormalArray);
        glBindBuffer(GL_ARRAY_BUFFER, uiNormalArray);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * nNumVerts, vNorms, GL_DYNAMIC_DRAW);
        }
    else {	// Just bind to existing object
        glBindBuffer(GL_ARRAY_BUFFER, uiNormalArray);

        // Copy the data in
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 3 * nNumVerts, vNorms);
        pNormals = NULL;
    }
}

void GLBatch::CopyColorData4f(M3DVector4f *vColors)
{
    // First time, create the buffer object, allocate the space
    if(uiColorArray == 0) {
        glGenBuffers(1, &uiColorArray);
        glBindBuffer(GL_ARRAY_BUFFER, uiColorArray);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * nNumVerts, vColors, GL_DYNAMIC_DRAW);
    }
    else {	// Just bind to existing object
        glBindBuffer(GL_ARRAY_BUFFER, uiColorArray);

        // Copy the data in
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 4 * nNumVerts, vColors);
        pColors = NULL;
    }
}

void GLBatch::CopyTexCoordData2f(M3DVector2f *vTexCoords, GLuint uiTextureLayer)
{
    // First time, create the buffer object, allocate the space
    if(uiTextureCoordArray[uiTextureLayer] == 0) {
        glGenBuffers(1, &uiTextureCoordArray[uiTextureLayer]);
        glBindBuffer(GL_ARRAY_BUFFER, uiTextureCoordArray[uiTextureLayer]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 2 * nNumVerts, vTexCoords, GL_DYNAMIC_DRAW);
        }
    else {	// Just bind to existing object
        glBindBuffer(GL_ARRAY_BUFFER, uiTextureCoordArray[uiTextureLayer]);

        // Copy the data in
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 2 * nNumVerts, vTexCoords);
        pTexCoords[uiTextureLayer] = NULL;
    }
}

void GLBatch::Draw()
{
    if(!bBatchDone)
        return;

    #ifndef OPENGL_ES
    // Set up the vertex array object
    glBindVertexArray(vertexArrayObject);
    #else
    if(uiVertexArray !=0) {
        glEnableVertexAttribArray(GLT_ATTRIBUTE_VERTEX);
        glBindBuffer(GL_ARRAY_BUFFER, uiVertexArray);
        glVertexAttribPointer(GLT_ATTRIBUTE_VERTEX, 3, GL_FLOAT, GL_FALSE, 0, 0);
        }

    if(uiColorArray != 0) {
        glEnableVertexAttribArray(GLT_ATTRIBUTE_COLOR);
        glBindBuffer(GL_ARRAY_BUFFER, uiColorArray);
        glVertexAttribPointer(GLT_ATTRIBUTE_COLOR, 4, GL_FLOAT, GL_FALSE, 0, 0);
        }

    if(uiNormalArray != 0) {
        glEnableVertexAttribArray(GLT_ATTRIBUTE_NORMAL);
        glBindBuffer(GL_ARRAY_BUFFER, uiNormalArray);
        glVertexAttribPointer(GLT_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, 0);
        }

    // How many texture units
    for(unsigned int i = 0; i < nNumTextureUnits; i++)
        if(uiTextureCoordArray[i] != 0) {
            glEnableVertexAttribArray(GLT_ATTRIBUTE_TEXTURE0 + i),
            glBindBuffer(GL_ARRAY_BUFFER, uiTextureCoordArray[i]);
            glVertexAttribPointer(GLT_ATTRIBUTE_TEXTURE0 + i, 2, GL_FLOAT, GL_FALSE, 0, 0);
            }
    #endif


    glDrawArrays(primitiveType, 0, nNumVerts);

    #ifndef OPENGL_ES
    glBindVertexArray(0);
    #else
    glDisableVertexAttribArray(GLT_ATTRIBUTE_VERTEX);
    glDisableVertexAttribArray(GLT_ATTRIBUTE_NORMAL);
    glDisableVertexAttribArray(GLT_ATTRIBUTE_COLOR);

    for(unsigned int i = 0; i < nNumTextureUnits; i++)
        if(uiTextureCoordArray[i] != 0)
            glDisableVertexAttribArray(GLT_ATTRIBUTE_TEXTURE0 + i);

    #endif
}

void GLBatch::Reset()
{
    bBatchDone = false;
    nVertsBuilding = 0;
}

void GLBatch::Vertex3f(GLfloat x, GLfloat y, GLfloat z)
{
    // First see if the vertex array buffer has been created...
    if(uiVertexArray == 0) {	// Nope, we need to create it
        glGenBuffers(1, &uiVertexArray);
        glBindBuffer(GL_ARRAY_BUFFER, uiVertexArray);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * nNumVerts, NULL, GL_DYNAMIC_DRAW);
        }

    // Now see if it's already mapped, if not, map it
    if(pVerts == NULL) {
        glBindBuffer(GL_ARRAY_BUFFER, uiVertexArray);
        pVerts = (M3DVector3f *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        }

    // Ignore if we go past the end, keeps things from blowing up
    if(nVertsBuilding >= nNumVerts)
        return;

    // Copy it in...
    pVerts[nVertsBuilding][0] = x;
    pVerts[nVertsBuilding][1] = y;
    pVerts[nVertsBuilding][2] = z;
    nVertsBuilding++;
}

void GLBatch::Vertex3fv(M3DVector3f vVertex)
{
    // First see if the vertex array buffer has been created...
    if(uiVertexArray == 0) {	// Nope, we need to create it
        glGenBuffers(1, &uiVertexArray);
        glBindBuffer(GL_ARRAY_BUFFER, uiVertexArray);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * nNumVerts, NULL, GL_DYNAMIC_DRAW);
        }

    // Now see if it's already mapped, if not, map it
    if(pVerts == NULL) {
        glBindBuffer(GL_ARRAY_BUFFER, uiVertexArray);
        pVerts = (M3DVector3f *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        }

    // Ignore if we go past the end, keeps things from blowing up
    if(nVertsBuilding >= nNumVerts)
        return;

    // Copy it in...
    memcpy(pVerts[nVertsBuilding], vVertex, sizeof(M3DVector3f));
    nVertsBuilding++;
}

void GLBatch::Normal3f(GLfloat x, GLfloat y, GLfloat z)
{
    // First see if the vertex array buffer has been created...
    if(uiNormalArray == 0) {	// Nope, we need to create it
        glGenBuffers(1, &uiNormalArray);
        glBindBuffer(GL_ARRAY_BUFFER, uiNormalArray);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * nNumVerts, NULL, GL_DYNAMIC_DRAW);
        }

    // Now see if it's already mapped, if not, map it
    if(pNormals == NULL) {
        glBindBuffer(GL_ARRAY_BUFFER, uiNormalArray);
        pNormals = (M3DVector3f *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        }

    // Ignore if we go past the end, keeps things from blowing up
    if(nVertsBuilding >= nNumVerts)
        return;

    // Copy it in...
    pNormals[nVertsBuilding][0] = x;
    pNormals[nVertsBuilding][1] = y;
    pNormals[nVertsBuilding][2] = z;
}

void GLBatch::Normal3fv(M3DVector3f vNormal)
{
    // First see if the vertex array buffer has been created...
    if(uiNormalArray == 0) {	// Nope, we need to create it
        glGenBuffers(1, &uiNormalArray);
        glBindBuffer(GL_ARRAY_BUFFER, uiNormalArray);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * nNumVerts, NULL, GL_DYNAMIC_DRAW);
        }

    // Now see if it's already mapped, if not, map it
    if(pNormals == NULL) {
        glBindBuffer(GL_ARRAY_BUFFER, uiNormalArray);
        pNormals = (M3DVector3f *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        }

    // Ignore if we go past the end, keeps things from blowing up
    if(nVertsBuilding >= nNumVerts)
        return;

    // Copy it in...
    memcpy(pNormals[nVertsBuilding], vNormal, sizeof(M3DVector3f));
}

void GLBatch::Color4f(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
    // First see if the vertex array buffer has been created...
    if(uiColorArray == 0) {	// Nope, we need to create it
        glGenBuffers(1, &uiColorArray);
        glBindBuffer(GL_ARRAY_BUFFER, uiColorArray);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * nNumVerts, NULL, GL_DYNAMIC_DRAW);
        }

    // Now see if it's already mapped, if not, map it
    if(pColors == NULL) {
        glBindBuffer(GL_ARRAY_BUFFER, uiColorArray);
        pColors = (M3DVector4f *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        }

    // Ignore if we go past the end, keeps things from blowing up
    if(nVertsBuilding >= nNumVerts)
        return;

    // Copy it in...
    pColors[nVertsBuilding][0] = r;
    pColors[nVertsBuilding][1] = g;
    pColors[nVertsBuilding][2] = b;
    pColors[nVertsBuilding][3] = a;
}

void GLBatch::Color4fv(M3DVector4f vColor)
{
    // First see if the vertex array buffer has been created...
    if(uiColorArray == 0) {	// Nope, we need to create it
        glGenBuffers(1, &uiColorArray);
        glBindBuffer(GL_ARRAY_BUFFER, uiColorArray);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * nNumVerts, NULL, GL_DYNAMIC_DRAW);
        }

    // Now see if it's already mapped, if not, map it
    if(pColors == NULL) {
        glBindBuffer(GL_ARRAY_BUFFER, uiColorArray);
        pColors = (M3DVector4f *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        }

    // Ignore if we go past the end, keeps things from blowing up
    if(nVertsBuilding >= nNumVerts)
        return;

    // Copy it in...
    memcpy(pColors[nVertsBuilding], vColor, sizeof(M3DVector4f));
}

void GLBatch::MultiTexCoord2f(GLuint texture, GLclampf s, GLclampf t)
{
    // First see if the vertex array buffer has been created...
    if(uiTextureCoordArray[texture] == 0) {	// Nope, we need to create it
        glGenBuffers(1, &uiTextureCoordArray[texture]);
        glBindBuffer(GL_ARRAY_BUFFER, uiTextureCoordArray[texture]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 2 * nNumVerts, NULL, GL_DYNAMIC_DRAW);
    }

    // Now see if it's already mapped, if not, map it
    if(pTexCoords[texture] == NULL) {
        glBindBuffer(GL_ARRAY_BUFFER, uiTextureCoordArray[texture]);
        pTexCoords[texture] = (M3DVector2f *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        }

    // Ignore if we go past the end, keeps things from blowing up
    if(nVertsBuilding >= nNumVerts)
        return;

    // Copy it in...
    pTexCoords[texture][nVertsBuilding][0] = s;
    pTexCoords[texture][nVertsBuilding][1] = t;
}

void GLBatch::MultiTexCoord2fv(GLuint texture, M3DVector2f vTexCoord)
{
    // First see if the vertex array buffer has been created...
    if(uiTextureCoordArray[texture] == 0) {	// Nope, we need to create it
        glGenBuffers(1, &uiTextureCoordArray[texture]);
        glBindBuffer(GL_ARRAY_BUFFER, uiTextureCoordArray[texture]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 2 * nNumVerts, NULL, GL_DYNAMIC_DRAW);
        }

    // Now see if it's already mapped, if not, map it
    if(pTexCoords[texture] == NULL) {
        glBindBuffer(GL_ARRAY_BUFFER, uiTextureCoordArray[texture]);
        pTexCoords[texture] = (M3DVector2f *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        }

    // Ignore if we go past the end, keeps things from blowing up
    if(nVertsBuilding >= nNumVerts)
        return;

    // Copy it in...
    memcpy(pTexCoords[texture], vTexCoord, sizeof(M3DVector2f));
}
