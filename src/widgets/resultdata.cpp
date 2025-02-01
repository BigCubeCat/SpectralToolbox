#include "../../include/widgets/resultdata.hpp"

#include <QImage>
#include <QPainter>
#include <algorithm>

#include <qimage.h>
#include <qminmax.h>
#include <unistd.h>

#include <spdlog/spdlog.h>

#include "datamodel.hpp"

const int CELL_SIZE = 1;

resultdata::resultdata(QWidget *parent)
    : QWidget(parent), m_no_data_label(new QLabel(this)) {
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_image = QImage(0, 0, QImage::Format_RGB32);
    m_no_data_label->setText("Результатов пока нет");
}

void resultdata::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.drawImage(rect(), m_image, m_image.rect());
}

void resultdata::resizeEvent(QResizeEvent *event) {
    Q_UNUSED(event);
}

void resultdata::update_image() {
    if (!datamodel::instance()->calculation_is_done.load()) {
        return;
    }

    m_no_data_label->setText("");
    auto *data         = datamodel::instance();
    auto *red_reader   = data->red_reader();
    auto *green_reader = data->green_reader();
    auto *blue_reader  = data->blue_reader();
    if (!red_reader || !green_reader || !blue_reader) {
        return;
    }

    const int rows = red_reader->max_inline() - red_reader->min_inline();
    if (rows == 0) {
        data->unlock_reader();
        return;
    }

    if (m_crossline == -1) {
        m_crossline = red_reader->min_crossline();
    }

    auto red_traces   = red_reader->get_crossline_layer(m_crossline);
    auto green_traces = green_reader->get_crossline_layer(m_crossline);
    auto blue_traces  = blue_reader->get_crossline_layer(m_crossline);

    auto size = red_traces.size();
    if (size == 0) {
        data->unlock_reader();
        return;
    }

    const int cols = static_cast<int>(red_reader->trace(red_traces[0]).size());

    m_max_value = 0;
    m_min_value = INT_MAX;

    m_image_data.resize(size);
    for (int i = 0; i < size; ++i) {
        auto red_tr   = red_reader->trace(i);
        auto green_tr = green_reader->trace(i);
        auto blue_tr  = blue_reader->trace(i);

        auto row = std::vector<rgb_t>(static_cast<long>(cols));
#pragma omp parallel for
        for (int j = 0; j < cols - 1; ++j) {
            auto x = (red_reader->trace_inline(i) - red_reader->min_inline());
            row[j] = { .r = red_tr[j], .g = green_tr[j], .b = blue_tr[j] };
            m_max_value =
                std::max({ m_max_value, red_tr[j], green_tr[j], blue_tr[j] });
            m_min_value =
                std::min({ m_min_value, red_tr[j], green_tr[j], blue_tr[j] });
        }
        m_image_data[i] = row;
    }
    data->unlock_reader();
    render_image();
    datamodel::instance()->calculation_is_done.store(true);
}

void resultdata::render_image() {
    if (!datamodel::instance()->calculation_is_done.load()) {
        return;
    }
    auto rows = static_cast<int>(m_image_data.size());
    if (rows == 0) {
        return;
    }
    auto cols = static_cast<int>(m_image_data[0].size());

    m_image = QImage(rows, cols, QImage::Format_RGB32);
    m_image.fill(QColor(0, 0, 0));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            m_image.setPixelColor(i, j, pixel(m_image_data[i][j]));
        }
    }
    update();
}

QColor resultdata::pixel(rgb_t value) {
    return { normalize_pixel(value.r),
             normalize_pixel(value.g),
             normalize_pixel(value.b) };
};

void resultdata::set_crossline(int crossline) {
    m_crossline = crossline;
    update_image();
}


int resultdata::normalize_pixel(float value) {
    return std::min(
        std::max(
            static_cast<int>(
                (255 * (value - m_min_value)) / (m_max_value - m_min_value)
            ),
            0
        ),
        255
    );
}

resultdata::~resultdata() { }
