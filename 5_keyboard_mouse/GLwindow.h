#ifndef GLWINDOW_H
#define GLWINDOW_H
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include<QOpenGLShader>
#include<QOpenGLShaderProgram>
#include<QOpenGLTexture>
#include<QString>
#include<QTime>
#include"trackball.h"
#include<QPointF>
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
    GLuint vertexNormalbuffer;
    GLuint programID;
    //trackball camera
    TrackBall m_trackBalls;


protected:
    GLuint LoadShaders(QString vertexShaderFile,QString fragmentShaderFile);
    void initializeGL();
    void paintGL();
    void loadGLTextures();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
public:
    QPointF pixelPosToViewPos(const QPointF& p);

};

#endif // GLWINDOW_H
