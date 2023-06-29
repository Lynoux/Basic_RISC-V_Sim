#include <QWheelEvent>
#include <iostream>
#include <QSvgRenderer>
#include <QLabel>
#include "qgraphicsscene.h"
#include "SVG_Viewer.h"
#include <QMessageBox>
#include <QTextStream>
#include <QFile>
#include <QDataStream>
#include <QChar>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QByteArray>
#include <QDir>
#include <QTime>
#include <QWindow>
#include <QRegularExpression>




#define id_sep ":"
// example id is "svg30/id345/d0", tag is "text", coming from <text id = "d0" ...>
// but we know nothing about the tags of svg or svg/id345

/* Searches for the element according to the hierarchy contained in "hierarchy_element".
 * This hierarchy corresponds to a set of SVG elements encapsulated as groups (XML tag <g></g>).
 * Each element is designated by its unique "id" attribute.
 * For hierarchy_element, each element id is separated with the constant 'id_sep'.
 * If the hierarchy doesn't exist, the function returns a 'null' address.
 */
QDomElement findDomElement(QString hierarchy_element, QDomDocument dom)
{
    QStringList strlist = hierarchy_element.split(id_sep, Qt::SkipEmptyParts);

    /* Extraction pour la hiérarchie xml */
    for(int i=0 ; i<strlist.length() ; i++)
     std::cout<<strlist.at(i).toStdString()<<std::endl;

    bool last_parent = false;

    QDomElement dom_el;

    /* Establishment of a list of nodes according to the searched tag */

    QDomNodeList listD;


    QDomNode node;

    node = dom.firstChildElement();
    std::cout<<"lenght:"<<strlist.length()<<std::endl;
    int i=0;

    //    while((!(node.isNull()) && (i<strlist.length())) || last_parent)
    while((!(node.isNull()) || last_parent) && (i<strlist.length()))
    {

        /* Attribute found : move to the next attribute for the next loop *1 */
        if((node.toElement().attribute("id") == strlist[i]) && (!last_parent))
        {
            std::cout<<i<<std::endl;
            std::cout<<"found:"<<node.toElement().attribute("id").toStdString()<<std::endl;

            if(!(node.firstChild().isNull()))
                node = node.firstChild();

            std::cout<<"final:"<<node.toElement().attribute("id").toStdString()<<std::endl;
            i++;
            std::cout<<i<<std::endl;
        }

        /* Otherwise search in the next node, if there is a next node in the same hierarchy */
        else if(!(node.nextSibling().isNull()))
        {
            node = node.nextSibling();
            std::cout<<"next!"<<std::endl;
        }

        /* Otherwise return to the parent node in case there is no next node and a parent exists */
        else if((node.nextSibling().isNull()) && (!(node.parentNode().isNull())))
        {
            node = node.parentNode();

            /* To repeat the transition to the parent node, the depth level for 'node' must be greater than 0 */
        /* This translates into the passage in this condition when it is the last node of a sub-node */
        if(node.nextSibling().isNull() && i>0)
        {
            std::cout<<"last parent"<<std::endl;
            last_parent = true; // Additional loop if it is the last beacon
        }

        /* Otherwise move to the next node until the node variable is null */
            else
            {
                node = node.nextSibling(); // Moving on to the next node
                last_parent = false;
            }

            std::cout<<node.isNull()<<std::endl;

//            std::cout<<"parent!"<<std::endl;
            i--;
            std::cout<<"parent:"<<i<<"tag:"<<node.nodeName().toStdString()<<std::endl;
        }
    }

    /* As the condition *1 implies a passage to the child node, it is necessary to return to the parent node for the searched node */
    dom_el = node.parentNode().toElement();

    std::cout<<"retour final:"<<dom_el.attribute("id").toStdString()<<" "<<dom_el.isNull()<<std::endl;


    return dom_el;
}


/* Change the style attribute of the SVG element by adding "display:none".
 * This hides the element.
 */
