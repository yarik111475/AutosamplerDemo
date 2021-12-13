#ifndef HT1500LCONNECTOR_H
#define HT1500LCONNECTOR_H

#include <memory>
#include <QtCore>
#include <QObject>
#include <QSerialPort>

class HT1500LConnector : public QObject
{
    Q_OBJECT
private:
    const char m_prefix{0x24};
    const int m_prefixLength{0x01};

    const char m_suffix{0x0D};
    const int m_suffixLength{0x01};

    const int m_operandLength{0x02};
    const int m_errorLength{0x04};
    const int m_statusLength{0x04};
    const int m_lengthLength{0x04};
    const int m_checksumLength{0x02};

    QByteArray m_readBuffer;

    QSerialPort* m_pSerialPort;
    QString m_portName;
    void delay(int interval);
    char createChecksum(const QByteArray& request);
    QByteArray readResponse(const QByteArray& request, int responseLength);

private slots:
    void slotReadyRead();
public:
    explicit HT1500LConnector(const QString& portName,QObject *parent = nullptr);
    void readCfgAndSwReleaseParameters();
signals:

};

#endif // HT1500LCONNECTOR_H
