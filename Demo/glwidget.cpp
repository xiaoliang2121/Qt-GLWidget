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

    glClearColor(0.7f,0.7f,0.7f,1.0f);
    glEnable(GL_DEPTH_TEST);

    shaderManager.InitializeStockShaders();

    transformPipeline.SetMatrixStacks(modelViewMatrix,projectionMatrix);

    cameraFrame.MoveForward(-15.0f);

    // Some points, more or less in the shape of Florida
    GLfloat vCoast[24][3] = {{2.80, 1.20, 0.0 }, {2.0,  1.20, 0.0 },
                            {2.0,  1.08, 0.0 },  {2.0,  1.08, 0.0 },
                            {0.0,  0.80, 0.0 },  {-.32, 0.40, 0.0 },
                            {-.48, 0.2, 0.0 },   {-.40, 0.0, 0.0 },
                            {-.60, -.40, 0.0 },  {-.80, -.80, 0.0 },
                            {-.80, -1.4, 0.0 },  {-.40, -1.60, 0.0 },
                            {0.0, -1.20, 0.0 },  { .2, -.80, 0.0 },
                            {.48, -.40, 0.0 },   {.52, -.20, 0.0 },
                            {.48,  .20, 0.0 },   {.80,  .40, 0.0 },
                            {1.20, .80, 0.0 },   {1.60, .60, 0.0 },
                            {2.0, .60, 0.0 },    {2.2, .80, 0.0 },
                            {2.40, 1.0, 0.0 },   {2.80, 1.0, 0.0 }};

    pointBatch.Begin(GL_POINTS,24);
    pointBatch.CopyVertexData3f(vCoast);
    pointBatch.End();

    lineBatch.Begin(GL_LINES,24);
    lineBatch.CopyVertexData3f(vCoast);
    lineBatch.End();

    lineStripBatch.Begin(GL_LINE_STRIP,24);
    lineStripBatch.CopyVertexData3f(vCoast);
    lineStripBatch.End();

    lineLoopBatch.Begin(GL_LINE_LOOP,24);
    lineLoopBatch.CopyVertexData3f(vCoast);
    lineLoopBatch.End();

    // For Triangles, we'll make a Pyramid
    GLfloat vPyramid[12][3] = { -2.0f, 0.0f, -2.0f,
                                2.0f, 0.0f, -2.0f,
                                0.0f, 4.0f, 0.0f,

                                2.0f, 0.0f, -2.0f,
                                2.0f, 0.0f, 2.0f,
                                0.0f, 4.0f, 0.0f,

                                2.0f, 0.0f, 2.0f,
                                -2.0f, 0.0f, 2.0f,
                                0.0f, 4.0f, 0.0f,

                                -2.0f, 0.0f, 2.0f,
                                -2.0f, 0.0f, -2.0f,
                                 0.0f, 4.0f, 0.0f};

    triangleBatch.Begin(GL_TRIANGLES,12);
    triangleBatch.CopyVertexData3f(vPyramid);
    triangleBatch.End();

    // For a Triangle fan, just a 6 sided hex. Raise the center up a bit
    GLfloat vPoints[100][3];    // Scratch array, more than we need
    int nVerts = 0;
    GLfloat r = 3.0f;
    vPoints[nVerts][0] = 0.0f;
    vPoints[nVerts][1] = 0.0f;
    vPoints[nVerts][2] = 0.0f;

    for(GLfloat angle = 0; angle < M3D_2PI; angle += M3D_2PI / 6.0f) {
        nVerts++;
        vPoints[nVerts][0] = float(cos(angle)) * r;
        vPoints[nVerts][1] = float(sin(angle)) * r;
        vPoints[nVerts][2] = -0.5f;
    }

    // Close the fan
    nVerts++;
    vPoints[nVerts][0] = r;
    vPoints[nVerts][1] = 0;
    vPoints[nVerts][2] = 0.0f;

    // Load it up
    triangleFanBatch.Begin(GL_TRIANGLE_FAN, 8);
    triangleFanBatch.CopyVertexData3f(vPoints);
    triangleFanBatch.End();

    // For triangle strips, a little ring or cylinder segment
    int iCounter = 0;
    GLfloat radius = 3.0f;
    for(GLfloat angle = 0.0f; angle <= (2.0f*M3D_PI); angle += 0.3f)
    {
        GLfloat x = radius * sin(angle);
        GLfloat y = radius * cos(angle);

        // Specify the point and move the Z value up a little
        vPoints[iCounter][0] = x;
        vPoints[iCounter][1] = y;
        vPoints[iCounter][2] = -0.5;
        iCounter++;

        vPoints[iCounter][0] = x;
        vPoints[iCounter][1] = y;
        vPoints[iCounter][2] = 0.5;
        iCounter++;
    }

    // Close up the loop
    vPoints[iCounter][0] = vPoints[0][0];
    vPoints[iCounter][1] = vPoints[0][1];
    vPoints[iCounter][2] = -0.5;
    iCounter++;

    vPoints[iCounter][0] = vPoints[1][0];
    vPoints[iCounter][1] = vPoints[1][1];
    vPoints[iCounter][2] = 0.5;
    iCounter++;

    triangleStripBatch.Begin(GL_TRIANGLE_STRIP,iCounter);
    triangleStripBatch.CopyVertexData3f(vPoints);
    triangleStripBatch.End();
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    modelViewMatrix.PushMatrix();
        M3DMatrix44f mCamera;
        cameraFrame.GetCameraMatrix(mCamera);
        modelViewMatrix.MultMatrix(mCamera);

        M3DMatrix44f mObjectFrame;
        objectFrame.GetMatrix(mObjectFrame);
        modelViewMatrix.MultMatrix(mObjectFrame);

        shaderManager.UseStockShader(GLT_SHADER_FLAT,transformPipeline.GetModelViewProjectionMatrix(),
                                     vBlack);
        switch(nStep)
        {
        case 0:
            glPointSize(4.0f);
            pointBatch.Draw();
            glPointSize(1.0f);
            break;
        case 1:
            glLineWidth(2.0f);
            lineBatch.Draw();
            glLineWidth(1.0f);
            break;
        case 2:
            glLineWidth(2.0f);
            lineStripBatch.Draw();
            glLineWidth(1.0f);
            break;
        case 3:
            glLineWidth(2.0f);
            lineLoopBatch.Draw();
            glLineWidth(1.0f);
            break;
        case 4:
            DrawWireFramedBatch(&triangleBatch);
            break;
        case 5:
            DrawWireFramedBatch(&triangleFanBatch);
            break;
        case 6:
            DrawWireFramedBatch(&triangleStripBatch);
        }
    modelViewMatrix.PopMatrix();
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

    viewFrustum.SetPerspective(35.0f,fAspect,1.0f,500.0f);
    projectionMatrix.LoadMatrix(viewFrustum.GetProjectionMatrix());
    modelViewMatrix.LoadIdentity();
}

