#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>   //Serial port include

QT_BEGIN_NAMESPACE  //possible solution to unresolved external simbols
// The solution was to run qmake

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QSerialPort *serial; // itt már létrehozunk egy serialportra mutató pointert

private slots:
    void ButtonSet();
    void RealSet();
    void ButtonStop();
    void readData(); // Read serialport
    void save();
    void refresh();
    void automatic();
    void ManualOrAutomatic();

signals:
    void WriteReady();
};

#endif // MAINWINDOW_H
