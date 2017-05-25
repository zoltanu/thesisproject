//Improvement needs
//Clearing the buffer after every stop - Implemented
//Option for only including forces in the records - Implemented
//Including set forces in the records - Implemented
//Step size should be set to 0,1 - Implemented


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "chai3d.h"
#include <QtSerialPort/QSerialPort> // itt is megtörténik az include nem csak a header fileban...
#include <QSerialPortInfo>
#include <QMutex>
#include <QTime>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QTimer>
#include <cmath>


using namespace chai3d;

bool flag=false;
bool flag2=false;
bool flag3=true;
QString MyHaptic="Not yet";
QString PositionX=" ";
QString PositionY=" ";
QString PositionZ=" ";

QByteArray sensordata;
QString sdata;

//Timestamp

QTime t;
QString elapsedtime;
QMutex mutex;
bool lineread=false;
bool firstset=false;

//Saving file
QStringList lines;
bool savenotmade=true;
QString FirstLine;
QString CountsOrNewtons;

//Setting COM port
QList<QSerialPortInfo> Ports;

//Displaing in Newtons and Newtonmilimeters
QString Display;
double fx,fy,fz,tx,ty,tz;

//Automatic force set
QTimer *timer;
QString SetForce;
int ForceStepCount;
double ForceStepMax;
double ActualSetForceX, ActualSetForceY,ActualSetForceZ;
int StepPrefix;


