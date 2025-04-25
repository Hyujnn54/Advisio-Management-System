#include "arduino.h"
#include <QObject>
#include <QDebug>
Arduino::Arduino() : QObject(nullptr) {
    serialPort = new QSerialPort(this);
    connect(serialPort, &QSerialPort::readyRead, this, &Arduino::dataAvailable);
}

Arduino::~Arduino() {
    if (serialPort->isOpen()) {
        serialPort->close();
    }
    delete serialPort;
}

bool Arduino::connectArduino(const QString &portName) {
    serialPort->setPortName(portName);
    serialPort->setBaudRate(QSerialPort::Baud9600);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);

    if (serialPort->open(QIODevice::ReadWrite)) {
        return true;
    } else {
        qDebug() << "Failed to open serial port:" << serialPort->errorString();
        return false;
    }
}

QByteArray Arduino::readData() {
    return serialPort->readAll();
}
