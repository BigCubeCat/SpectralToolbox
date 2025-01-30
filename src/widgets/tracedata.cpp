#include "../../include/widgets/tracedata.hpp"

#include <QImage>
#include <QPainter>

#include <qimage.h>

#include <spdlog/spdlog.h>

#include "datamodel.hpp"

const int COL_SIZE = 50;

tracedata::tracedata(QWidget *parent)
    : QWidget(parent), m_no_data_label(new QLabel(this)) {
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_image = QImage(0, 0, QImage::Format_RGB32);
    m_no_data_label->setText("Загрузите файл с данными");
    this->resize(200, 200);
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

    m_image = QImage((rows + 1) * COL_SIZE, cols, QImage::Format_RGB32);

    for (int i = 0; i < size; ++i) {
        int traceno = traces[i];
        auto tr     = reader->trace(traceno);
        // Нормализация значения
        for (int j = 0; j < cols; ++j) {
            float normalized = tr[j];
            normalized       = qBound(0.0f, normalized, 1.0f);
            int intensity    = static_cast<int>(normalized * 255);
            auto x = (reader->trace_inline(traceno) - reader->min_inline());
            x *= COL_SIZE;
            auto x_end = x + COL_SIZE;
            for (; x < x_end; ++x) {
                m_image.setPixelColor(
                    x, j, QColor(intensity, intensity, intensity)
                );
            }
        }
    }

    data->unlock_reader();
}

void tracedata::set_crossline(int crossline) {
    update_image();
}
