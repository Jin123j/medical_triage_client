#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QHostAddress>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , tcpSocket(new QTcpSocket(this))
{
    ui->setupUi(this);

    setWindowIcon(QIcon(":/icons/hospital.png"));
    setWindowTitle("医疗分诊系统");
    setMinimumSize(1400, 1000);


    // 应用样式表
    QFile styleFile(":/styles/medical.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(styleFile.readAll());
        setStyleSheet(styleSheet);
        styleFile.close();
    } else {
        // 直接内联样式表作为备选
        setStyleSheet(
            "QMainWindow { background-color: #f0f8ff; }"
            "QPushButton { background-color: #3498db; color: white; border-radius: 4px; padding: 8px 15px; }"
            "QPushButton:hover { background-color: #2980b9; }"
            "QGroupBox { border: 1px solid #bdc3c7; border-radius: 5px; margin-top: 15px; font-weight: bold; }"
            "QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top center; padding: 0 5px; }"
            "QLineEdit, QTextEdit { border: 1px solid #bdc3c7; border-radius: 4px; padding: 6px; }"
            "QLabel#departmentLabel { font-size: 16pt; font-weight: bold; color: #2c3e50; }"
            "QStatusBar { background-color: #ecf0f1; }"
            );
    }

    //QVBoxLayout* mainLayout = new QVBoxLayout(this->centralWidget());
    //mainLayout->setContentsMargins(20, 20, 20, 20);
    // mainLayout->setSpacing(15);




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
