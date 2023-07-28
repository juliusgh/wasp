Authored by Chance B. Loveday July 2023
# Wasp Material Overview

Wasp Material, which contains the Materials Composition Database project, was developed as a tool to handle radiation transport code input generation.

## Table of Contents
1. [Materials] (#materials)- The material composition databases used for this project
    * NISTmasses
    * ARH-600
    * CRC_ICdatabase
    * CRC_OCdatabase
    * doe-std-3013-2018
    * ICRP89
    * ICRU44
    * ICRU46
    * MarksTable6.1.5
    * MCNP5APrimer
    * NIST_124_mats
    * NIST_126_table2
    * PNNL-15870r2_eff
    * RxHndbkVolCh19-Ch48
    * RxHndbkVolCh51
    * scaleAlloyMixs
    * scaleCompounds
    * ShultisAndFaw1996
2. [Mass Database] (#massdatabase)- The package that defines objects for a masses database
    * Masses Class
    * Isotope Class
    * Element Class
    * Builds
    * Checks
3. [Material Database] (#materialdatabase)- The package that defines objects for a material composition database
4. [Stand-in GUI] (#fakegui)
5. [Testing Framework] (#testingframework)
6. Notes of Interest (#todos)- Notes that are significant to consider before this project is integrated

# Materials
The materials folder inside Material contains 17 different JSON material composition files and one mass database file. Each file utilizes a universal format to store data in a series of data objects and data arrays. Refer to the JSON Object Parser in wasp JSON and the SULI Summer 2023 Report for further details.

# Mass Database
* The Masses class defines the various components found within a masses database. It includes methods to build private members and the members of its subclasses. A series of build functions invoke the JSON object parser and assign values to members of the three classes defined in this header. The Masses class retains a check and a display method that call similar methods in the Element subclass.

* The Element class decribes the components found within each element in a masses database. Each element provides basic atomic information as well as a list of isotopes. An element takes an atomic number, a symbol, a mass, a list of isotopes, and optional notes. The Check Abundances method checks whether the natural abundances (if any) of an element sums to 1. This method serves as an internal check to the masses databases and returns a boolean. A display method also exists for every element.

* The Isotope class defines the isotopes of a given element inside a material masses file. It creates members to store an isotope's mass number, atomic mass, and abundance. The class serves as a subclass to the later-defined Element class. An isotope consists of a mass number, mass, and natural abundance.

# Material Database
* The Database class defines a material composition database that includes a list of materials. It includes methods to build private members and members of its subclasses. A series of build functions invoke the JSON object parser and assign values to members of the four classes defined in this header. This class also contains a display and head check method.

* The Material class defines a material with atomic and compositional information. Certain fields are exclusive to specific database. A material can retain at most 19 unique data members; however, the basic criteria for a material object is a name, type, density, and "contains" (mentioned before material build function).
    * The convert method changes the material compositions to a desired types. There are three compositional types and seven defined conversions between these types are defined. Note that the "Native" type merely calls a copy of the initial components.
    * The getInputFormat method utilizes the convert method to transform compositional data into a formatted radiation transport code input. This method currently supports formats for SCALE (MAVRIC/Keno, ORIGEN), MCNP, and a generic standard.
    * Material has a head check method that calls a check that either ensures the fractional compositions sum to one or that the atom amounts match the amounts derived from the given chemical formula. CountAtoms serves as a helper function for checkAtoms; it builds a map that links atom amounts to an elemental symbol from the given formula. This function works with all materials in the project's 17 databases but currently isn't compatable with formulas containing nested parentheses and multiple coefficents.

* The Component class defines a compositional component of a material. The three specified types of components are "Weight Fractions", "Atom Fractions", and "Chemical Formula"/"Atoms Per Molecule." A component contains a symbol, an amount, mass number, and two booleans representing the composition type.

* The Contact class descibes the relevant contact information. Note that the Contact field is exclusive to the PNNL database.


# FakeGUI
* The fake GUI implementation uses the MatCompLib header, which builds the mass and composition databases for user interaction in the interface. Note that a future version of this project intends to identify compatable files within a folder directory and load the desired databases. The fake GUI is still in early stages of testing; as a result, the interface only accesses one composition database at a time.
* The user is presented a list of materials in the selected database, similar to scroll-down menu, and asked for an index. The user is then prompted for one of seven commands represented by the numbers 0-7; -1 is treated as an escape sequence.
0- Displays the masses as listed in the masses database file
1- Displays the information provided for a given material
2- Searches the database based on the name identifier
3- Conducts an advanced search based several identifiers (currently compatable with name and density)
4- Changes to a new material within the database based on the provided index
5- Checks the fractional or atomic compositions of a given material
6- Generates a radiation transport code input based on three user-provided parameters (code system, desired composition type, and calculation type)

# Testing Framework
* tstMatObject tests the validity of each build within MatCompLib.h.
* tstDBObject tests the validity of the individual database builds and the masses database build. The abundance and composition checks under their respective check methods are called and tested for each element or material. The code input formats, which incorporate conversions, began to be tested as well. The convert and getInputFormat methods still require non-void return types and several test cases before depoyment into a legimate GUI.

# Todos
