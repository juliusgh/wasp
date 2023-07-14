#ifndef WASP_MASSDATABASE_H
#define WASP_MASSDATABASE_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>

#include "waspjson\JSONObjectParser.hpp"
#include "waspcore\Object.h"
using namespace std;
using namespace wasp;

/** The Isotope class defines the isotopes of a given element inside a material masses file.
 * It creates members to store an isotope's mass number, atomic mass, and abundance.
 * The class serves as a subclass to the later-defined Element class.
*/

class Isotope {
    int massNum;
    double mass;
    double abundance;
    static const string MASSNUMBER;
    static const string MASS;
    static const string ABUNDANCE;

    public:
        Isotope() {
            massNum = 0;
            mass = 0.0;
            abundance = 0.0;
        }

        void setMassNum(int a) {massNum=a;}
        void setMass(double a) {mass=a;}
        void setAbundance(double a) {abundance=a;}

        int getMassNum() {return massNum;}
        double getMass() {return mass;}
        double getAbundance() {return abundance;}
};

/** The Masses class defines the various components found within a masses database.
 * It includes methods to build private members and the members of its subclasses.
*/
class Masses {
    /** The Element class decribes the components found within each element in a masses database.
    */
    class Element{
        int atomNum;
        string symbol;
        double mass;
        string notes;
        vector<Isotope> isotopes;

        static const string ATOMNUM;
        static const string SYMBOL;
        static const string MASS;
        static const string NOTES;
        static const string ISOTOPES;

        public:
            Element() {
                atomNum = 0;
                symbol = string();
                mass = 0.0;
                notes = string();
                isotopes = vector<Isotope>();
            }

            void setAtomNum(int a) {atomNum=a;}
            void setSymbol(string a) {symbol=a;}
            void setMass(double a) {mass=a;}
            void setNotes(string a) {notes=a;}
            void setIsotopes(vector<Isotope> a) {isotopes=a;}

            int getAtomNum() {return atomNum;}
            string getSymbol() {return symbol;}
            double getMass() {return mass;}
            string getNotes() {return notes;}
            vector<Isotope> getIsotopes() {return isotopes;}

            void checkAbundances() {
                int imax = isotopes.size();
                double sum = 0.0;
                for (int i=0; i<imax; i++) {
                    Isotope c = isotopes.at(i);
                    sum += c.getAbundance();
                }
                bool good = sum == 1;
                if(good) {
                    cout << "Natural abundances add to " << 1 << endl;
                }
                else if (sum != 0) {
                    cout << "Natural abundances do not add to 0 or 1" << sum << endl;
                }
            }
            // Displays the members of Element
            void display() {
                cout << "\t" << "Element" << endl;
                cout << "\t\t" << "Atomic Number:  " << atomNum << endl;
                cout << "\t\t" << "Symbol:  " << symbol << endl;
                if (mass>0) {cout << "\t\t" << "Mass:  " << mass << endl;}
                if (notes != "") {cout << "\t\t" << "Notes:  " << notes << endl;}
                // if (comments.size() > 0) {
                //     cout << "\t\t" << "Comments:  ";
                //     for (int i=0; i<comments.size(); i++) {cout << comments.at(i) << "  " << endl;}
                // }
                if (isotopes.size()>0) {
                    cout << "\t\t" << "Isotopes:" << endl;
                    for (int i=0; i<isotopes.size(); i++) {
                        Isotope it = isotopes.at(i);
                        cout << "\t\t   " << it.getMassNum() << "     " << it.getMass();
                        if (it.getAbundance()>0) {cout << " \t" << it.getAbundance();} 
                        cout << endl;
                    }
                }

            }
    };

    string name;
    string reference;
    vector<Element> elements;
    vector<string> notes;

    vector<Element> elemVec{};
    vector<Isotope> isoVec{};

    public:
        Masses() {
            name = string();
            reference = string();
            elements = vector<Element>();
            notes = vector<string>();
        }

        void setName(string a) {name=a;}
        void setRef(string a) {reference=a;}
        void setElements(vector<Element> a) {elements=a;}
        void setNotes(vector<string> a) {notes=a;}

        string getName() {return name;}
        string getRef() {return reference;}
        int getElems() {return elements.size();}
        Element getElem(int i) {return elements[i];}
        vector<string> getNotes() {return notes;}

        /** Invoke the parser to build members of the entire file.
         * @param path      path to masses json database
         * @param cerr      stream name for error messages
         * @return          true if successfully built
         */
        bool build(const std::string& path, std::ostream& cerr){
            std::ifstream input(path);
            DataObject::SP json_ptr;
            {
                JSONObjectParser generator(json_ptr, input, std::cerr, nullptr);
                if (0 != generator.parse()) {
                    return false;
                }
            }
            DataObject* json = (json_ptr.get());
            return build_masses(json, cerr);
        }

        /** Builds members of the Masses class.
         * @param masses    head data object of the masses database file
         * @param cerr      stream name for error messages
         * @return          true if successfully built
         */
        bool build_masses(DataObject* masses, std::ostream& cerr) {
            wasp_require(masses != nullptr);
            auto itr = masses->find("Masses");
            if (itr == masses->end()) {
                cerr << "Unable to find Masses database!" << endl;
                return false;
            }
            if (!itr-> second.is_object()){
                cerr << "Masses is not an object and is in unrecognized format." << endl;
                return false;
            }
            DataObject* ms = itr -> second.to_object();

            itr = ms->find("Name");
            setName(itr->second.to_string());
            itr = ms->find("Reference");
            setRef(itr->second.to_string());
            
            itr = ms->find("Notes");
            vector<string> notesVec{};
            if (itr != ms->end()) {
                DataArray* notes = itr->second.to_array();
                for (auto nitr = notes->begin(); nitr != notes->end(); nitr++) {
                    auto n = *nitr;
                    notesVec.push_back(n.to_string());
                }
                setNotes(notesVec);
            }

            itr = ms->find("Elements");
            if (itr == ms->end()) {
                cerr << "Unable to find Elements array!" << endl;
                return false;
            }
            else if (!itr-> second.is_array()){
                cerr << "Materials is not an array!" << endl;
                return false;
            }
            DataArray* elements = itr->second.to_array();
            return build_elements(elements, cerr);
        }

