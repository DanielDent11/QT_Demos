#include "maybechat.h"
#include "ui_maybechat.h"

MaybeChat::MaybeChat(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MaybeChat)
{
    ui->setupUi(this);
}

MaybeChat::MaybeChat(QString username, int port, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MaybeChat),
    udpSocket(new QUdpSocket(this)),
    username(username),
    port(port)
{
    ui->setupUi(this);
    this->setWindowTitle("MaybeChat - " + username + "@" + QString("%1").arg(port, 4, 10, QChar('0')));

    ui->lineEdit->setFocusPolicy(Qt::StrongFocus);
    ui->textEdit->setReadOnly(true);
    ui->textEdit->setFocusPolicy(Qt::NoFocus);
    ui->listWidget->setFocusPolicy(Qt::NoFocus);
    ui->listWidget->setSortingEnabled(true);

    udpSocket->bind(QHostAddress::Any, 1234, QUdpSocket::ShareAddress |
                    QUdpSocket::ReuseAddressHint);

    connect(ui->lineEdit, SIGNAL(returnPressed()), this, SLOT(onReturnPressed()));
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));

    requestUsersOnline();
}

MaybeChat::~MaybeChat()
{
    delete ui;
}

void MaybeChat::closeEvent(QCloseEvent *event) {
    userLeft();
}

void MaybeChat::onReturnPressed() {
    QString text = ui->lineEdit->text();

    if (text.isEmpty())
        return;

    QByteArray block;

    QJsonObject jsonMessage;
    jsonMessage["type"] = "MESSAGE";
    jsonMessage["sender"] = username;
    jsonMessage["port"] = port;
    jsonMessage["text"] = text;

    block.append(QJsonDocument(jsonMessage).toJson(QJsonDocument::Compact));

    isWaitingForOwnMessage = true;
    udpSocket->writeDatagram(block, QHostAddress::Broadcast, 1234);

    ui->lineEdit->clear();
}

void MaybeChat::userJoined() {
    qDebug("userJoined");

    QByteArray block;

    QJsonObject jsonMessage;
    jsonMessage["type"] = "USER_JOINED";
    jsonMessage["username"] = username;

    block.append(QJsonDocument(jsonMessage).toJson(QJsonDocument::Compact));

    udpSocket->writeDatagram(block, QHostAddress::Broadcast, 1234);
}

void MaybeChat::userLeft() {
    QByteArray block;

    QJsonObject jsonMessage;
    jsonMessage["type"] = "USER_LEFT";
    jsonMessage["username"] = username;

    block.append(QJsonDocument(jsonMessage).toJson(QJsonDocument::Compact));

    udpSocket->writeDatagram(block, QHostAddress::Broadcast, 1234);
}

void MaybeChat::onReadyRead() {
    qDebug("OnReadyRead");
    QByteArray block;
    block.resize(udpSocket->pendingDatagramSize());
    QHostAddress sender;
    quint16 senderPort;
    udpSocket->readDatagram(block.data(), block.size(), &sender, &senderPort);

    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(block, &parseError);
    QJsonObject jsonMessage;

    if (parseError.error == QJsonParseError::NoError) {
        if (jsonDoc.isObject()) {
            jsonMessage = jsonDoc.object();
        }
        else {
            qDebug("Error 1");
        }
    }
    else {
        qDebug("Error 2");
    }

    qDebug(QString::number(jsonMessage.size()).toLatin1());
    QJsonValue typeVal = jsonMessage.value("type");

    if (typeVal.toString().compare("USER_JOINED", Qt::CaseSensitive) == 0) {
        QJsonValue usernameVal = jsonMessage.value("username");
        addUser(usernameVal.toString());
    } else if (typeVal.toString().compare("USER_LEFT", Qt::CaseSensitive) == 0) {
        QJsonValue usernameVal = jsonMessage.value("username");
        removeUser(usernameVal.toString());
    } else if (typeVal.toString().compare("MESSAGE", Qt::CaseSensitive) == 0) {
        QJsonValue senderVal = jsonMessage.value("sender");
        QJsonValue textVal = jsonMessage.value("text");
        appendMessage(senderVal.toString(), textVal.toString(), isWaitingForOwnMessage);
        isWaitingForOwnMessage = false;
    } else if (typeVal.toString().compare("REQUEST_USERSONLINE", Qt::CaseSensitive) == 0) {
        sendUsersOnline();
    } else if (typeVal.toString().compare("RESPONSE_USERSONLINE", Qt::CaseSensitive) == 0 &&
               isWaitingForOnlineUsers) {
        qDebug("response");
        QJsonArray users = jsonMessage["users"].toArray();
        isWaitingForOnlineUsers = false;
        updateUsersWidget(users);
        userJoined();
    }
}

