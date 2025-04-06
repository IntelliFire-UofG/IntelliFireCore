#include "keyLogger.h"
#include <QDebug>

KeyLogger::KeyLogger(QWidget *parent) : QWidget(parent) {
    setFocusPolicy(Qt::StrongFocus);
}

void KeyLogger::setKeyCallback(const std::function<void(const KeyEventInfo&)>& callback) {
    keyCallback = callback;
}

void KeyLogger::keyPressEvent(QKeyEvent *event) {
    const QString rawText = event->text();
    QString keyPressed;

    switch (event->key()) {
        case Qt::Key_W:      keyPressed = "Forward"; break;
        case Qt::Key_A:      keyPressed = "Turning Left"; break;
        case Qt::Key_S:      keyPressed = "Backward"; break;
        case Qt::Key_D:      keyPressed = "Turning Right"; break;
        case Qt::Key_Up:     keyPressed = "Forward"; break;
        case Qt::Key_Down:   keyPressed = "Backward"; break;
        case Qt::Key_Left:   keyPressed = "Turning Left"; break;
        case Qt::Key_Right:  keyPressed = "Turning Right"; break;
        case Qt::Key_X:      keyPressed = "Stop"; break;
        default:             keyPressed = rawText; break;
    }

    const KeyEventInfo keyInfo(keyPressed, event->key(), rawText);

    qDebug() << "Key Pressed:" << keyInfo.keyName
             << "| KeyCode:" << keyInfo.keyCode
             << "| Raw Text:" << keyInfo.rawText;

    if (keyCallback) {
        keyCallback(keyInfo);
    }
}

void KeyLogger::keyReleaseEvent(QKeyEvent *event) {
    const QString rawText = event->text();
    QString keyReleased;

    switch (event->key()) {
        case Qt::Key_W:      keyReleased = "Stopped Forward"; break;
        case Qt::Key_A:      keyReleased = "Stopped Turning Left"; break;
        case Qt::Key_S:      keyReleased = "Stopped Backward"; break;
        case Qt::Key_D:      keyReleased = "Stopped Turning Right"; break;
        case Qt::Key_Up:     keyReleased = "Stopped Forward"; break;
        case Qt::Key_Down:   keyReleased = "Stopped Backward"; break;
        case Qt::Key_Left:   keyReleased = "Stopped Turning Left"; break;
        case Qt::Key_Right:  keyReleased = "Stopped Turning Right"; break;
        case Qt::Key_X:      keyReleased = "Released Stop"; break;
        default:             keyReleased = rawText; break;
    }

    const KeyEventInfo keyInfo(keyReleased, event->key(), rawText);

    qDebug() << "Key Released:" << keyInfo.keyName
             << "| KeyCode:" << keyInfo.keyCode
             << "| Raw Text:" << keyInfo.rawText;

    if (keyCallback) {
        keyCallback(keyInfo);
    }
}
