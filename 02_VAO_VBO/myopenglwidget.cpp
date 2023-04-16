#include "myopenglwidget.h"

MyOpenGLWidget::MyOpenGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{

}

// 声明VAO,VBO对象
unsigned int VBO,VAO;

float vertices[] = {
    -0.5f,-0.5f,0.0f,
    0.5f,-0.5f,0.0f,
    0.0f,0.5f,0.0f
};

void MyOpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    // 创建VAO,VBO对象
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);

    // 绑定VAO,VBO对象
    glBindVertexArray(VAO); // 绑定一个点数组对象
    glBindBuffer(GL_ARRAY_BUFFER,VBO); // GL_ARRAY_BUFFER顶点属性

    // 为当前绑定到target的缓冲区对象创建一个新的数据存储
    // 如果data不是NULL，则使用来自此指针的数据初始化数据存储
    glBufferData(GL_ARRAY_BUFFER,sizeof (vertices),vertices,GL_STATIC_DRAW);

    // 告知显卡如何解析缓冲区里的属性值
    // 定义通用顶点属性数据的数组
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof (float),(void*)0);

    // 开启VAO管理的第一个属性值
    glEnableVertexAttribArray(0);

    // 绑定为默认值（养成好习惯）
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
}

void MyOpenGLWidget::resizeGL(int w, int h)
{

}

void MyOpenGLWidget::paintGL()
{
    // 设置状态
    glClearColor(0.2f,0.3f,0.3f,1.0f);
    // 使用状态
    glClear(GL_COLOR_BUFFER_BIT);

    //
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES,0,3);
}
