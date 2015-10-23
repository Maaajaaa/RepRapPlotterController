#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){

    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->preview_graphicsView->setSceneRect(0, 0, 9962, 14230);
    xMaximum = 9960;
    yMaximum = 14228;
    /*QBrush blackBrush;
    blackBrush.setColor(Qt::black);
    ui->preview_graphicsView->setBackgroundBrush(blackBrush);*/
    ui->preview_graphicsView->setScene(scene);
    QPen rectPen;
    rectPen.setColor(Qt::blue);
    ///TODO: correct SceneRect
    if(ui->preview_graphicsView->width() / 9962.0 <= ui->preview_graphicsView->height() / 14230.0 )  //narrow view like 9:16
    {
      sceneScale = (qreal) ui->preview_graphicsView->width() / 9962;
      qDebug() << "sceneScale: " << sceneScale;
      //ui->sceneScale_label->setText(QString::number(sceneScale));
      rectPen.setWidth(1/sceneScale);
      scene->addRect(0, 0, 9962, 14230, rectPen);
    }
    else  //wide view like 16:9
    {
      sceneScale = (qreal) ui->preview_graphicsView->height() / 14230;
      qDebug() << "sceneScalE: " << sceneScale;
      //ui->sceneScale_label->setText(QString::number(sceneScale));
      rectPen.setWidth(1/sceneScale);
      scene->addRect(0, 0, 9962, 14230, rectPen);
    }

    //ui->preview_pushButton->setText(QString::number(ui->preview_graphicsView->height()) + QString("|") +
    //                                QString::number(ui->preview_graphicsView->width()));
    ui->preview_graphicsView->scale(sceneScale, sceneScale);

    ui->geoShape_comboBox->setItemData(4, 0, Qt::UserRole -1);      //disable Triangle
    ui->unit_comboBox->setItemData(0, 0, Qt::UserRole -1);      //disable millimeters

//============================================================SERIAL============================================================//

    serial = new QSerialPort(this);
    serial->setPortName("/dev/ttyUSB0");
    serial->setBaudRate(115200);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    if (serial->open(QIODevice::ReadWrite))
    {
        ui->serialPort_label->setText(serial->portName());  ui->serialPort_label->setStyleSheet("QLabel { color: green }");
        ui->baudrate_label->setText(QString::number(serial->baudRate()));
        ui->sendSerial_pushButton->setEnabled(true);
        connect(serial,SIGNAL(readyRead()),this,SLOT(serialRead()));
    }
    else
    {
        serial->setPortName("/dev/ttyUSB1");
        if (serial->open(QIODevice::ReadWrite))
        {
            ui->serialPort_label->setText(serial->portName());  ui->serialPort_label->setStyleSheet("QLabel { color: green }");
            ui->baudrate_label->setText(QString::number(serial->baudRate()));
            ui->sendSerial_pushButton->setEnabled(true);
            connect(serial,SIGNAL(readyRead()),this,SLOT(serialRead()));
            connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(serialError()));
        }
        else
        {
            serial->setPortName("/dev/ttyUSB2");
            if (serial->open(QIODevice::ReadWrite))
            {
                ui->serialPort_label->setText(serial->portName());  ui->serialPort_label->setStyleSheet("QLabel { color: green }");
                ui->baudrate_label->setText(QString::number(serial->baudRate()));
                ui->sendSerial_pushButton->setEnabled(true);
                connect(serial,SIGNAL(readyRead()),this,SLOT(serialRead()));
                connect(serial, SIGNAL(aboutToClose()), this, SLOT(serialError()));
            }
            else
            {
                ui->serialPort_label->setText("NO CONNECTION"); ui->serialPort_label->setStyleSheet("QLabel { color: red }");
                ui->baudrate_label->setText("No connection, no baudrate");
                ui->sendSerial_pushButton->setDisabled(true);
                ui->tab_2->setDisabled(true);
            }
        }

    }
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::serialRead()
{
    QByteArray data = serial->readAll();
    ui->serial_plainTextEdit->setPlainText(ui->serial_plainTextEdit->toPlainText()+data);
}

