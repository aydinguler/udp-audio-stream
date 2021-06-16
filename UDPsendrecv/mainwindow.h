#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>//Note: You must first operate 3 and then 4, otherwise the system will prompt no such file or directory
#include <QAudio>//These five are QT's audio processing libraries
#include <QAudioFormat>
#include <QAudioInput>
#include <QAudioOutput>
#include <QIODevice>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QAudioInput *input;
    QAudioOutput *output;
    QIODevice *inputDevice, *outputDevice;
    struct audioSend{
        char audioDataSend[1024];
        int lensSend;
    };

public slots:
void RecvData();

private slots:
    void onReadyRead();

private:
    Ui::MainWindow *ui;
    QUdpSocket  *m_qudpSocket;
};
#endif // MAINWINDOW_H
