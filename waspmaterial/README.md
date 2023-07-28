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
Material contains 17 different JSON material composition files. Each file utilizes a universal format to store data in a series of data objects and data arrays. Refer to the JSON Object Parser in wasp JSON for further details.

# Mass Database
* The Masses class defines the various components found within a masses database. It includes methods to build private members and the members of its subclass (Element).

* The Isotope class defines the isotopes of a given element inside a material masses file. It creates members to store an isotope's mass number, atomic mass, and abundance. The class serves as a subclass to the later-defined Element class.

# Material Database

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
