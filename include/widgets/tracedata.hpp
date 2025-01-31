#ifndef TRACEDATA_HPP
#define TRACEDATA_HPP

#include <QImage>
#include <QLabel>
#include <QWidget>
#include <thread>

class tracedata : public QWidget {
private:
    QLabel *m_no_data_label;
    QImage m_image;
    int m_traceno       = 0;
    int32_t m_crossline = -1;
    float m_max_value   = 0;

    std::vector<std::vector<std::pair<float, int32_t>>> m_image_data;
    std::thread m_render_thread;


    [[nodiscard]] QColor pixel(float value) const;
    void render_image();
    static void *routine(void *arg);

public:
    tracedata(QWidget *parent = nullptr);

    void update_image();

    std::atomic<bool> need_update;

    ~tracedata() override;

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

public slots:
    void set_crossline(int crossline);
    void set_traceid(int traceno);
};

#endif
