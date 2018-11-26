#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <GLTools.h>
#include <QOpenGLWidget>
#include <GLFrame.h>
#include <GLFrustum.h>
#include <GLMatrixStack.h>
#include <GLGeometryTransform.h>
#include <StopWatch.h>

class GLWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent);
    ~GLWidget();

    GLfloat getxRot() const;
    GLfloat getyRot() const;
    void setxRot(GLfloat value);
    void setyRot(GLfloat value);

    void SetupRC();

protected:
    virtual void initializeGL();
    virtual void paintGL();
    virtual void resizeGL(int w, int h);

    virtual void keyPressEvent(QKeyEvent *ev);

public slots:
    void ProcessMenu(int value);

signals:
    void changeTitle(QString title);

private:
    void DrawWireFramedBatch(GLTriangleBatch* pBatch);
    bool LoadTGATexture(const char *szFileName, GLenum minFilter, GLenum magFilter,
                        GLenum wrapMode);
    void MakePyramid(GLBatch& pyramidBatch);

    GLfloat xRot;
    GLfloat yRot;

    int nStep;

    GLShaderManager     shaderManager;
    GLFrame             cameraFrame;
    GLFrame             objectFrame;

    GLFrustum           viewFrustum;
    GLMatrixStack       modelViewMatix;
    GLMatrixStack       projectionMatrix;
    GLGeometryTransform transformPipeline;
    M3DMatrix44f        shadowMatrix;

    GLBatch             pyramidBatch;
    GLuint              textureID;
};

#endif // GLWIDGET_H
