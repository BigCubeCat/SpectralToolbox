#include <QApplication>

#include "../include/widgets/mainwindow.hpp"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    main_window w;
    w.show();
    return QApplication::exec();
}
