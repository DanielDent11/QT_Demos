#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "puzzlepiece.h"

#include <QGraphicsScene>
#include <QVector>
#include <QRandomGenerator>
#include <QDebug>
#include <QLabel>

class GameManager : public QGraphicsScene
{
    typedef struct connectorStruct {
        connectorStruct() :
            north(PuzzlePiece::None),
            east(PuzzlePiece::None),
            south(PuzzlePiece::None),
            west(PuzzlePiece::None)
        {
        }
        connectorStruct(PuzzlePiece::ConnectorPosition _north,
                        PuzzlePiece::ConnectorPosition _east,
                        PuzzlePiece::ConnectorPosition _south,
                        PuzzlePiece::ConnectorPosition _west) :
            north(_north),
            east(_east),
            south(_south),
            west(_west)
        {

        }
        PuzzlePiece::ConnectorPosition north;
        PuzzlePiece::ConnectorPosition east;
        PuzzlePiece::ConnectorPosition south;
        PuzzlePiece::ConnectorPosition west;
    } ConnectorBox;
public:
    GameManager();
    GameManager(QSize puzzleSize, QString imageFilePath);

    QSize _puzzleSize;

private:
    qreal pieceSize;
    QPixmap* pixmap;

    void setup(QSize puzzleSize, QString imagePath = QString());
};

#endif // GAMEMANAGER_H
