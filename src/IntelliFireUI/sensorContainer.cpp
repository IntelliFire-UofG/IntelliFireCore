#include "sensorContainer.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>

SensorContainer::SensorContainer(int containerNumber, QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    
    // Image placeholder (blue square)
    QLabel *image = new QLabel;
    QPixmap pixmap(64, 64);
    pixmap.fill(Qt::blue);
    image->setPixmap(pixmap);
    image->setFixedSize(64, 64);
    image->setStyleSheet("border: 1px solid #cccccc;");
    
    // Labels
    QLabel *title = new QLabel(QString("Flame sensor value %1").arg(containerNumber));
    QLabel *value = new QLabel("25.5°C");  // Example value
    
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