void hideDomElement(QDomElement *dm)
{
    /* Store old content for the style attribute */
    QString prior_attr = dm->toElement().attribute("style");

//    QString prior_attr = dm.toElement().attribute("style");

    std::cout<<"Hide!"<<std::endl;

    /* Added 'display:none' parameter to hide the object from the old content */

    /* Pas la peine d'ajouter cet attribut s'il existe déja */
    if(!(prior_attr.contains("display:none;")))
        dm->setAttribute("style","display:none;"+prior_attr);

//    std::cout<<"Size DOM:"<<dm-><<std::endl;
}



/* Change the style attribute of the SVG element by adding "display:none".
 * This displays the element.
 */
void showDomElement(QDomElement *dm)
{
    std::cout<<"Show!"<<std::endl;
    /* Store old content for the style attribute */
    QString prior_attr = dm->toElement().attribute("style");

    /* Search for the string 'display:none' and delete it from the old content */
    QString disp = "display:none;";
    prior_attr.remove(disp);

//    std::cout<<prior_attr.toStdString()<<std::endl;
    /* Update attribute */
    dm->setAttribute("style", prior_attr);
}


/*
 * Not used for this simulation
 */
void setTextDomElement(QDomElement *dm, QString str_line)
{
    dm->firstChild().setNodeValue(str_line);
}


/* Scroll through the SVG element 'dm' to reach the XML nodes whose attribute is 'hex_out' and 'hex_in', in order to modify the value.
 * This value is a character string contained in the function's 'str_line' parameter.
 */
void setTextDomElementSim(QDomElement dm, QString str_line)
{
    QDomNode node;
    bool ok;
    uint32_t val;

    /* Affichage XX sur le SVG si la valeur de la simulation contient X */
    if(str_line.contains("X"))
    {
        str_line="XX";
    }

    /* Display of the value converted to hexadecimal (character string corresponding to a binary value from the simulation) */
    else
    {
        val = str_line.toUInt(&ok,2); // Convert string to decimal value
        str_line = "0x"+QString::number(val, 16).toUpper(); // Using the decimal value to convert to hexadecimal as a character string
    }

    /* Navigating the SVG file's XML nodes */
    node = dm.firstChildElement();

    node=node.nextSibling();
    node=node.firstChild();

    /* Assign this value only in SVG subgroups with an attribute: sub_id:hex_out or sub_id:hex_in */
    while(!(node.isNull()))
    {
        if(node.toElement().attribute("sub_id") == "hex_out")
        {
            std::cout<<"hex_out!"<<std::endl;
            node.firstChild().firstChild().setNodeValue(str_line);
        }

        else if(node.toElement().attribute("sub_id") == "hex_in")
        {
            std::cout<<"hex_in!"<<std::endl;
            node.firstChild().firstChild().setNodeValue(str_line);
        }
        node = node.nextSibling(); // Moving on to the next node
    }
}


/* Modify the 'style' attribute of the SVG element 'dm' to set color values
 * The color is a string representing a 3-byte hexadecimal value (RGB).
 * For example, ff0000 represents a red color.
 *
 */
void setColorDomElement(QDomElement dm, QString color)
{
    bool ret = false;

    dm=dm.firstChildElement();


    /* ret == false when another ascii than hexa ascii is found */
    color.toUInt(&ret, 16);
    /* Test of the lenght of ascii to be compatible with svg standards */
    if((ret  == false) || (color.length() != 6))
    {
        std::cout<<"Wrong color!"<<std::endl;
    }

    else
    {
        /* Update attribute with new color */
        QString color_attribute = "stroke:#"+color+";stroke-opacity:1;fill:#"+color+";fill-opacity:1";
        dm.setAttribute("style", color_attribute);
    }

    std::cout<<"ret"<<std::endl;
    std::cout<<ret<<std::endl;
    std::cout<<"hex"<<std::endl;

}


