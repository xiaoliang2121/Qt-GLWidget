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
    yRot(0.0f),
    iCull(0),
    iDepth(0)
{
    QSurfaceFormat format;
    format.setProfile(QSurfaceFormat::CompatibilityProfile);
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    format.setDepthBufferSize(16);
    format.setStencilBufferSize(8);
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

/**
 * @brief GLWidget::SetupRC
 * This function does any needed initialization on the rendering context
 */
void GLWidget::SetupRC()
{
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

    shaderManager.InitializeStockShaders();
    viewFrame.MoveForward(7.0f);

    gltMakeTorus(torusBatch,1.0f,0.3f,52,26);

    glPointSize(4.0f);
}

void GLWidget::initializeGL()
{
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
        return;
    }

    printf("Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
    // 获取OpenGL版本号和厂商信息
    const GLubyte *name = glGetString(GL_VENDOR);
    const GLubyte *label = glGetString(GL_RENDERER);
    const GLubyte *glVersion = glGetString(GL_VERSION);
    const GLubyte *gluVersion = glGetString(GLU_VERSION);

    printf("OpenGL实现厂商的名字：%s\n",name);
    printf("渲染器标识符：%s\n",label);
    printf("OpenGL实现的版本号：%s\n",glVersion);
    printf("GLU工具库版本：%s\n",gluVersion);

    SetupRC();
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(iCull)
        glEnable(GL_CULL_FACE);
    else
        glDisable(GL_CULL_FACE);

    if(iDepth)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);

    modelViewMatix.PushMatrix(viewFrame);
        GLfloat vRed[] = {1.0f,0.0f,0.0f,1.0f};

//        shaderManager.UseStockShader(GLT_SHADER_FLAT,transformPipeline.GetModelViewProjectionMatrix(),
//                                     vRed);
        shaderManager.UseStockShader(GLT_SHADER_DEFAULT_LIGHT,transformPipeline.GetModelViewMatrix(),
                                     transformPipeline.GetProjectionMatrix(),vRed);

        torusBatch.Draw();
    modelViewMatix.PopMatrix();
}

void GLWidget::resizeGL(int w, int h)
{
    GLdouble fAspect;

    if(h == 0)
        h = 1;

    glViewport(0,0,w,h);

//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();

    fAspect = (GLdouble)w/(GLdouble)h;
//    gluPerspective(35.0,fAspect,1.0,65.0);

//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();

    viewFrustum.SetPerspective(35.0f,fAspect,1.0f,100.0f);
    projectionMatrix.LoadMatrix(viewFrustum.GetProjectionMatrix());
//    modelViewMatix.LoadIdentity();
    transformPipeline.SetMatrixStacks(modelViewMatix,projectionMatrix);
}

/**
 * @brief GLWidget::keyPressEvent
 * @param ev
 */
void GLWidget::keyPressEvent(QKeyEvent *ev)
{
    if(ev->key() == Qt::Key_Up)
        viewFrame.RotateWorld(m3dDegToRad(-5.0), 1.0f, 0.0f, 0.0f);

    if(ev->key() == Qt::Key_Down)
        viewFrame.RotateWorld(m3dDegToRad(5.0), 1.0f, 0.0f, 0.0f);

    if(ev->key() == Qt::Key_Left)
        viewFrame.RotateWorld(m3dDegToRad(-5.0), 0.0f, 1.0f, 0.0f);

    if(ev->key() == Qt::Key_Right)
        viewFrame.RotateWorld(m3dDegToRad(5.0), 0.0f, 1.0f, 0.0f);

    update();
    QOpenGLWidget::keyPressEvent(ev);
}

void GLWidget::ProcessMenu(int value)
{
    makeCurrent();
    switch(value)
    {
    case 1:
        iDepth = !iDepth;
        break;

    case 2:
        iCull = !iCull;
        break;

    case 3:
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        break;

    case 4:
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        break;

    case 5:
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        break;
    }
    doneCurrent();

    update();
}
