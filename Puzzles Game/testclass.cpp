#include "testclass.h"

TestClass::TestClass()
{

}

void TestClass::testPuzzlePiecesDrawning(QGraphicsScene* graphicsScene, QGraphicsView* graphicsView)
{
    PuzzlePiece::ConnectorPosition pos[3] = {PuzzlePiece::Out, PuzzlePiece::In, PuzzlePiece::None};
    int offset = 0;
    int _north, _east, _south, _west;

    QPixmap* pixmap = new QPixmap("C:\\Users\\senio\\Downloads\\frendzone.jpg");

    for (_north = 0; _north < 3; ++_north)
    {
        for (_east = 0; _east < 3; ++_east)
        {
            for (_south = 0; _south < 3; ++_south)
            {
                for (_west = 0; _west < 3; ++_west)
                {
                    int x = offset % 4;
                    int y = offset / 4;

                    PuzzlePiece* piece = new PuzzlePiece(pos[_north], pos[_east], pos[_south], pos[_west]);
                    int cellSize = 70;
                    QGraphicsPathItem* pathItem =  graphicsScene->addPath(piece->translateMatrix(x * cellSize,
                    y * cellSize).map(piece->path()));
                    piece->setPixmap(*pixmap);
                    piece->setPos(x * cellSize, y * cellSize);
                    graphicsScene->addItem(piece);
                    ++offset;
                    QString text = "" + QString::number(_north) + " " +
                            QString::number(_east) + " " + QString::number(_south) + " " +
                            QString::number(_west);

                  QGraphicsTextItem* textItem = graphicsScene->addText(text);
                  textItem->setPos(x * cellSize - 20, y * cellSize + 25);
                }
            }
        }
    }
}
