#include "../../include/widgets/tracedata.hpp"

#include <QImage>
#include <QPainter>
#include <algorithm>

#include <qimage.h>
#include <qminmax.h>
#include <unistd.h>

#include <spdlog/spdlog.h>

#include "datamodel.hpp"

const int CELL_SIZE = 1;

tracedata::tracedata(QWidget *parent)
    : QWidget(parent), m_no_data_label(new QLabel(this)) {
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_image = QImage(0, 0, QImage::Format_RGB32);
    m_no_data_label->setText("Загрузите файл с данными");
    this->resize(200, 200);

    m_render_thread = std::thread(tracedata::routine, this);
}

void tracedata::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.drawImage(rect(), m_image, m_image.rect());
}

void tracedata::resizeEvent(QResizeEvent *event) {
    Q_UNUSED(event);
}

void tracedata::update_image() {
    this->need_update.store(false);

    auto *data   = datamodel::instance();
    auto *reader = data->reader();
    if (!reader) {
        data->unlock_reader();
        return;
    }

    const int rows = reader->max_inline() - reader->min_inline();
    if (rows == 0) {
        data->unlock_reader();
        return;
    }

    if (m_crossline == -1) {
        m_crossline = reader->min_crossline() + 100;
    }

    auto traces = reader->get_crossline_layer(m_crossline);
    auto size   = traces.size();
    if (size == 0) {
        data->unlock_reader();
        return;
    }

    const int cols = static_cast<int>(reader->trace(traces[0]).size());

    m_max_value = 0;
    m_image_data.resize(size);
    for (int i = 0; i < size; ++i) {
        int traceno = traces[i];
        auto tr     = reader->trace(traceno);
        auto row =
            std::vector<std::pair<float, int32_t>>(static_cast<long>(cols));
#pragma omp parallel for
        for (int j = 0; j < cols - 1; ++j) {
            float normalized = tr[j];
            auto x = (reader->trace_inline(traceno) - reader->min_inline());
            row[j] = { tr[j], i };
            m_max_value = std::max(m_max_value, tr[j]);
        }
        m_image_data[i] = row;
    }
    data->unlock_reader();
    render_image();
}

void tracedata::render_image() {
    auto rows = static_cast<int>(m_image_data.size());
    if (rows == 0) {
        return;
    }
    auto cols = static_cast<int>(m_image_data[0].size());

    m_image = QImage(rows, cols, QImage::Format_RGB32);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            auto p = pixel(m_image_data[i][j].first);
            if (m_image_data[i][j].second == m_traceno) {
                p.setRed(0);
                p.setGreen(200);
                p.setBlue(0);
            }
            m_image.setPixelColor(i, j, p);
        }
    }
    // auto selected_color = QColor(255, 128, 0);
    // for (const auto &p : current_trace_point) {
    //     m_image.setPixelColor(p.first, p.second, selected_color);
    // }
}

void *tracedata::routine(void *arg) {
    auto *self = static_cast<tracedata *>(arg);
    while (true) {
        if (self->need_update.load()) {
            self->update_image();
        }
    }
}

QColor tracedata::pixel(float value) const {
    auto v = static_cast<int>(qBound(0.0f, value / m_max_value, 1.0f) * 255);
    return { v, v, v };
}

void tracedata::set_crossline(int crossline) {
    m_crossline = crossline;
    this->need_update.store(true);
}

void tracedata::set_traceid(int traceno) {
    m_traceno = traceno;
    this->need_update.store(true);
}

tracedata::~tracedata() {
    m_render_thread.join();
}
