#include "keyLogger.h"
#include <QDebug>

KeyLogger::KeyLogger(QWidget *parent) : QWidget(parent) {
    setFocusPolicy(Qt::StrongFocus);
}

void KeyLogger::setKeyCallback(std::function<void(KeyEventInfo)> callback) {
    keyCallback = callback;
}

void KeyLogger::keyPressEvent(QKeyEvent *event) {
    QString keyPressed;

    switch (event->key()) {
        case Qt::Key_W: keyPressed = "Forward"; break;
        case Qt::Key_A: keyPressed = "Turning Left"; break;
        case Qt::Key_S: keyPressed = "Backward"; break;
        case Qt::Key_D: keyPressed = "Turning Right"; break;
        case Qt::Key_Up: keyPressed = "Forward"; break;
        case Qt::Key_Down: keyPressed = "Backward"; break;
        case Qt::Key_Left: keyPressed = "Turning Left"; break;
        case Qt::Key_Right: keyPressed = "Turning Right"; break;
        default: keyPressed = event->text();  // Default to character representation
    }

    KeyEventInfo keyInfo(keyPressed, event->key(), event->text());

    qDebug() << "Key Pressed:" << keyInfo.keyName
             << "| KeyCode:" << keyInfo.keyCode
             << "| Raw Text:" << keyInfo.rawText;

    if (keyCallback) {
        keyCallback(keyInfo);
    }
}
