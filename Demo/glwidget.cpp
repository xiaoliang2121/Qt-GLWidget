#include "glwidget.h"

#ifdef WIN32
#include <gl/GLU.h>
#else
#include <GL/glu.h>
#endif

#include <QKeyEvent>
#include <QMouseEvent>
#include <QDebug>

GLfloat vGreen[] = { 0.0f, 1.0f, 0.0f, 1.0f };
GLfloat vBlack[] = { 0.0f, 0.0f, 0.0f, 1.0f };

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
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f );

    shaderManager.InitializeStockShaders();

    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

    gltMakeTorus(torusBatch,0.4f,0.15,30,30);

    gltMakeSphere(sphereBatch,0.1f, 26, 13);

    floorBatch.Begin(GL_LINES,324);
        for(GLfloat x=-20.0f; x<20.0f; x+=0.5f)
        {
            floorBatch.Vertex3f(x,-0.55f,20.0f);
            floorBatch.Vertex3f(x,-0.55f,-20.0f);

            floorBatch.Vertex3f(20.0f,-0.55f,x);
            floorBatch.Vertex3f(-20.0f,-0.55f,x);
        }
    floorBatch.End();
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
    // Color values
    static GLfloat vFloorColor[] = { 0.0f, 1.0f, 0.0f, 1.0f };
    static GLfloat vTorusColor[] = { 1.0f, 0.0f, 0.0f, 1.0f };
    static GLfloat vSphereColor[] = { 0.0f, 0.0f, 1.0f, 1.0f };

    yRot = rotTimer.GetElapsedSeconds()*60.0f;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    modelViewMatix.PushMatrix();
        M3DMatrix44f mCamera;
        cameraFrame.GetCameraMatrix(mCamera);
        modelViewMatix.PushMatrix(mCamera);

            shaderManager.UseStockShader(GLT_SHADER_FLAT,transformPipeline.GetModelViewProjectionMatrix(),
                                         vFloorColor);
            floorBatch.Draw();

            modelViewMatix.Translate(0.0f,0.0f,-2.5f);

            modelViewMatix.PushMatrix();
                modelViewMatix.Rotate(yRot,0.0f,1.0f,0.0f);
                shaderManager.UseStockShader(GLT_SHADER_FLAT,transformPipeline.GetModelViewProjectionMatrix(),
                                             vTorusColor);
                torusBatch.Draw();
            modelViewMatix.PopMatrix();

            modelViewMatix.Rotate(yRot*-2.0f,0.0f,1.0f,0.0f);
            modelViewMatix.Translate(0.8f,0.0f,0.0f);
            shaderManager.UseStockShader(GLT_SHADER_FLAT,transformPipeline.GetModelViewProjectionMatrix(),
                                         vSphereColor);
            sphereBatch.Draw();
        modelViewMatix.PopMatrix();
    modelViewMatix.PopMatrix();

    update();
}

void GLWidget::resizeGL(int w, int h)
{
    GLdouble fAspect;

    if(h == 0)
        h = 1;

    glViewport(0,0,w,h);

    fAspect = (GLdouble)w/(GLdouble)h;

    viewFrustum.SetPerspective(35.0f,fAspect,1.0f,100.0f);
    projectionMatrix.LoadMatrix(viewFrustum.GetProjectionMatrix());

    transformPipeline.SetMatrixStacks(modelViewMatix,projectionMatrix);
}

/**
 * @brief GLWidget::keyPressEvent
 * @param ev
 */
void GLWidget::keyPressEvent(QKeyEvent *ev)
{
    float linear = 0.1f;
    float angular = float(m3dDegToRad(5.0f));

    if(ev->key() == Qt::Key_Up)
        cameraFrame.MoveForward(linear);

    if(ev->key() == Qt::Key_Down)
        cameraFrame.MoveForward(-linear);

    if(ev->key() == Qt::Key_Left)
        cameraFrame.RotateWorld(angular, 0.0f, 1.0f, 0.0f);

    if(ev->key() == Qt::Key_Right)
        cameraFrame.RotateWorld(-angular, 0.0f, 1.0f, 0.0f);

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
