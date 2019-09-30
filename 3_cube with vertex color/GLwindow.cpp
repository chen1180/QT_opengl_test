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
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    QOpenGLFunctions_4_4_Core *f=QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_4_Core>();

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
    //color buffer
    // One color for each vertex. They were generated randomly.
    static const GLfloat g_color_buffer_data[] = {
        0.583f,  0.771f,  0.014f,
        0.609f,  0.115f,  0.436f,
        0.327f,  0.483f,  0.844f,
        0.822f,  0.569f,  0.201f,
        0.435f,  0.602f,  0.223f,
        0.310f,  0.747f,  0.185f,
        0.597f,  0.770f,  0.761f,
        0.559f,  0.436f,  0.730f,
        0.359f,  0.583f,  0.152f,
        0.483f,  0.596f,  0.789f,
        0.559f,  0.861f,  0.639f,
        0.195f,  0.548f,  0.859f,
        0.014f,  0.184f,  0.576f,
        0.771f,  0.328f,  0.970f,
        0.406f,  0.615f,  0.116f,
        0.676f,  0.977f,  0.133f,
        0.971f,  0.572f,  0.833f,
        0.140f,  0.616f,  0.489f,
        0.997f,  0.513f,  0.064f,
        0.945f,  0.719f,  0.592f,
        0.543f,  0.021f,  0.978f,
        0.279f,  0.317f,  0.505f,
        0.167f,  0.620f,  0.077f,
        0.347f,  0.857f,  0.137f,
        0.055f,  0.953f,  0.042f,
        0.714f,  0.505f,  0.345f,
        0.783f,  0.290f,  0.734f,
        0.722f,  0.645f,  0.174f,
        0.302f,  0.455f,  0.848f,
        0.225f,  0.587f,  0.040f,
        0.517f,  0.713f,  0.338f,
        0.053f,  0.959f,  0.120f,
        0.393f,  0.621f,  0.362f,
        0.673f,  0.211f,  0.457f,
        0.820f,  0.883f,  0.371f,
        0.982f,  0.099f,  0.879f
    };
    f->glGenBuffers(1,&colorbuffer);
    f->glBindBuffer(GL_ARRAY_BUFFER,colorbuffer);
    f->glBufferData(GL_ARRAY_BUFFER,sizeof(g_color_buffer_data),g_color_buffer_data,GL_STATIC_DRAW);
    try
    {
        // Create and compile our GLSL program from the shaders
        programID = LoadShaders( "../3_/vertexShader.vert", "../3_/fragmentShader.frag" );
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
    f->glEnableVertexAttribArray(1);
    f->glBindBuffer(GL_ARRAY_BUFFER,colorbuffer);
    f->glVertexAttribPointer(
                1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
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
    f->glDrawArrays(GL_TRIANGLES, 0, 12*3); // Starting from vertex 0; 3 vertices total -> 1 triangle
    f->glDisableVertexAttribArray(0);
    f->glDisableVertexAttribArray(1);
    this->update();
}
void MyGLwidget::resizeGL(int w, int h){
//    QOpenGLFunctions_4_4_Core *f=QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_4_Core>();
//    f->glViewport(0,0,w,h);
//    GLfloat aspect;
//    if (w>=h){
//        aspect=w/h;
//    }else{
//        aspect=h/w;
//    }
//    gluOrtho2D(-w/2,w/2,-h/2*aspect,h/2*aspect);

}
