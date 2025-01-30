#ifndef FILE_WIDGET_HPP
#define FILE_WIDGET_HPP


#include <QHBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QWidget>

#include "tracedata.hpp"


/*!
 * \brief виджет для работы с файлом данных
 */
class file_widget : public QWidget {
private:
    int m_current_trace = 0;
    int m_current_layer = 0;

    QVBoxLayout m_layout;

    QHBoxLayout m_footer_widget;
    QVBoxLayout m_trace_choose_widget;
    QVBoxLayout m_slider_layout;
    QLabel m_title;
    QLabel m_title1;
    QSpinBox m_spin_box;
    QSpinBox m_spin_layer;

    QWidget m_w;
    QWidget m_wdt;
    QWidget m_wgt;
    QWidget m_spacer;

    tracedata m_data;

    void setup_ui();

public:
    file_widget(QWidget *parent = nullptr);

private slots:
    void set_traceno(int traceno);

public slots:
    void refresh();
};

#endif
