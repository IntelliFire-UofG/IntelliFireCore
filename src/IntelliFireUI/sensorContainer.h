#ifndef SENSORCONTAINER_H
#define SENSORCONTAINER_H
#include <atomic>
#include <QLabel>
#include <QWidget>
#include <QTimer>

class SensorContainer : public QWidget
{
    Q_OBJECT
public:
    explicit SensorContainer(int containerNumber, QWidget *parent = nullptr);
private slots:
    int updateSensorValue(); 


private:
    QTimer *timer;  // Store QTimer in the class to prevent crashes
    QLabel *sensorLabel; 
signals:



};

#endif // SENSORCONTAINER_H