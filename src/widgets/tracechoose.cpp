#include "../../include/widgets/tracechoose.hpp"

#include <QImage>
#include <QMouseEvent>
#include <QPainter>

#include <qminmax.h>

#include <spdlog/spdlog.h>

#include "datamodel.hpp"


using Qt::KeepEmptyParts;


const int WIDTH = 101;

trace_choose ::trace_choose() {
    setFixedWidth(110);
    setSizePolicy(
        QSizePolicy::Fixed, QSizePolicy::Expanding
    );    // Высота - максимально возможная
    m_image = QImage(WIDTH, this->height(), QImage::Format_RGB32);
    m_image.fill(QColor(128, 128, 255));
}

void trace_choose::mousePressEvent(QMouseEvent *event) {
    const QPoint p = event->pos();
    spdlog::info("clicked at {} {}", p.x(), p.y());

    if (m_counter == 0) {
        m_red = p.x();
        emit red_changed();
    }
    else if (m_counter == 1) {
        m_green = p.x();
        emit green_changed();
    }
    else {
        m_blue = p.x();
        emit blue_changed();
    }
    m_counter = (m_counter + 1) % 3;
}

void trace_choose::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.drawImage(rect(), m_image, m_image.rect());
}

void trace_choose::resizeEvent(QResizeEvent *event) {
    Q_UNUSED(event);
    this->resize(WIDTH, event->size().height());
    update();
}

void trace_choose::reset() {
    m_red   = 0;
    m_green = 0;
    m_blue  = 0;
    update();
}


void trace_choose::set_red(int value) {
    m_red = value;
    update();
}

void trace_choose::set_green(int value) {
    m_green = value;
    update();
}

void trace_choose::set_blue(int value) {
    m_blue = value;
    update();
}

void trace_choose::set_traceno(int traceno) {
    m_traceno   = traceno;
    auto *model = datamodel::instance();
    model->set_current_traceno(traceno);
    refresh();
}

void trace_choose::refresh() {
    auto *model = datamodel::instance();
    auto trace  = model->current_trace();
    // получили трассу, надо обработать
    spdlog::info("here");
    auto traces = m_mp.decompose_signal(
        trace, static_cast<int>(trace.size()), m_step_time_s, m_max_amplitude
    );
    spdlog::info("here");

    m_image =
        QImage(WIDTH, static_cast<int>(traces[0].size()), QImage::Format_RGB32);
    m_image.fill(QColor(255, 255, 255));
    for (int i = 0; i < traces.size(); ++i) {
        for (int j = 0; j < traces[i].size(); ++j) {
            float normalized = qBound(0.0f, traces[i][j], 1.0f);
            int value        = 255 * static_cast<int>(normalized);
            m_image.setPixelColor(i, j, QColor(value, value, value));
        }
    }
    update();
}

void trace_choose::set_step_time(float step) {
    m_step_time_s = step;
}

void trace_choose::set_amp(float amp) {
    m_max_amplitude = amp;
}