/**
 * @brief GLWidget::keyPressEvent
 * @param ev
 */
void GLWidget::keyPressEvent(QKeyEvent *ev)
{
    if(ev->key() == Qt::Key_Up)
        objectFrame.RotateWorld(m3dDegToRad(-5.0f),1.0f,0.0f,0.0f);

    if(ev->key() == Qt::Key_Down)
        objectFrame.RotateWorld(m3dDegToRad(5.0f),1.0f,0.0f,0.0f);

    if(ev->key() == Qt::Key_Left)
        objectFrame.RotateWorld(m3dDegToRad(-5.0f),0.0f,1.0f,0.0f);

    if(ev->key() == Qt::Key_Right)
        objectFrame.RotateWorld(m3dDegToRad(5.0f),0.0f,1.0f,0.0f);


    if(ev->key() == Qt::Key_Space)
    {
        nStep++;

        if(nStep > 6)
            nStep = 0;
    }

    QString str;
    switch(nStep)
    {
    case 0:
        str = QString("GL_POINTS");
        break;
    case 1:
        str = QString("GL_LINES");
        break;
    case 2:
        str = QString("GL_LINE_STRIP");
        break;
    case 3:
        str = QString("GL_LINE_LOOP");
        break;
    case 4:
        str = QString("GL_TRIANGLES");
        break;
    case 5:
        str = QString("GL_TRIANGLE_STRIP");
        break;
    case 6:
        str = QString("GL_TRIANGLE_FAN");
        break;
    }
    emit changeTitle(str);

    update();
    QOpenGLWidget::keyPressEvent(ev);
}

void GLWidget::DrawWireFramedBatch(GLBatch *pBatch)
{
    // Draw the batch solid green
    shaderManager.UseStockShader(GLT_SHADER_FLAT,transformPipeline.GetModelViewProjectionMatrix(),
                                 vGreen);
    pBatch->Draw();

    // Draw black outline
    glPolygonOffset(-1.0f,-1.0f);
    glEnable(GL_POLYGON_OFFSET_LINE);

    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    glLineWidth(2.5f);
    shaderManager.UseStockShader(GLT_SHADER_FLAT,transformPipeline.GetModelViewProjectionMatrix(),
                                 vBlack);
    pBatch->Draw();

    // 恢复
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glDisable(GL_POLYGON_OFFSET_LINE);
    glLineWidth(1.0f);
    glDisable(GL_BLEND);
    glDisable(GL_LINE_SMOOTH);
}