SVG_Viewer::SVG_Viewer(QObject *parent)
{

    QFile file(QString( QDir::currentPath()+"/debug/Schematic_Paco.svg")); // Construction of the file object according to the relative path of the SVG file

    file.open(QIODevice::ReadOnly); // Read-only access to SVG files

    scene = new QGraphicsScene(); // Building a scene into which SVG rendering will be loaded

//    str_line = new QString();

    bytes = file.readAll(); // Read the file and assign it to the 'bytes' variable, which will be used to update the SVG rendering.

    dom = new QDomDocument(); // Allows access to the SVG file under a Document Object Model in order to use XML file manipulation functions

    txt = new QGraphicsSvgItem(); // For SVG rendering

    dom->setContent(bytes); // Loading the DOM according to the SVG file in memory 'bytes'.


    /* To interface systemC with SVG rendering */
    strDom = new QStringList();
    listElem = new QList<QDomElement>();

    index_elem = new QList<unsigned int>();

    elem = new QDomElement();
    strDomValues = new QStringList();
    prior_value = new QStringList();

    /* Update rendering according to data in 'bytes', enabling first SVG display */
    bytes = dom->toByteArray();
    renderer = new QSvgRenderer(bytes);
    txt->setSharedRenderer(renderer);
}


/*
 * Not used for this simulation
 */
void SVG_Viewer::checkLeft(bool state) {
    left_txt = state;
}

/*
 * Not used for this simulation
 */
void SVG_Viewer::checkRight(bool state) {
    right_txt = state;
}

/* Get a previous value of systemC to highlight when the value is different from the previous one */
QStringList *SVG_Viewer::getPrior_value() const
{
    return prior_value;
}


/* Each systemC element corresponding to an SVG file equivalence must be indexed.
 * The raw values are updated by systemC, but not all of them are used.
 * This is why there is an array of integers of the same size as the number of equivalences between systemC and the SVG ids.
 * This array stores the SVG element indexes for the QStringList 'strDomValues'.
 */
QList<unsigned int> *SVG_Viewer::getIndex_elem() const
{
    return index_elem;
}


/* List of character strings corresponding to all systemC simulation values.
 * This list is updated each time the 'clock' button is pressed.
 * Some values are used to update the SVG file
 */
QStringList *SVG_Viewer::getStrDomValues() const
{
    return strDomValues;
}


/*
 *  Not used for this simulation
 */
QDomElement *SVG_Viewer::getElem() const
{
    return elem;
}

/* Every element in the SVG file that matches the systemC element is in this list
 *
 */
QList<QDomElement> *SVG_Viewer::getListElem() const
{
    return listElem;
}


/*
 *  Not used for this simulation
 */
QDomElement SVG_Viewer::getDom_el() const
{
    return dom_el;
}


/*
 *  Not used for this simulation
 */
void SVG_Viewer::setHighlight(bool state) {

    if(state)
    {   color_left_dom = "0000ff";
        color_right_dom = "ff0000";
        setColorDomElement(dom_el_left, color_left_dom);
        setColorDomElement(dom_el_right, color_right_dom);
        std::cout<<bytes.toStdString()<<std::endl;
    }

    else
    {
        color_left_dom = "ff0000";
        color_right_dom = "0000ff";
        setColorDomElement(dom_el_left, color_left_dom);
        setColorDomElement(dom_el_right, color_right_dom);
    }

    /* Rafraîchissement de l'item txt */
    bytes = dom->toByteArray();
    auto prior_renderer = txt->renderer();
    txt->setSharedRenderer(new QSvgRenderer(bytes));
    delete prior_renderer;

 }

void SVG_Viewer::refresh()
{
    bytes = dom->toByteArray();
    auto prior_renderer = txt->renderer();
    txt->setSharedRenderer(new QSvgRenderer(bytes));
    delete prior_renderer;
}


void SVG_Viewer::on_lineEdit_textEdited(const QString &arg1)
{
    *str_line = arg1;
    std::cout<< "str_line"<<std::endl;

}


/* Makes the 'dm' element disappear
 * This is achieved by adding the "display:none" parameter to the 'style' attribute
 */
