#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSerialPort/QSerialPort>
#include <QDebug>
QSerialPort *serial;
bool isSerialInitialised = false;
QString str;
QSerialPort::BaudRate baudrate = QSerialPort::Baud9600;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    char buffer[6];
    for (int i=0;i<20;++i)
    {
        sprintf(buffer,"COM%d",i);
        ui->list_port->addItem(buffer);
    }

    ui->list_baud->addItem("4800");
    ui->list_baud->addItem("9600");
    ui->list_baud->addItem("19200");
    ui->list_baud->addItem("38400");
    ui->list_baud->addItem("57600");
    ui->list_baud->addItem("115200");
//    serial->write("ok");
//    ui->list_baud->on_list_baud_itemClicked(ui->list_baud->item(1));



    for (int i=0;i<ui->list_port->count();++i){
        ui->list_port->item(i)->setForeground(QBrush(Qt::gray));
    }

    for (int i=0;i<ui->list_baud->count();++i){
        ui->list_baud->item(i)->setForeground(QBrush(Qt::gray));
    }
    ui->list_baud->item(1)->setTextColor(Qt::blue);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::serialReceived()
{
    str += serial->readAll();

    if(str.count(QLatin1Char('\n'))>10)
    {
        str = str.section('\n',2,-1); // extract from 2nd section onwards
    }

    qDebug()<<str.toLocal8Bit();
    ui->textOut->setPlainText(str.toLocal8Bit());
}

void MainWindow::on_list_port_itemClicked(QListWidgetItem *item)
{
    if(isSerialInitialised)
    {
        serial->close();
        isSerialInitialised = false;
    }

    for (int i=0;i<ui->list_port->count();++i){
        ui->list_port->item(i)->setForeground(QBrush(Qt::gray));
    }
    item->setTextColor(Qt::blue);
    serial = new QSerialPort(this);
    serial->setPortName(item->text());
    serial->setBaudRate(baudrate);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    if(serial->open(QIODevice::ReadWrite))
    {
        isSerialInitialised = true;
        connect(serial,SIGNAL(readyRead()),this,SLOT(serialReceived()));
        ui->statusBar->showMessage("Serial connection established.",2000);
    }
    else
    {
        ui->statusBar->showMessage("Serial connection unavailable.",2000);
    }

}


void MainWindow::on_list_baud_itemClicked(QListWidgetItem *item)
{

    if(item->text()=="4800") baudrate = QSerialPort::Baud4800;
    else if(item->text()=="9600") baudrate = QSerialPort::Baud9600;
    else if(item->text()=="19200") baudrate = QSerialPort::Baud19200;
    else if(item->text()=="38400") baudrate = QSerialPort::Baud38400;
    else if(item->text()=="57600") baudrate = QSerialPort::Baud57600;
    else if(item->text()=="115200") baudrate = QSerialPort::Baud115200;

    for (int i=0;i<ui->list_baud->count();++i){
        ui->list_baud->item(i)->setForeground(QBrush(Qt::gray));
    }
    item->setTextColor(Qt::blue);
}

void MainWindow::on_textIn_textChanged()
{
    QString tmp = ui->textIn->toPlainText();
    if(tmp.length()>0)
    {
        qDebug() << "Last char: " << tmp[tmp.length()-1] << "\n";
        if(tmp[tmp.length()-1]=='\xa')
        {
            serial->write(tmp.toLocal8Bit());
            ui->textIn->clear();
        }
    }
}
