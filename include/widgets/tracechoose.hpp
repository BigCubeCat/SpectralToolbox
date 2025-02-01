#pragma once

#include <QWidget>

#include <qwidget.h>

#include "matching_pursuit.h"


/*!
 * виджет для выбора частоты
 */
class trace_choose : public QWidget {
    Q_OBJECT
public:
    trace_choose();

private:
    QImage m_image;
    int m_red   = 0;
    int m_green = 0;
    int m_blue  = 0;

    int m_counter = 0;

    int m_traceno = 0;

    float m_step_time_s;
    float m_max_amplitude;

    matching_pursuit<float> m_mp;

    void mousePressEvent(QMouseEvent *event) override;

    void paintEvent(QPaintEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;

    void refresh();

public slots:
    void reset();

    void set_red(int value);
    void set_green(int value);
    void set_blue(int value);

    void set_traceno(int traceno);

    void set_step_time(float step);
    void set_amp(float amp);

signals:
    void red_changed();
    void green_changed();
    void blue_changed();
};
