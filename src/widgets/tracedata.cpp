#include "../../include/widgets/tracedata.hpp"

#include <QImage>
#include <QPainter>

#include <qimage.h>

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

    const int cols = reader->count_traces();
    if (cols == 0) {
        return;
    }
    const int rows = static_cast<int>(reader->trace(0).size());

    m_image = QImage(cols, rows, QImage::Format_RGB32);

    for (int x = 0; x < cols; ++x) {
        auto row = reader->trace(x);
#pragma omp parallel for
        for (int y = 0; y < rows; ++y) {
            // Нормализация значения
            float normalized = row[y];
            normalized       = qBound(0.0f, normalized, 1.0f);

            // Преобразование в градации серого
            int intensity = static_cast<int>(normalized * 255);
            m_image.setPixelColor(
                x, y, QColor(intensity, intensity, intensity)
            );
        }
    }

    m_image =
        m_image.scaled(size(), Qt::KeepAspectRatio, Qt::FastTransformation);
    data->unlock_reader();
}
