﻿#include "gltrianglebatch.h"

/**
 * @brief GLTriangleBatch::GLTriangleBatch
 * Constructor, does what constructors do... set everything to zero or NULL
 */
GLTriangleBatch::GLTriangleBatch()
{
    pIndexes = NULL;
    pVerts = NULL;
    pNorms = NULL;
    pTexCoords = NULL;

    nMaxIndexes = 0;
    nNumIndexes = 0;
    nNumVerts = 0;
}

/**
 * @brief GLTriangleBatch::~GLTriangleBatch
 * Free any dynamically allocated memory. For those C programmers
 * coming to C++, it is perfectly valid to delete a NULL pointer.
 */
GLTriangleBatch::~GLTriangleBatch()
{
    // Just in case these still are allocated when the object is destroyed
    delete [] pIndexes;
    delete [] pVerts;
    delete [] pNorms;
    delete [] pTexCoords;

    // Delete buffer objects
    glDeleteBuffers(4, bufferObjects);

    #ifndef OPENGL_ES
    glDeleteVertexArrays(1, &vertexArrayBufferObject);
    #endif
}

/**
 * @brief GLTriangleBatch::BeginMesh
 * Start assembling a mesh. You need to specify a maximum amount
 * of indexes that you expect. The EndMesh will clean up any uneeded memory.
 * This is far better than shreading your heap with STL containers...
 * At least that's my humble opinion.
 * @param nMaxVerts
 */
void GLTriangleBatch::BeginMesh(GLuint nMaxVerts)
{
    // Just in case this gets called more than once...
    delete [] pIndexes;
    delete [] pVerts;
    delete [] pNorms;
    delete [] pTexCoords;

    nMaxIndexes = nMaxVerts;
    nNumIndexes = 0;
    nNumVerts = 0;

    // Allocate new blocks. In reality, the other arrays will be
    // much shorter than the index array
    pIndexes = new GLushort[nMaxIndexes];
    pVerts = new M3DVector3f[nMaxIndexes];
    pNorms = new M3DVector3f[nMaxIndexes];
    pTexCoords = new M3DVector2f[nMaxIndexes];
}

/**
 * @brief GLTriangleBatch::AddTriangle
 * Add a triangle to the mesh. This searches the current list for identical
 * (well, almost identical - these are floats you know...) verts. If one is found, it
 * is added to the index array. If not, it is added to both the index array and the vertex
 * array grows by one as well.
 * @param verts
 * @param vNorms
 * @param vTexCoords
 */
void GLTriangleBatch::AddTriangle(M3DVector3f verts[], M3DVector3f vNorms[], M3DVector2f vTexCoords[])
{
    const  float e = 0.00001f; // How small a difference to equate

    // First thing we do is make sure the normals are unit length!
    // It's almost always a good idea to work with pre-normalized normals
    m3dNormalizeVector3(vNorms[0]);
    m3dNormalizeVector3(vNorms[1]);
    m3dNormalizeVector3(vNorms[2]);


    // Search for match - triangle consists of three verts
    for(GLuint iVertex = 0; iVertex < 3; iVertex++)
        {
        GLuint iMatch = 0;
        for(iMatch = 0; iMatch < nNumVerts; iMatch++)
            {
            // If the vertex positions are the same
            if(m3dCloseEnough(pVerts[iMatch][0], verts[iVertex][0], e) &&
               m3dCloseEnough(pVerts[iMatch][1], verts[iVertex][1], e) &&
               m3dCloseEnough(pVerts[iMatch][2], verts[iVertex][2], e) &&

               // AND the Normal is the same...
               m3dCloseEnough(pNorms[iMatch][0], vNorms[iVertex][0], e) &&
               m3dCloseEnough(pNorms[iMatch][1], vNorms[iVertex][1], e) &&
               m3dCloseEnough(pNorms[iMatch][2], vNorms[iVertex][2], e) &&

                // And Texture is the same...
                m3dCloseEnough(pTexCoords[iMatch][0], vTexCoords[iVertex][0], e) &&
                m3dCloseEnough(pTexCoords[iMatch][1], vTexCoords[iVertex][1], e))
                {
                // Then add the index only
                pIndexes[nNumIndexes] = iMatch;
                nNumIndexes++;
                break;
                }
            }

        // No match for this vertex, add to end of list
        if(iMatch == nNumVerts && nNumVerts < nMaxIndexes && nNumIndexes < nMaxIndexes)
            {
            memcpy(pVerts[nNumVerts], verts[iVertex], sizeof(M3DVector3f));
            memcpy(pNorms[nNumVerts], vNorms[iVertex], sizeof(M3DVector3f));
            memcpy(pTexCoords[nNumVerts], vTexCoords[iVertex], sizeof(M3DVector2f));
            pIndexes[nNumIndexes] = nNumVerts;
            nNumIndexes++;
            nNumVerts++;
            }
    }
}

