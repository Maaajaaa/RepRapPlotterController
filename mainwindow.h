#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QtSerialPort>
#include <QMessageBox>
#include <QGraphicsItem>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_serialCleaer_toolButton_clicked();
    void on_sendSerial_pushButton_clicked();

    void serialRead();

    void serialError();

    void on_serialData_lineEdit_returnPressed();


    void on_disconnect_pushButton_clicked();

    void on_resetX_pushButton_clicked();

    void on_resetY_pushButton_clicked();

    void on_resetXY_pushButton_clicked();

    void on_enableY_checkBox_toggled(bool checked);

    void on_enableX_checkBox_toggled(bool checked);

    void on_enabkeXY__pushButton_clicked();

    void on_moveX_pushButton_clicked();

    void on_moveY_pushButton_clicked();

    void on_preview_pushButton_clicked();

    void on_geoShape_comboBox_currentIndexChanged(int index);

private:
    QGraphicsScene *scene;
    qreal sceneScale, xMaximum, yMaximum;
    Ui::MainWindow *ui;
    QSerialPort *serial;
};

#endif // MAINWINDOW_H
