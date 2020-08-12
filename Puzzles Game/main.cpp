#include "configurationdialog.h"
#include "puzzlepiece.h"
#include "testclass.h"
#include "gamemanager.h"

#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPainter>
#include <QDialog>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    ConfigurationDialog dialog;
    QGraphicsView graphicsView;

    dialog.resize(700, 700);
    dialog.setWindowTitle("Welcome to MaybePuzzles");
    if (dialog.exec() == QDialog::Accepted)
    {
        GameManager* manager = new GameManager(dialog.puzzleSize(), dialog.imageFilePath());
        graphicsView.setRenderHint(QPainter::Antialiasing);
        graphicsView.setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
        graphicsView.setWindowTitle("MaybePuzzles -- Game");
        graphicsView.setScene(manager);
        graphicsView.resize(500, 500);
        graphicsView.show();
    }

    return app.exec();
}
