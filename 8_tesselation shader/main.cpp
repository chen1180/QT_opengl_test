#include <QApplication>
#include <QSurfaceFormat>
#include "GLwindow.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(resource);
    QApplication a(argc, argv);
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(4, 4);
    format.setProfile(QSurfaceFormat::CoreProfile);
    MyGLwidget widget;
    widget.setFormat(format);
    widget.show();
    return a.exec();
}
