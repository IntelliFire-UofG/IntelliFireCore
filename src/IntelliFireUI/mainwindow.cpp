#include "mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QDebug>
#include "ads1115manager.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Welcome to IntelliFire UI");
    resize(1000, 800);

    QWidget *centralWidget = new QWidget;
    setCentralWidget(centralWidget);
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

    // Left: Sensor Grid
    QGridLayout *sensorGrid = new QGridLayout;
    SensorContainer *container_1 = new SensorContainer(1);
    SensorContainer *container_2 = new SensorContainer(2);
    SensorContainer *container_3 = new SensorContainer(3);
    SensorContainer *container_4 = new SensorContainer(4);
    SensorContainer *container_5 = new SensorContainer(5);
    SensorContainer *container_6 = new SensorContainer(6);

    sensorGrid->addWidget(container_1, 0, 0);
    sensorGrid->addWidget(container_2, 0, 1);
    sensorGrid->addWidget(container_3, 1, 0);
    sensorGrid->addWidget(container_4, 1, 1);
    sensorGrid->addWidget(container_5, 2, 0);
    sensorGrid->addWidget(container_6, 2, 1);

    QVBoxLayout *leftLayout = new QVBoxLayout;
    QLabel *subtitle = new QLabel("Device: IntelliFire Pro v2.0");
    subtitle->setStyleSheet(R"(
        QLabel {
            font-family: Arial;
            font-size: 14px;
            color: #666666;
            padding: 10px;
        }
    )");

    leftLayout->addWidget(subtitle);
    leftLayout->addLayout(sensorGrid);
    mainLayout->addLayout(leftLayout);

    // Right: UI controls
    QVBoxLayout *sliderLayout = new QVBoxLayout;
    mainLayout->addLayout(sliderLayout);
    createSliders();

    // Key logger
    keyLogger = std::make_unique<KeyLogger>();
    keyDisplayLabel = new QLabel("Key Pressed: None");
    keyDisplayLabel->setStyleSheet("font-size: 24px; color: #0078d4;");

    // Pump label
    pumpStatusLabel = new QLabel("Pump Status: Unknown");
    pumpStatusLabel->setStyleSheet("font-size: 24px; color: #ff4500;");

    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(keyLogger.get());
    rightLayout->addWidget(keyDisplayLabel);
    rightLayout->addWidget(pumpStatusLabel);
    mainLayout->addLayout(rightLayout);

    // Connect key events
    keyLogger->setKeyCallback([this](const KeyEventInfo &keyInfo) {
        updateKeyDisplay(keyInfo);
    });

    // ADS1115 connection
    initializeADS1115(container_1, container_2, container_3, container_4);

    // Pump controller
    pump_control = std::make_unique<PumpControl>();
    connect(pump_control.get(), &PumpControl::pumpStatusChanged,
            this, &MainWindow::updatePumpStatus);
    pump_control->start();
}

MainWindow::~MainWindow() {
    if (pump_control) {
        pump_control->stop();
    }
}

KeyLogger* MainWindow::getKeyLogger() {
    return keyLogger.get();
}

void MainWindow::createSliders() {
    if (!centralWidget() || !centralWidget()->layout()) return;

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

void MainWindow::updateKeyDisplay(KeyEventInfo keyInfo) {
    keyDisplayLabel->setText("Key Pressed: " + keyInfo.keyName);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    keyLogger->keyPressEvent(event);
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
    keyLogger->keyReleaseEvent(event);
}

void MainWindow::handleSpeedButton() {
    // Reserved for future control logic
}

void MainWindow::handleParamButton() {
    // Reserved for future parameter control
}

void MainWindow::initializeADS1115(SensorContainer *c1, SensorContainer *c2,
                                   SensorContainer *c3, SensorContainer *c4) {
    ADS1115Manager *adsManager = new ADS1115Manager(this);

    connect(adsManager, &ADS1115Manager::newSensorValue, c1, &SensorContainer::sensorValueUpdated);
    connect(adsManager, &ADS1115Manager::newSensorValue, c2, &SensorContainer::sensorValueUpdated);
    connect(adsManager, &ADS1115Manager::newSensorValue, c3, &SensorContainer::sensorValueUpdated);
    connect(adsManager, &ADS1115Manager::newSensorValue, c4, &SensorContainer::sensorValueUpdated);

    adsManager->start();
}

void MainWindow::updatePumpStatus(float pump_status) {
    if (pump_status) {
        pumpStatusLabel->setText("Pump Status: Pump Activated");
    } else {
        pumpStatusLabel->setText("Pump Status: Pump Deactivated");
    }
}
