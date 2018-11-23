﻿#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <GLTools.h>
#include <QOpenGLWidget>

#include "GLShaderManager.h"

class GLWidget : public QOpenGLWidget
{
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

private:
    GLfloat xRot;
    GLfloat yRot;

    GLBatch	triangleBatch;
    GLShaderManager	shaderManager;
};

#endif // GLWIDGET_H
