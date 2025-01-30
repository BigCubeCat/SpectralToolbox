#include "../../include/widgets/tracedata.hpp"

#include <QImage>
#include <QPainter>

#include <qimage.h>

#include <spdlog/spdlog.h>

#include "datamodel.hpp"

tracedata::tracedata(QWidget *parent)
    : QWidget(parent), m_no_data_label(new QLabel(this)) {
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_image = QImage(0, 0, QImage::Format_RGB32);
    m_no_data_label->setText("Загрузите файл с данными");
}

void tracedata::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.drawImage(rect(), m_image, m_image.rect());
}

void tracedata::resizeEvent(QResizeEvent *event) {
    Q_UNUSED(event);
    update_image();
}

void tracedata::update_image() {
    auto *data   = datamodel::instance();
    auto *reader = data->reader();
    if (!reader) {
        data->unlock_reader();
        return;
    }

    const int cols = reader->max_crossline() - reader->min_crossline();
    const int rows = reader->max_inline() - reader->min_inline();

    spdlog::info("rows, cols = {} {} ", rows, cols);

    m_image = QImage(rows + 1, cols + 1, QImage::Format_RGB32);
    // this->resize(rows, cols);

    const int count_traces = reader->count_traces();

#pragma omp parallel for
    for (int traceno = 0; traceno < count_traces; ++traceno) {
        auto pixel = reader->trace_point(traceno, 100);
        // Нормализация значения
        float normalized = pixel;
        spdlog::info("pixel={}", pixel);
        normalized    = qBound(0.0f, normalized, 1.0f);
        int intensity = static_cast<int>(normalized * 255);
        auto x        = reader->trace_inline(traceno) - reader->min_inline();
        auto y = reader->trace_crossline(traceno) - reader->min_crossline();
        spdlog::info("x, y = {} {}", x, y);
        m_image.setPixelColor(x, y, QColor(intensity, intensity, intensity));
    }

    m_image = m_image.scaled(rows, cols);

    data->unlock_reader();
}
