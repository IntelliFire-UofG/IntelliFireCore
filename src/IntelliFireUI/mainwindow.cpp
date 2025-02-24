#include "mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("Welcome to IntelliFire UI");
    resize(1000, 800);


    QWidget *centralWidget = new QWidget;
    setCentralWidget(centralWidget);

    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

    // Left side - Sensor grid
    QGridLayout *sensorGrid = new QGridLayout;

    // Stored pointers for each sensor container
    SensorContainer *container_1 = new SensorContainer(1);
    SensorContainer *container_2 = new SensorContainer(2);
    SensorContainer *container_3 = new SensorContainer(3);
    SensorContainer *container_4 = new SensorContainer(4);
    //SensorContainer *container_5 = new SensorContainer(5);
    //SensorContainer *container_6 = new SensorContainer(6);
    
    // Sensor containers mapped into layout
    sensorGrid->addWidget(container_1, 0, 0); // Flame Sensor 1
    sensorGrid->addWidget(container_2, 0, 1); // Flame Sensor 2
    sensorGrid->addWidget(container_3, 1, 0); // Flame Sensor 3
    sensorGrid->addWidget(container_4, 1, 1); // Flame Sensor 4
    //sensorGrid->addWidget(container_5, 2, 0); // Ultrasonic Sensor
    //sensorGrid->addWidget(container_6, 2, 1); // IR Sensor

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

    // Key Logger
    keyLogger = new KeyLogger;
    keyDisplayLabel = new QLabel("Key Pressed: None");
    keyDisplayLabel->setStyleSheet("font-size: 16px; color: #0078d4;");

    // Combine everything
    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(keyLogger);
    rightLayout->addWidget(keyDisplayLabel);
    mainLayout->addLayout(rightLayout);
    
    // Register key press callback
    keyLogger->setKeyCallback([this](const KeyEventInfo &keyInfo) {
        updateKeyDisplay(keyInfo);
    });

    
}

void MainWindow::createSliders()
{
    if (!centralWidget() || !centralWidget()->layout()) {
        qDebug() << "Error: centralWidget() or layout() is NULL!";
        return;
    }

    QVBoxLayout *sliderLayout = qobject_cast<QVBoxLayout*>(centralWidget()->layout()->itemAt(1)->layout());
    if (!sliderLayout) return;

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
}

void MainWindow::updateKeyDisplay(KeyEventInfo keyInfo)
{
    keyDisplayLabel->setText("Key Pressed: " + keyInfo.keyName);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    keyLogger->keyPressEvent(event);
}

void MainWindow::handleSpeedButton() {}
void MainWindow::handleParamButton() {}

KeyLogger* MainWindow::getKeyLogger()
{
    return keyLogger;
}


/* 
void MainWindow::initializeADS1115(SensorContainer *container_1, SensorContainer *container_2,
    SensorContainer *container_3, SensorContainer *container_4)
{
// Instantiate the ADS1115Manager
ADS1115Manager *adsManager = new ADS1115Manager(this);

// Connect the newSensorValue signal to each container's updateSensorValue slot
connect(adsManager, &ADS1115Manager::newSensorValue, container_1, &SensorContainer::updateSensorValue);
connect(adsManager, &ADS1115Manager::newSensorValue, container_2, &SensorContainer::updateSensorValue);
connect(adsManager, &ADS1115Manager::newSensorValue, container_3, &SensorContainer::updateSensorValue);
connect(adsManager, &ADS1115Manager::newSensorValue, container_4, &SensorContainer::updateSensorValue);

// Start reading sensor values from ADS1115
adsManager->start();
} */
