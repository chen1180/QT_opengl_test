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
GLuint MyGLwidget::LoadShaders(QString vertexShaderFile, QString fragmentShaderFile,QString geometryShaderFile){
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
    glEnable(GL_MULTISAMPLE);
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
        1.0f, 1.0f,-1.0f, // triangle 2 : begin
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
        1.0f,-1.0f, 1.0f
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
        1.000023f, 1.0f-0.000013f,
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
        0.667979f, 1.0f-0.335851f
    };
    f->glGenBuffers(1,&uvbuffer);
    f->glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    // Give our vertices to OpenGL.
    f->glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);
    //trianle normal vector
    //use vertex coordinate to calculate normal vector of triangles
    GLfloat v_normal_buffer_data[sizeof(g_vertex_buffer_data)/sizeof(g_vertex_buffer_data[0])/3][3];
    for (unsigned int i=0;i<sizeof(g_vertex_buffer_data)/sizeof(g_vertex_buffer_data[0])/3;i++){
        v_normal_buffer_data[i][0]=g_vertex_buffer_data[i*3];
        v_normal_buffer_data[i][1]=g_vertex_buffer_data[i*3+1];
        v_normal_buffer_data[i][2]=g_vertex_buffer_data[i*3+2];
    }
    //debug
    //    for (unsigned int i=0;i<sizeof(g_vertex_buffer_data)/sizeof(g_vertex_buffer_data[0])/3;i++){
    //        qDebug()<<v_normal_buffer_data[i][0]<<v_normal_buffer_data[i][1]<<v_normal_buffer_data[i][2];
    //    }
    GLuint norml_buffer_size=sizeof(v_normal_buffer_data)/sizeof(v_normal_buffer_data[0]);
    QVector3D normal_buffer_data[norml_buffer_size];
    for (unsigned int i=0;i<norml_buffer_size;i+=3){
        //
        QVector3D v1=QVector3D(v_normal_buffer_data[i][0],v_normal_buffer_data[i][1],v_normal_buffer_data[i][2]);
        QVector3D v2=QVector3D(v_normal_buffer_data[i+1][0],v_normal_buffer_data[i+1][1],v_normal_buffer_data[i+1][2]);
        QVector3D v3=QVector3D(v_normal_buffer_data[i+2][0],v_normal_buffer_data[i+2][1],v_normal_buffer_data[i+2][2]);
        QVector3D triangle_normal=QVector3D::crossProduct(v2-v1,v3-v1).normalized();
        normal_buffer_data[i]=triangle_normal;
        normal_buffer_data[i+1]=triangle_normal;
        normal_buffer_data[i+2]=triangle_normal;
    }
    //debug
    //    for (unsigned int i=0;i<norml_buffer_size;i++){
    //        qDebug()<<normal_buffer_data[i];
    //    }
    //bind normal buffer
    glGenBuffers(1,&vertexNormalbuffer);
    glBindBuffer(GL_ARRAY_BUFFER,vertexNormalbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normal_buffer_data), &normal_buffer_data[0], GL_STATIC_DRAW);
    //compile shaders
    defaultShader = LoadShaders( ":/default shader/vertexShader.vert", ":/default shader/fragmentShader.frag");
    normalShader=LoadShaders( ":/normal shader/normalVertex.vert",  ":/normal shader/normalFragment.frag",":/normal shader/normalGeometry.gs");
    int index=0;
    GLfloat offset = 4.0f;
    for(GLint y =-50; y < 50; y ++)
    {
        for(GLint x =-50; x < 50; x ++)
        {
            QVector3D translation=QVector3D(x*offset,y*offset,0);
            translations[index++] = translation;
        }
    }
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
    //3rd attribute buffer: Normal
    f->glEnableVertexAttribArray(2);
    f->glBindBuffer(GL_ARRAY_BUFFER,vertexNormalbuffer);
    f->glVertexAttribPointer(
                2,                  // attribute 2. No particular reason for 0, but must match the layout in the shader.
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                nullptr            // array buffer offset
                );
    //Camera transformation
    QMatrix4x4 Projection,View,Model;
    Projection.perspective(45.0f,(float)(this->width()/this->height()),0.1f,100.0f);
    View.rotate(m_trackBalls.getRotation());
    View(2, 3) -=m_trackBalls.getZoomFactor();
    glUseProgram(defaultShader);
    //Transfer data into Opengl pipeline, communnicate with shader
    f->glUniformMatrix4fv(f->glGetUniformLocation(defaultShader,"projection"),1,GL_FALSE,Projection.constData());
    f->glUniformMatrix4fv(f->glGetUniformLocation(defaultShader,"view"),1,GL_FALSE,View.constData());
    f->glUniformMatrix4fv(f->glGetUniformLocation(defaultShader,"model"),1,GL_FALSE,Model.constData());
    f->glUniform1f(f->glGetUniformLocation(defaultShader,"myTextureSampler"),textureID->textureId());
    for(GLuint i = 0; i < 10000; i++)
    {
        QString index="offsets["+QString::number(i)+"]";
        f->glUniform3f(glGetUniformLocation(defaultShader,index.toStdString().c_str()),translations[i].x(),translations[i].y(),translations[i].z());
    }
    textureID->bind(0);
    // Draw the triangle !
    // Use our shader
    glUseProgram(defaultShader);
//    f->glDrawArrays(GL_TRIANGLES, 0, 12*3); // Starting from vertex 0; 3 vertices total -> 1 triangle
    f->glDrawArraysInstanced(GL_TRIANGLES, 0, 12*3,10000);
    //Transfer data into Opengl pipeline, communnicate with shader
    f->glUniformMatrix4fv(f->glGetUniformLocation(normalShader,"projection"),1,GL_FALSE,Projection.constData());
    f->glUniformMatrix4fv(f->glGetUniformLocation(normalShader,"view"),1,GL_FALSE,View.constData());
    f->glUniformMatrix4fv(f->glGetUniformLocation(normalShader,"model"),1,GL_FALSE,Model.constData());
    f->glUniform1f(f->glGetUniformLocation(normalShader,"myTextureSampler"),textureID->textureId());
    //Display normal vector of each vertices
    glUseProgram(normalShader);
    f->glDrawArrays(GL_TRIANGLES, 0, 12*3);
    f->glViewport(int(this->width()/2),0,this->width()/2,this->height()/2);
    f->glDisableVertexAttribArray(0);
    f->glDisableVertexAttribArray(1);
    f->glDisableVertexAttribArray(2);
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
