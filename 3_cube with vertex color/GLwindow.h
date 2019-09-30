#ifndef GLWINDOW_H
#define GLWINDOW_H
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include<QString>
#include<QOpenGLShader>
#include<QOpenGLShaderProgram>
#include<QTime>
class MyGLwidget:public QOpenGLWidget,protected QOpenGLFunctions
{
private:
    QTime timer;
    QOpenGLShader *m_VertexShader;
    QOpenGLShader *m_FragmentShader;
    QOpenGLShaderProgram *m_Program;
    GLuint VertexArrayID;
    // This will identify our vertex buffer
    GLuint vertexbuffer;
    // This will identify our color buffer
    GLuint colorbuffer;
    GLuint programID;
protected:
    GLuint LoadShaders(QString vertexShaderFile,QString fragmentShaderFile);
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);
};

#endif // GLWINDOW_H
