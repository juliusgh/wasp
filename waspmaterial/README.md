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

# Materials
Material contains 17 different JSON material composition files. Each file utilizes a universal format to store data in a series of data objects and data arrays. Refer to the JSON Object Parser in wasp JSON for further details.

# Mass Database
* The Masses class defines the various components found within a masses database. It includes methods to build private members and the members of its subclass (Element).

*The Isotope class defines the isotopes of a given element inside a material masses file. It creates members to store an isotope's mass number, atomic mass, and abundance. The class serves as a subclass to the later-defined Element class.

# Material Database

# FakeGUI

# Testing Framework