void MainWindow::serialError()
{
    ui->tab_2->setDisabled(true);
    QMessageBox::critical(this,QString("Serial ERROR"),serial->errorString());
}

void MainWindow::on_serialCleaer_toolButton_clicked()
{
    ui->serial_plainTextEdit->clear();
}

void MainWindow::on_sendSerial_pushButton_clicked()
{
    serial->write(ui->serialData_lineEdit->text().toLatin1());
    ui->serialData_lineEdit->clear();
}

void MainWindow::on_serialData_lineEdit_returnPressed()
{
    on_sendSerial_pushButton_clicked();
}

void MainWindow::on_disconnect_pushButton_clicked()
{
    if(serial->isOpen())
        serial->close();
    else
        QMessageBox::information(this,QString("TODO"),QString("list ports and choose"));
}

void MainWindow::on_resetX_pushButton_clicked()
{
    if(serial->isOpen())
    {
        serial->write("resetX()");
    }
    else
        QMessageBox::critical(this,QString("ERROR"),QString("resetting not possible. Is the device connected?"));
}

void MainWindow::on_resetY_pushButton_clicked()
{
    if(serial->isOpen())
    {
        serial->write("resetY()");
    }
    else
        QMessageBox::critical(this,QString("ERROR"),QString("resetting not possible. Is the device connected?"));
}

void MainWindow::on_resetXY_pushButton_clicked()
{
    if(serial->isOpen())
    {
        serial->write("reset()");
    }
    else
        QMessageBox::critical(this,QString("ERROR"),QString("resetting not possible. Is the device connected?"));
}

void MainWindow::on_enableX_checkBox_toggled(bool checked)
{
    if(checked)
    {
        serial->write("enableX()");
        ui->move_groupBox->setEnabled(true);
        ui->reset_groupBox->setEnabled(true);
        if(ui->enableY_checkBox->isChecked())
            ui->enabkeXY__pushButton->setText("disable both");
    }else{
        serial->write("disableX()");
        if(!ui->enableY_checkBox->isChecked()){
            ui->move_groupBox->setDisabled(true);
            ui->reset_groupBox->setDisabled(true);
            ui->enabkeXY__pushButton->setText("enable both");
        }
    }
}

void MainWindow::on_enableY_checkBox_toggled(bool checked)
{
    if(checked)
    {
        serial->write("enableY()");
        ui->move_groupBox->setEnabled(true);
        ui->reset_groupBox->setEnabled(true);
        if(ui->enableX_checkBox->isChecked())
            ui->enabkeXY__pushButton->setText("disable both");
    }else
     {
        serial->write("disableY()");
        if(!ui->enableX_checkBox->isChecked()){
            ui->move_groupBox->setDisabled(true);
            ui->reset_groupBox->setDisabled(true);
            ui->enabkeXY__pushButton->setText("enable both");
        }
     }
}

void MainWindow::on_enabkeXY__pushButton_clicked()
{
    if(ui->enableX_checkBox->isChecked() && ui->enableY_checkBox->isChecked())
    {
        //serial->write("disable()");
        ui->enableX_checkBox->setChecked(false);
        ui->enableY_checkBox->setChecked(false);
    }else if(!ui->enableX_checkBox->isChecked() && !ui->enableY_checkBox->isChecked())
    {
        //serial->write("enable()");
        ui->enableX_checkBox->setChecked(true);
        ui->enableY_checkBox->setChecked(true);
    }
}

void MainWindow::on_moveX_pushButton_clicked()
{
    if(ui->relative_radioButton->isChecked() && ui->moveSteps_radioButton->isChecked())
    {
        serial->write("moveX(" + QString(ui->move_lineEdit->text()).toLatin1() + ")");
    }
}

void MainWindow::on_moveY_pushButton_clicked()
{
    if(ui->relative_radioButton->isChecked() && ui->moveSteps_radioButton->isChecked())
    {
        serial->write("moveY(" + QString(ui->move_lineEdit->text()).toLatin1() + ")");
    }
}