//Create a haptic device handler
       cHapticDeviceHandler* handler;
       // a pointer to the current haptic device
       cGenericHapticDevicePtr hapticDevice;

       cVector3d position;

       cHapticDeviceInfo info;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    serial = new QSerialPort(this); // creating the serialport within the main window function

    connect(serial, &QSerialPort::readyRead, this, &MainWindow::readData); //Everytime the QserialPort readyRead signal
    // is emitted the readData runs
      //  connect(serial, &MainWindow::WriteReady, this, &MainWindow::readData);

   // t.start();
    Ports =QSerialPortInfo::availablePorts();
    QStringList availablePorts;
     for(int i=0;i<Ports.size();i++)
        availablePorts.append(Ports.at(i).portName());
    ui->comboBox->addItems(availablePorts);

    //Set value max and min value for the SpinBoxes

    ui->doubleSpinBox->setMinimum(-1000);
    ui->doubleSpinBox->setMaximum(1000);
    ui->doubleSpinBox_2->setMinimum(-1000);
    ui->doubleSpinBox_2->setMaximum(1000);
    ui->doubleSpinBox_3->setMinimum(-1000);
    ui->doubleSpinBox_3->setMaximum(1000);    
    ui->spinBox_4->setMinimum(-1000);
    ui->spinBox_4->setMaximum(1000);
    ui->spinBox_5->setMinimum(-1000);
    ui->spinBox_5->setMaximum(1000);
    ui->spinBox_6->setMinimum(-1000);
    ui->spinBox_6->setMaximum(1000);
    ui->spinBox_7->setMinimum(-1000);
    ui->spinBox_7->setMaximum(1000);
    ui->spinBox_8->setMinimum(-1000);
    ui->spinBox_8->setMaximum(1000);
    ui->spinBox_9->setMinimum(-1000);
    ui->spinBox_9->setMaximum(1000);

    ui->doubleSpinBox->setSingleStep(0.1);
    ui->doubleSpinBox_2->setSingleStep(0.1);
    ui->doubleSpinBox_3->setSingleStep(0.1);
    ui->doubleSpinBox->setDecimals(1);
    ui->doubleSpinBox_2->setDecimals(1);
    ui->doubleSpinBox_3->setDecimals(1);

    ui->radioButton_2->setChecked(true);
    ui->radioButton_3->setChecked(true);

    //Automatic Force Measurement part

    ui->doubleSpinBox_5->setVisible(false);
    ui->doubleSpinBox_6->setVisible(false);
    ui->label_32->setVisible(false);
    ui->label_31->setVisible(false);

    ui->doubleSpinBox_4->setMaximum(50);
    ui->doubleSpinBox_4->setMinimum(-50);
    ui->doubleSpinBox_5->setMaximum(50);
    ui->doubleSpinBox_5->setMinimum(-50);
    ui->doubleSpinBox_6->setMaximum(50);
    ui->doubleSpinBox_6->setMinimum(-50);
    ui->doubleSpinBox_7->setMaximum(50);
    ui->doubleSpinBox_7->setMinimum(0.1);
    ui->doubleSpinBox_4->setSingleStep(0.1);
    ui->doubleSpinBox_5->setSingleStep(0.1);
    ui->doubleSpinBox_6->setSingleStep(0.1);
    ui->doubleSpinBox_7->setSingleStep(0.1);
    ui->doubleSpinBox_4->setDecimals(1);
    ui->doubleSpinBox_5->setDecimals(1);
    ui->doubleSpinBox_6->setDecimals(1);
    ui->doubleSpinBox_7->setDecimals(1);
    ui->doubleSpinBox_7->setValue(0.1);
    ui->spinBox->setMaximum(10);
    ui->spinBox->setMinimum(1);
    ui->spinBox->setValue(2);

    ui->doubleSpinBox_4->setValue(1);

    ui->doubleSpinBox_4->setEnabled(false);
    ui->doubleSpinBox_5->setEnabled(false);
    ui->doubleSpinBox_6->setEnabled(false);
    ui->doubleSpinBox_7->setEnabled(false);
    ui->spinBox->setEnabled(false);

    ui->checkBox->setEnabled(false);
    ui->checkBox_2->setEnabled(false);
    ui->checkBox_3->setEnabled(false);

    ui->radioButton_6->setChecked(true);

    connect(ui->radioButton_6, SIGNAL(toggled(bool)), this, SLOT(ManualOrAutomatic()));

    timer= new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(automatic()));

    //hiding console
    ui->plainTextEdit->setVisible(false);
    ui->label_29->setVisible(false);


 /*   handler = new cHapticDeviceHandler();
    handler->getDevice(hapticDevice, 0);
    hapticDevice->open();
    hapticDevice->calibrate();
    info = hapticDevice->getSpecifications();
   // MyHaptic = QString("Your Haptic Device is: %1\n").arg(QString::fromStdString(info.m_modelName));
    MyHaptic = QString::fromStdString(info.m_modelName);
    ui->textBrowser->setText(MyHaptic);
    ui->textBrowser->repaint();*/



    timer= new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(automatic()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ButtonSet()
{


/*
// create a haptic device handler
       cHapticDeviceHandler* handler;
       handler = new cHapticDeviceHandler();


       // a pointer to the current haptic device
       cGenericHapticDevicePtr hapticDevice;

       // get a handle to the first haptic device
        handler->getDevice(hapticDevice, 0);

        // open a connection to haptic device
        hapticDevice->open();

        // calibrate device (if necessary)
        hapticDevice->calibrate();

        // retrieve information about the current haptic device
        cHapticDeviceInfo info = hapticDevice->getSpecifications();

        MyHaptic = QString("My Haptic Device is: %1\n").arg(QString::fromStdString(info.m_modelName));
        ui->textBrowser->setText(MyHaptic);

        // read position
            cVector3d position;
            hapticDevice->getPosition(position);
        Position = QString("The vector from origin to manipulandum center point is: %1, %2, %3\n")
                .arg(position.x()).arg(position.y()).arg(position.z());
       // ui->textBrowser_2->setText(Position);

        */



        bool button0 = false;

        if (firstset==true)
        {
            t.start(); firstset=false;

            //Automatic
            ui->radioButton_6->setEnabled(false);
            ui->radioButton_7->setEnabled(false);
            if (ui->radioButton_7->isChecked()==true)
        {
            timer->start(ui->spinBox->value()*1000);
            ForceStepMax=ui->doubleSpinBox_4->value()/ui->doubleSpinBox_7->value();
            ForceStepCount=0;
            if(ui->doubleSpinBox_4->value()>0)
            {StepPrefix=1;}
            else if(ui->doubleSpinBox_4->value()<0)
            {StepPrefix=-1;}

            ui->label_36->setText("Processing");
            SetForce="0,\t0,\t0,\t";

        }
            else
            {

            }

        }

        if (flag2 == true)
           {
            hapticDevice->open();

            // Serial port part
            serial->open(QIODevice::ReadWrite);
          //  serial->write(QByteArray("SB\r"));
            serial->write(QByteArray("QS\r"));

            //Timestamp
            t.restart();
            flag2=false;
            Display.clear();
            lines.clear();

            //Automatic
            ui->radioButton_6->setEnabled(false);
            ui->radioButton_7->setEnabled(false);
            if (ui->radioButton_7->isChecked()==true)
        {
            timer->start(ui->spinBox->value()*1000);
            ForceStepMax=ui->doubleSpinBox_4->value()/ui->doubleSpinBox_7->value();
            ForceStepCount=0;
            if(ui->doubleSpinBox_4->value()>0)
            {StepPrefix=1;}
            else if(ui->doubleSpinBox_4->value()<0)
            {StepPrefix=-1;}
            ui->label_36->setText("Processing");
            SetForce="0,\t0,\t0,\t";
        }
         else
            {

            }

           }
        else
           {}

         if (flag == true)
         {
             flag3=false;


            // ui->spinBox->setEnabled(false);
            // ui->spinBox_2->setEnabled(false);
            // ui->spinBox_3->setEnabled(false);



        while(button0 == false && flag3==false){

               //hapticDevice->getUserSwitch(0, button0);

               hapticDevice->getPosition(position);

               //Automatic

               if(ui->radioButton_6->isChecked()==true)
               {

               cVector3d force = cVector3d(ui->doubleSpinBox->value(),ui->doubleSpinBox_2->value(),ui->doubleSpinBox_3->value());
               //force = -250*position;
               //cVector3d force = cVector3d(5,0,0);
             //  hapticDevice->setForce(force);
               cVector3d dummy(0,0,0);
               double dummydouble=0.0;
               hapticDevice->setForceAndTorqueAndGripperForce(force,dummy,dummydouble);
               SetForce=QString::number(ui->doubleSpinBox->value())+",\t"+QString::number(ui->doubleSpinBox_2->value())+",\t"+QString::number(ui->doubleSpinBox_3->value())+",\t";
                }
               else
               {
               }

               ui->textBrowser->setText(MyHaptic);
               ui->textBrowser->repaint();
               PositionX = QString::number(position.x());
               PositionY = QString::number(position.y());
               PositionZ = QString::number(position.z());

              // PositionX = QString("The vector from origin to manipulandum center point is: %1, %2, %3\n")
                 //      .arg(position.x()).arg(position.y()).arg(position.z());
               ui->textBrowser_2->setText(PositionX);
               ui->textBrowser_2->repaint();
               ui->textBrowser_3->setText(PositionY);
               ui->textBrowser_3->repaint();
               ui->textBrowser_4->setText(PositionZ);
               ui->textBrowser_4->repaint();

               //Serial port part
             //  QString sdata = QString(sensordata);

elapsedtime=QString::number(t.elapsed()/(double)1000);
mutex.lock();
 if (lineread==true)
 {
     //mutex.lock();
     sdata = QString(sensordata);
     //mutex.unlock();
     //QString meanwhile = sdata;

     //if (sdata.contains(",")==true)
     QList<QString> CoolList;
     CoolList=sdata.split(",");

     int size =CoolList.size();

     if (size==7)
     {

     if (ui->radioButton_2->isChecked())
     {
     fx=(sdata.split(",")[1].toDouble())/1024;
     fy=(sdata.split(",")[2].toDouble())/1024;
     fz=(sdata.split(",")[3].toDouble())/1024;
     tx=(sdata.split(",")[4].toDouble())/80;
     ty=(sdata.split(",")[5].toDouble())/80;
     tz=(sdata.split(",")[6].toDouble())/80;

     CountsOrNewtons="N/Nmm";

     }
     else
     {
         fx=(sdata.split(",")[1].toDouble());
         fy=(sdata.split(",")[2].toDouble());
         fz=(sdata.split(",")[3].toDouble());
         tx=(sdata.split(",")[4].toDouble());
         ty=(sdata.split(",")[5].toDouble());
         tz=(sdata.split(",")[6].toDouble());

         CountsOrNewtons="Counts/Counts";
     }

     if (ui->radioButton_5->isChecked()==true)
    {
     Display.append(QString::number(fx)+",\t");
     Display.append(QString::number(fy)+",\t");
     Display.append(QString::number(fz)+",\t");
     Display.append(QString::number(tx)+",\t");
     Display.append(QString::number(ty)+",\t");
     Display.append(QString::number(tz));

     FirstLine="The following data is:\nElapsed time, Position X, Position Y, Position Z, Set Force X, Set Force Y, Set Force Z, Output Force X, Output Force Y, Output Force Z, Torque Roll, Torque Pitch, Torque Yaw";
    }
     else if (ui->radioButton_3->isChecked()==true)
     {
         Display.append(QString::number(fx)+",\t");
         Display.append(QString::number(fy)+",\t");
         Display.append(QString::number(fz));

         FirstLine="The following data is:\nElapsed time, Position X, Position Y, Position Z, Set Force X, Set Force Y, Set Force Z, Output Force X, Output Force Y, Output Force Z";

     }

     else if (ui->radioButton_4->isChecked()==true)
     {
         Display.append(QString::number(tx)+",\t");
         Display.append(QString::number(ty)+",\t");
         Display.append(QString::number(tz));


         FirstLine="The following data is:\nElapsed time, Position X, Position Y, Position Z, Set Force X, Set Force Y, Set Force Z, Torque Roll, Torque Pitch, Torque Yaw";
     }
     }


     else
     {
         Display="";
     }
//lines.append(elapsedtime+","+"\t"+PositionX+"\t"+PositionY+"\t"+PositionZ+"\t"+QString::number(ui->doubleSpinBox->value())+",\t"+QString::number(ui->doubleSpinBox_2->value())+",\t"+QString::number(ui->doubleSpinBox_3->value())+",\t"+Display+"\r");

lines.append(elapsedtime+","+"\t"+PositionX+"\t"+PositionY+"\t"+PositionZ+"\t"+SetForce+Display+"\r");


//ui->plainTextEdit->insertPlainText(elapsedtime+",\t"+sdata);
ui->plainTextEdit->insertPlainText(elapsedtime+",\t"+Display+"\n");
QTextCursor c = ui->plainTextEdit->textCursor();
c.movePosition(QTextCursor::End);
ui->plainTextEdit->setTextCursor(c);

ui->textBrowser_5->setText(QString::number(fx));
ui->textBrowser_6->setText(QString::number(fy));
ui->textBrowser_7->setText(QString::number(fz));
ui->textBrowser_8->setText(QString::number(tx));
ui->textBrowser_9->setText(QString::number(ty));
ui->textBrowser_10->setText(QString::number(tz));
ui->textBrowser_11->setText(elapsedtime);
Display.clear();
lineread=false;

 }
 else
 {}
mutex.unlock();
               emit WriteReady();
               qApp->processEvents();

                                 }

          hapticDevice->close();
          //ui->plainTextEdit->clear();

          //Serial port part
          serial->close();


          //Enable the setting of the forces again
         // ui->spinBox->setEnabled(true);
         // ui->spinBox_2->setEnabled(true);
         // ui->spinBox_3->setEnabled(true);
          flag2=true;
          sdata=" ";

         }
         else
         {}
}

void MainWindow::RealSet()
{
           handler = new cHapticDeviceHandler();
           handler->getDevice(hapticDevice, 0);
           hapticDevice->open();
           hapticDevice->calibrate();
           info = hapticDevice->getSpecifications();
          // MyHaptic = QString("Your Haptic Device is: %1\n").arg(QString::fromStdString(info.m_modelName));
           MyHaptic = QString::fromStdString(info.m_modelName);
           ui->textBrowser->setText(MyHaptic);
           ui->textBrowser->repaint();

           hapticDevice->getPosition(position);
        //   Position = QString("The vector from origin to manipulandum center point is:\n %1, %2, %3\n")
          //     .arg(position.x()).arg(position.y()).arg(position.z());

           PositionX = QString::number(position.x());
           PositionY = QString::number(position.y());
           PositionZ = QString::number(position.z());

            ui->textBrowser_2->setText(PositionX);
            ui->textBrowser_2->repaint();
            ui->textBrowser_3->setText(PositionY);
            ui->textBrowser_3->repaint();
            ui->textBrowser_4->setText(PositionZ);
            ui->textBrowser_4->repaint();
            flag=true;
            firstset=true;
            ui->pushButton_2->setEnabled(false);

       /*     ui->spinBox->setMinimum(-1000);
            ui->spinBox->setMaximum(1000);
            ui->spinBox_2->setMinimum(-1000);
            ui->spinBox_2->setMaximum(1000);
            ui->spinBox_3->setMinimum(-1000);
            ui->spinBox_3->setMaximum(1000);
            ui->spinBox_4->setMinimum(-1000);
            ui->spinBox_4->setMaximum(1000);
            ui->spinBox_5->setMinimum(-1000);
            ui->spinBox_5->setMaximum(1000);
            ui->spinBox_6->setMinimum(-1000);
            ui->spinBox_6->setMaximum(1000);
            ui->spinBox_7->setMinimum(-1000);
            ui->spinBox_7->setMaximum(1000);
            ui->spinBox_8->setMinimum(-1000);
            ui->spinBox_8->setMaximum(1000);
            ui->spinBox_9->setMinimum(-1000);
            ui->spinBox_9->setMaximum(1000);

*/


            //Setting serialport values

            //serial->setPortName("COM2");
            serial->setPortName(ui->comboBox->currentText());
            serial->setBaudRate(QSerialPort::Baud9600);
            serial->setDataBits(QSerialPort::Data8);
            serial->setParity(QSerialPort::NoParity);
            serial->setStopBits(QSerialPort::OneStop);
           // serial->setFlowControl(QSerialPort::SoftwareControl);
           serial->setFlowControl(QSerialPort::NoFlowControl);

           int XValue= (ui->spinBox_4->value())/0.128;
           int YValue= (ui->spinBox_5->value())/0.128;
           int ZValue= (ui->spinBox_6->value())/0.128;


           int RollValue= (ui->spinBox_7->value())*10;
           int PitchValue= (ui->spinBox_8->value())*10;
           int YawValue= (ui->spinBox_9->value())*10;

           QString ToolFrame = QString("TC 2, HAPTIC, %1, %2, %3, %4, %5, %6\r").arg(XValue).arg(YValue).arg(ZValue).arg(RollValue).arg(PitchValue).arg(YawValue);

           //First start of serialport
            serial->open(QIODevice::ReadWrite);

            serial->write(QByteArray("TF 0\r"));
          //  serial->write(QByteArray(ToolFrame.toLatin1()));
            //serial->write(ToolFrame.toLatin1());
            //serial->write("TC 2, HAPTIC, 0, 0, 0, 0, 0, 0\r");
           // serial->write(QByteArray("TF 2\r"));

          //  serial->write(QByteArray("SB\r"));
            //serial->write(QByteArray("CV 7\r"));
            serial->write(QByteArray("CV 3F\r"));
            serial->write(QByteArray("SB\r"));
            serial->write(QByteArray("QS\r"));

            ui->pushButton_5->setEnabled(false);
            ui->comboBox->setEnabled(false);
            ui->spinBox_4->setEnabled(false);
            ui->spinBox_5->setEnabled(false);
            ui->spinBox_6->setEnabled(false);
            ui->spinBox_7->setEnabled(false);
            ui->spinBox_8->setEnabled(false);
            ui->spinBox_9->setEnabled(false);
            ui->radioButton_3->setEnabled(false);
            ui->radioButton_4->setEnabled(false);
            ui->radioButton_5->setEnabled(false);

            emit WriteReady();


}

void MainWindow::ButtonStop()
{
    if (flag3==false)
   { flag3=true;
     savenotmade=true;

     ui->radioButton_6->setEnabled(true);
     ui->radioButton_7->setEnabled(true);
     timer->stop();
   }
}

void MainWindow::readData()
{
    QByteArray data;
  // QByteArray data = serial->readAll();
  while (serial->canReadLine())
  {

      data = serial->readLine();
      mutex.lock();
      lineread=true;
      sensordata = data;
      mutex.unlock();

  }
  // sensordata = data;

}

void MainWindow::save()
{
    if (flag3==true  && flag2==true && savenotmade==true)
    {
    //lines =ui->plainTextEdit->toPlainText().split('\r');
  /*  QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                               "/home/jana/untitled.txt",
                               tr("Text files (*.txt)"));*/

        QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                       "C:/Users/haptik/Documents/A folder for records/untitled.txt",
                                       tr("Text files (*.txt)"));



    QFile file( fileName );
    if ( file.open(QIODevice::ReadWrite | QIODevice::Text) )
    {
        QTextStream stream( &file );

        stream << FirstLine << "\n" << "Time is in ms, Position is in m, Set Force is in Newton, Output Force/Torque is in " << CountsOrNewtons << "\n\n";

        for ( QStringList::Iterator it = lines.begin(); it != lines.end(); ++it )
                       { stream << *it << "\n";}
    }
    file.close();
    ui->plainTextEdit->clear();
    lines.clear();
    savenotmade=false;
    }
    else
    {
    }
}

