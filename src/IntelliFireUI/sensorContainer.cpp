#include "sensorContainer.h"
#include "UltraSonicSensor.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QDebug>
#include <QTimer>

SensorContainer::SensorContainer(int containerNumber, QWidget *parent)
    : QWidget(parent), sensorNumber(containerNumber), sensor(new UltraSonicSensor(this))
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    
    // Load the corresponding icon
    QLabel *image = new QLabel;
    QString imagepath = QString(":/assets/icon%1.png").arg(containerNumber);
    QPixmap pixmap(imagepath); // Adjust path if needed
    
    if (pixmap.isNull()) {
        qDebug() << "Failed to load image:"<<containerNumber;
        // If image fails to load, use a blue square as a placeholder
        pixmap = QPixmap(64, 64);
        pixmap.fill(Qt::blue);
    }

    image->setPixmap(pixmap);
    image->setScaledContents(true); 
    image->setFixedSize(64, 64);
    image->setStyleSheet("border: 1px solid #cccccc;");
    
    QLabel *title = new QLabel;
    value_label = new QLabel("0");
    
    if (containerNumber < 5)
    {
        title -> setText(QString("Flames sensor value: %1").arg(containerNumber));
        value_label->setText(QString::number(25.5) + " °C");// Example value
        
    }
    else if (containerNumber == 5)
    {
        title -> setText("Ultrasonic sensor distance:");
        
        connect(sensor,&UltraSonicSensor::measuredDistance,this,&SensorContainer::updateUI);
        sensor->start("/dev/gpiochip0", 23, 24);
        /* code */
    }
    else
    {
        title -> setText("Ultrasonic sensor distance:");
        value_label ->setText(QString::number(0) + " cm");// Example value 
    }
    
    layout->addWidget(image, 0, Qt::AlignHCenter);
    layout->addWidget(title, 0, Qt::AlignHCenter);
    layout->addWidget(value_label, 0, Qt::AlignHCenter);
    
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
/*
void SensorContainer::updateSensorValue(float sensor_value) {
    value_label->setText(QString::number(sensor_value, 'f', 2));

}

//Ultrasonic sensor measurements
void SensorContainer::updateUltrasonicSensorValue(){
    int sensorValue = ultrasonicDistance();  
    
    //sensorValue = 100;//debugging to test value
    //return rand()% 200;//test for when no raspberry pi connected
    emit sensorValueUpdated(sensorValue);//return value from ultrasonic sensor
    }
**/
//update the UI with the new value from updateSensorValue()
void SensorContainer::updateUI(int newValue) {
    value_label->setText(QString::number(newValue) + " cm");
}
