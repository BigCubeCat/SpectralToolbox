#pragma once

#include <QWidget>

#include <qwidget.h>

#include "matching_pursuit.h"
#include "trace.hpp"


/*!
 * виджет для выбора частоты
 */
class trace_choose : public QWidget {
    Q_OBJECT
public:
    trace_choose();

    void refresh();

    void set_need_update(bool need) {
        m_need_update.store(need);
    }
    bool need_update() {
        return m_need_update.load();
    }

private:
    std::atomic<bool> m_need_update;
    QImage m_image;
    int m_red   = -1;
    int m_green = -1;
    int m_blue  = -1;

    int m_counter = 0;

    int m_traceno = 0;

    float m_step_time_s;
    float m_max_amplitude;

    matching_pursuit<float> m_mp;
    std::mutex m_data_mutex;
    std::vector<std::vector<float>> m_data;

    void mousePressEvent(QMouseEvent *event) override;

    void paintEvent(QPaintEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;

    void render();

public slots:
    void reset();

    void set_red(int value);
    void set_green(int value);
    void set_blue(int value);

    void set_traceno(int traceno);

    void set_step_time(float step);
    void set_amp(float amp);

signals:
    void red_changed(int);
    void green_changed(int);
    void blue_changed(int);
};
