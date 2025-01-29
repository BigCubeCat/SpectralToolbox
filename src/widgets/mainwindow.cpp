#include "../../include/widgets/mainwindow.hpp"

#include <QFileDialog>

#include <spdlog/spdlog.h>

#include "datamodel.hpp"
#include "ui_mainwindow.h"

main_window::main_window(QWidget *parent)
    : QMainWindow(parent), m_ui(new Ui::MainWindow) {
    m_ui->setupUi(this);
    m_param_widget = std::make_shared<procedure_config_widget>(this);
    m_ui->paramLayout->addWidget(m_param_widget.get());

    connect(
        m_ui->actionOpen_SEG_Y_file,
        &QAction::triggered,
        this,
        &main_window::open_segy_file
    );
}

main_window::~main_window() {
    delete m_ui;
}

void main_window::open_segy_file() {
    QString file_name = QFileDialog::getOpenFileName(
        this, "Выберите файл", "", "Все файлы (*.*)"
    );

    if (!file_name.isEmpty()) {
        datamodel::instance()->open_file(file_name.toStdString());
    }
    else {
        spdlog::error("filename is isEmpty");
    }
}
