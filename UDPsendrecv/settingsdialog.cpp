#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include <QMessageBox>
#include <QDebug>

settingsDialog::settingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::settingsDialog)
{
    ui->setupUi(this);
    fileFormatInfo = new QFile(this);
}

settingsDialog::~settingsDialog()
{
    delete ui;
}

//Get format preferences from user and save it to text file.
//Later, this text file will be read by MainWindow class to use as a format settings
void settingsDialog::on_pushButton_clicked()
{
    fileFormatInfo->setFileName(QCoreApplication::applicationDirPath() + "/lastUpdatedFormatSettings.txt");
    if (!fileFormatInfo->open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox msgBox;
        msgBox.setText("Choose a file to start streaming");
        msgBox.exec();

        qDebug() << QString(QCoreApplication::applicationDirPath());
        return;
    }
    QTextStream out(fileFormatInfo);
    out << ui->comboBox->currentText()+" ";
    out << ui->comboBox_2->currentText()+" ";
    out << ui->comboBox_3->currentText()+" ";
    out << ui->comboBox_4->currentText()+" ";
    out << ui->comboBox_5->currentText()+" ";
    out << ui->comboBox_6->currentText();
    out.flush();
    fileFormatInfo->close();
    settingsDialog::close();
}

    /*!
    out << "Sample Rate: " + ui->comboBox->currentText() << Qt::endl;
    out << "Channel Count: " + ui->comboBox_2->currentText() << Qt::endl;
    out << "Sample Size: " + ui->comboBox_3->currentText() << Qt::endl;
    out << "Codec: " + ui->comboBox_4->currentText() << Qt::endl;
    out << "Sample Type: " + ui->comboBox_5->currentText() << Qt::endl;
    out << "Byte Order: " + ui->comboBox_6->currentText() << Qt::endl;
    */
