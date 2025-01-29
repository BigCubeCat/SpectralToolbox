#include "../../include/widgets/procedure_config_widget.hpp"

#include <QWidget>

#include "datamodel.hpp"


procedure_config_widget::procedure_config_widget(QWidget *parent)
    : QWidget(parent), m_file_exists(false) { }


void procedure_config_widget::refresh() {
    auto inst = datamodel::instance();
}
