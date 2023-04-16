# QT+OpenGL

[opengl函数](https://registry.khronos.org/OpenGL-Refpages/es3/)

[opengl中文教程](https://learnopengl-cn.github.io/)

## 1、OpenGL简介

openGL是C库,它是一种规范。



### 1.1 核心模式

也叫做可编程模式，提供了更多的灵活性和更高的效率，可以更深入的理解图形编程。

![image-20230416191059094](https://test-123456-md-images.oss-cn-beijing.aliyuncs.com/img/image-20230416191059094.png)

主要的就是顶点着色器和片段着色器



### 1.2 立即渲染模式

- 早期的oepngl模式
- 效率低、开发者不能控制opengl进行计算



### 1.3 状态机

- opengl是一个巨大的状态机，描述该如何操作的所有变量的大集合；
- opengl的状态通常被称为**上下文（Context）**；
- 状态设置函数(State-changing Function);
- 状态应用的函数(State-using Function)

通过改变上下文变量来改变opengl状态，从而高速opengl如何去绘图 

![image-20230416191931713](https://test-123456-md-images.oss-cn-beijing.aliyuncs.com/img/image-20230416191931713.png)



### 1.4 对象（Onject）

- 一个对象是指一些选项的集合,代表opengl状态的一个子集；
- 如我们可以用一个对象来表示绘图窗口的设置：设置大小、支持的颜色位数等；

通常的话把opengl的上下文比作一个大的结构体，包含了很多子集。

![image-20230416192228923](https://test-123456-md-images.oss-cn-beijing.aliyuncs.com/img/image-20230416192228923.png)

10个子集，每个子集10种不同的状态，那么需要100个小助理

**使用案例**

```c++
// 创建对象
GLuint objectId = 0;
glGenObject(1,&objectId); // 给小助理一个编号

// 绑定对象至上下文
glBindObject(GL_WINDOW_TARGET,onjectId); // 该小助理的工作内容，记录GL_WINDOW_TARGET的状态

// 设置GL_WINDOW_TARGET对象的一些选项
glSetObjectOption(GL_WINDOW_TARGET,GL_OPTION_WINDOW_WIDTH,800);
glSetObjectOption(GL_WINDOW_TARGET,GL_OPTION_WINDOW_HEIGHT,600);

// 将上下文的GL_WINDOW_TARGET对象设置为默认
glBindObject(GL_window_targe,0); // 小助理已经记录了内容（宽度改为800，高度改为600），可以休息了，需要查看的时候再叫来

// 一旦重新绑定这个小助理到GL_WINDOW_TARGET位置，这些选项就会重新生效
```

## 2、Qt中的opengl

### 2.1 QOpenGLWidget

不需要GLFW

QOpenGLWidget提供了三个便捷的虚函数，可以重载，用来重新实现典型的OpenGL任务

- paintGL：渲染OpenGL:场景。widget需要更新时调用。
- resizeGL：设置OpenGLi视口、投影等。widget调整大小（或首次显示）时调用。
- initializeGL：设置OpenGL资源和状态。第一次调用resizeGL（）/paintGL（）之前调用一次。

如果需要从`paintGL()`以外的位置触发重新绘制（典型示例是使用计时器设置场景动画），则应调用widget的`update()`函数来安排更新。

调用`paintGL（）`、`resizeGL（）`或`initializeGL（）`时，widget的OpenGL呈现上下文将变为当前。如果需要从其他位置（例如，在widget的构造函数或自己的绘制函数中）调用标准OpenGL APi函数，则必须首先调用`makeCurrent（）`

在`paintGL()`以外的地方调用绘制函数，没有意义。绘制图像最终将被`paintGL()`覆盖。



### 2.2 QOpenGLFunctions

提供了opengl X.X版本核心模式的所有功能，是对opengl函数的封装

`initializeOpenGLFunctions`：初始化opengl函数，将qt里的函数指针指向显卡的函数



### 2.3 初识opengl

#### 2.3.1 图形界面

添加openglwidget窗口

![image-20230416201635946](https://test-123456-md-images.oss-cn-beijing.aliyuncs.com/img/image-20230416201635946.png)



#### 2.3.2 继承新类

创建一个新的类MyOpenGLWidget，让openglwidget窗口继承它

```c++
// mympenglWidget.h
#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H

#include <QObject>
#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>

class MyOpenGLWidget : public QOpenGLWidget,QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    explicit MyOpenGLWidget(QWidget *parent = nullptr);

    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();
signals:

};

#endif // MYOPENGLWIDGET_H

```

重写3个虚函数

```c++
// myopenglwidget.cpp
#include "myopenglwidget.h"

MyOpenGLWidget::MyOpenGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{

}

void MyOpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
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
}

```

#### 2.3.3 输出

![image-20230416202031861](https://test-123456-md-images.oss-cn-beijing.aliyuncs.com/img/image-20230416202031861.png)



## 2、VAO和VBO

1. 顶点着色器：他会在GPU上创建内存，用于存储我们的顶点数据，通过顶点缓冲对象（VBO）管理，顶点缓冲对象的缓冲类型是`GL_ARRAY_BUFFER`；
2. 配置OpenGL如何解释这些内存，通过顶点数组对象（VAO）管理。数组里的每一个项都对应一个属性的解析；
3. opengl允许我们同时绑定多个缓冲，只要他们是不同的缓冲类型（每一个缓冲类型类似于前面说的子集，每个VBO是一个小助理）；
4. VAO存储的不是真实数据点，而是数据点的结构。



![image-20230416203509898](https://test-123456-md-images.oss-cn-beijing.aliyuncs.com/img/image-20230416203509898.png)

### 2.1 绘制三角形

![image-20230416211054294](https://test-123456-md-images.oss-cn-beijing.aliyuncs.com/img/image-20230416211054294.png)

代码示例

```c++
// myopenglwidget.cpp
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

    // 绑定VAO并绘制图元
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES,0,3);
}

```

函数解析

1. glGenVertexArrays：创建一个VAO对象；
2. glBindVertexArray：绑定一个vertex arrar对象（VAO），`void glBindVertexArray(GLuint array);`，array为`glGenVertexArrays`生成的对象；
3. glBindBuffer：绑定一个buffer对象（VBO）；
4. glBufferData：creates and initializes a buffer object's data store

```c++
void glBufferData(	GLenum target,
 	GLsizeiptr size,
 	const void * data,
 	GLenum usage);
```

5. glVertexAttribPointer :定义通用顶点属性数据的数组

```c++
void glVertexAttribPointer(	GLuint index,
 	GLint size,
 	GLenum type,
 	GLboolean normalized,
 	GLsizei stride,
 	const void * pointer);
```

6. glDrawArrays：从数组数据渲染图元

```c++
void glDrawArrays(	GLenum mode,
 	GLint first,
 	GLsizei count);
```







