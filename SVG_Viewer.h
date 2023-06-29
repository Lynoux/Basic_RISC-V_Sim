#ifndef TEST_SIG_H
#define TEST_SIG_H

#include <QGraphicsSvgItem>
#include <QObject>
#include <QWheelEvent>
#include <QLabel>
#include <QSvgWidget>
#include <QDomDocument>
#include <QDomDocument>
#include <QApplication>
#include <QThread>
#include <QCoreApplication>


QDomElement findDomElement(QString hierarchy_element, QDomDocument dom);
void showDomElement(QDomElement *dm);
void hideDomElement(QDomElement *dm);
void setTextDomElement(QDomElement *dm, QString str_line);
void setColorDomElement(QDomElement dm, QString color);
void setTextDomElementSim(QDomElement dm, QString str_line);
void hideDomElementSim(QDomElement dm);
void showDomElementSim(QDomElement dm);
void setStyleAttrDomElementSim(QDomElement dm, QString disp_attr);


// A class that has signals and slots.
class SVG_Viewer: public QThread
{
    Q_OBJECT
public:
    SVG_Viewer(QObject *parent);
    QGraphicsSvgItem *getItem();
    QGraphicsSvgItem *getItem1();
    void wheelEvent(QWheelEvent *event);
    QGraphicsSvgItem *getText( void );
    QGraphicsSvgItem *getTotal( void );
    QSvgRenderer *getRenderer ( void );
    QGraphicsSvgItem *getTxt();

    QByteArray getBytes() const;

    QGraphicsScene *getScene() const;

    QDomDocument *getDom() const;
    void setApp(QApplication newApp);

    QDomElement getDom_el() const;

    QStringList *getStrDom() const;

    QDomElement *getElem() const;

    QStringList *getStrDomValues() const;

    QList<unsigned int> *getIndex_elem() const;
    QList<QDomElement> *getListElem() const;
    void refresh();

    QStringList *getPrior_value() const;

public slots:
    void setHighlight(bool);
    void on_verticalSlider_valueChanged(int value);
    void on_lineEdit_textEdited(const QString &arg1);
    void on_lineEdit_returnPressed();
    void checkLeft(bool state);
    void checkRight(bool state);

private:
    QGraphicsSvgItem *item;
    QGraphicsSvgItem *item1;

    QGraphicsSvgItem *text;
    // QGraphicsSvgItem *total;
    QGraphicsSvgItem *txt;
    QGraphicsSvgItem *red;
    QSvgRenderer *renderer;
    QLabel *labl;
    QPaintDevice *paint;
    QSvgWidget * widg;
    QByteArray bytes;
    QGraphicsScene *scene;
    QDomDocument *dom;
    QString *str_line;
    QString display_left;
    QString display_right;
    QString color_left;
    QString color_left_dom;
    QString color_right_dom;
    QString color_right;
    QStringList *prior_value;

    bool left_txt;
    bool right_txt;
    bool ret_dom_el_left, ret_dom_el_right, ret_dom_el_left_txt, ret_dom_el_right_txt;

    QDomElement dom_el_left, dom_el_right;
    QDomElement dom_el_left_txt, dom_el_right_txt;
    QDomElement dom_el;


    QStringList *strDom;
    QStringList *strDomValues;
    QList<QDomElement> *listElem;
    QList<unsigned int> *index_elem;
    QDomElement *elem;

//protected:
//    void run() override;



signals:
    void monSignal(bool value);


};

#endif // TEST_SIG_H
