#include "sensorContainer.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QDebug>
#include <QTimer>

SensorContainer::SensorContainer(int containerNumber, QWidget *parent)
    : QWidget(parent), sensorNumber(containerNumber) {

    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *image = new QLabel;
    QString imagepath = QString(":/assets/icon%1.png").arg(containerNumber);
    QPixmap pixmap(imagepath);

    if (pixmap.isNull()) {
        qDebug() << "SensorContainer: Failed to load image for sensor" << containerNumber;
        pixmap = QPixmap(64, 64);
        pixmap.fill(Qt::blue);
    }

    image->setPixmap(pixmap);
    image->setScaledContents(true);
    image->setFixedSize(64, 64);
    image->setStyleSheet("border: 1px solid #cccccc;");

    title = new QLabel;
    value_label = new QLabel("0");

    switch (containerNumber) {
    case 5: // Ultrasonic
        title->setText("Ultrasonic Sensor Distance:");
        ultrasonicSensor = std::make_unique<UltraSonicSensor>(this);
        connect(ultrasonicSensor.get(), &UltraSonicSensor::measuredDistance,
                this, &SensorContainer::updateUltrasonicUI);
        try {
            ultrasonicSensor->start("/dev/gpiochip0", 23, 24);
        } catch (...) {
            qWarning() << "Ultrasonic sensor failed to start.";
        }
        break;

    case 6: // IR
        title->setText("IR Sensor Status:");
        irSensor = std::make_unique<IRSensor>();
        irSensor->registerCallback(this);
        try {
            irSensor->start("/dev/gpiochip0", 12);
        } catch (...) {
            qWarning() << "IR sensor failed to start.";
        }
        break;

    default: // Flame sensor
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
}

SensorContainer::~SensorContainer() {
    if (irSensor) {
        irSensor->stop();
    }
    if (ultrasonicSensor) {
        ultrasonicSensor->stop();
    }
}

// IR sensor callback
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

void SensorContainer::updateIRUI(const QString& message) {
    value_label->setText(message);
}

void SensorContainer::updateUltrasonicUI(int newValue) {
    value_label->setText(QString::number(newValue) + " cm");
}

void SensorContainer::sensorValueUpdated(int id, float value) {
    if ((id + 1) == sensorNumber) {
        value_label->setText(QString::number(value, 'f', 3));
    }
}