void MainWindow::refresh()
{
    Ports =QSerialPortInfo::availablePorts();
    QStringList availablePorts;
     for(int i=0;i<Ports.size();i++)
        availablePorts.append(Ports.at(i).portName());
     ui->comboBox->clear();
    ui->comboBox->addItems(availablePorts);


  /*  handler = new cHapticDeviceHandler();
    handler->getDevice(hapticDevice, 0);
    hapticDevice->open();
    hapticDevice->calibrate();
    info = hapticDevice->getSpecifications();
   // MyHaptic = QString("Your Haptic Device is: %1\n").arg(QString::fromStdString(info.m_modelName));
    MyHaptic = QString::fromStdString(info.m_modelName);
    ui->textBrowser->setText(MyHaptic);
    ui->textBrowser->repaint();*/


}

void MainWindow::automatic()
{

    if (ui->checkBox->isChecked()==true)
    {
        ActualSetForceX+=(ui->doubleSpinBox_7->value()*StepPrefix);
    }
    else if (ui->checkBox->isChecked()==false)
    {ActualSetForceX=0;}
    if (ui->checkBox_2->isChecked()==true)
    {
        ActualSetForceY+=(ui->doubleSpinBox_7->value()*StepPrefix);
    }
    else if (ui->checkBox_2->isChecked()==false)
    {ActualSetForceY=0;}
    if (ui->checkBox_3->isChecked()==true)
    {
        ActualSetForceZ+=(ui->doubleSpinBox_7->value()*StepPrefix);
    }
    else if (ui->checkBox_3->isChecked()==false)
    {ActualSetForceZ=0;}

    cVector3d force = cVector3d(ActualSetForceX,ActualSetForceY,ActualSetForceZ);
    //force = -250*position;
    //cVector3d force = cVector3d(5,0,0);
  //  hapticDevice->setForce(force);
    cVector3d dummy(0,0,0);
    double dummydouble=0.0;
    hapticDevice->setForceAndTorqueAndGripperForce(force,dummy,dummydouble);
    SetForce=QString::number(ActualSetForceX)+",\t"+QString::number(ActualSetForceY)+",\t"+QString::number(ActualSetForceZ)+",\t";



    ForceStepCount=ForceStepCount+StepPrefix;

    if (abs(ForceStepCount)>=abs((int)ForceStepMax))
    {
        StepPrefix=-StepPrefix;
    }
    else {}

    if (ForceStepCount==0)
    {
        timer->stop();
        ui->label_36->setText("Finished");
        flag3=true;
        savenotmade=true;
        ui->radioButton_6->setEnabled(true);
        ui->radioButton_7->setEnabled(true);
    }
}

