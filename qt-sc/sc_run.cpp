#include "sc_run.h"
#include <systemc.h>

static int scThreadResult = 0;
static sc_thread_run_t *scThreadRun = nullptr;

int sc_getRunResultFromUI(void) { return scThreadResult; }
void sc_setRunFromUI(sc_thread_run_t *theRun) { scThreadRun = theRun; }
void sc_setRunResult(int theResult) {scThreadResult = theResult; }

void sc_setReportFromSC(std::vector<std::string> &rep) {
    //cout << "DEBUG: sc_setReportFromSC()" << endl;
    scThreadRun->report = rep;
}

void sc_setHierarchyFromSC(std::vector<std::string> &hier)
{
    //cout << "DEBUG: sc_setHierarchyFromSC()" << endl;
    scThreadRun->hierarchy = hier;
}

void sc_NotifyUIFromSC(int newState)
{
    //cout << "DEBUG: sc_NotifyUIFromSC()" << endl;
    scThreadRun->state = newState;
    if (pthread_cond_signal(&scThreadRun->condvar) != 0) {
        cout << "sc_NotifyUIFromSC() ERROR: Internal error pthread_cond_signal()" << endl;
        return;
    }
    //cout << "DEBUG: SC cond signal start" << endl;
    if (pthread_mutex_unlock(&scThreadRun->condvar_mutex) != 0) {
        cout << "sc_NotifyUIFromSC() ERROR: Internal error pthread_mutex_unlock()" << endl;
    }
    //cout << "DEBUG: SC mutex unlock" << endl;
}

int sc_waitStateChange(void)
{
    //cout << "DEBUG: sc_waitStateChange()" << endl;
    if (scThreadRun == nullptr) {
        cout << "sc_waitUiState() ERROR: Internal error scThreadRun is NULL" << endl;
        // Non MT-safe change of state
        return SC_ST_COMMAND_EXIT;
    }
    if (pthread_mutex_lock(&scThreadRun->condvar_mutex) != 0) {
        cout << "sc_waitUiState() ERROR: Internal error pthread_mutex_lock()" << endl;
        // Non MT-safe change of state
        scThreadRun->state = SC_ST_RESPONSE_ABORTED;
        return SC_ST_COMMAND_EXIT;
    }
    //cout << "DEBUG: SC mutex lock" << endl;
    while (scThreadRun->state >= SC_ST_MINRESPONSE) {
       //cout << "DEBUG: SC cond wait start" << endl;
       if (pthread_cond_wait(&scThreadRun->condvar, &scThreadRun->condvar_mutex) != 0) {
           cout << "sc_waitUiState() ERROR: Internal error pthread_cond_wait()" << endl;
           // Non thread-safe change of state
           sc_NotifyUIFromSC(SC_ST_RESPONSE_ABORTED);
           return SC_ST_COMMAND_EXIT;
       }
       //cout << "DEBUG: SC cond wait end" << endl;
    }
    if (scThreadRun->state == SC_ST_COMMAND_EXIT) {
        //cout << "DEBUG: sc_waitStateChange() gonna stop" << endl;
        sc_stop();
        sc_NotifyUIFromSC(SC_ST_RESPONSE_FINISHED);
    }
    return scThreadRun->state;
}

int sc_StartTransactionFromUI()
{
    //cout << "DEBUG: sc_StartTransactionFromUI()" << endl;
    if (scThreadRun == nullptr) {
        cout << "sc_StartTransactionFromUI() ERROR: Internal error scThreadRun is NULL" << endl;
        return 0;
    }
    if (pthread_mutex_lock(&scThreadRun->condvar_mutex) != 0) {
        cout << "sc_StartTransactionFromUI() ERROR: Internal error pthread_mutex_lock()" << endl;
        return 0;
    }
    //cout << "DEBUG: GUI mutex lock" << endl;
    return 1;
}

