#include "../../include/widgets/mainwindow.hpp"

#include <QFileDialog>

#include <spdlog/spdlog.h>

#include "../../include/routine.hpp"
#include "datamodel.hpp"
#include "ui_mainwindow.h"


main_window::main_window(QWidget *parent)
    : QMainWindow(parent), m_ui(new Ui::MainWindow), m_arg(new routine_arg) {
    m_ui->setupUi(this);
    this->setWindowTitle("Spectral Toolbox");

    m_ui->prevLayout->addWidget(&m_choose);

    m_arg->running = true;
    m_arg->td      = &m_data;
    m_arg->rd      = &m_result;
    m_arg->tc      = &m_choose;

    m_ui->renderLayout->addWidget(&m_data);
    m_ui->renderLayout1->addWidget(&m_result);

    m_ui->tab->setLayout(m_ui->tab1Layout);
    m_ui->tab_2->setLayout(m_ui->mpLayout);

    m_ui->tabInput->setLayout(m_ui->renderLayout);
    m_ui->tabOutput->setLayout(m_ui->renderLayout1);

    connect(
        m_ui->actionOpen_SEG_Y_file,
        &QAction::triggered,
        this,
        &main_window::open_segy_file
    );

    connect(
        m_ui->actionOpen_Result,
        &QAction::triggered,
        this,
        &main_window::open_result_file
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

    connect(
        m_ui->redSpinBox, &QSpinBox::valueChanged, this, &main_window::set_red
    );
    connect(
        m_ui->greenSpinBox,
        &QSpinBox::valueChanged,
        this,
        &main_window::set_green
    );
    connect(
        m_ui->blueSpinBox, &QSpinBox::valueChanged, this, &main_window::set_blue
    );

    connect(&m_choose, &trace_choose::red_changed, this, &main_window::set_red);
    connect(
        &m_choose, &trace_choose::green_changed, this, &main_window::set_green
    );
    connect(
        &m_choose, &trace_choose::blue_changed, this, &main_window::set_blue
    );

    connect(
        m_ui->resetButton,
        &QPushButton::clicked,
        &this->m_choose,
        &trace_choose::reset
    );

    connect(
        m_ui->stepTimeS,
        &QDoubleSpinBox::valueChanged,
        &m_choose,
        &trace_choose::set_step_time
    );
    connect(
        m_ui->maxAmplitude,
        &QDoubleSpinBox::valueChanged,
        &m_choose,
        &trace_choose::set_amp
    );

    m_thread = std::thread(routine, m_arg);
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
    m_result.set_need_update(true);
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
    m_choose.set_traceno(traceno);
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

void main_window::open_result_file() {
    auto red_filename = QFileDialog::getOpenFileName(
        this, "Выберите красный файл", "", "SEG-Y (*.sgy *.segy)"
    );
    auto green_filename = QFileDialog::getOpenFileName(
        this, "Выберите зеленый файл", "", "SEG-Y (*.sgy *.segy)"
    );
    auto blue_filename = QFileDialog::getOpenFileName(
        this, "Выберите синий файл", "", "SEG-Y (*.sgy *.segy)"
    );

    if (!red_filename.isEmpty() && !green_filename.isEmpty()
        && !blue_filename.isEmpty()) {
        datamodel::instance()->open_result(
            red_filename.toStdString(),
            green_filename.toStdString(),
            blue_filename.toStdString()
        );
        on_open_file();
    }
    else {
        spdlog::error("filename is isEmpty");
    }
}

void main_window::run_emd() {
    datamodel::instance()->start_calculation();
}

void main_window::set_red(int red) {
    m_choose.set_red(red);
    if (m_ui->redSpinBox->value() != red) {
        m_ui->redSpinBox->setValue(red);
    }
}

void main_window::set_green(int green) {
    m_choose.set_green(green);
    if (m_ui->greenSpinBox->value() != green) {
        m_ui->greenSpinBox->setValue(green);
    }
}

void main_window::set_blue(int blue) {
    m_choose.set_blue(blue);
    if (m_ui->blueSpinBox->value() != blue) {
        m_ui->blueSpinBox->setValue(blue);
    }
}

main_window::~main_window() {
    m_arg->running = false;
    sleep(1);
    m_thread.join();
    delete m_ui;
    delete m_arg;
}
