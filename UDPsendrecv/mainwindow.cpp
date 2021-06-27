#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    senderSocket = new QUdpSocket(this);
    socket = new QUdpSocket(this);
    file = new QFile(this);

    QAudioFormat format;//Define the type of audio processing
    format.setSampleRate(16000);//The acquisition frequency is 1s 16000 times
    format.setChannelCount(1);//Set to 1 channel
    format.setSampleSize(16);//Set the sample size, 8 is also OK, but the sender and receiver must match
    format.setCodec("audio/pcm");//Set to PCM encoding
    format.setSampleType(QAudioFormat::SignedInt);
    format.setByteOrder(QAudioFormat::LittleEndian);//Set the data type of Xiaowei
    input = new QAudioInput(format,this);
    output = new QAudioOutput(format,this);
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
    ui->textBrowser->setPlainText("Message from: "+sender.toString()+"\n"+
                                  "Message port: "+QString::number(senderPort)+"\n"+
                                  "Message size:"+QString::number(ap.lensRecv)+"\n"+
                                  "Message data:"+ap.audioDataRecv+"\n");
    //qDebug() << "Message from: " << sender.toString();
    //qDebug() << "Message port: " << QString::number(senderPort);
    //qDebug() << "Message size: " << QString::number(ap.lensRecv);
    //qDebug() << "Message data: " << ap.audioDataRecv;
}
void MainWindow::onReadyRead()
{
    qDebug()<<"It's sending audio!"<<Qt::endl;
    audioSend ap;
    memset(&ap,0,sizeof(ap));
    QByteArray dummy;
    dummy = inputDevice->readAll();
    ap.lensSend = file->read(ap.audioDataSend,1280);//Read audio
    //qDebug() << ap.lensSend;
    ui->textBrowser->setPlainText(ap.audioDataSend);
    senderSocket->writeDatagram((const char*)&ap,sizeof(ap),*targetAddress,*targetPort);
    //Send this structure to the target host, the port and the IP are declared in the if statements
}
void MainWindow::on_pushButton_clicked(bool checked)
{
    if (ui->comboBox->currentText() == "Sender"
            && (ui->comboBox_2->currentText() == "Unicast")
            && ui->pushButton->isChecked()==true)
    {
        if (!file->open(QIODevice::ReadOnly))
        {
            QMessageBox msgBox;
            msgBox.setText("File can not open");
            msgBox.exec();
            return;
        }
        targetAddress = new QHostAddress("192.168.1.35");
        targetPort = new quint16(45000);
        inputDevice = input->start();//input starts to read the input audio signal and writes it into QIODevice, here is inputDevice
        connect(inputDevice,SIGNAL(readyRead()),this,SLOT(onReadyRead()));//Slot function, when inputDevice receives the audio data written by input,
        ui->pushButton->setText("Stop");                                  //it calls the onReadyRead function to send the data to the target host
    }
    else if (ui->comboBox->currentText() == "Sender"
             && (ui->comboBox_2->currentText() == "Multicast")
             && ui->pushButton->isChecked()==true)
    {
        if (!file->open(QIODevice::ReadOnly))
        {
            QMessageBox msgBox;
            msgBox.setText("File can not open");
            msgBox.exec();
            return;
        }
        targetAddress = new QHostAddress("224.0.0.2");
        targetPort = new quint16(9999);
        inputDevice = input->start();//input starts to read the input audio signal and writes it into QIODevice, here is inputDevice
        connect(inputDevice,SIGNAL(readyRead()),this,SLOT(onReadyRead()));//Slot function, when inputDevice receives the audio data written by input,
        ui->pushButton->setText("Stop");                                  //it calls the onReadyRead function to send the data to the target host
    }
    else if (ui->comboBox->currentText() == "Receiver"
             && ui->comboBox_2->currentText() == "Unicast"
             && ui->pushButton->isChecked()==true)
    {
        outputDevice = output->start();//Start playing
        socket->bind(QHostAddress::AnyIPv4,45000);
        connect(socket,SIGNAL(readyRead()),this,SLOT(readyRead()));//Slot function, when socket has data sent by onReadyRead function,
        ui->pushButton->setText("Stop");                           //it calls the readyRead function to receive the data
    }
    else if (ui->comboBox->currentText() == "Receiver"
             && ui->comboBox_2->currentText() == "Multicast"
             && ui->pushButton->isChecked()==true)
    {
        outputDevice = output->start();//Start playing
        socket->bind(QHostAddress(QHostAddress::AnyIPv4),9999,QUdpSocket::ReuseAddressHint|QUdpSocket::ShareAddress);
        socket->joinMulticastGroup(QHostAddress("224.0.0.2"));//Join the multicast group ip: 224.0.0.2
        connect(socket,SIGNAL(readyRead()),this,SLOT(readyRead()));//Slot function, when socket has data sent by onReadyRead function,
        ui->pushButton->setText("Stop");                           //it calls the readyRead function to receive the data
    }
    else if (ui->comboBox->currentText() == "Receiver"
             && ui->comboBox_2->currentText() == "Multicast"
             && ui->pushButton->isChecked()==false)
    {
        socket->leaveMulticastGroup(QHostAddress("224.0.0.2"));//Leave the multicast group ip: 224.0.0.2
        socket->close();
        senderSocket->close();
        output->stop();
        input->stop();
        ui->pushButton->setChecked(false);
        ui->pushButton->setText("Start");
    }
    else{
        socket->close();
        senderSocket->close();
        output->stop();
        input->stop();
        file->close();
        ui->pushButton->setChecked(false);
        ui->pushButton->setText("Start");
    }
    qDebug() << checked;
}


void MainWindow::on_pushButton_2_clicked()
{
    QString filter = "Audio File (*.wav)";
    QString fileName = QFileDialog::getOpenFileName(this, "Open a file", "C://", filter);
    file->setFileName(fileName);
}

