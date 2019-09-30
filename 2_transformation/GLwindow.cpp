#include "GLwindow.h"
#include <QOpenGLFunctions_4_4_Core>
#include <QOpenGLContext>
#include<iostream>
#include<fstream>
#include<sstream>
#include<QVector4D>
#include<QMatrix4x4>
#include<GL/glu.h>
#include<glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_transform.hpp>
GLuint MyGLwidget::LoadShaders(QString vertexShaderFile, QString fragmentShaderFile){
    m_VertexShader = new QOpenGLShader(QOpenGLShader::Vertex);
    bool isOk=m_VertexShader->compileSourceFile(vertexShaderFile);
    if (!isOk)
    {
        delete m_VertexShader;
        m_VertexShader=nullptr;
        throw("couldn't load vertexShader");
    }
    m_FragmentShader = new QOpenGLShader(QOpenGLShader::Fragment);
    if (!m_FragmentShader->compileSourceFile(fragmentShaderFile))
    {
        delete m_VertexShader;
        delete m_FragmentShader;
        m_FragmentShader = nullptr;
        throw("couldn't load fragmentShader");
    }
    m_Program = new QOpenGLShaderProgram(this);
    m_Program->addShader(m_VertexShader);
    m_Program->addShader(m_FragmentShader);
    m_Program->link();
    return m_Program->programId();
}
void MyGLwidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0,0,0,1);
    QOpenGLFunctions_4_4_Core *f=QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_4_Core>();

    f->glGenVertexArrays(1, &VertexArrayID);
    f->glBindVertexArray(VertexArrayID);
    static const GLfloat g_vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f,  1.0f, 0.0f,
    };
    // Generate 1 buffer, put the resulting identifier in vertexbuffer
    f->glGenBuffers(1, &vertexbuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    f->glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    // Give our vertices to OpenGL.
    f->glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    try
    {
        // Create and compile our GLSL program from the shaders
        programID = LoadShaders( "../vertexShader.vert", "../fragmentShader.frag" );
        // Use our shader
        glUseProgram(programID);

    }catch (const char* msg) {
        std::cerr<< msg << std::endl;
    }
    //Timer that can pass to vertex shader for interesting effect!
    timer.start();
    QMatrix4x4 mat=QMatrix4x4();
    //print out the matrix for debug
    for (int i=1;i<=16;i++)
    {
        std::cout<<*(mat.data()+i-1);
        if (i%4==0){
            std::cout<<std::endl;
        }
    }

}
void MyGLwidget::paintGL(){
    GLint timelapsed=timer.elapsed();
    QOpenGLFunctions_4_4_Core *f=QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_4_Core>();
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

    //projection matrix
    glm::mat4 Projection=glm::perspective(glm::radians(45.0f),(float)(this->width()/this->height()),0.1f,100.0f);
    glm::mat4 View=glm::lookAt(glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
                               glm::vec3(0,0,0), // and looks at the origin
                               glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
                               );
    // Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 Model = glm::mat4(1.0f);
    // Our ModelViewProjection : multiplication of our 3 matrices
    glm::mat4 mvp = Projection * View * Model; // Remember, matrix multiplication is the other way around

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
    f->glUniformMatrix4fv(f->glGetUniformLocation(programID,"MVP"),1,GL_FALSE,&mvp[0][0]);
    f->glUniform2f(f->glGetUniformLocation(programID,"mouse_loc"),this->pos().x(),this->pos().y());
    f->glUniform2f(f->glGetUniformLocation(programID,"resolution"),this->width(),this->height());
    f->glUniform1f(f->glGetUniformLocation(programID,"time"),timelapsed);
    // Draw the triangle !
    f->glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
    f->glDisableVertexAttribArray(0);
    this->update();
}
void MyGLwidget::resizeGL(int w, int h){
    QOpenGLFunctions_4_4_Core *f=QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_4_Core>();
    f->glViewport(0,0,w,h);
    GLfloat aspect;
    if (w>=h){
        aspect=w/h;
    }else{
        aspect=h/w;
    }
    gluOrtho2D(-w/2,w/2,-h/2*aspect,h/2*aspect);

}
