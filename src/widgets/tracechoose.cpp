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
    m_image.fill(QColor(255, 255, 255));
    m_data.resize(100);
    for (int i = 0; i < 100; ++i) {
        m_data[i].resize(100);
        for (int j = 0; j < 100; ++j) {
            m_data[i][j] = 0;
        }
    }
}

void trace_choose::mousePressEvent(QMouseEvent *event) {
    const QPoint p = event->pos();
    spdlog::info("clicked at {} {}", p.x(), p.y());

    if (m_counter == 0) {
        m_red = p.x();
        emit red_changed(m_red);
    }
    else if (m_counter == 1) {
        m_green = p.x();
        emit green_changed(m_green);
    }
    else {
        m_blue = p.x();
        emit blue_changed(m_blue);
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
    emit red_changed(0);
    emit green_changed(0);
    emit blue_changed(0);
    render();
    update();
}


void trace_choose::set_red(int value) {
    m_red = value;
    render();
    update();
}

void trace_choose::set_green(int value) {
    m_green = value;
    render();
    update();
}

void trace_choose::set_blue(int value) {
    m_blue = value;
    render();
    update();
}

void trace_choose::set_traceno(int traceno) {
    m_traceno = traceno;
    m_need_update.store(true);
}

void trace_choose::refresh() {
    return;
    auto *model = datamodel::instance();
    model->set_current_traceno(m_traceno);
    auto trace = model->current_trace();
    // получили трассу, надо обработать
    spdlog::info("here");
    m_data_mutex.lock();
    auto d = m_mp.decompose_signal(
        trace, static_cast<int>(trace.size()), m_step_time_s, m_max_amplitude
    );
    m_data.resize(d.size());
    for (int i = 0; i < d.size(); ++i) {
        m_data[i].resize(d[i].size());
        for (int j = 0; j < d[i].size(); ++j) {
            m_data[i][j] = d[i][j];
        }
    }
    spdlog::info("here");
    m_data_mutex.unlock();
    render();
    m_need_update.store(false);
    update();
}

void trace_choose::set_step_time(float step) {
    m_step_time_s = step;
}

void trace_choose::set_amp(float amp) {
    m_max_amplitude = amp;
}

void trace_choose::render() {
    m_data_mutex.lock();
    if (m_data.size() == 0) {
        m_data_mutex.unlock();
        return;
    }
    m_image =
        QImage(WIDTH, static_cast<int>(m_data[0].size()), QImage::Format_RGB32);
    m_image.fill(QColor(255, 255, 255));
    for (int i = 0; i < 100; ++i) {
        for (int j = 0; j < m_data[0].size(); ++j) {
            QColor color;
            if (i == m_red || i == m_green || i == m_blue) {
                color = QColor(
                    255 * static_cast<int>(i == m_red),
                    255 * static_cast<int>(i == m_green),
                    255 * static_cast<int>(i == m_blue)
                );
            }
            else {
                auto point = m_data[i][j];
                if (point > 0) {
                    spdlog::info("point={}", point);
                }
                float normalized = 0.0f;
                if (point < 0.5f) {
                    color = QColor(
                        0,
                        0,
                        static_cast<int>(255 * qBound(0.0f, point * 10, 1.0f))
                    );
                }
                else {
                    auto v = static_cast<int>(255 * qBound(0.0f, point, 1.0f));
                    color  = QColor(v, v, 0);
                }
            }
            m_image.setPixelColor(i, j, color);
        }
    }
    m_data_mutex.unlock();
}
