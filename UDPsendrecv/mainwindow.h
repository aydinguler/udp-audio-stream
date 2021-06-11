#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>//Note: You must first operate 3 and then 4, otherwise the system will prompt no such file or directory


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QUdpSocket       *m_qudpSocket;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
void RecvData();

private slots:
void on_pushButton_clicked();

void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
