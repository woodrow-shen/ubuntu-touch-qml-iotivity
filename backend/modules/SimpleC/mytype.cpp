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
    QString output = "Enter IOT";
    if (!program.isEmpty()) output = program;
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

    OCPlatform::Configure(cfg);

    return output;
}
