#include "gamemanager.h"

GameManager::GameManager() :
    pieceSize(31)
{
    setup(QSize(), QString());
}

GameManager::GameManager(QSize puzzleSize, QString imageFilePath) :
    pieceSize(31)
{
    setup(puzzleSize, imageFilePath);
}

void GameManager::setup(QSize puzzleSize, QString imagePath)
{
    QPixmap pixmap(imagePath);
    _puzzleSize = puzzleSize;

    QVector<QVector<ConnectorBox>> connectors(puzzleSize.height(),
                                              QVector<ConnectorBox>(puzzleSize.width(),
                                                                    ConnectorBox(PuzzlePiece::None,
                                                                                 PuzzlePiece::None,
                                                                                 PuzzlePiece::None,
                                                                                 PuzzlePiece::None)));
    for (int row = 0; row < puzzleSize.height(); ++row)
    {
        for (int col = 0; col < puzzleSize.width(); ++col)
        {

            // South and north connectors
            if (row == puzzleSize.height() - 1)
            {
                if (PuzzlePiece::Out == connectors[row - 1][col].south)
                {
                    connectors[row][col].north = PuzzlePiece::In;
                } else
                {
                    connectors[row][col].north = PuzzlePiece::Out;
                }
                connectors[row][col].south = PuzzlePiece::None;
            } else {
                if (row == 0)
                {
                    connectors[row][col].north = PuzzlePiece::None;
                } else
                {
                    if (PuzzlePiece::Out == connectors[row - 1][col].south)
                    {
                        connectors[row][col].north = PuzzlePiece::In;
                    } else
                    {
                        connectors[row][col].north = PuzzlePiece::Out;
                    }
                }
                quint32 randomNumber = QRandomGenerator::global()->generate();
                if (randomNumber%2 == 0)
                {
                    connectors[row][col].south = PuzzlePiece::Out;
                } else
                {
                    connectors[row][col].south = PuzzlePiece::In;
                }
            }
            // !South and north connectors

            // East and west connectors
            if (puzzleSize.width() - 1 == col)
            {
                if (PuzzlePiece::Out == connectors[row][col - 1].east)
                {
                    connectors[row][col].west = PuzzlePiece::In;
                } else
                {
                    connectors[row][col].west = PuzzlePiece::Out;
                }
                connectors[row][col].east = PuzzlePiece::None;
            } else {
                if (col == 0)
                {
                    connectors[row][col].west = PuzzlePiece::None;
                } else
                {
                    if (PuzzlePiece::Out == connectors[row][col - 1].east)
                    {
                        connectors[row][col].west = PuzzlePiece::In;
                    } else
                    {
                        connectors[row][col].west = PuzzlePiece::Out;
                    }
                }
                quint32 randomNumber = QRandomGenerator::global()->generate();
                if (randomNumber%2 == 0)
                {
                    connectors[row][col].east = PuzzlePiece::Out;
                } else
                {
                    connectors[row][col].east = PuzzlePiece::In;
                }
            }
            // !East and west connectors

            // Puzzle piece creating

            PuzzlePiece* piece = new PuzzlePiece(connectors[row][col].north,
                                                 connectors[row][col].east,
                                                 connectors[row][col].south,
                                                 connectors[row][col].west);
            addItem(piece);
            qreal x = col * piece->elementLength;
            qreal y = row * piece->elementLength;
            qint32 w = width();
            qint32 h = height();
            qint32 randX = QRandomGenerator::global()->generate() % w;
            qint32 randY = QRandomGenerator::global()->generate() % h;
            piece->setPos(randX, randY);
            piece->setCoordinates(QPoint(col, row));
            QRect rect(0, 0, 50, 50);
            rect.translate(x + (col ? -10 : 0), y + (row ? -10 : 0));
            pixmap = pixmap.scaled((piece->elementLength) * puzzleSize.width(),
                                   (piece->elementLength) * puzzleSize.height());
            QPixmap px = pixmap.copy(rect);

            piece->setPixmap(px);
        }
    }

}
