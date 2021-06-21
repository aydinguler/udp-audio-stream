#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QUdpSocket();
    socket->bind(QHostAddress::LocalHost,1234);//127.0.0.1
    //Unicast
    //quint16 nport = 3956;
    //QString strIP = "127.0.0.1";
    //m_qudpSocket->bind(QHostAddress(strIP),nport);//Bind the local receiver IP and port (for unicast reception)
    //connect(m_qudpSocket,SIGNAL(readyRead()),this,SLOT(RecvData()));
    //Multicast
    //m_qudpSocket->bind(QHostAddress::AnyIPv4,3956,QUdpSocket::ShareAddress);//Bind all ips to receive multicast group information
    //m_qudpSocket->joinMulticastGroup(QHostAddress("224.0.0.10"));//Join the multicast group ip: 224.0.0.10
    //m_qudpSocket->setSocketOption(QAbstractSocket::ReceiveBufferSizeSocketOption,1024*1024*8);//Set buffer
    //connect(m_qudpSocket,SIGNAL(readyRead()),this,SLOT(RecvData()));//Connect the receiving signal slot

    QAudioFormat format;//Define the type of audio processing
    format.setSampleRate(16000);//The acquisition frequency is 1s 16000 times
    format.setChannelCount(1);//Set to 1 provincial highway
    format.setSampleSize(16);//Set the sample size, 8 is also OK, but the sender and receiver must correspond
    format.setCodec("audio/pcm");//Set to PCM encoding
    format.setSampleType(QAudioFormat::SignedInt);
    format.setByteOrder(QAudioFormat::LittleEndian);//Set the data type of Xiaowei
    input = new QAudioInput(format,this);
    //inputDevice = input->start();//input starts to read the input audio signal and writes it into QIODevice, here is inputDevice
    output = new QAudioOutput(format,this);
    //outputDevice = output->start();//Start playing
    timer = new QTimer(this);

    //connect(inputDevice,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
    //Slot function, when inputDevice receives the audio data written by input, it calls the onReadyRead function to send the data to the target host
    //connect(inputDevice,SIGNAL(readyRead()),this,SLOT(readyRead()));
    //Slot function, when outputDevice receives the audio data written by output, it calls the readyRead function
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::readyRead()
{
    qDebug()<<"Audio is being received..."<<Qt::endl;
    audioRecv ap;
    memset(&ap,0,sizeof(ap));
    QHostAddress sender;
    quint16 senderPort;
    socket->readDatagram((char*)&ap,sizeof(ap),&sender,&senderPort);
    outputDevice->write(ap.audioDataRecv,ap.lensRecv);
    //ui->textBrowser->setPlainText(sender.toString()+"\n"+senderPort+"\n"+ap.lensRecv+"\n"+ap.audioDataRecv+"\n");
    ui->textBrowser->setPlainText(ap.audioDataRecv);
    qDebug() << "Message from: " << sender.toString();
    qDebug() << "Message port: " << senderPort;
    qDebug() << "Message size: " << ap.lensRecv;
    qDebug() << "Message data: " << ap.audioDataRecv;
}
void MainWindow::onReadyRead()
{
    qDebug()<<"It's sending audio!"<<Qt::endl;
    audioSend ap;
    memset(&ap,0,sizeof(ap));
    ap.lensSend = inputDevice->read(ap.audioDataSend,1280);//Read audio
    //qDebug() << ap.lensSend;
    ui->textBrowser->setPlainText(ap.audioDataSend);
    socket->writeDatagram((const char*)&ap,sizeof(ap),QHostAddress::LocalHost,1234);
    //Send this structure to the target host, the port is 1234, and the IP is 127.0.0.1
}
void MainWindow::on_pushButton_clicked(bool checked)
{
    inputDevice = input->start();
    timer->stop();
    if (ui->comboBox->currentText() == "Sender" && ui->pushButton->isChecked()==true){
        connect(inputDevice,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
        ui->pushButton->setText("Stop");
    }
    else if (ui->comboBox->currentText() == "Receiver" && ui->pushButton->isChecked()==true){
        outputDevice = output->start();//Start playing
        timer->start();
        connect(timer,SIGNAL(timeout()),this,SLOT(readyRead()));
        ui->pushButton->setText("Stop");
    }
    else{
        ui->pushButton->setChecked(false);
        ui->pushButton->setText("Start");
        timer->stop();
    }
    qDebug() << checked;
}

