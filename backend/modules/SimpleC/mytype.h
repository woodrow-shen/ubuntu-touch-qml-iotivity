#ifndef LAUNCHER_H
#define LAUNCHER_H

#include <QObject>
#include <QProcess>
#include <QQmlEngine>
#include <QQmlContext>
#include <sstream>
#include "OCPlatform.h"
#include "OCApi.h"

using namespace OC;
namespace PH = std::placeholders;

typedef std::map<OCResourceIdentifier, std::shared_ptr<OCResource>> DiscoveredResourceMap;

class Launcher : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString IotStatus READ getIotStatus WRITE setIotStatus NOTIFY iotStatusChanged)

public:
    explicit Launcher(QObject *parent = 0);
    ~Launcher();
    Q_INVOKABLE QString launch(const QString &program);
    QString getIotStatus() const
	{
		return m_message;
	}
    void setIotStatus(QString msg);
	static Launcher *getInstance();
	std::ostringstream m_output;

signals:
    void iotStatusChanged(QString newMsg);

protected:
    QString m_message;
    QProcess *m_process;
private:
	static Launcher *m_cur_launcher;
};

#endif // LAUNCHER_H
