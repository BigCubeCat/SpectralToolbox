#include "../../include/widgets/tracechoose.hpp"

#include <QImage>
#include <QMouseEvent>
#include <QPainter>

#include <qminmax.h>

#include <spdlog/spdlog.h>

#include "datamodel.hpp"


const int WIDTH = 201;
const int SCALE = 2;

const std::vector<QColor> magma_palette = {
    QColor(4, 0, 30),     QColor(48, 18, 59),   QColor(84, 10, 63),
    QColor(120, 15, 66),  QColor(156, 26, 68),  QColor(189, 42, 68),
    QColor(218, 61, 66),  QColor(241, 86, 59),  QColor(252, 118, 50),
    QColor(253, 156, 39), QColor(252, 194, 30), QColor(249, 232, 24)
};

QColor interpolate(float t) {
    auto n         = static_cast<int>(magma_palette.size());
    t              = std::clamp(t, 0.0f, 1.0f);
    float scaled_t = t * static_cast<float>(n - 1);
    int idx        = static_cast<int>(scaled_t);
    float alpha    = scaled_t - static_cast<float>(idx);

    if (idx >= n - 1)
        return magma_palette.back();

    QColor c1 = magma_palette[idx];
    QColor c2 = magma_palette[idx + 1];
    int red =
        c1.red()
        + static_cast<int>(alpha * static_cast<float>(c2.red() - c1.red()));
    int green =
        c1.green()
        + static_cast<int>(alpha * static_cast<float>(c2.green() - c1.green()));
    int blue =
        c1.blue()
        + static_cast<int>(alpha * static_cast<float>(c2.blue() - c1.blue()));
    return { red, green, blue };
}

using Qt::KeepEmptyParts;

void *trace_choose::routine(void *arg) {
    auto *self = static_cast<trace_choose *>(arg);
    spdlog::info(
        "refresh started; step_time={}; amp={}",
        self->m_step_time_s,
        self->m_max_amplitude
    );
    auto *model = datamodel::instance();
    model->set_current_traceno(self->m_traceno);
    auto trace = model->current_trace();
    spdlog::info("trace={}", log);
    // получили трассу, надо обработать
    auto d = self->m_mp.decompose_signal(
        trace,
        static_cast<int>(trace.size()),
        self->m_step_time_s,
        self->m_max_amplitude
    );
    self->m_data_mutex.lock();
    self->m_data.resize(d.size());
    for (int i = 0; i < d.size(); ++i) {
        self->m_data[i].resize(d[i].size());
        for (int j = 0; j < d[i].size(); ++j) {
            self->m_data[i][j] = d[i][j];
        }
    }
    self->m_data_mutex.unlock();
    self->render();
    self->m_need_update.store(false);
    spdlog::info("preview success");
    return nullptr;
}


trace_choose ::trace_choose() {
    setFixedWidth(WIDTH);
    setSizePolicy(
        QSizePolicy::Fixed, QSizePolicy::Expanding
    );    // Высота - максимально возможная
    m_image = QImage(WIDTH, this->height(), QImage::Format_RGB32);
    m_image.fill(QColor(255, 255, 255));
    m_data.resize(WIDTH);
    for (int i = 0; i < WIDTH; ++i) {
        m_data[i].resize(WIDTH);
        for (int j = 0; j < WIDTH; ++j) {
            m_data[i][j] = 0;
        }
    }
}

void trace_choose::mousePressEvent(QMouseEvent *event) {
    const QPoint p = event->pos();
    spdlog::info("clicked at {} {}", p.x(), p.y());

    if (m_counter == 0) {
        m_red = p.x() / 2;
        emit red_changed(m_red);
    }
    else if (m_counter == 1) {
        m_green = p.x() / 2;
        emit green_changed(m_green);
    }
    else {
        m_blue = p.x() / 2;
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
    render();
}

void trace_choose::reset() {
    m_red   = 0;
    m_green = 0;
    m_blue  = 0;
    emit red_changed(0);
    emit green_changed(0);
    emit blue_changed(0);
    render();
}


void trace_choose::set_red(int value) {
    m_red = value;
    render();
}

void trace_choose::set_green(int value) {
    m_green = value;
    render();
}

void trace_choose::set_blue(int value) {
    m_blue = value;
    render();
}

void trace_choose::set_traceno(int traceno) {
    m_traceno = traceno;
    m_need_update.store(true);
}

void trace_choose::refresh() {
    m_refresh_thread = std::thread(trace_choose::routine, this);
    m_refresh_thread.detach();
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
                auto point = m_data[i][j] * 2;
                if (point > 0) {
                    spdlog::info("point={}", point);
                }
                color = interpolate(qBound(0.0f, point, 1.0f));
            }
            for (int k = 0; k < SCALE; ++k) {
                m_image.setPixelColor((i * SCALE) + k, j, color);
            }
        }
    }
    m_data_mutex.unlock();
    update();
}
