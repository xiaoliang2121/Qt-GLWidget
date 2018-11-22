#include "glwidget.h"

#ifdef WIN32
#include <gl/GLU.h>
#else
#include <GL/glu.h>
#endif

#include <QKeyEvent>
#include <QMouseEvent>
#include <QDebug>

GLWidget::GLWidget(QWidget *parent):
    QOpenGLWidget(parent),
    xRot(0.0f),
    yRot(0.0f)
{
    QSurfaceFormat format;
    format.setProfile(QSurfaceFormat::CompatibilityProfile);
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    format.setDepthBufferSize(16);
    setFormat(format);

    setFocusPolicy(Qt::ClickFocus);
}

GLfloat GLWidget::getxRot() const
{
    return xRot;
}

GLfloat GLWidget::getyRot() const
{
    return yRot;
}

void GLWidget::setxRot(GLfloat value)
{
    xRot = value;

    update();
}

void GLWidget::setyRot(GLfloat value)
{
    yRot = value;

    update();
}

void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    // 获取OpenGL版本号和厂商信息
    const GLubyte *name = glGetString(GL_VENDOR);
    const GLubyte *label = glGetString(GL_RENDERER);
    const GLubyte *glVersion = glGetString(GL_VERSION);
    const GLubyte *gluVersion = glGetString(GLU_VERSION);

    printf("OpenGL实现厂商的名字：%s\n",name);
    printf("渲染器标识符：%s\n",label);
    printf("OpenGL实现的版本号：%s\n",glVersion);
    printf("GLU工具库版本：%s\n",gluVersion);

    glClearColor(0.7f,0.7f,0.7f,1.0f);
    glEnable(GL_DEPTH_TEST);
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glPushMatrix();
        glTranslatef(0.0f,0.0f,-3.0f);
        glRotatef(xRot,1.0f,0.0f,0.0f);
        glRotatef(yRot,0.0f,1.0f,0.0f);

        glBegin(GL_TRIANGLES);
            glColor3f(1.0f,0.0f,0.0f);
            glVertex3f(0.0f,0.75f,0.0f);

            glColor3f(0.0f,1.0f,0.0f);
            glVertex3f(-0.5f,0.0f,0.0f);

            glColor3f(0.0f,0.0f,1.0f);
            glVertex3f(0.5f,0.0f,0.0f);
        glEnd();
    glPopMatrix();
}

void GLWidget::resizeGL(int w, int h)
{
    GLdouble fAspect;

    if(h == 0)
        h = 1;

    glViewport(0,0,w,h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    fAspect = (GLdouble)w/(GLdouble)h;
    gluPerspective(35.0,fAspect,1.0,65.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/**
 * @brief GLWidget::keyPressEvent
 * @param ev
 */
void GLWidget::keyPressEvent(QKeyEvent *ev)
{
    if(ev->key() == Qt::Key_Up)
        xRot -= 5.0f;

    if(ev->key() == Qt::Key_Down)
        xRot += 5.0f;

    if(ev->key() == Qt::Key_Left)
        yRot -= 5.0f;

    if(ev->key() == Qt::Key_Right)
        yRot += 5.0f;

    if(xRot > 360.0f)
        xRot = 5.0f;

    if(yRot > 360.0f)
        yRot = 5.0f;

    if(xRot < 0.0f)
        xRot = 355.0f;

    if(yRot < 0.0f)
        yRot = 355.0f;

    update();
    QOpenGLWidget::keyPressEvent(ev);
}
