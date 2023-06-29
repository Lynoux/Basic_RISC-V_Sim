#ifndef SCQT_WORKER_H
#define SCQT_WORKER_H

#include <QObject>
#include <QMutex>
#include "sc_main.h"
#include "sc_run.h"
#include "sc_config.h"

class scQtWorker : public QObject
{
    Q_OBJECT
public:
    explicit scQtWorker(QObject *parent = nullptr);
    ~scQtWorker();

public slots:
    void getHierarchy();
    void getReport();
    void setTraceList(QStringList tl);
    void setNewState(int newState);
    void doStart(void);
    void setRunParameters(sc_thread_run_t &runParams);
    void setConfigParameters(sc_thread_config_t &configParams);

signals:
    void Message(QString const &msg_origin, QString const &msg_level, QString const &msg);
    void signalHierarchy(QStringList hier);
    void signalReport(QStringList rep);

private:
    void _sendMessage(QString const &msg_level, QString const &msg);

    QMutex m_DataMutex;
    pthread_t m_scThreadHandle;
    sc_thread_config_t m_ConfigParams;
    sc_thread_run_t m_RunParams;
};

#endif // SCQT_WORKER_H
