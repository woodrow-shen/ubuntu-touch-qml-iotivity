#ifndef LAUNCHER_H
#define LAUNCHER_H

#include <QObject>
#include <QProcess>
#include <QQmlEngine>
#include <QQmlContext>

class Launcher : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString IotStatus READ getIotStatus WRITE setIotStatus NOTIFY iotStatusChanged)

public:
    explicit Launcher(QObject *parent = 0);
    ~Launcher();
    Q_INVOKABLE QString launch(const QString &program);

signals:
    void iotStatusChanged();

protected:
    QString getIotStatus();
    void setIotStatus(QString msg);

    QString m_message;
    QProcess *m_process;
};

#endif // LAUNCHER_H

