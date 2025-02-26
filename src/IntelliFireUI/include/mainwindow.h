#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSlider>
#include <QKeyEvent>
#include "sensorContainer.h"
#include "keyLogger.h"
#include "libcam2opencv.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    
    explicit MainWindow(QWidget *parent = nullptr);
    QLabel       *image;
    void updateImage(const cv::Mat &mat);
    KeyLogger *getKeyLogger();  // Make KeyLogger accessible
    void keyPressEvent(QKeyEvent *event) override;

    struct MyCallback : Libcam2OpenCV::Callback {
        MainWindow* mainwindow = nullptr;
        virtual void hasFrame(const cv::Mat &frame, const libcamera::ControlList &) {
            if (nullptr != mainwindow) {
                mainwindow->updateImage(frame);
            }
        }
    };
    
    Libcam2OpenCV camera;
    MyCallback myCallback; 


private Q_SLOTS:
    void handleSpeedButton();
    void handleParamButton();
    void initializeADS1115(SensorContainer *container_1, SensorContainer *container_2, SensorContainer *container_3, SensorContainer *container_4);
    void updateKeyDisplay(KeyEventInfo keyInfo);

private:
    // void setupUI();
    void createSliders();
    // void createSensorGrid();
    
    QSlider *speedSlider;
    QSlider *paramSlider;

    QLabel *keyDisplayLabel;
    KeyLogger *keyLogger;

    void updateKeyDisplay(QString key);

};

#endif // MAINWINDOW_H
