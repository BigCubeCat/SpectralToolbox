#include "../../include/widgets/file_widget.hpp"

#include "datamodel.hpp"


file_widget::file_widget(QWidget *parent)
    : QWidget(parent),
      m_layout(QVBoxLayout(this)),
      m_trace_choose_widget(QVBoxLayout(this)),
      m_slider_layout(QVBoxLayout(this)),
      m_footer_widget(QHBoxLayout(this)),
      m_title(QLabel(this)),
      m_title1(QLabel(this)),
      m_spin_box(QSpinBox(this)),
      m_spin_layer(QSpinBox(this)),
      m_spacer(QWidget(this)),
      m_w(QWidget(this)),
      m_wdt(QWidget(this)),
      m_wgt(QWidget(this)),
      m_data(tracedata(this)) {

    this->setLayout(&m_layout);
    m_layout.addWidget(&m_data);
    m_layout.addWidget(&m_wgt);

    setup_ui();

    connect(
        &m_spin_layer,
        QOverload<int>::of(&QSpinBox::valueChanged),
        &m_data,
        &tracedata::set_crossline
    );
}

void file_widget::setup_ui() {
    m_w.setLayout(&m_slider_layout);
    m_slider_layout.addWidget(&m_title1);
    m_slider_layout.addWidget(&m_spin_layer);
    m_wdt.setLayout(&m_trace_choose_widget);
    m_wgt.setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    m_spacer.setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);
    m_title.setText("Номер трассы");
    m_title1.setText("crossline");

    m_trace_choose_widget.addWidget(&m_title);
    m_trace_choose_widget.addWidget(&m_spin_box);

    m_footer_widget.addWidget(&m_spacer);
    m_footer_widget.addWidget(&m_w);
    m_footer_widget.addWidget(&m_wdt);
    m_wgt.setLayout(&m_footer_widget);
}

void file_widget::set_traceno(int traceno) { }

void file_widget::refresh() {
    m_data.update_image();
    m_spin_layer.setValue(0);
    m_spin_layer.setMinimum(0);
    auto *context = datamodel::instance();
    if (context) {
        auto *reader = context->reader();
        if (reader) {
            m_spin_layer.setMaximum(reader->min_crossline());
            m_spin_layer.setMaximum(reader->max_crossline());
        }
        context->unlock_reader();
    }
}
