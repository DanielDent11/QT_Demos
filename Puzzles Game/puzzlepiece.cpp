#include "puzzlepiece.h"
#include "gamemanager.h"


PuzzlePiece::PuzzlePiece() :
    m_neighbours(QVector<PuzzlePiece*>(4, nullptr))
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
}

PuzzlePiece::PuzzlePiece(PuzzlePiece::ConnectorPosition _north, PuzzlePiece::ConnectorPosition _east,
                         PuzzlePiece::ConnectorPosition _south, PuzzlePiece::ConnectorPosition _west) :
    north(_north),
    east(_east),
    south(_south),
    west(_west),
    pieceSize(50.0),
    m_neighbours(QVector<PuzzlePiece*>(4, nullptr))
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    constructShape();
}

void PuzzlePiece::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF target(-25 + (west == None ? 10 : 0),-25 + (north == None ? 10 : 0),50,50);
    //painter->setPen(QPen(QColor(0, 100, 0)));
    //painter->drawRect(boundingRect());
    //painter->setPen(QPen(QColor(0,0,100)));
    //painter->drawRect(target);
    painter->setClipPath(path());
    painter->drawPixmap(target,pixmap,QRectF(0, 0, 50, 50));
    painter->setPen(QPen(QColor(0, 0, 0)));
    painter->drawPath(path());

}

QRectF PuzzlePiece::boundingRect() const
{
    return QRectF(-25, -25, 50, 50);
}

void PuzzlePiece::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QSet<QPair<int,int>> checked;
    checkNeighbours(checked);
    GameManager* gameManager = dynamic_cast<GameManager*>(scene());
    if (gameManager->_puzzleSize.width() * gameManager->_puzzleSize.height() ==
            checked.count())
    {
        scene()->addText("Congrats!!!You got the puzzle right!!!");
    }
    QGraphicsPathItem::mouseReleaseEvent(event);
}

QVariant PuzzlePiece::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionHasChanged && scene())
    {
        QPointF newPos = value.toPointF();
        QVector<std::pair<int,int>> adds = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
        for (int dir = 0; dir < 4; ++dir)
        {
            if (nullptr == m_neighbours[dir])
                continue;
            QPointF neighPos(newPos.x() + adds[dir].first * elementLength,
                             newPos.y() + adds[dir].second * elementLength);
            m_neighbours[dir]->setPos(neighPos);
        }
    }
    return QGraphicsItem::itemChange(change, value);
}

QMatrix PuzzlePiece::translateMatrix(qreal x, qreal y)
{
    QMatrix mat;
    mat.translate(x, y);
    return mat;
}

QPixmap PuzzlePiece::pixmapValue() const
{
    return pixmap;
}

void PuzzlePiece::setPixmap(const QPixmap &_pixmap)
{
    pixmap = _pixmap;
    update();
}

QPoint PuzzlePiece::coordinatesValue() const
{
    return coordinates;
}

void PuzzlePiece::setCoordinates(const QPoint &coords)
{
    coordinates = coords;
}

void PuzzlePiece::link(PuzzlePiece *otherPiece, PuzzlePiece::Direction otherPieceDir)
{
    m_neighbours[otherPieceDir] = otherPiece;
}

