#ifndef PUZZLEPIECE_H
#define PUZZLEPIECE_H

#include <QGraphicsPathItem>
#include <QMatrix>
#include <QBrush>
#include <QPixmap>
#include <QRectF>
#include <QPoint>
#include <QPainterPath>
#include <QPointF>
#include <QPainter>
#include <QColor>
#include <QPen>
#include <QLabel>
#include <QVector>
#include <QGraphicsScene>
#include <QTransform>
#include <QSet>
#include <QPair>

class GameManager;
class PuzzlePiece : public QGraphicsPathItem
{
public:
    enum ConnectorPosition { None, Out, In };
    enum Direction { North, East, South, West };
public:
    PuzzlePiece();
    PuzzlePiece(ConnectorPosition _north, ConnectorPosition _east,
                ConnectorPosition _south, ConnectorPosition _west);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    QRectF boundingRect() const override;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value) override;


    QPixmap pixmapValue() const;
    void setPixmap(const QPixmap& _pixmap);
    QPoint coordinatesValue() const;
    void setCoordinates(const QPoint& coords);
    void link(PuzzlePiece* otherPiece, Direction otherPieceDir);

    QMatrix translateMatrix(qreal x, qreal y);

    qreal r1;
    qreal r2;
    qreal elementLength;

    ConnectorPosition north;
    ConnectorPosition east;
    ConnectorPosition south;
    ConnectorPosition west;
private:
    qreal pieceSize;
    QPixmap pixmap;
    QPoint coordinates;
    QVector<PuzzlePiece*> m_neighbours;

    void constructShape();
    void findNeighbour(Direction dir);
    void checkNeighbours(QSet<QPair<int,int>> &checked);
};

#endif // PUZZLEPIECE_H
