#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextBrowser>
#include <QThread>
#include "scqt_worker.h"

#include "SVG_Viewer.h"
#include <QGraphicsSvgItem>
#include <QFileDialog>
#include "ui_mainwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QGraphicsSvgItem *getItem();
    QGraphicsSvgItem *getItem1();
    Ui::MainWindow *getUi();
    MainWindow(QWidget *parent = nullptr);
    void Refresh();
    ~MainWindow();


public slots:
    void onMessage(QString const &msg_origin, QString const &msg_level, QString const &msg);
//    void onResetClicked(bool checked = false);
//    void onClockClicked(bool checked = false);
    void on_pushButton_3_clicked(bool checked);
    void on_pushButton_clicked(bool checked);
    void on_pushButton_2_clicked(bool checked = false);
    void newHierarchy(QStringList hier);
    void newReport(QStringList rep);

signals:
    void Message(QString const &msg_origin, QString const &msg_level, QString const &msg);
    void setNewState(int newState);
    void requestHierarchy();
    void requestReport();
    void setTraceList(QStringList tl);



private slots:
    void on_checkBox_clicked(bool checked);

    void on_actionExit_triggered();

    void on_actionOpen_source_triggered();

private:
    void _sendMessage(QString const &msg_level, QString const &msg);
    void setupUi();
    void setupSim();

    QThread *m_scqtThread;
    scQtWorker *m_scqtWorker;
    sc_thread_config_t m_ConfigParams;
//    QTextBrowser *logConsole;
    Ui::MainWindow *ui;
    QGraphicsSvgItem *item;
    QGraphicsSvgItem *item1;
    SVG_Viewer *SVGs;
    QFileDialog WinSelectFile;

};
#endif // MAINWINDOW_H
