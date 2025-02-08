#include "mainwindow.h"
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QDebug>
#include "sensorContainer.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Welcome to Intellifire UI");
    resize(800, 600);

    QWidget *centralWidget = new QWidget;
    setCentralWidget(centralWidget);

    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

    // Left side - Sensor grid
    QGridLayout *sensorGrid = new QGridLayout;
    
    sensorGrid->addWidget(new SensorContainer(1), 0, 0);
    sensorGrid->addWidget(new SensorContainer(2), 0, 1);
    sensorGrid->addWidget(new SensorContainer(3), 1, 0);
    sensorGrid->addWidget(new SensorContainer(4), 1, 1);

    // Right side - Sliders
    QVBoxLayout *sliderLayout = new QVBoxLayout;

    // Subtitle
    QLabel *subtitle = new QLabel("Device: IntelliFire Pro v2.0");
    subtitle->setStyleSheet(R"(
        QLabel {
            font-family: Arial;
            font-size: 14px;
            color: #666666;
            padding: 10px;
        }
    )");

    // Combine everything
    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addWidget(subtitle);
    leftLayout->addLayout(sensorGrid);

    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(sliderLayout);

    // ✅ Now call `createSliders()` after setting up `mainLayout`
    createSliders();
}

void MainWindow::createSliders()
{
    if (!centralWidget() || !centralWidget()->layout()) {
        qDebug() << "Error: centralWidget() or layout() is NULL!";
        return;
    }

    QVBoxLayout *sliderLayout = qobject_cast<QVBoxLayout*>(
        centralWidget()->layout()->itemAt(1)->layout()
    );

    if (!sliderLayout) {
        qDebug() << "Error: sliderLayout is NULL!";
        return;
    };
    
    // Speed controls
    speedSlider = new QSlider(Qt::Vertical);
    QPushButton *speedButton = new QPushButton("Set Speed");
    connect(speedButton, &QPushButton::clicked, this, &MainWindow::handleSpeedButton);
    
    // Parameter controls
    paramSlider = new QSlider(Qt::Vertical);
    QPushButton *paramButton = new QPushButton("Set Parameter");
    connect(paramButton, &QPushButton::clicked, this, &MainWindow::handleParamButton);
    
    sliderLayout->addWidget(speedSlider);
    sliderLayout->addWidget(speedButton);
    sliderLayout->addSpacing(20);
    sliderLayout->addWidget(paramSlider);
    sliderLayout->addWidget(paramButton);
    
    // Styling
    QString sliderStyle = R"(
        QSlider::groove:vertical {
            background: #e0e0e0;
            width: 6px;
            border-radius: 3px;
        }
        QSlider::handle:vertical {
            background: #0078d4;
            height: 16px;
            margin: 0 -8px;
            border-radius: 8px;
        }
    )";
    
    speedSlider->setStyleSheet(sliderStyle);
    paramSlider->setStyleSheet(sliderStyle);
}

void MainWindow::handleSpeedButton()
{
    // Implementation for speed setting
}

void MainWindow::handleParamButton()
{
    // Implementation for parameter setting
}