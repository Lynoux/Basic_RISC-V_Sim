RISC-V Basic Simulation
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
The project contains an SVG_Viewer class for RAM storage and SVG file management. It contains methods that call on those of QDomDocument and QDomElement.

For example, the method for finding an SVG element according to the group hierarchy is: findDomElement from SVG_Viewer. 

The SVG_Viewer class also contains methods for changing :
* the appearance of an element (with the style attribute)
* the content of an XML neud, which corresponds to SVG text values (e.g. 0x221298 being the default values)


Installation & configuration
-----------
