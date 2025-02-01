#include <QApplication>

#include <spdlog/spdlog.h>

#include "../include/widgets/mainwindow.hpp"

int main(int argc, char *argv[]) {
    spdlog::set_level(spdlog::level::debug);
    QApplication a(argc, argv);
    main_window w;
    w.show();
    return QApplication::exec();
}
