#include "../../include/widgets/file_widget.hpp"


file_widget::file_widget(QWidget *parent)
    : QWidget(parent),
      m_layout(new QVBoxLayout(this)),
      m_trace_choose_widget(new QVBoxLayout(this)),
      m_footer_widget(new QHBoxLayout(this)),
      m_title(new QLabel(this)),
      m_spin_box(new QSpinBox(this)),
      m_spacer(new QWidget(this)),
      m_wdt(new QWidget(this)),
      m_wgt(new QWidget(this)),
      m_data(new tracedata(this)) {

    this->setLayout(m_layout);
    m_layout->addWidget(m_data);
    m_layout->addWidget(m_wgt);

    setup_ui();
}

void file_widget::setup_ui() {
    m_wdt->setLayout(m_trace_choose_widget);
    m_wgt->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    m_spacer->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);
    m_title->setText("Номер трассы");
    m_trace_choose_widget->addWidget(m_title);
    m_trace_choose_widget->addWidget(m_spin_box);

    m_footer_widget->addWidget(m_spacer);
    m_footer_widget->addWidget(m_wdt);
    m_wgt->setLayout(m_footer_widget);
}

void file_widget::set_traceno(int traceno) { }

void file_widget::refresh() {
    m_data->update_image();
}
