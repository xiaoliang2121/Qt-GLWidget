#include "glwidget.h"

#ifdef WIN32
#include <gl/GLU.h>
#else
#include <GL/glu.h>
#endif

#include <QKeyEvent>
#include <QMouseEvent>
#include <QDebug>

GLfloat blockSize = 0.1f;
GLfloat vVerts[] = { -blockSize, -blockSize, 0.0f,
                      blockSize, -blockSize, 0.0f,
                      blockSize,  blockSize, 0.0f,
                     -blockSize,  blockSize, 0.0f};

GLWidget::GLWidget(QWidget *parent):
    QOpenGLWidget(parent),
    xRot(0.0f),
    yRot(0.0f),
    xPos(0.0f),
    yPos(0.0f)
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

void GLWidget::SetupRC()
{
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

    shaderManager.InitializeStockShaders();

    squareBatch.Begin(GL_TRIANGLE_FAN, 4);
    squareBatch.CopyVertexData3f(vVerts);
    squareBatch.End();
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
    // Clear the window with current clearing color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    GLfloat vRed[] = { 1.0f, 0.0f, 0.0f, 1.0f };

    M3DMatrix44f mFinalTransform, mTranslationMatrix, mRotationMatrix;
    m3dTranslationMatrix44(mTranslationMatrix,xPos,yPos,0.0f);

    yRot += 5.0f;
    m3dRotationMatrix44(mRotationMatrix,m3dDegToRad(yRot),0.0f,0.0f,1.0f);
    m3dMatrixMultiply44(mFinalTransform,mTranslationMatrix,mRotationMatrix);

    shaderManager.UseStockShader(GLT_SHADER_FLAT, mFinalTransform, vRed);
    squareBatch.Draw();
}

void GLWidget::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
}

/**
 * @brief GLWidget::keyPressEvent
 * @param ev
 */
void GLWidget::keyPressEvent(QKeyEvent *ev)
{
    GLfloat stepSize = 0.025f;

    if(ev->key() == Qt::Key_Up)
        yPos += stepSize;

    if(ev->key() == Qt::Key_Down)
        yPos -= stepSize;

    if(ev->key() == Qt::Key_Left)
        xPos -= stepSize;

    if(ev->key() == Qt::Key_Right)
        xPos += stepSize;

    // Collision detection
    if(xPos < (-1.0f + blockSize)) xPos = -1.0f + blockSize;

    if(xPos > (1.0f - blockSize)) xPos = 1.0f - blockSize;

    if(yPos < (-1.0f + blockSize))  yPos = -1.0f + blockSize;

    if(yPos > (1.0f - blockSize)) yPos = 1.0f - blockSize;

    update();
    QOpenGLWidget::keyPressEvent(ev);
}
