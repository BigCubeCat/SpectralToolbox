#include "../../include/widgets/mainwindow.hpp"

#include <QFileDialog>

#include <spdlog/spdlog.h>

#include "datamodel.hpp"
#include "ui_mainwindow.h"

main_window::main_window(QWidget *parent)
    : QMainWindow(parent), m_ui(new Ui::MainWindow) {
    m_ui->setupUi(this);
    this->setWindowTitle("Spectral Toolbox");

    m_ui->renderLayout->addWidget(&m_data);

    connect(
        m_ui->actionOpen_SEG_Y_file,
        &QAction::triggered,
        this,
        &main_window::open_segy_file
    );

    connect(this, &main_window::data_loaded, this, &main_window::on_open_file);
    connect(
        m_ui->tracenoSpin,
        &QSpinBox::valueChanged,
        this,
        &main_window::set_traceno
    );
    connect(
        m_ui->crosslineSpin,
        &QSpinBox::valueChanged,
        this,
        &main_window::set_crossline
    );
}

void main_window::open_segy_file() {
    QString file_name = QFileDialog::getOpenFileName(
        this, "Выберите файл", "", "Все файлы (*.*)"
    );

    if (!file_name.isEmpty()) {
        m_filename = file_name.toStdString();
        datamodel::instance()->open_file(m_filename);
        emit data_loaded();
    }
    else {
        spdlog::error("filename is isEmpty");
    }
    refresh(true);
}

void main_window::refresh(bool update_settings) {
    m_data.need_update.store(true);
    m_data.render_image();
    if (!update_settings)
        return;
    auto *context = datamodel::instance();
    if (context) {
        auto *reader = context->reader();
        if (reader) {
            m_ui->tracenoSpin->setMinimum(0);
            m_ui->tracenoSpin->setValue(0);
            m_ui->tracenoSpin->setMaximum(reader->count_traces());

            m_ui->crosslineSpin->setMinimum(reader->min_crossline());
            m_ui->crosslineSpin->setValue(reader->min_crossline());
            m_ui->crosslineSpin->setMaximum(reader->max_crossline());
        }
        context->unlock_reader();
    }
}

void main_window::set_traceno(int traceno) {
    m_current_trace = traceno;
    m_data.set_traceid(traceno);
    refresh(false);
}

void main_window::set_crossline(int crossline) {
    m_current_layer = crossline;
    m_data.set_crossline(crossline);
    refresh(false);
}

void main_window::on_open_file() {
    std::string res = "Spectral Toolbox [" + m_filename + "]";
    this->setWindowTitle(QString::fromStdString(res));
}

main_window::~main_window() {
    delete m_ui;
}
