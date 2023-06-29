RISC-V Basic Simulation
=======

Approach
-----------
With the QT library, you can use the QSvgRenderer class to render SVG in a viewer such as QGraphicsScene.

To load the SVG into volatile memory and make modifications, the program loads the SVG file as a string into a QByteArray, interprets it as a Document Object Model (DOM) into a QDomDocument and makes modifications using the methods of the QDomElement class. This facilitates the browsing and manipulation of XML nodes and attributes in the SVG file.

Once the modifications have been made, the program reconverts to a QByteArray and finally updates the object using the setSharedRenderer method of the QGraphicSvgItem class.
SVG rendering is updated by deleting the old rendering and creating a new one (SVG_Viewer::refresh() method).
