#ifndef MAYBECHAT_H
#define MAYBECHAT_H

#include <QDialog>
#include <QUdpSocket>
#include <QHostAddress>
#include <QByteArray>
#include <QDataStream>
#include <QJsonObject>
#include <QJsonDocument>
#include <QStringList>
#include <QJsonArray>
#include <QTime>
#include <QDateTime>

namespace Ui {
class MaybeChat;
}

class MaybeChat : public QDialog
{
    Q_OBJECT

public:
    explicit MaybeChat(QString username, int port, QWidget *parent=nullptr);
    explicit MaybeChat(QWidget *parent = nullptr);
    ~MaybeChat();

protected:
    void closeEvent(QCloseEvent *event) override;

public slots:
    void onReturnPressed();

private slots:
    void onReadyRead();

private:
    Ui::MaybeChat *ui;
    QUdpSocket *udpSocket;
    QString username;
    bool isWaitingForOnlineUsers = false;
    bool isWaitingForOwnMessage = false;
    int port;

    void userJoined();
    void userLeft();
    void addUser(QString username);
    void removeUser(QString username);
    void appendMessage(QString username, QString text, bool ownMessage = false);
    void requestUsersOnline();
    void updateUsersWidget(QJsonArray users);
    void sendUsersOnline();
};

#endif // MAYBECHAT_H
