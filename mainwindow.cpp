#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHostAddress>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , tcpSocket(new QTcpSocket(this))
{
    ui->setupUi(this);
    // 连接socket信号到槽函数
    connect(tcpSocket, &QTcpSocket::readyRead, this, &MainWindow::on_readyRead);
    connect(tcpSocket, &QTcpSocket::errorOccurred, this, &MainWindow::on_errorOccurred);  // 连接错误信号
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_connectButton_clicked()
{
    tcpSocket->connectToHost("192.168.58.129", 8080);

    if (tcpSocket->waitForConnected(3000)) {
        QMessageBox::information(this, "连接", "已连接到服务器!");
    } else {
        QMessageBox::warning(this, "连接", "连接服务器失败.");
    }
}

void MainWindow::on_sendButton_clicked()
{
    QString message = ui->messageInput->text();
    qDebug()<<"this is send message:"<<message;
    if (!message.isEmpty()) {
        tcpSocket->write(message.toUtf8());
    }
}

void MainWindow::on_readyRead()
{
    QByteArray data = tcpSocket->readAll();
    qDebug()<<"this is recv message:"<< QString(data);
    ui->responseOutput->setText(QString::fromUtf8(data));
}

// 错误处理槽函数
void MainWindow::on_errorOccurred(QAbstractSocket::SocketError socketError)
{
    QString errorMessage;
    switch (socketError) {
    case QAbstractSocket::HostNotFoundError:
        errorMessage = "服务器未找到。";
        break;
    case QAbstractSocket::ConnectionRefusedError:
        errorMessage = "连接被拒绝。";
        break;
    case QAbstractSocket::RemoteHostClosedError:
        errorMessage = "远程主机关闭了连接。";
        break;
    case QAbstractSocket::NetworkError:
        errorMessage = "网络错误。";
        break;
    default:
        errorMessage = "发生了一个未知错误。";
    }
    QMessageBox::warning(this, "连接错误", errorMessage);
}
