#ifndef ARDUINOY_H
#define ARDUINOY_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>

class Arduinoy : public QObject
{
    Q_OBJECT

public:
    explicit Arduinoy(QObject *parent = nullptr);
    ~Arduinoy();

    int connectArduino();              // 0: Arduino connected, 1: not available, -1: error
    int closeArduino();
    QByteArray readFromArduino();
    bool writeToArduino(QByteArray data);
    QSerialPort* getSerial() const;

private:
    QSerialPort *serial;
    static const quint16 arduino_uno_vendor_id = 9025;
    static const quint16 arduino_uno_product_id = 67;
    QString arduino_port_name;
    bool arduino_available;
};

#endif // ARDUINOY_H
