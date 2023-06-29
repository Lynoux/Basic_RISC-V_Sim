#include "scqt_worker.h"
#include <QThread>
#include "sc_run.h"

scQtWorker::scQtWorker(QObject *parent) : QObject(parent)
{
    m_RunParams.state = SC_ST_RESPONSE_STOP;
    if (pthread_mutex_init(&m_RunParams.run_mutex, NULL)) {
        pthread_mutex_destroy(&m_RunParams.run_mutex);
        cout << "ERROR: Can't create run-control mutex for systemC simulation" << endl;
        return;
    }
    if (pthread_mutex_init(&m_RunParams.condvar_mutex, NULL)) {
        pthread_mutex_destroy(&m_RunParams.run_mutex);
        pthread_mutex_destroy(&m_RunParams.condvar_mutex);
        cout << "ERROR: Can't create condvar-mutex for systemC simulation" << endl;
        return;
    }
    if (pthread_cond_init(&m_RunParams.condvar, nullptr)) {
        pthread_cond_destroy(&m_RunParams.condvar);
        pthread_mutex_destroy(&m_RunParams.run_mutex);
        pthread_mutex_destroy(&m_RunParams.condvar_mutex);
        cout << "ERROR: Can't create condvar for systemC simulation" << endl;
        return;
    }
}

scQtWorker::~scQtWorker()
{
    void *result;
    _sendMessage("DEBUG", "scQtThread destructor called");
    pthread_cond_broadcast(&m_RunParams.condvar);
    pthread_cond_destroy(&m_RunParams.condvar);
    pthread_mutex_destroy(&m_RunParams.run_mutex);
    pthread_mutex_destroy(&m_RunParams.condvar_mutex);
    pthread_join(m_scThreadHandle, &result);
}

void scQtWorker::getHierarchy()
{
    QStringList qsl;
    std::vector<std::string> hier = sc_getHierarchyFromUI();
    for (size_t i=0; i < hier.size(); i++) {
        qsl.push_back(hier.at(i).c_str());
    }
    emit signalHierarchy(qsl);
}

void scQtWorker::getReport()
{
    QStringList qsl;
    std::vector<std::string> hier = sc_getReportFromUI();
    for (size_t i=0; i < hier.size(); i++) {
        qsl.push_back(hier.at(i).c_str());
    }
    emit signalReport(qsl);
}

void scQtWorker::setTraceList(QStringList tl)
{
    std::vector<std::string> sv;
    for (qsizetype i=0; i < tl.size(); i++) {
        sv.push_back(tl.at(i).toStdString());
    }
    sc_setTraceListFromUI(sv);
}

void scQtWorker::setNewState(int newState)
{
    switch(newState) {
    case SC_ST_RESPONSE_STOP:
    case SC_ST_COMMAND_RUN:
    case SC_ST_COMMAND_EXIT:
    case SC_ST_RESPONSE_FINISHED:
    case SC_ST_RESPONSE_ABORTED:
        sc_NewStateFromUI(newState);
        break;
    default:
        _sendMessage("ERROR", "scQtThread received bad newState");
    }
#if 0
    if (newState == SC_ST_EXIT) {
        this->thread()->exit();
    }
#endif
}

void scQtWorker::doStart(void)
{
    pthread_attr_t attr;
    int pthread_create_result = 0;

    sc_setRunFromUI (&m_RunParams);
    sc_setConfig(&m_ConfigParams);

    _sendMessage("DEBUG", "scQtThread::doStart() called");
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_create_result = pthread_create(&m_scThreadHandle, &attr, scSimMain, nullptr);
    pthread_attr_destroy(&attr);
    if (pthread_create_result) {
        pthread_cond_destroy(&m_RunParams.condvar);
        pthread_mutex_destroy(&m_RunParams.run_mutex);
        pthread_mutex_destroy(&m_RunParams.condvar_mutex);
        _sendMessage("ERROR", "scQtThread can't start SystemC thread");
    }
}

void scQtWorker::setRunParameters(sc_thread_run_t &runParams)
{
    m_DataMutex.lock();
    m_RunParams = runParams;
    m_DataMutex.unlock();
}

void scQtWorker::setConfigParameters(sc_thread_config_t &configParams)
{
    m_DataMutex.lock();
    m_ConfigParams = configParams;
    m_DataMutex.unlock();
}

void scQtWorker::_sendMessage(QString const &msg_level, QString const &msg)
{
    emit Message("SC", msg_level, msg);
}

