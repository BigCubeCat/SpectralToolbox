#ifndef RESULT_DATA_HPP
#define RESULT_DATA_HPP

#include <QImage>
#include <QLabel>
#include <QWidget>

struct rgb_t {
    float r, g, b;
};

class resultdata : public QWidget {
private:
    std::atomic<bool> m_need_update;
    QLabel *m_no_data_label;
    QImage m_image;
    int32_t m_crossline = -1;
    float m_max_red     = 0;
    float m_max_green   = 0;
    float m_max_blue    = 0;

    std::vector<std::vector<rgb_t>> m_image_data;


    QColor pixel(rgb_t color);
    static void *routine(void *arg);

    void render_image();


public:
    resultdata(QWidget *parent = nullptr);

    void update_image();

    void set_need_update(bool need) {
        m_need_update.store(need);
    }
    bool need_update() {
        return m_need_update.load();
    }

    ~resultdata() override;

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

public slots:
    void set_crossline(int crossline);
};

#endif
