#ifndef PUZZLESIZEWIDGET_H
#define PUZZLESIZEWIDGET_H

#include "configurationdialog.h"

#include <QFrame>
#include <QPixmap>

class PuzzleSizeWidget : public QFrame
{
    Q_OBJECT
    Q_PROPERTY(QSize size READ value WRITE setValue)

public:
    PuzzleSizeWidget(QWidget *parent = nullptr);

    QSize sizeHint() const override;

    QSize value() const;
    QSize minimumValue() const;
    QSize maximumValue() const;
    QPixmap pixmapValue() const;

signals:
    void valueChanged(const QSize &size);
    void horizontalValueChanged(int hSize);
    void verticalValueChanged(int vSize);

    void minimumValueChanged(const QSize &size);
    void horizontalMinimumValueChanged(int hSize);
    void verticalMinimumValueChanged(int vSize);

    void maximumValueChanged(const QSize &size);
    void horizontalMaximumValueChanged(int hSize);
    void verticalMaximumValueChanged(int vSize);

    void pixmapValueChanged(const QPixmap &p);

public slots:
    void setValue(const QSize &s);
    void setMinimumValue(const QSize &s);
    void setMaximumValue(const QSize &s);
    void setPixmapValue(QPixmap* p);

private:
    QSize size;
    QSize minimum;
    QSize maximum;
    QPixmap* pixmap;

    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

    void renderGrid(QPainter* painter = nullptr);
    void renderValue(QPainter* painter = nullptr);
    QSize cellSize() const;
    QPoint cellAt(const QPoint& point) const;
};

#endif // PUZZLESIZEWIDGET_H
