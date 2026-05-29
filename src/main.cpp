#include <QApplication>

#include "gui/MainWindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    QApplication::setApplicationName(QStringLiteral("川麻向听分析助手"));

    MainWindow window;
    window.show();

    return app.exec();
}
