#include "sensorContainer.h"
#include "UltraSonicSensor.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QDebug>
#include <QTimer>

SensorContainer::SensorContainer(int containerNumber, QWidget *parent)
    : QWidget(parent), sensorNumber(containerNumber), ultrasonicSensor(new UltraSonicSensor(this))
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    
    // Load the corresponding icon from the new assets location.
    QLabel *image = new QLabel;
    QString imagepath = QString(":/assets/icon%1.png").arg(containerNumber);
    QPixmap pixmap(imagepath); // Adjust path if needed
    
    if (pixmap.isNull()) {
        qDebug() << "Failed to load image:" << containerNumber;
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

    switch (containerNumber) {
        case 5:  // **Ultrasonic Sensor**
            title->setText("Ultrasonic Sensor Distance:");
            ultrasonicSensor = new UltraSonicSensor(this);
            connect(ultrasonicSensor, &UltraSonicSensor::measuredDistance, this, &SensorContainer::updateUltrasonicUI);
            ultrasonicSensor->start("/dev/gpiochip0", 25, 24);
            break;

        case 6:  // **IR Sensor**
            title->setText("IR Sensor Status:");
            irSensor = new IRSensor();
            irSensor->registerCallback(this);
            irSensor->start("/dev/gpiochip0", 16); // Adjust GPIO pin as needed
            break;

        default:  // **Flame Sensor (or other)**
            title->setText(QString("Flame Sensor Value: %1").arg(containerNumber));
            //value_label->setText("25.5 °C"); // Placeholder value
            break;
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

SensorContainer::~SensorContainer() {
    if (irSensor) {
        irSensor->stop();
        delete irSensor;
    }
    if (ultrasonicSensor) {
        delete ultrasonicSensor;
    }
}

// **Callback for IR sensor**
void SensorContainer::hasEvent(gpiod_line_event& e) {
    QString message;
    if (e.event_type == GPIOD_LINE_EVENT_FALLING_EDGE) {
        message = "Obstacle Detected!";
    } else if (e.event_type == GPIOD_LINE_EVENT_RISING_EDGE) {
        message = "Obstacle Removed!";
    } else {
        message = "Unknown Event!";
    }
    updateIRUI(message);
}

// **Update UI for IR sensor**
void SensorContainer::updateIRUI(const QString& message) {
    value_label->setText(message);
}

// **Update UI for Ultrasonic sensor**
void SensorContainer::updateUltrasonicUI(int newValue) {
    value_label->setText(QString::number(newValue) + " cm");
}

void SensorContainer::sensorValueUpdated(int id, float value)
{
    if ((id + 1) == sensorNumber)
    {
        value_label->setText(QString::number(value, 'f', 3));
    }
}