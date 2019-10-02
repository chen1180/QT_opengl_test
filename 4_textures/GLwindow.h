#ifndef GLWINDOW_H
#define GLWINDOW_H
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include<QOpenGLShader>
#include<QOpenGLShaderProgram>
#include<QOpenGLTexture>
#include<QString>
#include<QTime>
class MyGLwidget:public QOpenGLWidget,protected QOpenGLFunctions
{
private:
    QTime timer;
    QOpenGLShader *m_VertexShader;
    QOpenGLShader *m_FragmentShader;
    QOpenGLShaderProgram *m_Program;
    QOpenGLTexture *textureID;

    GLuint VertexArrayID;
    GLuint vertexbuffer;
    // This will identify our color buffer
    GLuint uvbuffer;
    GLuint programID;
protected:
    GLuint LoadShaders(QString vertexShaderFile,QString fragmentShaderFile);
    void initializeGL();
    void paintGL();
    void loadGLTextures();

};

#endif // GLWINDOW_H
