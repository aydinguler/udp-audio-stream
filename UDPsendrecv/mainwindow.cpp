#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_qudpSocket = new QUdpSocket();

    //Unicast
    //quint16 nport = 3956;
    //QString strIP = "127.0.0.1";
    //m_qudpSocket->bind(QHostAddress(strIP),nport);//Bind the local receiver IP and port (for unicast reception)
    //connect(m_qudpSocket,SIGNAL(readyRead()),this,SLOT(RecvData()));

    //Multicast
    m_qudpSocket->bind(QHostAddress::AnyIPv4,3956,QUdpSocket::ShareAddress);//Bind all ips to receive multicast group information
    m_qudpSocket->joinMulticastGroup(QHostAddress("224.0.0.10"));//Join the multicast group ip: 224.0.0.10
    m_qudpSocket->setSocketOption(QAbstractSocket::ReceiveBufferSizeSocketOption,1024*1024*8);//Set buffer
    connect(m_qudpSocket,SIGNAL(readyRead()),this,SLOT(RecvData()));//Connect the receiving signal slot

    QAudioFormat format;//Define the type of audio processing
    format.setSampleRate(8000);//The acquisition frequency is 1s 8000 times
    format.setChannelCount(1);//Set to 1 provincial highway
    format.setSampleSize(16);//Set the sample size, 8 is also OK, but the sender and receiver must correspond
    format.setCodec("audio/pcm");//Set to PCM encoding
    format.setSampleType(QAudioFormat::SignedInt);
    format.setByteOrder(QAudioFormat::LittleEndian);//Set the data type of Xiaowei
    input = new QAudioInput(format,this);
    inputDevice = input->start();//input starts to read the input audio signal and writes it into QIODevice, here is inputDevice
    output = new QAudioOutput(format,this);
    outputDevice = output->start();//Start playing


}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::RecvData()
{
    QByteArray baRecv;
    while(m_qudpSocket->hasPendingDatagrams())
    {
        baRecv.resize(m_qudpSocket->pendingDatagramSize());
        m_qudpSocket->readDatagram(baRecv.data(),baRecv.size());
        QString strRecvData = baRecv;
        ui->textEdit_2->setPlainText(strRecvData);
        qDebug()<<baRecv.data();
    }
    qDebug()<<"break\n";
}


//Unicast
void MainWindow::on_pushButton_clicked()
{
     m_qudpSocket->writeDatagram(ui->textEdit->toPlainText().toUtf8(),QHostAddress("127.0.0.1"),3956);
}

//Multicast
void MainWindow::on_pushButton_2_clicked()
{
     m_qudpSocket->joinMulticastGroup(QHostAddress("224.0.0.10")); //Join multicast ip: 224.0.0.10
     m_qudpSocket->writeDatagram(ui->textEdit->toPlainText().toUtf8(),QHostAddress("224.0.0.10"),3956);
}

