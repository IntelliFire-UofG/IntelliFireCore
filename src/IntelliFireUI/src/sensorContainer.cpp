#include "sensorContainer.h"
#include "UltraSonicSensor.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QDebug>
#include <QTimer>
#include <exception>
#include <mutex>

#define ULTRASONIC_ECHO 24 
#define ULTRASONIC_TRIGGER 25
#define IR_PRESENCE 16

SensorContainer::SensorContainer(int containerNumber, QWidget *parent)
    : QWidget(parent), sensorNumber(containerNumber), ultrasonicSensor(new UltraSonicSensor(this))
{
    try {
        QVBoxLayout *layout = new QVBoxLayout(this);
        
        QLabel *image = new QLabel;
        QString imagepath = QString(":/assets/icon%1.png").arg(containerNumber);
        QPixmap pixmap(imagepath);

        if (pixmap.isNull()) {
            qWarning() << "Failed to load image for sensor" << containerNumber << "using placeholder.";
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
                if (!ultrasonicSensor) {
                    qCritical() << "UltrasonicSensor initialization failed.";
                    break;
                }
                connect(ultrasonicSensor.get(), &UltraSonicSensor::measuredDistance, this, &SensorContainer::updateUltrasonicUI);
                try {
                    ultrasonicSensor->start("/dev/gpiochip0", ULTRASONIC_TRIGGER, ULTRASONIC_ECHO);
                } catch (const std::exception& ex) {
                    qCritical() << "Failed to start UltrasonicSensor:" << ex.what();
                }
                break;

            case 6:  // **IR Sensor**
                title->setText("IR Sensor Status:");
                try {
                    irSensor = std::make_unique<IRSensor>();
                    irSensor->registerCallback(this);
                    irSensor->start("/dev/gpiochip0", IR_PRESENCE);
                    connect(this, &SensorContainer::irMessageReceived, this, &SensorContainer::updateIRUI);

                } catch (const std::exception& ex) {
                    qCritical() << "Failed to initialize or start IRSensor:" << ex.what();
                }
                break;

            default:  // **Flame Sensor (or other)**
                title->setText(QString("Flame Sensor Value: %1").arg(containerNumber));
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

    } catch (const std::exception& ex) {
        qCritical() << "Unhandled exception in SensorContainer constructor:" << ex.what();
    } catch (...) {
        qCritical() << "Unknown exception in SensorContainer constructor.";
    }
}

SensorContainer::~SensorContainer() {
    try {
        if (irSensor) {
            irSensor->stop();
            irSensor.reset();
        }
        if (ultrasonicSensor) {
            ultrasonicSensor.reset();
        }
    } catch (const std::exception& ex) {
        qWarning() << "Exception during SensorContainer destruction:" << ex.what();
    }
}

// **Callback for IR sensor**
void SensorContainer::hasEvent(gpiod_line_event& e) {
    try {
        QString message;
        if (e.event_type == GPIOD_LINE_EVENT_FALLING_EDGE) {
            message = "Obstacle Detected!";
        } else if (e.event_type == GPIOD_LINE_EVENT_RISING_EDGE) {
            message = "Obstacle Removed!";
        } else {
            message = "Unknown Event!";
            qWarning() << "IR sensor event type unknown:" << e.event_type;
        }
        emit irMessageReceived(message);  // 🔥 Thread-safe!
    } catch (const std::exception& ex) {
        qWarning() << "Exception in hasEvent:" << ex.what();
    }
}

// **Update UI for IR sensor**
void SensorContainer::updateIRUI(const QString& message) {
    if (!value_label) {
        qWarning() << "value_label is null in updateIRUI.";
        return;
    }
    value_label->setText(message);
}

// **Update UI for Ultrasonic sensor**
void SensorContainer::updateUltrasonicUI(int newValue) {
    if (!value_label) {
        qWarning() << "value_label is null in updateUltrasonicUI.";
        return;
    }
    value_label->setText(QString::number(newValue) + " cm");
}

void SensorContainer::sensorValueUpdated(int id, float value) {
    if ((id + 1) == sensorNumber) {
        if (!value_label) {
            qWarning() << "value_label is null in sensorValueUpdated.";
            return;
        }
        value_label->setText(QString::number(value, 'f', 3));
    }
}