/**
 * @brief GLTriangleBatch::End
 * Compact the data. This is a nice utility, but you should really
 * save the results of the indexing for future use if the model data
 * is static (doesn't change).
 */
void GLTriangleBatch::End()
{
#ifndef OPENGL_ES
    // Create the master vertex array object
    glGenVertexArrays(1, &vertexArrayBufferObject);
    glBindVertexArray(vertexArrayBufferObject);
#endif

    // Create the buffer objects
    glGenBuffers(4, bufferObjects);

    // Copy data to video memory
    // Vertex data
    glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[VERTEX_DATA]);
    glEnableVertexAttribArray(GLT_ATTRIBUTE_VERTEX);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*nNumVerts*3, pVerts, GL_STATIC_DRAW);
    glVertexAttribPointer(GLT_ATTRIBUTE_VERTEX, 3, GL_FLOAT, GL_FALSE, 0, 0);


    // Normal data
    glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[NORMAL_DATA]);
    glEnableVertexAttribArray(GLT_ATTRIBUTE_NORMAL);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*nNumVerts*3, pNorms, GL_STATIC_DRAW);
    glVertexAttribPointer(GLT_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Texture coordinates
    glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[TEXTURE_DATA]);
    glEnableVertexAttribArray(GLT_ATTRIBUTE_TEXTURE0);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*nNumVerts*2, pTexCoords, GL_STATIC_DRAW);
    glVertexAttribPointer(GLT_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // Indexes
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferObjects[INDEX_DATA]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*nNumIndexes, pIndexes, GL_STATIC_DRAW);


    // Done
#ifndef OPENGL_ES
    glBindVertexArray(0);
#endif

    // Free older, larger arrays
    delete [] pIndexes;
    delete [] pVerts;
    delete [] pNorms;
    delete [] pTexCoords;

    // Reasign pointers so they are marked as unused
    pIndexes = NULL;
    pVerts = NULL;
    pNorms = NULL;
    pTexCoords = NULL;

    // Unbind to anybody
#ifndef OPENGL_ES
    glBindVertexArray(0);
#endif
}

/**
 * @brief GLTriangleBatch::Draw
 * Draw - make sure you call glEnableClientState for these arrays
 */
void GLTriangleBatch::Draw()
{
#ifndef OPENGL_ES
    glBindVertexArray(vertexArrayBufferObject);
#else
    glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[VERTEX_DATA]);
    glEnableVertexAttribArray(GLT_ATTRIBUTE_VERTEX);
    glVertexAttribPointer(GLT_ATTRIBUTE_VERTEX, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Normal data
    glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[NORMAL_DATA]);
    glEnableVertexAttribArray(GLT_ATTRIBUTE_NORMAL);
    glVertexAttribPointer(GLT_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Texture coordinates
    glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[TEXTURE_DATA]);
    glEnableVertexAttribArray(GLT_ATTRIBUTE_TEXTURE0);
    glVertexAttribPointer(GLT_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // Indexes
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferObjects[INDEX_DATA]);
#endif


    glDrawElements(GL_TRIANGLES, nNumIndexes, GL_UNSIGNED_SHORT, 0);

#ifndef OPENGL_ES
    // Unbind to anybody
    glBindVertexArray(vertexArrayBufferObject);
#else
    glDisableVertexAttribArray(GLT_ATTRIBUTE_VERTEX);
    glDisableVertexAttribArray(GLT_ATTRIBUTE_NORMAL);
    glDisableVertexAttribArray(GLT_ATTRIBUTE_TEXTURE0);
#endif
}