void PuzzlePiece::constructShape()
{
    QPainterPath elementUp(QPointF(-25.0, -25.0));

    r1 = 2.5, r2 = (2 * r1 + 10.0) / 2.0;
    elementLength = pieceSize - 2 * (r1 + r2);

    elementUp.moveTo(-pieceSize / 2.0 + (r1 + r2), -pieceSize / 2.0 + (r1 + r2));
    elementUp.lineTo(-5, elementUp.currentPosition().y());
    elementUp.arcTo(QRectF(elementUp.currentPosition().x() - r1,
                             elementUp.currentPosition().y() - 2 * r1,
                             2 * r1, 2 * r1),
                             270, -90);
    elementUp.arcTo(QRectF(elementUp.currentPosition().x(),
                             elementUp.currentPosition().y() - r2,
                             2 * r2, 2 * r2),
                             180, -180);
    elementUp.arcTo(QRectF(elementUp.currentPosition().x() - 2 * r1,
                             elementUp.currentPosition().y() - r1,
                             2 * r1, 2 * r1),
                             0, -90);
    elementUp.lineTo(pieceSize / 2.0 - (r1 + r2), -pieceSize / 2.0 + (r1 + r2));

    QMatrix mat;
    mat.rotate(90);

    QPainterPath elementRight = mat.map(elementUp);
    QPainterPath elementDown = mat.map(elementRight);
    QPainterPath elementLeft = mat.map(elementDown);

    QPainterPath lineUp;
    lineUp.moveTo(-pieceSize / 2.0 + (r1 + r2), -pieceSize / 2.0 + (r1 + r2));
    lineUp.lineTo(lineUp.currentPosition().x() + elementLength, lineUp.currentPosition().y());

    QPainterPath lineRight = mat.map(lineUp);

    QPainterPath northOut(elementUp);
    QPainterPath northIn(translateMatrix(0, -elementLength).map(elementDown).toReversed());
    QPainterPath northNone(lineUp);
    QPainterPath eastOut(elementRight);
    QPainterPath eastIn(translateMatrix(elementLength, 0).map(elementLeft).toReversed());
    QPainterPath eastNone(lineRight);
    QPainterPath southOut(elementDown);
    QPainterPath southIn(translateMatrix(0, elementLength).map(elementUp).toReversed());
    QPainterPath southNone(translateMatrix(0, elementLength).map(lineUp).toReversed());
    QPainterPath westOut(elementLeft);
    QPainterPath westIn(translateMatrix(-elementLength, 0).map(elementRight).toReversed());
    QPainterPath westNone(translateMatrix(-elementLength, 0).map(lineRight).toReversed());

    QPainterPath border;
    if (north == Out)
    {
        border.connectPath(northOut);
    } else if (north == In)
    {
        border.connectPath(northIn);
    } else {
        border.connectPath(northNone);
    }
    if (east == Out)
    {
        border.connectPath(eastOut);
    } else if (east == In)
    {
        border.connectPath(eastIn);
    } else {
        border.connectPath(eastNone);
    }
    if (south == Out)
    {
        border.connectPath(southOut);
    } else if (south == In)
    {
        border.connectPath(southIn);
    } else {
        border.connectPath(southNone);
    }
    if (west == Out)
    {
        border.connectPath(westOut);
    } else if (west == In)
    {
        border.connectPath(westIn);
    } else {
        border.connectPath(westNone);
    }

    setPath(border);
}

void PuzzlePiece::findNeighbour(PuzzlePiece::Direction dir)
{
    if (m_neighbours[dir])
    {
        return;
    }


    QVector<std::pair<int, int>> adds = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};

    QPoint expectedNeighbourCoords = QPoint(coordinatesValue().x() + adds[dir].first,
                                             coordinatesValue().y() + adds[dir].second);

    QPointF p = pos();
    QPointF supposedNeighbourPos(p.x() + adds[dir].first * elementLength,
                                 p.y() + adds[dir].second * elementLength);
    PuzzlePiece* factNeighbour = dynamic_cast<PuzzlePiece*>(scene()->itemAt(supposedNeighbourPos, QTransform()));

    if (factNeighbour == nullptr)
    {
        return;
    }
    if (factNeighbour->coordinatesValue() == expectedNeighbourCoords)
    {
        link(factNeighbour, dir);

        Direction otherDir = North;

        if (dir == North)
        {
            otherDir = South;
        } else if (dir == South)
        {
            otherDir = North;
        } else if (dir == West)
        {
            otherDir = East;
        } else if (dir == East)
        {
            otherDir = West;
        }

        factNeighbour->link(this, otherDir);
        QPointF otherPos = factNeighbour->pos();
        QPointF thisPos(otherPos.x() + adds[otherDir].first * elementLength,
                        otherPos.y() + adds[otherDir].second * elementLength);
        setPos(thisPos);
    }
}

void PuzzlePiece::checkNeighbours(QSet<QPair<int,int>> &checked)
{
    QPair<int,int> pair(coordinatesValue().x(),
                                coordinatesValue().y());
    if (checked.contains(pair))
    {
        return;
    }

    checked.insert(pair);
    findNeighbour(North);
    findNeighbour(East);
    findNeighbour(South);
    findNeighbour(West);

    if (m_neighbours[North])
        m_neighbours[North]->checkNeighbours(checked);
    if (m_neighbours[East])
        m_neighbours[East]->checkNeighbours(checked);
    if (m_neighbours[South])
        m_neighbours[South]->checkNeighbours(checked);
    if (m_neighbours[West])
        m_neighbours[West]->checkNeighbours(checked);
}
