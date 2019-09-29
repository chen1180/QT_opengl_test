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
protected:
    GLuint LoadShaders(QString vertexShaderFile,QString fragmentShaderFile);
    void initializeGL();
    void paintGL();
};

#endif // GLWINDOW_H