void MaybeChat::addUser(QString username) {
    ui->listWidget->addItem(username);
    QColor color = ui->textEdit->textColor();
    ui->textEdit->moveCursor(QTextCursor::End);
    ui->textEdit->setTextColor(Qt::gray);
    ui->textEdit->insertPlainText(tr("*%1 has joined the chat\n").arg(username));
    ui->textEdit->moveCursor(QTextCursor::End);
    ui->textEdit->setTextColor(color);
}

void MaybeChat::removeUser(QString username) {
    QList<QListWidgetItem *> items = ui->listWidget->findItems(username, Qt::MatchExactly);

    if (items.isEmpty())
        return;

    delete items.at(0);

    QColor color = ui->textEdit->textColor();
    ui->textEdit->moveCursor(QTextCursor::End);
    ui->textEdit->setTextColor(Qt::gray);
    ui->textEdit->insertPlainText(tr("*%1 has left the chat\n").arg(username));
    ui->textEdit->moveCursor(QTextCursor::End);
    ui->textEdit->setTextColor(color);
}

void MaybeChat::appendMessage(QString username, QString text, bool ownMessage) {
    QString time = QTime::currentTime().toString();
    if (ownMessage) {
        QColor color = ui->textEdit->textColor();
        int font = ui->textEdit->fontWeight();
        ui->textEdit->moveCursor(QTextCursor::End);
        ui->textEdit->setTextColor(Qt::red);
        ui->textEdit->insertPlainText("["+time+"]");
        ui->textEdit->setFontWeight(QFont::Bold);
        ui->textEdit->insertPlainText(username);
        ui->textEdit->moveCursor(QTextCursor::End);
        ui->textEdit->setTextColor(color);
        ui->textEdit->setFontWeight(font);
    } else {
        QColor color = ui->textEdit->textColor();
        int font = ui->textEdit->fontWeight();
        ui->textEdit->moveCursor(QTextCursor::End);
        ui->textEdit->setTextColor(Qt::blue);
        ui->textEdit->insertPlainText("["+time+"]");
        ui->textEdit->setFontWeight(QFont::Bold);
        ui->textEdit->insertPlainText(username);
        ui->textEdit->moveCursor(QTextCursor::End);
        ui->textEdit->setTextColor(color);
        ui->textEdit->setFontWeight(font);
    }
    ui->textEdit->insertPlainText(": " + text + "\n");
    ui->textEdit->moveCursor(QTextCursor::End);
}

void MaybeChat::requestUsersOnline() {
    isWaitingForOnlineUsers = true;
    QByteArray block;
    QJsonObject jsonMessage;
    jsonMessage["type"] = "REQUEST_USERSONLINE";
    block.append(QJsonDocument(jsonMessage).toJson(QJsonDocument::Compact));
    udpSocket->writeDatagram(block, QHostAddress::Broadcast, 1234);
}

void MaybeChat::updateUsersWidget(QJsonArray users) {
    foreach (QJsonValue userVal, users) {
        ui->listWidget->addItem(userVal.toString());
    }
}

void MaybeChat::sendUsersOnline() {
    QByteArray block;
    QJsonObject jsonMessage;
    jsonMessage["type"] = "RESPONSE_USERSONLINE";
    QStringList users;

    for (int i = 0; i < ui->listWidget->count(); ++i) {
        QListWidgetItem *item = ui->listWidget->item(i);
        users.append(item->text());
    }

    QJsonArray usersArray = QJsonArray::fromStringList(users);

    jsonMessage["users"] = usersArray;

    block.append(QJsonDocument(jsonMessage).toJson(QJsonDocument::Compact));

    udpSocket->writeDatagram(block, QHostAddress::Broadcast, 1234);
}


