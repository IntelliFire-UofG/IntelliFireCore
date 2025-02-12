#ifndef SENSORCONTAINER_H
#define SENSORCONTAINER_H

#include <QWidget>

class SensorContainer : public QWidget
{
    Q_OBJECT
public:
    explicit SensorContainer(int containerNumber, QWidget *parent = nullptr);
};

#endif // SENSORCONTAINER_H