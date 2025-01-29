#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "file_widget.hpp"
#include "procedure_config_widget.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class main_window : public QMainWindow {
    Q_OBJECT

public:
    main_window(QWidget *parent = nullptr);
    main_window(const main_window &)            = delete;
    main_window(main_window &&)                 = delete;
    main_window &operator=(const main_window &) = delete;
    main_window &operator=(main_window &&)      = delete;
    main_window(
        Ui::MainWindow *m_ui,
        std::shared_ptr<procedure_config_widget> m_param_widget
    )
        : m_ui(m_ui), m_param_widget(std::move(m_param_widget)) { }
    ~main_window() override;

private:
    Ui::MainWindow *m_ui;
    std::shared_ptr<procedure_config_widget> m_param_widget;
    std::shared_ptr<file_widget> m_file_widget;

public slots:
    void open_segy_file();
};
#endif    // MAINWINDOW_H
