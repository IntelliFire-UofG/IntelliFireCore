#include "mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QDebug>
#include "sensorContainer.h"
#include "ads1115manager.h" 
#include "pumpControl.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    //camera
    myCallback.mainwindow = this;
    camera.registerCallback(&myCallback);
    image = new QLabel;
    //camera added to the right
    QHBoxLayout *sensorAndCameraLayout = new QHBoxLayout;
    sensorAndCameraLayout-> addWidget(image);

    /////////////////////////////////////////////////////////////////
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
    SensorContainer *container_5 = new SensorContainer(5);
    SensorContainer *container_6 = new SensorContainer(6);
    
    // Sensor containers mapped into layout
    sensorGrid->addWidget(container_1, 0, 0); // Flame Sensor 1
    sensorGrid->addWidget(container_2, 0, 1); // Flame Sensor 2
    sensorGrid->addWidget(container_3, 1, 0); // Flame Sensor 3
    sensorGrid->addWidget(container_4, 1, 1); // Flame Sensor 4
    sensorGrid->addWidget(container_5, 2, 0); // Ultrasonic Sensor
    sensorGrid->addWidget(container_6, 2, 1); // IR Sensor

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

    // Initialize and start ADS1115Manager
    initializeADS1115(container_1, container_2, container_3, container_4);

    // Key Logger
    keyLogger = new KeyLogger;
    keyDisplayLabel = new QLabel("Key Pressed: None");
    keyDisplayLabel->setStyleSheet("font-size: 24px; color: #0078d4;");

    // Pump Status Label
    pumpStatusLabel = new QLabel("Pump Status: Unknown");
    pumpStatusLabel->setStyleSheet("font-size: 24px; color: #ff4500;");

    // Combine everything
    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(keyLogger);
    rightLayout->addWidget(keyDisplayLabel);
    rightLayout->addWidget(pumpStatusLabel);
    
    mainLayout->addLayout(rightLayout);
    mainLayout->addLayout(sensorAndCameraLayout); //camera 

    //start camera
    camera.start();
    
    // Register key press callback
    keyLogger->setKeyCallback([this](const KeyEventInfo &keyInfo) {
        updateKeyDisplay(keyInfo);
    });


    // Connect pump status signal to update function
    connect(PumpController::getInstance(), &PumpController::pumpStatusChanged,
    this, &MainWindow::updatePumpStatus);

    // Call updatePumpStatus initially
    updatePumpStatus();

}


KeyLogger* MainWindow::getKeyLogger()
{
    return keyLogger;
}

void MainWindow::updatePumpStatus() {
    // Assume we have a class "PumpController" that provides the status
    bool isPumpActive = PumpController::getInstance()->isPumpActive();

    if (isPumpActive) {
        pumpStatusLabel->setText("Pump Status: Pump Activated");
    } else {
        pumpStatusLabel->setText("Pump Status: Pump Deactivated");
    }
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

void MainWindow::updateImage(const cv::Mat &mat) {
    const QImage frame(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
    image->setPixmap(QPixmap::fromImage(frame));
    const int h = frame.height();
	const int w = frame.width();
	
    update();

}
 
void MainWindow::initializeADS1115(SensorContainer *container_1, SensorContainer *container_2,
    SensorContainer *container_3, SensorContainer *container_4)
{
// Instantiate the ADS1115Manager
ADS1115Manager *adsManager = new ADS1115Manager(this);

// Connect the newSensorValue signal to each container's updateSensorValue slot
connect(adsManager, &ADS1115Manager::newSensorValue, container_1, &SensorContainer::sensorValueUpdated);
connect(adsManager, &ADS1115Manager::newSensorValue, container_2, &SensorContainer::sensorValueUpdated);
connect(adsManager, &ADS1115Manager::newSensorValue, container_3, &SensorContainer::sensorValueUpdated);
connect(adsManager, &ADS1115Manager::newSensorValue, container_4, &SensorContainer::sensorValueUpdated);

// Start reading sensor values from ADS1115
adsManager->start();
} 

void MainWindow::updatePumpStatus() {
    // Assume we have a class "PumpController" that provides the status
    bool isPumpActive = PumpController::getInstance()->isPumpActive(); // This must be changed to the actual class and method

    if (isPumpActive) {
        pumpStatusLabel->setText("Pump Status: Pump Activated");
    } else {
        pumpStatusLabel->setText("Pump Status: Pump Deactivated");
    }
}
