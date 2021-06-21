#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>//Note: You must first operate 3 and then 4, otherwise the system will prompt no such file or directory
#include <QAudio>//These five are QT's audio processing libraries
#include <QAudioFormat>
#include <QAudioInput>
#include <QAudioOutput>
#include <QIODevice>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QTimer *timer;
    QAudioInput *input;
    QAudioOutput *output;
    QIODevice *inputDevice, *outputDevice;
    struct audioSend{
        char audioDataSend[1280];
        int lensSend;
    };
    struct audioRecv{
        char audioDataRecv[1280];
        int lensRecv;
    };

public slots:
    void readyRead();

private slots:
    void onReadyRead();

    void on_pushButton_clicked(bool checked);

private:
    Ui::MainWindow *ui;
    QUdpSocket  *socket;
};
#endif // MAINWINDOW_H
