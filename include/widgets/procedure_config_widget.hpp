#ifndef PROCEDURE_CONFIG_WIDGET_HPP
#define PROCEDURE_CONFIG_WIDGET_HPP


#include <QBoxLayout>
#include <QLabel>
#include <QWidget>

/*!
 * \brief виджет, который позволяет настроить параметры процедуры
 */
class procedure_config_widget : public QWidget {
    Q_OBJECT

public:
    procedure_config_widget(QWidget *parent = nullptr);

private:
    std::shared_ptr<QLabel> m_label;
    std::shared_ptr<QHBoxLayout> m_layout;
    bool m_file_exists;

    void setup_ui();

public slots:
    void refresh();
};

#endif
