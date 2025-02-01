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
    static void *routine(void *arg);

    void render_image();
    void update_image();

    std::atomic<bool> m_need_update;

public:
    tracedata(QWidget *parent = nullptr);

    bool need_update();

    void set_need_update(bool upd);

    ~tracedata() override;

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

public slots:
    void set_crossline(int crossline);
    void set_traceid(int traceno);
};

#endif
