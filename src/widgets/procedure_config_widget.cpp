#include "../../include/widgets/procedure_config_widget.hpp"

#include <QWidget>

#include "datamodel.hpp"


procedure_config_widget::procedure_config_widget(QWidget *parent)
    : QWidget(parent),
      m_file_exists(false),
      m_layout(std::make_shared<QHBoxLayout>()),
      m_label(std::make_shared<QLabel>()) {
    m_label->setText("Откройте файл");
    this->setLayout(m_layout.get());
    m_layout->addWidget(m_label.get());
}

void procedure_config_widget::refresh() {
    auto *inst = datamodel::instance();
}
