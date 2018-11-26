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
    nStep(0)
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
    // Black background
    glClearColor(0.7f, 0.7f, 0.7f, 1.0f );

    shaderManager.InitializeStockShaders();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    gltMakeTorus(torusBatch,0.4f,0.15f,30,30);
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

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

    yRot = rotTimer.GetElapsedSeconds()*60.0f;

    M3DMatrix44f mTranslate, mRotate, mModelView, mModelViewProjection;

    m3dTranslationMatrix44(mTranslate,0.0f,0.0f,-2.5f);
    m3dRotationMatrix44(mRotate,m3dDegToRad(yRot),0.0f,1.0f,0.0f);
    m3dMatrixMultiply44(mModelView,mTranslate,mRotate);
    m3dMatrixMultiply44(mModelViewProjection,viewFrustum.GetProjectionMatrix(),
                        mModelView);

    GLfloat vBlack[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    shaderManager.UseStockShader(GLT_SHADER_FLAT,mModelViewProjection,vBlack);
    torusBatch.Draw();

    update();
}

void GLWidget::resizeGL(int w, int h)
{
    GLdouble fAspect;

    if(h == 0)
        h = 1;

    glViewport(0,0,w,h);

    fAspect = (GLdouble)w/(GLdouble)h;

    viewFrustum.SetPerspective(35.0f,fAspect,1.0f,1000.0f);
}

/**
 * @brief GLWidget::keyPressEvent
 * @param ev
 */
void GLWidget::keyPressEvent(QKeyEvent *ev)
{
//    if(ev->key() == Qt::Key_Up)
//        objectFrame.RotateWorld(m3dDegToRad(-5.0), 1.0f, 0.0f, 0.0f);

//    if(ev->key() == Qt::Key_Down)
//        objectFrame.RotateWorld(m3dDegToRad(5.0), 1.0f, 0.0f, 0.0f);

//    if(ev->key() == Qt::Key_Left)
//        objectFrame.RotateWorld(m3dDegToRad(-5.0), 0.0f, 1.0f, 0.0f);

//    if(ev->key() == Qt::Key_Right)
//        objectFrame.RotateWorld(m3dDegToRad(5.0), 0.0f, 1.0f, 0.0f);

//    if(ev->key() == Qt::Key_Space)
//    {
//        nStep++;

//        if(nStep > 4)
//            nStep = 0;
//    }

//    QString str;
//    switch(nStep)
//    {
//    case 0:
//        str = QString("Sphere");
//        break;
//    case 1:
//        str = QString("Torus");
//        break;
//    case 2:
//        str = QString("Cylinder");
//        break;
//    case 3:
//        str = QString("Cone");
//        break;
//    case 4:
//        str = QString("Disk");
//        break;
//    }

//    emit changeTitle(str);

    update();
    QOpenGLWidget::keyPressEvent(ev);
}

void GLWidget::ProcessMenu(int value)
{
    makeCurrent();

    doneCurrent();

    update();
}
/*
void GLWidget::DrawWireFramedBatch(GLTriangleBatch *pBatch)
{
    shaderManager.UseStockShader(GLT_SHADER_FLAT, transformPipeline.GetModelViewProjectionMatrix(), vGreen);
    pBatch->Draw();

    // Draw black outline
    glPolygonOffset(-1.0f, -1.0f);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_POLYGON_OFFSET_LINE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glLineWidth(2.5f);
    shaderManager.UseStockShader(GLT_SHADER_FLAT, transformPipeline.GetModelViewProjectionMatrix(), vBlack);
    pBatch->Draw();

    // Restore polygon mode and depht testing
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_POLYGON_OFFSET_LINE);
    glLineWidth(1.0f);
    glDisable(GL_BLEND);
    glDisable(GL_LINE_SMOOTH);
}
*/
