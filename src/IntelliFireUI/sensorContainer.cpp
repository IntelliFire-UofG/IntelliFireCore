#include "sensorContainer.h"
#include "ultrasonicDistance.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <cstdlib>
#include <QDebug>
#include <QTimer>

SensorContainer::SensorContainer(int containerNumber, QWidget *parent)
    : QWidget(parent), timer(new QTimer(this))
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    
    // Load the corresponding icon
    QLabel *image = new QLabel;
    QString imagepath = QString(":/assets/icon%1.png").arg(containerNumber);// Adjust path if needed
    QPixmap pixmap(imagepath);
    
    if (pixmap.isNull()) {
        qDebug() << "Failed to load image";
        // If image fails to load, use a blue square as a placeholder
        pixmap = QPixmap(64, 64);
        pixmap.fill(Qt::blue);
    }
    

    image->setPixmap(pixmap);
    image->setScaledContents(true); 
    image->setFixedSize(64, 64);
    image->setStyleSheet("border: 1px solid #cccccc;");


    QLabel *title = new QLabel;
    value = new QLabel("0");
    
    // Labels
    if (containerNumber == 1){
    
    title -> setText("Ultrasonic sensor distance:");
    
    
    
    

    connect(timer, &QTimer::timeout, this,&SensorContainer::updateSensorValue);
    connect(this,&SensorContainer::sensorValueUpdated,this,&SensorContainer::updateUI);
    

    
    
    timer->start(200);


    }
    else if (containerNumber == 2){
    PLACEHOLDER = 4;
    title -> setText("Flames sensor:");
    value->setText(QString::number(4) + " Fire");


    }
    else if (containerNumber == 3){
    title -> setText("Water pump level:");
    value->setText(QString::number(1) + " L"); // Example value


    }
    else if (containerNumber == 4){
    title -> setText("Motor speed:");
    
    value->setText(QString::number(100000) + " km/h");// Example value


    }


    
    else{
    QLabel *title = new QLabel(QString("Flame sensor value %1").arg(containerNumber));
    QLabel *value = new QLabel("25.5°C");  // Example value
    
   


    }
    layout->addWidget(image, 0, Qt::AlignHCenter);
    layout->addWidget(title, 0, Qt::AlignHCenter);
    layout->addWidget(value, 0, Qt::AlignHCenter);
  
    
    setStyleSheet(R"(
        QWidget {
            background: white;
            border-radius: 5px;
            border: 1px solid #cccccc;
            padding: 10px;
        }
        QLabel {
            font-family: Arial;
            color: #333333;
        }
    )");
}
void SensorContainer::updateSensorValue(){
    int sensorValue = ultrasonicDistance();  
    
    //sensorValue = 100;//debugging to test value
    //return rand()% 200;//test for when no raspberry pi connected
    emit sensorValueUpdated(sensorValue);//return value from ultrasonic sensor
    }

//update the UI with the new value from updateSensorValue()
void SensorContainer::updateUI(int newValue) {
    value->setText(QString::number(newValue) + " cm");
}
