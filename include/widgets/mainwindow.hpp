#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "tracedata.hpp"


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class main_window : public QMainWindow {
    Q_OBJECT

public:
    main_window(QWidget *parent = nullptr);
    ~main_window() override;

private:
    Ui::MainWindow *m_ui;

    std::string m_filename;

    int m_current_trace = 0;
    int m_current_layer = 0;
    tracedata m_data;


signals:
    void data_loaded();


public slots:
    void open_segy_file();

    void on_open_file();

    void refresh(bool update_settings);

    void set_traceno(int traceno);

    void set_crossline(int crossline);
};
#endif    // MAINWINDOW_H
