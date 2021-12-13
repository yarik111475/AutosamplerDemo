#include "ht1500lconnector.h"

void HT1500LConnector::delay(int interval)
{
    QEventLoop eventLoop;
    QTimer::singleShot(interval,&eventLoop, SLOT(quit()));
    eventLoop.exec();
}

char HT1500LConnector::createChecksum(const QByteArray &request)
{
    int sum{0};
    for(int i=0;i<request.size();++i){
        sum^=request.at(i);
    }
    return static_cast<char>(sum);
}

QByteArray HT1500LConnector::readResponse(const QByteArray &request, int responseLength)
{
    const int delayInterval{250};
    m_readBuffer.clear();
    m_pSerialPort->write(request);
    delay(delayInterval);
    if(m_readBuffer.size()==responseLength){
        return m_readBuffer;
    }
    return QByteArray();
}

void HT1500LConnector::slotReadyRead()
{
    m_readBuffer.append(m_pSerialPort->readAll());
}

HT1500LConnector::HT1500LConnector(const QString &portName, QObject *parent)
    : QObject(parent),m_pSerialPort{nullptr}, m_portName{portName}
{

}

void HT1500LConnector::readCfgAndSwReleaseParameters()
{
    m_pSerialPort=new QSerialPort(m_portName);
    //here configure port settings if needed...

    if(!m_pSerialPort->open(QIODevice::ReadWrite)){
        m_pSerialPort->close();
        delete m_pSerialPort;
        throw QString("Can not open serial port");
    }

    const char operand{0x03};
    const char requestLength{0x10};
    const char responseLength{0x26};

    QByteArray request;
    request.push_back(m_prefix);
    request.append(m_operandLength-1, static_cast<char>(0x00));
    request.push_back(operand);

    request.append(m_errorLength, static_cast<char>(0x00));
    request.append(m_statusLength, static_cast<char>(0x00));
    request.append(m_lengthLength-1, static_cast<char>(0x00));
    request.push_back(requestLength);

    request.append(m_checksumLength-1, static_cast<char>(0x00));
    request.push_back(createChecksum(request));
    request.push_back(m_suffix);

    QByteArray response=readResponse(request,responseLength+m_checksumLength+m_suffixLength);
    if(response.isEmpty()){
        m_pSerialPort->close();
        delete m_pSerialPort;
        throw QString("No response from device");
    }

    m_pSerialPort->close();
    delete m_pSerialPort;
    //here parse response from device...
}
