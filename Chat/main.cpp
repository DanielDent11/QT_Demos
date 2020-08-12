#include <QApplication>
#include <QCommandLineParser>
#include <QDebug>
#include <QtGlobal>

#include "maybechat.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QCommandLineParser parser;

    QCommandLineOption nicknameOption("n", QApplication::translate("main", "Set client nickname"),
                                           QApplication::translate("main", "nickname"));
    parser.addOption(nicknameOption);

    QCommandLineOption portOption("p", QApplication::translate("main", "Set client port"),
                                       QApplication::translate("main", "port"));
    parser.addOption(portOption);
    parser.process(app);
    const QString nickname = parser.value(nicknameOption);
    QString port = parser.value(portOption);

    MaybeChat *chat;

    if (!nickname.isNull() && !nickname.isEmpty() &&
        !port.isNull() && !port.isEmpty()) {
        chat = new MaybeChat(nickname, port.toInt());
    } else {
        chat = new MaybeChat("Anonym", 4050);
    }

    chat->show();

    return app.exec();
}
