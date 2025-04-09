#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QAbstractSocket>  // 包含 QAbstractSocket 用于 SocketError

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_connectButton_clicked();
    void on_sendButton_clicked();
    void on_readyRead();  // 读取数据槽函数
    void on_errorOccurred(QAbstractSocket::SocketError socketError);  // 错误处理槽函数

private:
    Ui::MainWindow *ui;
    QTcpSocket *tcpSocket;
};

#endif // MAINWINDOW_H
