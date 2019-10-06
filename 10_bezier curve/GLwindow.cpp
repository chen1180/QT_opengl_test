#include "GLwindow.h"
#include <QOpenGLFunctions_4_4_Core>
#include <QOpenGLContext>
#include<iostream>
#include<fstream>
#include<sstream>
#include<QVector4D>
#include<QMatrix4x4>
#include<GL/glu.h>
//#include<glm/glm.hpp>
//#include<glm/gtc/type_ptr.hpp>
//#include<glm/gtc/matrix_transform.hpp>
#include<QImage>
#include<QDebug>
GLuint MyGLwidget::LoadShaders(QString vertexShaderFile, QString fragmentShaderFile, QString geometryShaderFile, QString tesControlShaderFile, QString tesEvalShaderFile){
    QOpenGLShaderProgram *m_Program = new QOpenGLShaderProgram(this);
    try{
        m_VertexShader = new QOpenGLShader(QOpenGLShader::Vertex);
        bool isOk=m_VertexShader->compileSourceFile(vertexShaderFile);
        if (!isOk)
        {
            throw("couldn't load vertexShader");
        }
        else m_Program->addShader(m_VertexShader);
    }catch (const char* msg) {
        std::cerr<< msg << std::endl;
        delete m_VertexShader;
        m_VertexShader=nullptr;
    }
    try{
        m_FragmentShader = new QOpenGLShader(QOpenGLShader::Fragment);
        if (!m_FragmentShader->compileSourceFile(fragmentShaderFile))
        {
            throw("couldn't load fragmentShader");
        }else m_Program->addShader(m_FragmentShader);
    }catch (const char* msg) {
        std::cerr<< msg << std::endl;
        delete m_FragmentShader;
        m_FragmentShader = nullptr;
    }
    if (!geometryShaderFile.isNull()){
        try{
            m_GeometryShader = new QOpenGLShader(QOpenGLShader::Geometry);
            if (!m_GeometryShader->compileSourceFile(geometryShaderFile))
            {
                throw("couldn't load geometrytShader");
            }else m_Program->addShader(m_GeometryShader);
        }catch (const char* msg) {
            std::cerr<< msg << std::endl;
            delete m_GeometryShader;
            m_GeometryShader = nullptr;
        }

    }
    if (!tesControlShaderFile.isNull()){
        try{
            m_TesControlShader = new QOpenGLShader(QOpenGLShader::TessellationControl);
            if (!m_TesControlShader->compileSourceFile(tesControlShaderFile))
            {
                throw("couldn't load TessellationControl shader");
            }else m_Program->addShader(m_TesControlShader);
        }catch (const char* msg) {
            std::cerr<< msg << std::endl;
            delete m_TesControlShader;
            m_TesControlShader = nullptr;
        }
    }
    if (!tesEvalShaderFile.isNull()){
        try{
            m_TesEvalShader = new QOpenGLShader(QOpenGLShader::TessellationEvaluation);
            if (!m_TesEvalShader->compileSourceFile(tesEvalShaderFile))
            {
                throw("couldn't load TessellationEvaluation shader");
            }else m_Program->addShader(m_TesEvalShader);
        }catch (const char* msg) {
            std::cerr<< msg << std::endl;
            delete m_TesEvalShader;
            m_TesEvalShader = nullptr;
        }
    }
    try{
        m_Program->link();

    }catch (const char* msg) {
        std::cerr<< msg << std::endl;
    }

    return m_Program->programId();
}
void MyGLwidget::initializeGL()
{
    initializeOpenGLFunctions();
    //initialize trackball camera
    m_trackBalls = TrackBall(0.0f, QVector3D(0, 1, 0), TrackBall::Sphere);
    glClearColor(0,0,0,1);
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    QOpenGLFunctions_4_4_Core *f=QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_4_Core>();
    //load textures
    loadGLTextures();
    f->glGenVertexArrays(1, &VertexArrayID);
    f->glBindVertexArray(VertexArrayID);
    static const GLfloat g_vertex_buffer_data[] = {
        -1.0f,-1.0f,-1.0f, // triangle 1 : begin
        -1.0f,-1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f, // triangle 1 : end
        1.0f, 1.0f,-1.0f/*, // triangle 2 : begin
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f, // triangle 2 : end
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f*/
    };
    // Generate 1 buffer, put the resulting identifier in vertexbuffer
    f->glGenBuffers(1, &vertexbuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    f->glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    // Give our vertices to OpenGL.
    f->glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    // Two UV coordinatesfor each vertex. They were created with Blender. You'll learn shortly how to do this yourself.
    static const GLfloat g_uv_buffer_data[] = {
        0.000059f, 1.0f-0.000004f,
        0.000103f, 1.0f-0.336048f,
        0.335973f, 1.0f-0.335903f,
        1.000023f, 1.0f-0.000013f/*,
        0.667979f, 1.0f-0.335851f,
        0.999958f, 1.0f-0.336064f,
        0.667979f, 1.0f-0.335851f,
        0.336024f, 1.0f-0.671877f,
        0.667969f, 1.0f-0.671889f,
        1.000023f, 1.0f-0.000013f,
        0.668104f, 1.0f-0.000013f,
        0.667979f, 1.0f-0.335851f,
        0.000059f, 1.0f-0.000004f,
        0.335973f, 1.0f-0.335903f,
        0.336098f, 1.0f-0.000071f,
        0.667979f, 1.0f-0.335851f,
        0.335973f, 1.0f-0.335903f,
        0.336024f, 1.0f-0.671877f,
        1.000004f, 1.0f-0.671847f,
        0.999958f, 1.0f-0.336064f,
        0.667979f, 1.0f-0.335851f,
        0.668104f, 1.0f-0.000013f,
        0.335973f, 1.0f-0.335903f,
        0.667979f, 1.0f-0.335851f,
        0.335973f, 1.0f-0.335903f,
        0.668104f, 1.0f-0.000013f,
        0.336098f, 1.0f-0.000071f,
        0.000103f, 1.0f-0.336048f,
        0.000004f, 1.0f-0.671870f,
        0.336024f, 1.0f-0.671877f,
        0.000103f, 1.0f-0.336048f,
        0.336024f, 1.0f-0.671877f,
        0.335973f, 1.0f-0.335903f,
        0.667969f, 1.0f-0.671889f,
        1.000004f, 1.0f-0.671847f,
        0.667979f, 1.0f-0.335851f*/
    };
    f->glGenBuffers(1,&uvbuffer);
    f->glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    // Give our vertices to OpenGL.
    f->glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);

    //patch vertices
    f->glGetIntegerv(GL_MAX_PATCH_VERTICES, &MaxPatchVertices);
    qDebug()<<MaxPatchVertices;
    f->glPatchParameteri(GL_PATCH_VERTICES, 4);
    qDebug()<<MaxPatchVertices;
    //compile shaders
    defaultShader = LoadShaders( ":/lighting.vert", ":/lighting.frag",NULL,":/lighting.cs",":/lighting.es");

}
void MyGLwidget::paintGL(){
    QOpenGLFunctions_4_4_Core *f=QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_4_Core>();
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // 1rst attribute buffer : vertices
    f->glEnableVertexAttribArray(0);
    f->glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    f->glVertexAttribPointer(
                0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                nullptr            // array buffer offset
                );
    // 2nd attribute buffer : UV
    f->glEnableVertexAttribArray(1);
    f->glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    f->glVertexAttribPointer(
                1,                  // attribute 1. No particular reason for 0, but must match the layout in the shader.
                2,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                nullptr            // array buffer offset
                );
    //Camera transformation
    QMatrix4x4 Projection,View,Model,MVP;
    Projection.perspective(45.0f,(float)(this->width()/this->height()),0.1f,100.0f);
    View.rotate(m_trackBalls.getRotation());
    View(2, 3) -=m_trackBalls.getZoomFactor();
    MVP=Projection*View*Model;
    //vertex shader attribute
    f->glUniformMatrix4fv(f->glGetUniformLocation(defaultShader,"gWorld"),1,GL_FALSE,Model.constData());
    //tesselation control shader attribute
    f->glUniform3f(f->glGetUniformLocation(defaultShader,"gEyeWorldPos"),m_trackBalls.getRotation().vector().x(),m_trackBalls.getRotation().vector().y(),m_trackBalls.getRotation().vector().z());
    //tesselation evaluator shader attribute
    f->glUniformMatrix4fv(f->glGetUniformLocation(defaultShader,"gVP"),1,GL_FALSE,MVP.constData());
    //fragment shader attribute
    f->glUniform1f(f->glGetUniformLocation(defaultShader,"gColorMap"),textureID->textureId());

    textureID->bind(0);
    // Draw the triangle !
    // Use our shader
    glUseProgram(defaultShader);
    f->glDrawArrays(GL_PATCHES, 0, 4*3); // Starting from vertex 0; 3 vertices total -> 1 triangle;
    f->glDisableVertexAttribArray(0);
    f->glDisableVertexAttribArray(1);
    this->update();
}
void MyGLwidget::loadGLTextures(){
    QImage tex, buf;
    if(!buf.load(":/texture.jpeg"))
    {
        qWarning("Cannot open the image...");
        QImage dummy(128, 128, QImage::Format_RGB32);
        dummy.fill(Qt::green);
        buf = dummy;
    }
    textureID=new QOpenGLTexture(buf.mirrored(),QOpenGLTexture::GenerateMipMaps);
    textureID->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    textureID->setMagnificationFilter(QOpenGLTexture::Linear);
}
void MyGLwidget::mousePressEvent(QMouseEvent *event){
    QOpenGLWidget::mousePressEvent(event);
    if (event->isAccepted())
        return;
    if (event->buttons() & Qt::MidButton) {
        m_trackBalls.push(pixelPosToViewPos(event->windowPos()),QQuaternion());
        qDebug()<<"rotation:"<<m_trackBalls.rotation();
        qDebug()<<"axis:"<<m_trackBalls.getAxis();
        event->accept();
    }
}
void MyGLwidget::mouseMoveEvent(QMouseEvent *event){
    QOpenGLWidget::mouseMoveEvent(event);
    if (event->isAccepted())
        return;
    if (event->buttons() & Qt::MidButton) {
        m_trackBalls.move(pixelPosToViewPos(event->windowPos()), QQuaternion());
        event->accept();
    } else {
        m_trackBalls.release(pixelPosToViewPos(event->windowPos()), QQuaternion());
    }
}
void MyGLwidget::mouseReleaseEvent(QMouseEvent *event){
    QOpenGLWidget::mouseReleaseEvent(event);
    if (event->isAccepted())
        return;
    if (event->button() == Qt::MidButton) {
        m_trackBalls.release(pixelPosToViewPos(event->windowPos()), QQuaternion());
        event->accept();
    }
}
void MyGLwidget::wheelEvent(QWheelEvent *event){
    QOpenGLWidget::wheelEvent(event);
    if (!event->isAccepted()) {
        m_trackBalls.zoom(event->delta());
        event->accept();
    }
}
QPointF MyGLwidget::pixelPosToViewPos(const QPointF &p){
    return QPointF(2.0 * float(p.x()) / width() - 1.0,
                   1.0 - 2.0 * float(p.y()) / height());
}