void MainWindow::ManualOrAutomatic()
{
    if (ui->radioButton_6->isChecked()==true)
    {

        ui->doubleSpinBox->setEnabled(true);
        ui->doubleSpinBox_2->setEnabled(true);
        ui->doubleSpinBox_3->setEnabled(true);

        ui->doubleSpinBox_4->setEnabled(false);
        ui->doubleSpinBox_5->setEnabled(false);
        ui->doubleSpinBox_6->setEnabled(false);
        ui->doubleSpinBox_7->setEnabled(false);
        ui->spinBox->setEnabled(false);
        ui->checkBox->setEnabled(false);
        ui->checkBox_2->setEnabled(false);
        ui->checkBox_3->setEnabled(false);
    }

    else if (ui->radioButton_7->isChecked()==true)
    {
        ui->doubleSpinBox->setEnabled(false);
        ui->doubleSpinBox_2->setEnabled(false);
        ui->doubleSpinBox_3->setEnabled(false);

        ui->doubleSpinBox_4->setEnabled(true);
        ui->doubleSpinBox_5->setEnabled(true);
        ui->doubleSpinBox_6->setEnabled(true);
        ui->doubleSpinBox_7->setEnabled(true);
        ui->spinBox->setEnabled(true);
        ui->checkBox->setEnabled(true);
        ui->checkBox_2->setEnabled(true);
        ui->checkBox_3->setEnabled(true);

    }
}
