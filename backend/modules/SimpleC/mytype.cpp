#include <cstdlib>
#include "mytype.h"
#include "OCPlatform.h"
#include "OCApi.h"

using namespace OC;

Launcher::Launcher(QObject *parent) :
    QObject(parent),
    m_message(""),
    m_process(new QProcess(this))
{
}

Launcher::~Launcher() {

}

QString Launcher::getIotStatus() 
{ 
	return m_message; 
}

void Launcher::setIotStatus(QString msg) 
{
	m_message = msg; 
	Q_EMIT iotStatusChanged(); 
}

static FILE* client_open(const char* /*path*/, const char *mode)
{
    return fopen("./oic_svr_db_client.json", mode);
}

QString Launcher::launch(const QString &program)
{
    //m_process->start(program);
    //m_process->waitForFinished(-1);
    //QByteArray bytes = m_process->readAllStandardOutput();
    //QString output = QString::fromLocal8Bit(bytes);
    QString output = "Iotivity Demo Status";
    OCPersistentStorage ps {client_open, fread, fwrite, fclose, unlink };

    // Create PlatformConfig object
    PlatformConfig cfg {
        OC::ServiceType::InProc,
        OC::ModeType::Both,
        "0.0.0.0",
        0,
        OC::QualityOfService::LowQos,
        &ps
    };

	QQmlEngine *engine = QQmlEngine::contextForObject(this)->engine();
	engine->rootContext()->setContextProperty("rootItem", (QObject *)this);

    OCPlatform::Configure(cfg);
	try
	{
		this->setIotStatus("OCPlatform::Configure");
	}
	catch(OCException& e)
	{
		oclog() << "Exception in main: "<<e.what();
	}

    return output;
}
