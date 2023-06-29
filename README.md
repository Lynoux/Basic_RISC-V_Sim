RISC-V Basic Visual Simulation
=======

The program
-----------
This program is a demonstration of SVG rendering with QT in order to simulate a RISC-V processor and have visiblity after each CPU clock cycle. 
The program uses the SystemC library to run the simulation.
After clicking on "clock", you can see the output values of each CPU component and the highlighting when a value changes with respect to the last clock cycle.

Approach
-----------
The advantage of SVG is that you can have the RISC-V CPU schematic in this format in the Windows application, and zoom in without any loss of quality.
With the QT library, you can use the QSvgRenderer class to render SVG in a viewer such as QGraphicsScene.

To load the SVG into volatile memory and make modifications, the program loads the SVG file as a string into a QByteArray, interprets it as a Document Object Model (DOM) into a QDomDocument and makes modifications using the methods of the QDomElement class. This facilitates the browsing and manipulation of XML nodes and attributes in the SVG file.

Once the modifications have been made in RAM by the QDomElement methods, this program reconverts the DOM into a QByteArray and finally updates the object using the setSharedRenderer method of the QGraphicSvgItem class.
SVG rendering is updated by deleting the old rendering and creating a new one (SVG_Viewer::refresh() method).


SVG file organization (Schematic_Paco.svg)
-----------
To make it easier to manage the elements to be modified in the SVG file, it was a good idea to create groups and sub-groups.
As a result, only elements contained in the "id=STROKES" group will be modified by the program.
Each element is characterized by a group containing wire connections and values.

You can view the SVG in XML format with a text editor or with the Inkscape editor.
In this way, it is possible to view groups, particularly those in the "id=STROKES" group.

The program thus modifies the values contained in the elements and changes the color when the value changes with respect to the old clock cycle.

How it works
-----------
The project contains an SVG_Viewer class for RAM storage and SVG file management. It contains methods that call on those of QDomDocument and QDomElement classes.

For example, the method for finding an SVG element according to the group hierarchy is: findDomElement from SVG_Viewer. 

The SVG_Viewer class also contains methods for changing :
* the appearance of an element (with the style attribute)
* the content of an XML neud, which corresponds to SVG text values (e.g. 0x221298 being the default values)


Once systemC has been configured and the objects required for SVG management have been built, the main algorithm for visually updating SVG is found in the "MainWindow::onMessage" method.

This method is called after each click on the "clock" button in the main window.

In the first case, when SystemC indicates that this is the first click (when logMessage.contains("Report received: 0 s")), the arraylists required to update the values are initialized. This provides flexibility if you wish to add more components to simulate. This would imply new elements in the "id=STROKED" group of the SVG file, but also new elements in SystemC. This is the reason for the arrayList.
Several ArrayLists are created: 
* QStringList strDom: a list of strings taken from the raw data of the SystemC simulation.
* QList\<QDomElement\> listElem: list of DOM elements found between the SVG file and the simulation data.
* QStringList prior_value: list of prior values (clock cycle before) of all raw data in the SystemC simulation.
* QList\<unsigned int\> index_elem : list of DOM element indexes that match the "id=STROKES" elements in the SVG file and the strDom list for the simulation's raw data. The size of this list is necessarily identical to that of 'QList\<QDomElement\> listElem'.

In the other case, (logMessage.contains("Report received")) i.e. all but "0 s", the program updates the array list necessary for the changes to be made in the SVG file loaded in memory.
These are all the arrayLists except the following:
*QStringList strDom
*QList\<unsigned int\>
After this, the SVG_Viewer::refresh() method performs the visual update.

Installation & configuration
-----------
