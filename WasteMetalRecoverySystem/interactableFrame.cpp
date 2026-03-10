#include "interactableFrame.h"

interactableFrame::interactableFrame(QWidget *parent) : QFrame(parent)
{
    m_bgColor = QColor(248, 252, 248);
    animation = new QVariantAnimation(this);
    isPressed = false;
    connect(animation, &QVariantAnimation::valueChanged, [=](const QVariant &value){
            setBgColor(value.value<QColor>());
        });

    this->setCursor(Qt::PointingHandCursor);
    setMouseTracking(true);

    //set shadow
    shadowEffect = new QGraphicsDropShadowEffect(this);
        shadowEffect->setBlurRadius(12);
        shadowEffect->setXOffset(0);
        shadowEffect->setYOffset(4);           // 向下偏移，模拟底部阴影
        shadowEffect->setColor(QColor(0, 100, 0, 60));  // 很淡的深绿色阴影 (alpha 低)
        this->setGraphicsEffect(shadowEffect);
}

interactableFrame::~interactableFrame()
{

}


void interactableFrame::enterEvent(QEvent *event)
{
    transition(QColor(235, 245, 235), 280);
    QFrame::enterEvent(event);
}

void interactableFrame::leaveEvent(QEvent *event)
{
    transition(QColor(248, 252, 248), 320);
    QFrame::leaveEvent(event);
}

void interactableFrame::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        isPressed = true;
        transition(QColor(200, 230, 201), 80);
    }
    QFrame::mousePressEvent(event);
}

void interactableFrame::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && isPressed) {
        isPressed = false;
        if (underMouse()) {
                    transition(QColor(235, 245, 235), 180);
                } else {
                    transition(QColor(248, 252, 248), 220);
                }
        emit clicked();
    }
}

void interactableFrame::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(m_bgColor);


    QColor borderColor;
        if (isPressed) {
            borderColor = QColor(76, 175, 80);
        } else if (underMouse()) {
            borderColor = QColor(129, 199, 132);
        } else {
            borderColor = QColor(188, 214, 196);
        }

        painter.setPen(QPen(borderColor, 1));

    QRect r = rect().adjusted(0,0,-1,-1);
    painter.drawRoundedRect(r, 10, 10);
    QFrame::paintEvent(event);
}

void interactableFrame::transition(const QColor &targetColor, int duration)
{
    animation->stop();
      animation->setDuration(duration);
    animation->setStartValue(m_bgColor);
    animation->setEndValue(targetColor);
    animation->start();
}
