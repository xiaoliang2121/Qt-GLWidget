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
    Q_OBJECT

public:
    GLWidget(QWidget *parent);

    GLfloat getxRot() const;
    GLfloat getyRot() const;
    void setxRot(GLfloat value);
    void setyRot(GLfloat value);

protected:
    virtual void initializeGL();
    virtual void paintGL();
    virtual void resizeGL(int w, int h);

    virtual void keyPressEvent(QKeyEvent *ev);

signals:
    void changeTitle(QString title);

private:
    void DrawWireFramedBatch(GLBatch *pBatch);

    GLfloat xRot;
    GLfloat yRot;

    GLShaderManager shaderManager;
    GLMatrixStack		modelViewMatrix;
    GLMatrixStack		projectionMatrix;
    GLFrame				cameraFrame;
    GLFrame             objectFrame;
    GLFrustum			viewFrustum;

    GLBatch				pointBatch;
    GLBatch				lineBatch;
    GLBatch				lineStripBatch;
    GLBatch				lineLoopBatch;
    GLBatch				triangleBatch;
    GLBatch             triangleStripBatch;
    GLBatch             triangleFanBatch;

    GLGeometryTransform transformPipeline;
    M3DMatrix44f shadowMatrix;

    int nStep;
};

#endif // GLWIDGET_H
