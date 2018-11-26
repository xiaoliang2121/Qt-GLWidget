#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <GLTools.h>
#include <QOpenGLWidget>
#include <GLFrame.h>
#include <GLFrustum.h>
#include <GLMatrixStack.h>
#include <GLGeometryTransform.h>

class GLWidget : public QOpenGLWidget
{
public:
    GLWidget(QWidget *parent);

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

private:
    GLfloat xRot;
    GLfloat yRot;

    GLFrame             viewFrame;
    GLFrustum           viewFrustum;
    GLTriangleBatch     torusBatch;
    GLMatrixStack       modelViewMatix;
    GLMatrixStack       projectionMatrix;
    GLGeometryTransform transformPipeline;
    GLShaderManager     shaderManager;

    // Flags for effects
    int iCull;
    int iDepth;
};

#endif // GLWIDGET_H
