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

    this->setWindowTitle("Spectral Toolbox");

    connect(
        m_ui->actionOpen_SEG_Y_file,
        &QAction::triggered,
        this,
        &main_window::open_segy_file
    );

    m_file_widget = std::make_shared<file_widget>(this);
    m_ui->mainLayout->addWidget(m_file_widget.get());

    connect(
        this,
        &main_window::data_loaded,
        m_file_widget.get(),
        &file_widget::refresh
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
        emit data_loaded();
    }
    else {
        spdlog::error("filename is isEmpty");
    }
}
