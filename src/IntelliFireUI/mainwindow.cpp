#include "mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QDebug>
#include "sensorContainer.h"
#include "ads1115manager.h"
#include "basicMotionV2.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("Welcome to IntelliFire UI");
    resize(1000, 800);

    QWidget *centralWidget = new QWidget;
    setCentralWidget(centralWidget);

    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

    QGridLayout *sensorGrid = new QGridLayout;

    // Smart pointers ensure cleanup
    auto container_1 = std::make_unique<SensorContainer>(1);
    auto container_2 = std::make_unique<SensorContainer>(2);
    auto container_3 = std::make_unique<SensorContainer>(3);
    auto container_4 = std::make_unique<SensorContainer>(4);
    auto container_5 = std::make_unique<SensorContainer>(5);
    auto container_6 = std::make_unique<SensorContainer>(6);

    sensorGrid->addWidget(container_1.get(), 0, 0);
    sensorGrid->addWidget(container_2.get(), 0, 1);
    sensorGrid->addWidget(container_3.get(), 1, 0);
    sensorGrid->addWidget(container_4.get(), 1, 1);
    sensorGrid->addWidget(container_5.get(), 2, 0);
    sensorGrid->addWidget(container_6.get(), 2, 1);

    QVBoxLayout *sliderLayout = new QVBoxLayout;

    QLabel *subtitle = new QLabel("Device: IntelliFire Pro v2.0");
    subtitle->setStyleSheet(R"(
        QLabel {
            font-family: Arial;
            font-size: 14px;
            color: #666666;
            padding: 10px;
        }
    )");

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addWidget(subtitle);
    leftLayout->addLayout(sensorGrid);

    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(sliderLayout);

    createSliders();

    initializeADS1115(container_1.get(), container_2.get(), container_3.get(), container_4.get());

    keyLogger = std::make_unique<KeyLogger>(this);
    keyDisplayLabel = new QLabel("Key Pressed: None");
    keyDisplayLabel->setStyleSheet("font-size: 24px; color: #0078d4;");

    pumpStatusLabel = new QLabel("Pump Status: Unknown");
    pumpStatusLabel->setStyleSheet("font-size: 24px; color: #ff4500;");

    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(keyLogger.get());
    rightLayout->addWidget(keyDisplayLabel);
    rightLayout->addWidget(pumpStatusLabel);

    mainLayout->addLayout(rightLayout);

    keyLogger->setKeyCallback([this](const KeyEventInfo &keyInfo) {
        updateKeyEvent(keyInfo);
    });

    pump_control = std::make_unique<PumpControl>(this);
    connect(pump_control.get(), &PumpControl::pumpStatusChanged, this, &MainWindow::updatePumpStatus);
    pump_control->start();

    //Initialize motors
    motor_left = std::make_shared<Motor>(0, 17, 27, 75, "left");
    motor_right = std::make_shared<Motor>(1, 22, 23, 75, "rigth");

    // Containers must be kept alive
    container_1.release();
    container_2.release();
    container_3.release();
    container_4.release();
    container_5.release();
    container_6.release();
}

KeyLogger* MainWindow::getKeyLogger() {
    return keyLogger.get();
}

void MainWindow::createSliders() {
    if (!centralWidget() || !centralWidget()->layout()) {
        qDebug() << "Error: centralWidget() or layout() is NULL!";
        return;
    }

    QVBoxLayout *sliderLayout = qobject_cast<QVBoxLayout*>(centralWidget()->layout()->itemAt(1)->layout());
    if (!sliderLayout) return;

    speedSlider = new QSlider(Qt::Vertical);
    QPushButton *speedButton = new QPushButton("Set Speed");
    connect(speedButton, &QPushButton::clicked, this, &MainWindow::handleSpeedButton);

    paramSlider = new QSlider(Qt::Vertical);
    QPushButton *paramButton = new QPushButton("Set Parameter");
    connect(paramButton, &QPushButton::clicked, this, &MainWindow::handleParamButton);

    sliderLayout->addWidget(speedSlider);
    sliderLayout->addWidget(speedButton);
    sliderLayout->addSpacing(20);
    sliderLayout->addWidget(paramSlider);
    sliderLayout->addWidget(paramButton);
}

void MainWindow::updateKeyEvent(KeyEventInfo keyInfo) {
    try 
    {
        keyDisplayLabel->setText("Key Pressed: " + keyInfo.keyName);
        keyboardEventControl(motor_left, motor_right, keyInfo);
    } catch (std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    keyLogger->keyPressEvent(event);
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
    keyLogger->keyReleaseEvent(event);
}

void MainWindow::handleSpeedButton() {}
void MainWindow::handleParamButton() {}

void MainWindow::initializeADS1115(SensorContainer *container_1, SensorContainer *container_2,
                                   SensorContainer *container_3, SensorContainer *container_4) {
    try
    {  
        auto *adsManager = new ADS1115Manager(this);

        connect(adsManager, &ADS1115Manager::newSensorValue, container_1, &SensorContainer::sensorValueUpdated);
        connect(adsManager, &ADS1115Manager::newSensorValue, container_2, &SensorContainer::sensorValueUpdated);
        connect(adsManager, &ADS1115Manager::newSensorValue, container_3, &SensorContainer::sensorValueUpdated);
        connect(adsManager, &ADS1115Manager::newSensorValue, container_4, &SensorContainer::sensorValueUpdated);

        adsManager->start();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    catch(const char* e)
    {
        std::cerr << e << '\n';
    }
    catch(...)  
    {
        std::cerr << "Unknown error occurred while initializing ADS1115." << '\n';
    }
}

void MainWindow::updatePumpStatus(float pump_status) {
    pumpStatusLabel->setText(pump_status ? "Pump Status: Pump Activated" : "Pump Status: Pump Deactivated");
}

MainWindow::~MainWindow()
{
    if (pump_control) {
        pump_control->stop();  
    }
}