        /** Builds the data objects inside the "Element" data array into Element objects.
         * @param elemArray    data array of element data objects
         * @param cerr         stream name for error messages
         * @return             true if successfully built
         */
        bool build_elements(DataArray* elemArray, std::ostream& cerr){
            wasp_require(elemArray != nullptr);
            elemVec = {};
            bool success = true;
            for (auto itr= elemArray->begin(); itr != elemArray->end(); itr++) {
                Value& element = *itr;
                if (!element.is_object()) {
                    cerr << "Element is not an object and is in an unrecognized format." << endl;
                    success = false;
                }
                else {
                    success &= build_element(element.to_object(), cerr);
                }
            }
            setElements(elemVec);
            return success;
        }

        /** Builds each data object inside the "Element" data array
         * @param element   data object representing an element
         * @param cerr      stream name for error messages
         * @return          true if successfully built
         */
        bool build_element(DataObject* element, std::ostream& cerr){
            wasp_require(element != nullptr);
            Element e;
            auto itr = element->find("AtomicNumber");
            if (itr == element->end()) {
                cerr << "Unable to find element atomic number!" << std::endl;
                return false;
            }
            else if (!itr->second.is_int()) {cerr << "Atomic number is not an int." << endl;}
            e.setAtomNum(itr->second.to_int());

            itr = element->find("Symbol");
            if (itr == element->end()) {
                cerr << "Unable to find element symbol!" << std::endl;
                return false;
            }
            else if (!itr->second.is_string()) {cerr << "Symbol is not a string." << endl;}
            e.setSymbol(itr->second.to_string());

            itr = element->find("Mass");
            if (itr != element->end() && itr->second.is_double()) {e.setMass(itr->second.to_double());}

            itr = element->find("Notes");
            if (itr != element->end() && itr->second.is_string()) {e.setNotes(itr->second.to_string());}

            itr = element->find("Isotopes");
            if (itr == element->end()) {
                cerr << "Unable to find element isotopes!" << endl;
            }
            else if (!itr->second.is_array()) {cerr << "Isotopes is not an array." << endl;}
            else {
                DataArray* isotopes = itr->second.to_array();
                if (!build_element_isotopes(isotopes, e, cerr)) {
                    cerr << "No isotopes!";
                    return false;
                }
            }
            e.setIsotopes(isoVec);
            elemVec.push_back(e);
            return true;
        }

        /** Builds the data objects inside the "Isotopes" data array
         * @param isotopes   data array of isotope data objects
         * @param cerr       stream name for error messages
         * @return           true if successfully built
         */
        bool build_element_isotopes(DataArray* isotopes, Element e, std::ostream& cerr) {
            isoVec = {};
            for (auto itr = isotopes->begin(); itr != isotopes->end(); itr++) {
                Value& iso = *itr;
                bool success = true;
                if (!iso.is_object()) {
                    cerr << "Isotope is not an object and is in an unrecognized format." << endl;
                }
                else {
                    success &= build_isotope(iso.to_object(), cerr);
                }
            }
            return true;
        }

        /** Builds each data object inside the "Isotopes" data array
         * @param iso     data object representing an isotope
         * @param cerr    stream name for error messages
         * @return        true if successfully built
         */
        bool build_isotope(DataObject* iso, std::ostream& cerr) {
            wasp_require(comp != nullptr);
            Isotope i;

            auto itr = iso->find("MassNumber");
            if (itr != iso->end() && itr->second.is_int()) {i.setMassNum(itr->second.to_int());}
            else {return false;}

            itr = iso->find("Mass");
            if (itr != iso->end() && itr->second.is_double()) {i.setMass(itr->second.to_double());}
            else {return false;}

            itr = iso->find("Abundance");
            if (itr != iso->end() && itr->second.is_double()) {i.setAbundance(itr->second.to_double());}

            isoVec.push_back(i);
            return true;
        }

        // Displays the members of Masses
        void display(bool verbose) {
            int eNum = getElems();
            cout << "Mass Database " << getName() << " with " << eNum << " elements" << endl;
            cout << "    Notes:";
            for (int n=0; n<notes.size(); n++) {cout << "     " << notes.at(n) << endl;}
            if(eNum>0){
                if(verbose){
                    for(int i=0; i<eNum; i++){
                        Element element = elements.at(i);
                        element.display();
                    }
                }
                else {
                Element element = elements.at(0);
                element.display();
                cout << endl << "     ..." << endl << endl;
                element = elements.at(eNum-1);
                element.display();
                }
            }
            cout << endl;
            cout << "    Reference:  " << endl << "        " << reference << endl;
        }
};

const string Masses::Element::ATOMNUM = "AtomicNumber";
const string Masses::Element::SYMBOL = "Symbol";
const string Masses::Element::MASS = "Mass";
const string Masses::Element::NOTES = "Notes";
const string Masses::Element::ISOTOPES = "Isotopes";

const string Isotope::MASSNUMBER = "MassNumber";
const string Isotope::MASS = "Mass";
const string Isotope::ABUNDANCE = "Abundance";
#endif // WASP_MASSDATABASE_H