#include "sensorContainer.h"
#include "ultrasonicDistance.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <cstdlib>
#include <QDebug>
#include <QTimer>

int SensorContainer::updateSensorValue(){
    int sensorValue = ultrasonicDistance();  
    
    // ✅ Validate the sensor valu
    
    
    //sensorValue = 100;//debugging to test value
    //return rand()% 200;//test for when no raspberry pi connected
    return sensorValue;//return value from ultrasonic sensor
    }

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
    
    
    // Labels
    if (containerNumber == 1){
    PLACEHOLDER = 0;
    QLabel *title = new QLabel(QString("Ultrasonic sensor distance:"));
    
    
    QLabel *value = new QLabel(QString::number(PLACEHOLDER));
    

    auto updateSensor = [this,value,layout](){
    PLACEHOLDER= this->updateSensorValue();
    
    //std::cout<<"Value:"<<PLACEHOLDER;//Debugging
    value->setText(QString::number(PLACEHOLDER)+" cm");
    
    
    };
    updateSensor();
    
    connect(timer, &QTimer::timeout, this,updateSensor);
    //std::cout<<"New Sensor value = "<<PLACEHOLDER<<std::endl;
    

    
    
    timer->start(1000);
    layout->addWidget(image, 0, Qt::AlignHCenter);
    layout->addWidget(title, 0, Qt::AlignHCenter);
    layout->addWidget(value, 0, Qt::AlignHCenter);

    }
    else if (containerNumber == 2){
    QLabel *title = new QLabel(QString("Flame sensor:"));
    QLabel *value = new QLabel("25.5°C");  // Example value
    layout->addWidget(image, 0, Qt::AlignHCenter);
    layout->addWidget(title, 0, Qt::AlignHCenter);
    layout->addWidget(value, 0, Qt::AlignHCenter);

    }
    else if (containerNumber == 3){
    QLabel *title = new QLabel(QString("Water pump level:"));
    QLabel *value = new QLabel("100");  // Example value
    layout->addWidget(image, 0, Qt::AlignHCenter);
    layout->addWidget(title, 0, Qt::AlignHCenter);
    layout->addWidget(value, 0, Qt::AlignHCenter);

    }
    else if (containerNumber == 4){
    QLabel *title = new QLabel(QString("Motor speed:"));
    QLabel *value = new QLabel("100000km/h");  // Example value
    layout->addWidget(image, 0, Qt::AlignHCenter);
    layout->addWidget(title, 0, Qt::AlignHCenter);
    layout->addWidget(value, 0, Qt::AlignHCenter);

    }


    
    else{
    QLabel *title = new QLabel(QString("Flame sensor value %1").arg(containerNumber));
    QLabel *value = new QLabel("25.5°C");  // Example value
    
    layout->addWidget(image, 0, Qt::AlignHCenter);
    layout->addWidget(title, 0, Qt::AlignHCenter);
    layout->addWidget(value, 0, Qt::AlignHCenter);


    }
  
    
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
