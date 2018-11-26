#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <GLTools.h>
#include <QOpenGLWidget>
#include <GLFrame.h>
#include <GLFrustum.h>
#include <GLMatrixStack.h>
#include <GLGeometryTransform.h>

// Array of small stars
#define SMALL_STARS     100
#define MEDIUM_STARS     40
#define LARGE_STARS      15

#define SCREEN_X        400
#define SCREEN_Y        300

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

    GLBatch smallStarBatch;
    GLBatch mediumStarBatch;
    GLBatch largeStarBatch;
    GLBatch mountainRangeBatch;
    GLBatch moonBatch;

    GLShaderManager shaderManager;
    GLFrustum viewFrustum;
};

#endif // GLWIDGET_H
