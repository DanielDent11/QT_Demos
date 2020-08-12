#include "puzzlesizewidget.h"

#include <QLabel>
#include <QApplication>
#include <QPainter>
#include <QDebug>
#include <QColor>
#include <QBrush>
#include <QBitmap>
#include <QPoint>
#include <QMouseEvent>
#include <QPaintEvent>

PuzzleSizeWidget::PuzzleSizeWidget(QWidget *parent) :
    size (QSize(5, 5)),
    minimum(QSize(2, 2)),
    maximum(QSize(8, 8)),
    pixmap(nullptr)
{

}

QSize PuzzleSizeWidget::sizeHint() const
{
    return QSize(700, 700);
}

QSize PuzzleSizeWidget::value() const
{
    return size;
}

QSize PuzzleSizeWidget::minimumValue() const
{
    return minimum;
}

QSize PuzzleSizeWidget::maximumValue() const
{
    return maximum;
}

QPixmap PuzzleSizeWidget::pixmapValue() const
{
    return *pixmap;
}

void PuzzleSizeWidget::setValue(const QSize &s)
{
    if (size == s)
        return;

    if (s.width() < minimumValue().width() ||
        s.width() > maximumValue().width())
        return;

    if (s.height() < minimumValue().height() ||
        s.height() > maximumValue().height())
        return;

    QSize old = size;
    size = s;
    emit valueChanged(s);
    if (old.width() != s.width())
        emit horizontalValueChanged(s.width());
    if (old.height() != s.height())
        emit verticalValueChanged(s.height());
    update();
}

void PuzzleSizeWidget::setMinimumValue(const QSize &s)
{
    if (minimum == s)
        return;

    if (size.width() < s.width() ||
        size.height() < s.height())
        return;

    QSize old = minimum;
    minimum = s;
    emit minimumValueChanged(s);
    if (old.width() != s.width())
        emit horizontalValueChanged(s.width());
    if (old.height() != s.height())
        emit verticalValueChanged(s.height());
}

void PuzzleSizeWidget::setMaximumValue(const QSize &s)
{
    if (maximum == s)
        return;

    if (size.width() > maximum.width() ||
        size.height() > maximum.height())
        return;

    QSize old = maximum;
    maximum = s;
    emit maximumValueChanged(s);
    if (old.width() != s.width())
        emit horizontalMaximumValueChanged(s.width());
    if (old.height() != s.height())
        emit verticalMaximumValueChanged(s.height());
}

void PuzzleSizeWidget::setPixmapValue(QPixmap* p)
{
    if (pixmap == p)
        return;
    pixmap = p;
    emit pixmapValueChanged(*p);
}

void PuzzleSizeWidget::renderGrid(QPainter *painter)
{
    if (painter == nullptr)
        return;

    painter->setBrush(Qt::NoBrush);
    for (int y = 0; y < maximum.height(); ++y)
    {
        for (int x = 0; x < maximum.width(); ++x)
        {

            painter->drawRect(x * cellSize().width(), y * cellSize().width(),
                              cellSize().width(), cellSize().height());
        }
    }
}

void PuzzleSizeWidget::renderValue(QPainter *painter)
{
    if (painter == nullptr)
        return;

    if (pixmap == nullptr)
    {
        painter->setBrush(QBrush(QColor(0, 100, 0)));
        for (int y = 0; y < value().height(); ++y)
        {
            for (int x = 0; x < value().width(); ++x)
            {
                painter->drawRect(x * cellSize().width(), y * cellSize().width(),
                                  cellSize().width(), cellSize().height());
            }
        }
    }
    else {
        painter->drawPixmap(0, 0, value().width() * cellSize().width(), value().height() * cellSize().height(),
                            pixmap->scaled(QSize(value().width() * cellSize().width(),
                                                 value().height() * cellSize().height())));
    }
}

void PuzzleSizeWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    renderValue(&painter);
    renderGrid(&painter);
}

void PuzzleSizeWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton)
    {
        QPoint curPos = cellAt(event->pos());
        if (curPos.x() >= minimum.width()-1 && curPos.x() <= maximum.width() &&
            curPos.y() >= minimum.height()-1 && curPos.y() <= maximum.height())
        {
            setValue(QSize(curPos.x() + 1, curPos.y() + 1));
        }
    }
}

void PuzzleSizeWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        QPoint curPos = cellAt(event->pos());
        if (curPos.x() >= minimum.width()-1 && curPos.x() <= maximum.width() &&
            curPos.y() >= minimum.height()-1 && curPos.y() <= maximum.height())
        {
            setValue(QSize(curPos.x() + 1, curPos.y() + 1));
        }
    }
}


QSize PuzzleSizeWidget::cellSize() const
{
    int w = width();
    int h = height();

    int mw = maximum.width();
    int mh = maximum.height();

    int extent = qMin(w / mw, h / mh);
    return QSize(extent, extent).
            expandedTo(QApplication::globalStrut()).expandedTo(QSize(4, 4));
}

QPoint PuzzleSizeWidget::cellAt(const QPoint& point) const
{
    return QPoint(point.x() / cellSize().width(), point.y() / cellSize().height());
}
