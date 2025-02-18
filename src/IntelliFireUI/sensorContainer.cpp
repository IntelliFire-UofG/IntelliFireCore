#include "sensorContainer.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QDebug>

SensorContainer::SensorContainer(int containerNumber, QWidget *parent)
    : QWidget(parent), sensorNumber(containerNumber)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    
    // Load the corresponding icon
    QLabel *image = new QLabel;
    QString imagepath = QString(":/assets/icon%1.png").arg(containerNumber);
    QPixmap pixmap(imagepath); // Adjust path if needed
    
    if (pixmap.isNull()) {
        qDebug() << "Failed to load image:";
        // If image fails to load, use a blue square as a placeholder
        pixmap = QPixmap(64, 64);
        pixmap.fill(Qt::blue);
    }

    image->setPixmap(pixmap);
    image->setScaledContents(true); 
    image->setFixedSize(64, 64);
    image->setStyleSheet("border: 1px solid #cccccc;");
    
    QLabel *title;
    QLabel *value_label;
    
    if (containerNumber < 5)
    {
        title = new QLabel(QString("Flame sensor value %1").arg(containerNumber));
        value_label = new QLabel("25.5°C");  // Example value
    }
    else
    {
        title = new QLabel(QString("Distance sensor value %1").arg(containerNumber));
        value_label = new QLabel("0.0 cm");  // Example value
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

void SensorContainer::updateSensorValue(float sensor_value) {
    value_label->setText(QString::number(sensor_value, 'f', 2));
}
