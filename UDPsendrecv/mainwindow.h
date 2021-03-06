#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "settingsdialog.h"

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
    struct audioSend{
        char audioDataSend[7680];
        int lensSend;
    };
    struct audioRecv{
        char audioDataRecv[7680];
        int lensRecv;
    };

    bool lastUpdatedFormatFileRead();
    void setAudioFormat();
    void makeUIElementsInvisible();
    void stopStream();
    bool fileOpen();
    QString getIPAddressFromUser();
    void setIPAdressAndPortNumber(QString giveThisTargetAddress, quint16 giveThisTargetPort);

public slots:
    void readyRead();

private slots:
    void onReadyReadFileStream();
    void onReadyReadLiveStream();
    void on_pushButton_clicked(bool checked);
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_comboBox_activated();
    void on_actionOptions_triggered();

private:
    Ui::MainWindow *ui;
    QStringList pieces;
    settingsDialog *mySettingsDialog;
    QAudioFormat *format;
    QFile *file;
    QFile *lastUpdatedFormatFile;
    QAudioDeviceInfo *infoInput;
    QAudioDeviceInfo *infoOutput;
    QAudioDecoder *decoder;
    QAudioInput *input;
    QAudioOutput *output;
    QIODevice *IODevice;
    QUdpSocket  *socket;
    QUdpSocket *senderSocket;
    QHostAddress *targetAddress;
    quint16 *targetPort;
};
#endif // MAINWINDOW_H
