#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QAudio>//These five are QT's audio processing libraries
#include <QAudioFormat>
#include <QAudioInput>
#include <QAudioOutput>
#include <QIODevice>
#include <QDebug>
#include <QAudioDecoder>
#include <QFile>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QFile *file;
    QAudioDecoder *decoder;
    QAudioInput *input;
    QAudioOutput *output;
    QIODevice *inputDevice, *outputDevice;
    struct audioSend{
        char audioDataSend[7680];
        int lensSend;
    };
    struct audioRecv{
        char audioDataRecv[7680];
        int lensRecv;
    };

public slots:
    void readyRead();

private slots:
    void onReadyRead();

    void on_pushButton_clicked(bool checked);

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    QUdpSocket  *socket;
    QUdpSocket *senderSocket;
    QHostAddress *targetAddress;
    quint16 *targetPort;
};
#endif // MAINWINDOW_H
