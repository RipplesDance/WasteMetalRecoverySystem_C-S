#ifndef INTERACTABLEFRAME_H
#define INTERACTABLEFRAME_H

#include <QWidget>
#include<QFrame>
#include <QVariantAnimation>
#include<QPainter>
#include<QDebug>
#include<QLabel>
#include<QMouseEvent>
#include <QGraphicsDropShadowEffect>

class interactableFrame : public QFrame
{
    Q_OBJECT
    Q_PROPERTY(QColor bgColor READ bgColor WRITE setBgColor)
    QColor bgColor() const { return m_bgColor; }
        void setBgColor(const QColor &color) {
            m_bgColor = color;
            update();
        }
public:
    explicit interactableFrame(QWidget *parent = nullptr);
    ~interactableFrame();
    void transition(const QColor &targetColor, int duration);

private:
    // override event function
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;



signals:
    void clicked();

private:
    QVariantAnimation *animation;
    QColor m_bgColor;
    bool isPressed;
    QGraphicsDropShadowEffect *shadowEffect;
};

#endif // INTERACTABLEFRAME_H
