#include "Arduino.h"

Arduino::Arduino(QObject *parent) : QObject(parent)
{
    serial = new QSerialPort(this);
    arduino_available = false;
    arduino_port_name = "";

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        if (info.hasVendorIdentifier() && info.hasProductIdentifier()) {
            if (info.vendorIdentifier() == arduino_uno_vendor_id && info.productIdentifier() == arduino_uno_product_id) {
                arduino_port_name = info.portName();
                arduino_available = true;
            }
        }
    }
}

Arduino::~Arduino()
{
    if (serial->isOpen())
        serial->close();
}

int Arduino::connectArduino()
{
    if (arduino_available) {
        serial->setPortName(arduino_port_name);
        if (serial->open(QIODevice::ReadWrite)) {
            serial->setBaudRate(QSerialPort::Baud9600);
            serial->setDataBits(QSerialPort::Data8);
            serial->setParity(QSerialPort::NoParity);
            serial->setStopBits(QSerialPort::OneStop);
            serial->setFlowControl(QSerialPort::NoFlowControl);
            return 0; // Connected
        } else {
            return -1; // Error opening
        }
    }
    return 1; // Arduino not available
}

int Arduino::closeArduino()
{
    if (serial->isOpen()) {
        serial->close();
        return 0;
    }
    return 1;
}

QByteArray Arduino::readFromArduino()
{
    if (serial->isOpen()) {
        return serial->readAll();
    }
    return QByteArray();
}

bool Arduino::writeToArduino(QByteArray data)
{
    if (serial->isOpen()) {
        serial->write(data);
        return true;
    }
    return false;
}

QSerialPort* Arduino::getSerial() const
{
    return serial;
}
