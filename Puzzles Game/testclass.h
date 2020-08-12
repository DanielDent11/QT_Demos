#ifndef TESTCLASS_H
#define TESTCLASS_H

#include "configurationdialog.h"
#include "puzzlepiece.h"

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QLabel>
#include <QGraphicsPathItem>
#include <QStyleOptionGraphicsItem>

class TestClass
{
public:
    TestClass();

    void testPuzzlePiecesDrawning(QGraphicsScene* graphicsScene, QGraphicsView* graphicsView);
};

#endif // TESTCLASS_H
