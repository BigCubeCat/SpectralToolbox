#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "resultdata.hpp"
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
    resultdata m_result;

    std::string m_dir_path;

    void on_open_file();

public slots:
    void open_segy_file();

    void run_emd();


    void refresh(bool update_settings);

    void set_traceno(int traceno);

    void set_crossline(int crossline);

    void dir_name();

    void done();
};
#endif    // MAINWINDOW_H