int sc_NotifyTransactionFromUI()
{
    //cout << "DEBUG: sc_NotifyTransactionFromUI()" << endl;
    if (scThreadRun == nullptr) {
        cout << "sc_NotifyTransactionFromUI() ERROR: Internal error scThreadRun is NULL" << endl;
        return 0;
    }
    if (pthread_cond_signal(&scThreadRun->condvar) != 0) {
        cout << "sc_NotifyTransactionFromUI() ERROR: Internal error pthread_cond_signal()" << endl;
        return 0;
    }
    //cout << "DEBUG: GUI cond signal start" << endl;
    return 1;
}

int sc_EndTransactionFromUI()
{
    //cout << "DEBUG: sc_EndTransactionFromUI()" << endl;
    if (scThreadRun == nullptr) {
        cout << "sc_EndTransactionFromUI() ERROR: Internal error scThreadRun is NULL" << endl;
        return 0;
    }
    if (pthread_mutex_unlock(&scThreadRun->condvar_mutex) != 0) {
        cout << "sc_EndTransactionFromUI() ERROR: Internal error pthread_mutex_unlock()" << endl;
        return 0;
    }
    //cout << "DEBUG: GUI mutex unlock" << endl;
    return 1;
}

int sc_waitStateChangeFromUI(void)
{
    //cout << "DEBUG: sc_waitStateChangeFromUI()" << endl;
    if (scThreadRun == nullptr) {
        cout << "sc_waitStateChangeFromUI() ERROR: Internal error scThreadRun is NULL" << endl;
        return 0;
    }
    while (scThreadRun->state <= SC_ST_MAXCOMMAND) {
        //cout << "DEBUG: GUI cond wait start" << endl;
        if (pthread_cond_wait(&scThreadRun->condvar, &scThreadRun->condvar_mutex) != 0) {
            cout << "sc_waitStateChangeFromUI() ERROR: Internal error pthread_cond_wait()" << endl;
            return 0;
        }
        //cout << "DEBUG: GUI cond wait end" << endl;
    }
    return 1;
}

int sc_NewStateFromUI(int newState)
{
    //cout << "DEBUG: sc_NewStateFromUI() start" << endl;
    if (!sc_StartTransactionFromUI()) {
        cout << "DEBUG: sc_NewStateFromUI() abort (1)" << endl;
        return 0;
    }
    scThreadRun->state = newState;
    if (!sc_NotifyTransactionFromUI() || !sc_EndTransactionFromUI()) {
        cout << "DEBUG: sc_NewStateFromUI() abort (2)" << endl;
        return 0;
    }
    //cout << "DEBUG: sc_NewStateFromUI() end" << endl;
    return 1;
}

std::vector<std::string> sc_getHierarchyFromUI() {
    std::vector<std::string> hier;

    //cout << "DEBUG: sc_getHierarchyFromUI() start" << endl;
    if (sc_NewStateFromUI(SC_ST_COMMAND_RUN)) {
        if (sc_StartTransactionFromUI()) {
            if (sc_waitStateChangeFromUI()) {
                hier = scThreadRun->hierarchy;
                sc_EndTransactionFromUI();
            }
        }
    }
    //cout << "DEBUG: sc_getHierarchyFromUI() end" << endl;
    return hier;
}

void sc_setTraceListFromUI(std::vector<std::string> &tl) {
    //cout << "DEBUG: sc_setTraceListFromUI() start" << endl;
    if (sc_StartTransactionFromUI()) {
        scThreadRun->tracelist = tl;
        sc_EndTransactionFromUI();
    }
    //cout << "DEBUG: sc_setTraceListFromUI() end" << endl;
}

std::vector<std::string> sc_getReportFromUI() {
    std::vector<std::string> rep;
    //cout << "DEBUG: sc_getReportFromUI() start" << endl;
    if (sc_StartTransactionFromUI()) {
        rep = scThreadRun->report;
        sc_EndTransactionFromUI();
    }
    //cout << "DEBUG: sc_getReportFromUI() end" << endl;
    return rep;
}
