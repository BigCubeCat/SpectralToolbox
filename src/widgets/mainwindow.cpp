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
    m_ui->renderLayout1->addWidget(&m_result);

    m_ui->tab->setLayout(m_ui->tab1Layout);

    m_ui->tabInput->setLayout(m_ui->renderLayout);
    m_ui->tabOutput->setLayout(m_ui->renderLayout1);

    connect(
        m_ui->actionOpen_SEG_Y_file,
        &QAction::triggered,
        this,
        &main_window::open_segy_file
    );

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

    connect(m_ui->dirBtn, &QPushButton::clicked, this, &main_window::dir_name);

    connect(m_ui->runEMD, &QPushButton::clicked, this, &main_window::run_emd);
}

void main_window::open_segy_file() {
    QString file_name = QFileDialog::getOpenFileName(
        this, "Выберите файл", "", "SEG-Y (*.sgy *.segy)"
    );

    if (!file_name.isEmpty()) {
        m_filename = file_name.toStdString();
        datamodel::instance()->open_file(m_filename);
        on_open_file();
    }
    else {
        spdlog::error("filename is isEmpty");
    }
    refresh(true);
}

void main_window::refresh(bool update_settings) {
    m_data.set_need_update(true);
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
    m_result.set_crossline(crossline);
    refresh(false);
}

void main_window::on_open_file() {
    std::string res = "Spectral Toolbox [" + m_filename + "]";
    this->setWindowTitle(QString::fromStdString(res));
}

void main_window::dir_name() {
    QString file_name = QFileDialog::getExistingDirectory(
        this, "Выберите директорию для сохранения результатов"
    );
    if (!file_name.isEmpty()) {
        // datamodel::instance()->set_rname(fileno.toStdString());
        auto vector       = file_name.split(QString("/"));
        const auto &label = vector.at(vector.size() - 1);
        m_ui->dirBtn->setText(label);

        datamodel::instance()->set_string("out", file_name.toStdString());
    }
}

void main_window::run_emd() {
    datamodel::instance()->start_calculation();
}

void main_window::done() {
    m_result.set_need_update(true);
}

main_window::~main_window() {
    delete m_ui;
}