void MainWindow::on_preview_pushButton_clicked()
{
  qreal lenght = ui->lenght_spinBox->value();
  qreal xVal = ui->xVal_spinBox->value() + 1;
  qreal yVal = yMaximum - (ui->yVal_spinBox->value() + 1);

  QPen pen1;
  pen1.setWidth(1 / sceneScale);

  //remember: switch + and - for yAxis bcs. y0 is on top

  if(ui->geoShape_comboBox->currentIndex() == 0) //line
  {
    if(ui->firstSetting_radioButton->isChecked()) //vertical
    {
      scene->addLine(xVal, yVal, xVal, yVal - lenght, pen1);
    }
    else  //horizontal
    {
      scene->addLine(xVal, yVal, xVal + lenght, yVal, pen1);
    }
  }

  if(ui->geoShape_comboBox->currentIndex() == 1) //diagonal
  {
    qreal diagonalLenght =  sqrt(lenght * lenght);
    if(ui->firstSetting_radioButton->isChecked()) //like f(x) = x
    {
      scene->addLine(xVal, yVal, xVal + diagonalLenght, yVal - diagonalLenght, pen1);
    }
    else //like f(x) = -x
    {
      scene->addLine(xVal, yVal, xVal + diagonalLenght, yVal + diagonalLenght, pen1);
    }

  }

  if(ui->geoShape_comboBox->currentIndex() == 2) //square
  {

    scene->addRect(xVal, yVal - lenght, lenght, lenght, pen1);
  }

  if(ui->geoShape_comboBox->currentIndex() == 3) //nicholas house
  {
    QPoint peakPoint;
    peakPoint.setX(xVal + 0.5 * lenght);
    peakPoint.setY(yVal);

    scene->addRect(xVal, yVal - 0.5 * lenght, lenght, lenght, pen1);
    scene->addLine(xVal, yVal - 0.5 * lenght, peakPoint.x(),  peakPoint.y(), pen1);         //roof 1
    scene->addLine(peakPoint.x(), peakPoint.y(), xVal + lenght, yVal - 0.5 * lenght, pen1); //roof 2
    scene->addLine(xVal, yVal - 0.5 * lenght, xVal + lenght, yVal - 1.5 * lenght, pen1);  //first diagonal
    scene->addLine(xVal, yVal - 1.5 * lenght, xVal + lenght, yVal - 0.5 * lenght, pen1);  //2nd diagonal
  }
}

void MainWindow::on_geoShape_comboBox_currentIndexChanged(int index)
{
  //ui->preview_pushButton->setText(QString::number(index));
    switch(index)
    {
      case 0:
        ui->lenght_spinBox->setMaximum(ui->yVal_spinBox->maximum());
        ui->firstSetting_radioButton->setVisible(true);
        ui->seconSetting_radioButton->setVisible(true);
        ui->firstSetting_radioButton->setText("vertical");
        ui->seconSetting_radioButton->setText("horizontal");
      break;

      case 1:
        /// TODO: add implement setMaximum correctly (based on pythagoras and x/y settings)
        ui->lenght_spinBox->setMaximum(ui->yVal_spinBox->maximum());
        ui->firstSetting_radioButton->setVisible(true);
        ui->seconSetting_radioButton->setVisible(true);
        ui->firstSetting_radioButton->setText("like f(x)=x");
        ui->seconSetting_radioButton->setText("like f(x)=-x");
      break;

      case 2:
        ui->lenght_spinBox->setMaximum(ui->xVal_spinBox->maximum());
        ui->firstSetting_radioButton->setVisible(false);
        ui->seconSetting_radioButton->setVisible(false);
      break;

      case 3:
        ui->lenght_spinBox->setMaximum(ui->xVal_spinBox->maximum());
        ui->firstSetting_radioButton->setVisible(false);
        ui->seconSetting_radioButton->setVisible(false);
      break;
    }
}
