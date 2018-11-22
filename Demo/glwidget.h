#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_2_1>

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions_2_1
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
};

#endif // GLWIDGET_H
