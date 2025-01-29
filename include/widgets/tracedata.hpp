#ifndef TRACEDATA_HPP
#define TRACEDATA_HPP


#include <QImage>
#include <QLabel>
#include <QWidget>

class tracedata : public QWidget {
private:
    QLabel *m_no_data_label;
    QImage m_image;


public:
    tracedata(QWidget *parent = nullptr);

    void update_image();

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
};

#endif
