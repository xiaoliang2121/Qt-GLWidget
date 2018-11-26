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

    GLfloat xRot;
    GLfloat yRot;

    int nStep;

    GLFrame				cameraFrame;
    GLFrame             objectFrame;
    GLFrustum           viewFrustum;
    GLMatrixStack       modelViewMatix;
    GLMatrixStack       projectionMatrix;

    GLShaderManager     shaderManager;
    GLGeometryTransform transformPipeline;
    M3DMatrix44f        shadowMatrix;

    GLTriangleBatch     sphereBatch;
    GLTriangleBatch     torusBatch;
    GLTriangleBatch     cylinderBatch;
    GLTriangleBatch     coneBatch;
    GLTriangleBatch     diskBatch;
};

#endif // GLWIDGET_H