void hideDomElementSim(QDomElement dm)
{
    QDomNode node;

    node = dm.firstChildElement();
    node=node.nextSibling();

    QString prior_attr = node.toElement().attribute("style");
    std::cout<<"hideSim:"<<prior_attr.toStdString()<<std::endl;

    if(prior_attr.contains("display:none;"))
    {
        std::cout<<"hideSim:"<<"already hide!"<<std::endl;
    }
    else
    {
        node.toElement().setAttribute("style","display:none;"+prior_attr);
    }
}

/* Allows you to modify the style attribute of the 'dm' element
 * You can set any attribute you like in disp_attr
 * Example: disp_attr=display:none to hide the element
 * Example: disp_attr=display:inline to make the element reappear
 */
void setStyleAttrDomElementSim(QDomElement dm, QString disp_attr)
{
    QDomNode node;

    node = dm.firstChildElement();
    node=node.nextSibling();

    QString prior_attr = node.toElement().attribute("style");
    prior_attr.replace(QRegularExpression("display:.*"), disp_attr);

    std::cout<<"12:"<<prior_attr.toStdString()<<std::endl;

    node.toElement().setAttribute("style", prior_attr);
}


/* Removes the 'dm' element
 * This is done by deleting the "display:none" parameter in the 'style' attribute
 */
void showDomElementSim(QDomElement dm)
{
    QDomNode node;

    node = dm.firstChildElement();
    node=node.nextSibling();

    std::cout<<"Show!"<<std::endl;
    /* Store old content for the style attribute */
    QString prior_attr = node.toElement().attribute("style");

    /* Search for the string 'display:none' and delete it from the old content */
    QString disp = "display:none;";
    prior_attr.remove(disp);

    //    std::cout<<prior_attr.toStdString()<<std::endl;

    /* Update attribute */
    node.toElement().setAttribute("style", prior_attr);
}


/*
 *  Not used for this simulation
 */
void SVG_Viewer::on_lineEdit_returnPressed()
{
    if(left_txt == true)
    {
        setTextDomElement(&dom_el_left_txt, *str_line);

        showDomElement(&dom_el_left);
        setColorDomElement(dom_el_left, color_left_dom);


    }
    else
    {
        hideDomElement(&dom_el_left);
        setColorDomElement(dom_el_left, color_left_dom);
    }
    if(right_txt == true)
    {
        setTextDomElement(&dom_el_right_txt, *str_line);
        showDomElement(&dom_el_right);
        setColorDomElement(dom_el_right, color_right_dom);
    }
    else
    {
        hideDomElement(&dom_el_right);
        setColorDomElement(dom_el_right, color_right_dom);
    }

    std::cout<<"left"<<left_txt<<std::endl;
    std::cout<<"right"<<right_txt<<std::endl;

    std::cout<<"enter!"<<std::endl;

    bytes = dom->toByteArray();
    std::cout<<bytes.toStdString()<<std::endl;

    auto prior_renderer = txt->renderer();
    txt->setSharedRenderer(new QSvgRenderer(bytes));
    delete prior_renderer;
}


/* Allows you to zoom in on the SVG file using the vertical sliding bar
 *
 */
void SVG_Viewer::on_verticalSlider_valueChanged(int value)
{
    double value_d = ((double)value/50.0)+1;
    std::cout<<value<<std::endl;
    std::cout<<value_d<<std::endl;
    txt->setScale(value_d);
}



/* All getters enabling access to attributes through another class */
QDomDocument *SVG_Viewer::getDom() const
{
    return dom;
}

QStringList *SVG_Viewer::getStrDom() const
{
    return strDom;
}

QGraphicsScene *SVG_Viewer::getScene() const
{
    return scene;
}

QByteArray SVG_Viewer::getBytes() const
{
    return bytes;
}

QGraphicsSvgItem * SVG_Viewer::getItem( void ) {
    return item;
 }

QGraphicsSvgItem * SVG_Viewer::getItem1( void ) {
    return item1;
 }

QGraphicsSvgItem * SVG_Viewer::getText( void ) {
    return text;
 }

QGraphicsSvgItem * SVG_Viewer::getTxt( void ) {
    return txt;
 }

QSvgRenderer * SVG_Viewer::getRenderer( void ) {
    return renderer;
 }



