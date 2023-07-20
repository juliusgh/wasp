#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <stack>
#include <tuple>
#include <cstdlib>
#include <sstream>
#include <stdexcept>
#include <functional>
#include <algorithm>

#include "waspjson\JSONObjectParser.hpp"
#include "waspcore\Object.h"
#include "waspmaterial/MassDatabase.h"
using namespace std;
using namespace wasp;


Masses mass;
class Database {
    class Component {
        string elem;
        double amount;
        int massNum;
        bool atom;
        bool iso;
        static const string ELEMENT;
        static const string WEIGHTFRACTION;
        static const string ATOMFRACTION;

        public:
            // Constructor:
            Component() {
                elem = string();
                amount = 0.0;
                massNum = 0;
                atom = true;
                iso = false;
            }

            // Set Methods:
            void setElement(string a) {elem = a;}
            void setAmount(double a) {amount = a;}
            void setMassNum(int a) {massNum = a;}
            void setAtom(bool a) {atom = a;}
            void setIso(bool a) {iso = a;}

            // Get Methods:
            string getElement() {return elem;}
            double getAmount() {return amount;}
            int getMassNum() {return massNum;}
            bool getAtom() {return atom;}
            bool getIso() {return iso;}
    };
    class Contact {
        string name;
        string phone;
        string email;
        static const string NAME;
        static const string PHONE;
        static const string EMAIL;

        public:
            // Contructor:
            Contact() {
                name = string();
                phone = string();
                email = string();
            }
            Contact(string n, string p, string e) {
                name = n;
                phone = p;
                email = e;
            }

            // Set Methods:
            void setName(string a) {name = a;}
            void setPhone(string a) {phone = a;}
            void setEmail(string a) {email = a;}

            // Get Methods:
            string getName() {return name;}
            string getPhone() {return phone;}
            string getEmail() {return email;}

            void display() {
                if (name != "" && phone != "" && email != "") {
                    cout << "\t\t" << "Contact" << endl;
                    cout << "\t\t   " << "Name:  " << name << endl;
                    cout << "\t\t   " << "Phone:  " << phone << endl;
                    cout << "\t\t   " << "Email:  " << email << endl;
                }
            }
    };
    class Material { 
        string name;
        string type;
        string native;
        string formula;
        string source;
        vector<Component> contains;
        vector<Component> nativeComps;
        string comment;
        vector<string> comments;
        string reference;
        vector<string> references;
        vector<string> abbreviations;
        string alias;
        vector<string> aliases;
        double density;
        Contact contact;
        vector<string> symb;
        bool isIso;

        static const string NAME;
        static const string FORMULA;
        static const string DENSITY;
        static const string COMMENT;
        static const string ELEMENTS;
        static const string REFERENCES;
        static const string ABBREVIATIONS;
        static const string ALIASES;
        static const string SOURCE;
        static const string CONTACT;

        public:
            //Constructor:
            Material(){
                name = string();
                formula = string();
                comment = string();
                comments = vector<string>();
                reference = string();
                references = vector<string>();
                abbreviations = vector<string>();
                alias = string();
                aliases = vector<string>();
                source = string();
                Contact *p = &contact; p = nullptr;
                contains = vector<Component>();
                nativeComps = vector<Component>();
                density = 0.0;
                symb = vector<string>();
                isIso = false;
            }

            // Set Methods:
            void setName (string a) {name = a;}
            void setType (string a) {type = a;}
            void setNative (string a) {if (a=="Chemical Formula") {native = "Atoms Per Molecule";} else {native = a;}}
            void setFormula (string a) {formula = a;}
            void setDensity (double a) {density = a;}
            void setComment (string a) {comment = a;}
            void setComments (vector<string> a) {comments = a;}
            void setElements (vector<Component> a) {contains = a;}
            void setNativeComps (vector<Component> a) {nativeComps = a;}
            void setRef (string a) {reference = a;}
            void setRefs (vector<string> a) {references = a;}
            void setAbbreviations (vector<string> a) {abbreviations = a;}
            void setAlias (string a) {alias = a;}
            void setAliases (vector<string> a) {aliases = a;}
            void setSource (string a) {source = a;}
            void setContact (Contact a) {contact = a;}
            void setSymb (vector<string> a) {symb = a;}
            void setIso (bool a) {isIso = a;}

            // Get Methods:
            string getName() {return name;}
            string getType() {return type;}
            string getNative() {return native;}
            string getFormula() {return formula;}
            double getDensity() {return density;}
            string getComment() {return comment;}
            vector<string> getComments() {return comments;}
            vector<Component> getElements() {return contains;}
            string getRef() {return reference;}
            vector<string> getRefs() {return references;}
            vector<string> getAbbreviations() {return abbreviations;}
            string getAlias() {return alias;}
            vector<string> getAliases() {return aliases;}
            string getSource() {return source;}
            Contact getContact() {return contact;}
            vector<string> getSymb() {return symb;}
            bool getIso() {return isIso;}

            void convert(string style, bool iso) { // if faster to use switch statement, come back and use enum+map to use on strings
                // cout << name << "  " << type << "_" << isIso << " to " << style << "_" << iso<< endl;
                if (style == "Native") {contains = nativeComps; type = native; 
                for (auto mat: contains) {cout << mat.getElement() << " " << mat.getAmount() << endl;}
                return;
                }
                vector<double> atomMasses {};
                
                // if (type == "Weight Fractions" && style == "Atom Fractions" && isIso) { // Iso to Elem
                //     for (int i=0; i<contains.size()-1; i++) {
                //         Component ci = contains.at(i);
                //         for (int m=0; m<mass.getElems(); m++) {
                //             if (ci.getElement() == mass.getElem(m).getSymbol()) {
                //                 auto e = mass.getElem(m);
                //                 if (ci.getElement() == contains.at(i+1).getElement()) {
                //                     for (int n=0; n<e.getIsotopes().size(); n++) {
                //                         if (ci.getMassNum() == e.getIsotopes().at(n).getMassNum()) {
                //                             contains.at(i).setAmount(ci.getAmount() / e.getIsotopes().at(n).getMass());
                //                             contains.at(i).setMassNum(0);
                //                             // cout << "here" << endl;
                //                             if (i>0 && contains.at(i-1).getElement() == ci.getElement()) {
                //                                 contains.at(i).setAmount(contains.at(i-1).getAmount()+contains.at(i).getAmount());
                //                                 contains.erase(contains.begin()+i); cout << "no" << endl;
                //                                 i--;
                //                                 ci = contains.at(i);
                //                             }
                //                         }
                //                     }
                //                 // cout << contains.at(i).getAmount() << endl;
                //                 contains.at(i).setAmount(contains.at(i).getAmount()*e.getMass());
                //                 }
                //             }
                //             break;
                //         }
                //     }
                // }
                // for (auto mat: contains) {cout << mat.getElement() << " " << mat.getAmount() << endl;}
                // else if (type == style && style == "Atom Fractions" && isIso != iso) {
                //     if (!isIso) {
                //         convert("Weight Fractions", !isIso);
                //         convert("Atom Fractions", isIso);
                //     }
                // }
                
                // 1-4) Combos between Weight and Atom Fractions
                if ((type == "Weight Fractions" || type == "Atom Fractions") && (style == "Weight Fractions" || style == "Atom Fractions")) {
                    if (type == style && style == "Weight Fractions" && isIso != iso) { //WF Elem to Iso
                        if (!isIso) { //Elem to Iso
                            convert("Atom Fractions", !isIso);
                            convert("Weight Fractions", isIso);
                        }
                        return;
                    }
                    vector<Component> newComps {};
                    double total = 0;
                    for (int i=0; i<contains.size(); i++) {
                        Component ci = contains.at(i);
                        for (int m=0; m<mass.getElems(); m++) {
                            if (ci.getElement() == mass.getElem(m).getSymbol()) {
                                auto e = mass.getElem(m);
                                if (iso) { // Do comps need to be AF to add/delete iso or can conversions be stacked?
                                    int count = -1;
                                    for (int n=0; n<e.getIsotopes().size(); n++) {
                                        if (e.getIsotopes().at(n).getMassNum() == ci.getMassNum()) { //Matching isotopes specified in JSON
                                            cout << "Specified Iso" << endl;
                                            // ci.setAmount(ci.getAmount());
                                            newComps.push_back(ci);
                                            // if (type == "Atom Fractions" && style == "Atom Fractions") {ci.setAmount(ci.getAmount());}
                                            // else if (type == "Atom Fractions" && style == "Weight Fractions") {ci.setAmount(ci.getAmount() * e.getIsotopes().at(n).getMass());}
                                            // else if (type == "Weight Fractions" && style == "Atom Fractions") {ci.setAmount(ci.getAmount() / e.getIsotopes().at(n).getMass());}
                                            total += ci.getAmount();
                                        }
                                        else if (e.getIsotopes().at(n).getAbundance() > 0) { //Renormalizing into isotopes
                                            if (ci.getMassNum() <= 0) {
                                                // cout << "Renorm Iso " << ci.getElement() << endl;
                                                // count ++;
                                                Component ciso;
                                                ciso.setElement(ci.getElement());
                                                
                                                if (type == "Atom Fractions" && style == "Atom Fractions") {ciso.setAmount(ci.getAmount() * e.getIsotopes().at(n).getAbundance());}
                                                else if (type == "Atom Fractions" && style == "Weight Fractions") {ciso.setAmount(ci.getAmount() * e.getIsotopes().at(n).getAbundance() * e.getIsotopes().at(n).getMass());}
                                                else if (type == "Weight Fractions" && style == "Atom Fractions") {ciso.setAmount(ci.getAmount() * e.getIsotopes().at(n).getAbundance() / e.getIsotopes().at(n).getMass());}
                                                ciso.setMassNum(e.getIsotopes().at(n).getMassNum()); // Use negative MassNums for expanded isotopes?
                                                // cout << ciso.getMassNum() << " " << ciso.getAmount() << endl;
                                                newComps.push_back(ciso);
                                                total += ciso.getAmount();
                                            }
                                        }
                                    }
                                }
                                
                                else if (!iso) { //Splitting into elements
                                // Modify to accomodate both for WF and AF
                                    //cout << "Elemental" << endl;
                                    
                                    for (int n=0; n<contains.size()-1; n++) {
                                        if (contains.at(n).getElement() == contains.at(n+1).getElement() && !contains.at(n).getIso()) {
                                            // if (contains.at(n).getMassNum() > 0) {contains.at(n).setMassNum(0);}
                                            contains.at(n).setMassNum(0);
                                            contains.at(n).setAmount(contains.at(n).getAmount()+contains.at(n+1).getAmount());
                                            // cout << contains.at(n+1).getElement() << contains.at(n+1).getAmount() << endl;
                                            contains.erase(contains.begin()+n+1);
                                            n--;
                                        }
                                        else {
                                        //     cout << contains.at(n).getElement() << " " << contains.at(n).getAmount()<< endl;
                                            // if (type == "Atom Fractions" && style == "Weight Fractions") {cout << n << contains.at(n).getElement() << " "; total += ci.getAmount() * e.getMass(); cout << total << endl;}
                                            // if (type == "Weight Fractions" && style == "Atom Fractions") {total += ci.getAmount() / e.getMass();}
                                        }
                                    }
                                    for (auto comp: contains) {newComps.push_back(comp);}
                                    total += ci.getAmount();
                                    i = contains.size()-1;
                                    // if (type == "Atom Fractions" && style == "Weight Fractions") {total += ci.getAmount() * e.getMass();}
                                    // if (type == "Weight Fractions" && style == "Atom Fractions") {total += ci.getAmount() / e.getMass();}
                                }

                                // else {
                                //     if (type == "Atom Fractions" && style == "Weight Fractions") {total += ci.getAmount() * mass.getElem(m).getMass();}
                                //     else if (type == "Weight Fractions" && style == "Atom Fractions") {total += ci.getAmount() / mass.getElem(m).getMass();}
                                // }
                                // cout << "newComps " << newComps.size() << endl;
                                if (type == "Atom Fractions" && style == "Atom Fractions") {atomMasses.push_back(ci.getAmount());}
                                else if (type == "Weight Fractions" && style == "Atom Fractions") {atomMasses.push_back(ci.getAmount() / mass.getElem(m).getMass());} 
                                // else {atomMasses.push_back(ci.getAmount() / mass.getElem(m).getMass());}
                                else if (type == "Atom Fractions" && style == "Weight Fractions") {atomMasses.push_back(ci.getAmount() * mass.getElem(m).getMass());}
                                break;
                            }
                        }
                    }
                    contains.clear();
                    contains = newComps;
                    // cout << contains.size() << " " << atomMasses.size() << endl;
                    // cout << total << endl;
                    for (int j=0; j<contains.size(); j++) {
                            if (type == "Atom Fractions" && style == "Atom Fractions") {}
                            else {contains.at(j).setAmount(atomMasses.at(j)/total);}
                            Component c = contains.at(j);
                            // cout << c.getElement() << " " << contains.at(j).getAmount() << endl;
                    }
                }

                else if (type != style) {


                    

                    // 1) Weight Fractions to Atom Fractions [Still need elem->iso]
                    // if (type == "Weight Fractions" && style == "Atom Fractions") {
                    //     // cout << type << " to " << style << endl;
                    //     double total = 0;
                    //     for (int i=0; i<contains.size(); i++) {
                    //         Component ci = contains.at(i);
                    //         for (int m=0; m<mass.getElems(); m++) {
                    //             if (ci.getElement() == mass.getElem(m).getSymbol()) {
                    //                 total += ci.getAmount() / mass.getElem(m).getMass(); 
                    //                 atomMasses.push_back(ci.getAmount() / mass.getElem(m).getMass());
                    //                 break;
                    //             }
                    //         }
                    //     }
                    //     for (int j=0; j<contains.size(); j++) {
                    //         contains.at(j).setAmount(atomMasses.at(j)/total);
                    //         Component c = contains.at(j);
                    //         cout << c.getElement() << " " << contains.at(j).getAmount() << endl;
                    //     }
                    // }

                    // 2) Atom Fractions to Weight Fractions [Still need iso-> elem]
                    // else if (type == "Atom Fractions" && style == "Weight Fractions") {
                        // cout << type << " to " << style << endl;
                        // double total = 0;
                        // for (int i=0; i<contains.size(); i++) {
                        //     Component ci = contains.at(i);
                        //     for (int m=0; m<mass.getElems(); m++) {
                        //         if (ci.getElement() == mass.getElem(m).getSymbol()) {
                        //             auto e = mass.getElem(m);
                        //             if (iso && ci.getMassNum() <= 0) {
                        //                 int count = -1;
                        //                 contains.erase(contains.begin()+i);
                        //                 for (int n=0; n<e.getIsotopes().size(); n++) {
                        //                     if (e.getIsotopes().at(n).getAbundance() > 0) {
                        //                         count ++;
                        //                         Component ciso;
                        //                         ciso.setElement(ci.getElement());
                        //                         ciso.setAmount(ci.getAmount() * e.getIsotopes().at(n).getAbundance() * e.getIsotopes().at(n).getMass());
                        //                         ciso.setMassNum(e.getIsotopes().at(n).getMassNum());
                        //                         contains.insert(contains.begin()+i+count, ciso);
                        //                         total += ciso.getAmount();
                        //                     }
                        //                 }
                        //             }
                        //             else if (isIso && !iso) {
                        //                 for (int n=0; n<contains.size()-1; n++) {
                        //                     if (contains.at(n).getElement() == contains.at(n+1).getElement()) {
                        //                         contains.at(n).setAmount(contains.at(n).getAmount()+contains.at(n+1).getAmount());
                        //                         contains.erase(contains.begin()+n);
                        //                         n--;
                        //                     } 
                        //                     else {total += ci.getAmount() * e.getMass();}
                        //                 }
                        //             }

                        //             else {total += ci.getAmount() * mass.getElem(m).getMass();}
                        //             atomMasses.push_back(ci.getAmount() * mass.getElem(m).getMass());
                        //             break;
                        //         }
                        //     }
                        // }
                    //     for (int j=0; j<contains.size(); j++) {
                    //         contains.at(j).setAmount(atomMasses.at(j)/total);
                    //         Component c = contains.at(j);
                    //         cout << c.getElement() << " " << contains.at(j).getAmount() << endl;
                    //     }
                    // }
                    
                    // 5) Atoms Per Molecule to Weight Fractions [Works]
                    if (type == "Atoms Per Molecule" && style == "Weight Fractions") {
                        double total = 0;
                        for (int i=0; i<contains.size(); i++) {
                            Component ci = contains.at(i);
                            for (int m=0; m<mass.getElems(); m++) {
                                auto e = mass.getElem(m);
                                if (ci.getElement() == e.getSymbol()) {
                                    if (ci.getMassNum()>0){
                                        for (int p=0; p<e.getIsotopes().size(); p++){
                                            if (ci.getMassNum() == e.getIsotopes().at(p).getMassNum()) {total += ci.getAmount() * e.getIsotopes().at(p).getMass();}
                                        }
                                    }
                                    else { // Expand elements into isotopes
                                        total += ci.getAmount() * mass.getElem(m).getMass();
                                    }
                                    atomMasses.push_back(ci.getAmount() * mass.getElem(m).getMass());
                                    break;
                                }
                            }
                        }
                        for (int j=0; j<contains.size(); j++) {
                            contains.at(j).setAmount(atomMasses.at(j) / total);
                            Component c = contains.at(j);
                            cout << c.getElement() << " " << c.getAmount() << endl;
                        }
                    }
                    
                    // 6) Atom Fractions to APM (Only for Elemental and clean numbers) [Works]
                    else if (type == "Atom Fractions" && style == "Atoms Per Molecule" && formula != "") {
                        cout << formula << endl;
                        double min = 1.0;
                        for (int i=0; i<contains.size(); i++) {
                            if (i != 0 && contains.at(i).getElement() == contains.at(i-1).getElement()) {
                                contains.at(i-1).setAmount(contains.at(i-1).getAmount()+contains.at(i).getAmount());
                                // cout << i << endl;
                                contains.erase(contains.begin()+i);
                                i--;
                            } 
                            else if (i != contains.size()-1 && contains.at(i).getElement() != contains.at(i+1).getElement()) {
                                double mAtom = contains.at(i).getAmount();
                                if (min > mAtom) {min = mAtom;}
                            }
                        }
                        for (int j=0; j<contains.size(); j++) {
                            // cout << contains.at(j).getAmount() << endl;
                            contains.at(j).setAmount(contains.at(j).getAmount()/min);
                        }
                        double mult = 1;
                        int len = contains.at(0).getElement().length();
                        int start = formula.find(contains.at(0).getElement());
                        if (start != string::npos) {
                                if (start != -1 && (isdigit(formula[start+len]) && formula[start+len] != char(contains.at(0).getAmount()))) {
                                    double digit = stod(string(1, formula[start+len]));
                                    if (isdigit(formula[start+len+1])) {
                                        digit = 10*digit + stod(string(1, formula[start+len+1]));
                                        if (isdigit(formula[start+len+2])) {digit = 10*digit + stod(string(1, formula[start+len+2]));}
                                    }
                                    // cout << digit << " " << contains.at(0).getAmount() << endl;
                                    mult = digit/contains.at(0).getAmount();
                                    // cout << mult << endl;
                                }
                        }
                        for (int k=0; k<contains.size(); k++) {
                            contains.at(k).setAmount(round(mult*contains.at(k).getAmount()));
                            Component ci = contains.at(k);
                            // cout << ci.getElement() << " " << ci.getAmount() << endl;
                        }
                    }
                    
                    // 7) Atoms Per Molecule to Atom Fractions [Adjust for Native conversion and increase precision to 16- or 32-bit)]
                    else if (type == "Atoms Per Molecule" && style == "Atom Fractions" && !iso) { // Reverting back elements to certain isotopes?
                        double total = 0;
                        for (int i=0; i<contains.size(); i++) {
                            total += contains.at(i).getAmount();
                        }
                        for (int j=0; j<contains.size(); j++) {
                            contains.at(j).setAmount(contains.at(j).getAmount()/total);
                            // cout << contains.at(j).getElement() << " " << contains.at(j).getAmount() << endl;
                        }
                    }

                    else {cout << "This conversion is not supported." << endl;}
                }

                if (style != "Native") {setType(style);}
                isIso = iso;
            }
            
            void getInputFormat(string code, string dataStyle, string calcType, string dbName) {
                if (type != dataStyle || isIso != (calcType=="Isotopic")) {convert(dataStyle, calcType == "Isotopic");}
                int aNum = 1;
                if (code == "MAVRIC/KENO") {
                    cout << "'  " << dbName << endl;
                    cout << "'  " << name << ", " << formula << ", " << density << " g/cm^3" << endl;
                    for (int k=0; k<comments.size(); k++) {
                        cout << "'  " << comments.at(k) << endl;
                    }
                    if (comment != "") {cout << "'  " << comment << endl;}


                    // Find a way to add index in Materials vector here
                    if (type == "Atoms Per Molecule") {cout << "     atom";}
                    else if (type == "Weight Fractions") {cout << "     wtpt";}
                    else {cout << "     atpt";}
                    cout << name.substr(0, 12) << "  " << 1 << "  " << density << "  " << contains.size();
                    for (int i=0; i<contains.size(); i++) {
                        Component c = contains.at(i);
                        for (int m=0; m<mass.getElems(); m++) {
                            if (c.getElement() == mass.getElem(m).getSymbol()) {
                                auto e = mass.getElem(m);
                                aNum = e.getAtomNum();
                                cout << endl << "         " << aNum*1000+c.getMassNum() << "   " << 100*c.getAmount();
                                // other 2 numbers stay constant (density && temp)?
                                break;
                            }
                        }
                        // if (calcType == "Isotopic") {
                        //     if (c.getMassNum() > 0) {cout << endl << "         " << aNum*1000+c.getMassNum() << "   " << c.getAmount();}
                        //     else {
                        //         for (int n=0; n<e.getIsotopes().size(); n++) {
                        //             cout << endl << "         " << aNum*1000+e.getIsotopes().at(n).getMassNum() << "   " << c.getAmount() * e.getIsotopes().at(n).getAbundance();
                        //         }
                        //     }
                        // }
                        // else {cout << endl << "         " << aNum*1000 << "   " << c.getAmount();}
                        // // other 2 numbers stay constant (density && temp)?
                    }
                    cout << "   " << "end" << endl << endl;

                    // standard comp (mass density)
                    // standard comp (atom density)
                    
                }
                else if (code == "ORIGEN") {
                    string unit;
                    if (type == "Weight Fractions") {unit="gram";}
                    else {unit="mole";}
                    cout << "% 1 " << unit << " of " << name << " using " << type << endl;

                    unit += "s";
                    cout << "mat {" << endl;
                    cout << "\t" << "iso= [ ";
                    
                    // Convert to atoms per molecule?
                    for (int i=0; i<contains.size(); i++) {
                        Component c = contains.at(i);
                        if (i != 0) {cout << "\t       ";}
                        cout << c.getElement() << "=" << c.getAmount() << " ";
                        if (i != contains.size()-1) {cout << endl;}
                    }
                    cout << "]" << endl << "\t" << "units=";
                    
                    cout << unit << endl << "}" << endl << endl;
                }
                else if (code == "MCNP") {
                    int id = 1;
                    cout << "Enter an ID number: " << endl; cin >> id;  // Should this be index #, or is this determined by the user?
                    cout << "c  " << dbName << endl;
                    cout << "c  " << name << ", " << density << " g/cm^3" << endl;
                    for (int k=0; k<comments.size(); k++) {
                        cout << "c  " << comments.at(k) << endl;
                    }
                    if (comment != "") {cout << "c  " << comment << endl;}
                    for (int i=0; i<contains.size(); i++) {
                        Component c = contains.at(i);
                        if (i==0) {cout << "  m" << id << " ";}
                        else {
                            cout << "     ";
                        }
                        for (int m=0; m<mass.getElems(); m++) {
                            if (c.getElement() == mass.getElem(m).getSymbol()) {aNum = mass.getElem(m).getAtomNum(); break;}
                        }
                        cout << aNum*1000+c.getMassNum() << "  ";
                        if (type == "Weight Fractions") {cout << '-';}
                        cout << c.getAmount() << endl;
                    }
                    // cout << "c "; check(); cout << endl << endl;
                }
                else { // Generic
                    cout << "#  " << dbName << endl;
                    cout << "#  " << name << ", " << density << " g/cm^3" << endl;
                    for (int i=0; i<contains.size(); i++) {
                        Component c = contains.at(i);
                        for (int m=0; m<mass.getElems(); m++) {
                            if (c.getElement() == mass.getElem(m).getSymbol()) {aNum = mass.getElem(m).getAtomNum(); break;}
                        }
                        cout << "       " << aNum << "\t" << c.getElement() << "\t" << c.getMassNum() << "  \t";
                        cout << c.getAmount() << endl;
                    }
                    // if (type == "Weight Fractions") {cout << "c "; check();}
                    cout << "c  Generic " << type << endl << endl;
                }
            }

            void check() {
                int cmax = contains.size();
                if (cmax>0) {
                    Component c = contains.at(0);
                    bool atom = c.getAtom();
                    if(atom){checkAtoms();}
                    else {checkFractions();}
                }
                else {
                    if(density>0.0){
                        cout << "    " << name << "\t" << "density only" << endl;
                    }
                    else {
                        // auto ref = references[0];
                        cout << "    " << name << "\t" << "no comp, no density" << endl;
                    } } }

            string countAtoms(string str) {
            // Using a LinkedHashmap to store elements in insertion order
            map<string, int> mp;
            bool cut = false; bool dec = false;
            int mult = 1; int coeff = 1;
            stack<int> stack;
            vector <tuple<int, int>> nestedVec{};
            while (str.find("sub>") != string::npos) { //Replacing troublesome chars
                if (str.find("<sub>x") != string::npos) {str.replace(str.find("<sub>x"), 6, "");} // Unnecessary since this means the amount is proportional
                if (str.find("<sub>") != string::npos) {str.replace(str.find("<sub>"), 5, "");} // Always followed by </sub>, so this char is also unnecessary
                if (str.find("</sub>") != string::npos) {str.replace(str.find("</sub>"), 6, " ");} // This can show the difference between a subscript number and a leading coefficient. 
                if (str.find("·") != string::npos) {str.replace(str.find("·"), 1, "*");} // Could possible just need " " or "" if mult can still be set to 1
            }
            // 2CH<sub>3</sub>·CH<sub>4</sub> works
            //cout << str << endl;
            for (int i = 0; i < str.length(); i++) {
                int count = 0;
                if (str[i] == '*') {cut = false; i++;}
                char c = str[i];
                string a = string(1, c);
                if (!cut) {mult = 1;}
    
                if (a.find_first_of("0123456789") != string::npos && stack.empty()) { //Leading coefficient
                    cut = true;
                    int z = i;
                    mult = 0; float dec = 0;
                    while (z < str.length()-2 && str[z] >= '0' && str[z] <= '9') {
                        mult = stoi(to_string(mult)+str[z]);
                        z++;
                    }
                    i = z;
                    a = string(1, str[i]);
                }
                if (a == ".") {i++; if (int(str[i])-48 >= 5) {dec = true;} a = string(1, str[i+1]); i++;} // Decimals
                
                if (a == "(" || a == "[") {
                    stack.push(i);  // Pushes an open parethesis or bracket index onto the stack
                    continue;
                }
                else if ((a == ")" || a == "]") && !stack.empty()) {
                    int end = i;
                    // Deals with trailing multipliers
                    if (i < str.length()-1 && string(1, str[i+1]) >= "0" && string(1, str[i+1]) <= "9") {
                        mult *= stoi(string(1, str[i+1]));
                        if (i+2 < str.length() && string(1, str[i+2]) >= "0" && string(1, str[i+2]) <= "9") {
                            // cout << mult << " to ";
                            mult = 10*mult + stoi(string(1, str[i+2]));
                        }
                    }
                    int start = stack.top()+1; // Starting element inside () or [] from the most recent ( or [
                    int len = to_string(mult).length();
                    int stop = -1;
                    // cout << str << endl;
                    for (int r=start; r<end; r++) { // Handles elements inside of () or []
                        string s = string(1, str[r]);
                        if ((s == "(" || s == "[") && !nestedVec.empty()) { // Prevents recounting elements inside of nested parentheses
                            for (int v = 0; v < nestedVec.size(); v++) {
                                // **Careful; the following line assumes the nested () will have a trailing multiplier.
                                if (std::get<0>(nestedVec.at(v)) == r && mult > 1) {mult--; mult *= stoi(string(1, str[std::get<1>(nestedVec.at(v))+1])); stop = std::get<1>(nestedVec.at(v));}
                                else if (std::get<0>(nestedVec.at(v)) == r) {r = std::get<1>(nestedVec.at(v));}
                            }
                        }
                        if (r == stop) {mult /= stoi(string(1, str[end+1])); mult++;}
                        // cout << str[r] << endl;
                        if (str[r] >= '0' && str[r] <= '9' && (str[r-1] == '(' || str[r-1] == '*')) {coeff = stoi(string(1, str[r])); mult *= coeff; continue;}
                        else if (str[r] == '*') {mult /= coeff; coeff = 1;}
                        if (s.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ") != string::npos) {
                            count = 0;
                            string k = "";
                            for (int t = r + 1; t < end; t++) {
                                char q = str.at(t);
                                string st = string(1, q);
                                if (st.find_first_of("abcdefghijklmnopqrstuvwxyz") != string::npos) {
                                    s += st;
                                    count = 0;
                                }
                                else if (st.find_first_of("0123456789") != string::npos) {
                                    k+=st;
                                    count = 1;
                                }
                                else {
                                    r = t - 1;
                                    break;
                                }
                            }
                            if (count == 0) {
                                if (s=="D" || s=="T") {s="H";}
                                if (mp.find(s) == mp.end()) {mp[s] = mult;}
                                else {mp[s] += mult;}
                                // cout << s << mp[s] << " " << mult << "    ";  // cout<< = println() in Java
                            }
                            if (a=="D") {a=" H";}
                            else if (a=="T") {a="H ";}
                            if (mp.find(s) == mp.end() && k != "") {if (dec) {mp[s] = stoi(k)*mult+1; dec = false;} else {mp[s] = stoi(k)*mult;}}
                            else if (k != "") {if (dec) {mp[s] += stoi(k)*mult+1; dec = false;} mp[s] += stoi(k)*mult;}
                            // cout << s << mp[s] << "    ";
                        }
                    }

                    cut = false;
                    i=end;
                    if (mult>1) {i+=len;}
                    // The tuple stores the startingand ending indices of the () or [].
                    tuple <int, int> nested = make_tuple(start-1, end); nestedVec.push_back(nested);
                    stack.pop();
                    continue;
                }
                else if (stack.empty()) { // Normal iteration
                if (a.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ") != string::npos) { //Finds the first letter
                    string k = "";
                    for (int j = i + 1; j < str.length(); j++) {
                        char d = str.at(j);
                        string b = string(1, d);
                        if (b.find_first_of("abcdefghijklmnopqrstuvwxyz") != string::npos) {// Finds the second letter if there is one
                            a += b;
                            // if (mp.find(a) == mp.end()) {mp[a] = 0;}
                            // else {mp[a] += mult;}
                            count = 0;
                        }
                        else if (b.find_first_of("0123456789") != string::npos) {// Finds the corresponding amount
                            k += b;
                            count=1;
                        }
                        else {
                            i = j - 1;
                            break;
                        }
                    }
                    if (count == 0) {// No number after the element -->1*mult
                        if (a=="D") {a=" H";}
                        else if (a=="T") {a="H ";}
                        if (mp.find(a) == mp.end()) {if (dec) {mp[a] = mult+1; dec = false;} mp[a] = mult;}
                        else {if (dec) {mp[a] += mult+1; dec = false;} mp[a] += mult;}
                    }
                    else if (i+1 == str.length()) {mp[a] += mult;}
                    else {// There is a number after the element in the formula
                        if (a=="D") {a=" H";}
                        else if (a=="T") {a="H ";}
                        if (mp.find(a) == mp.end() && k != "") {if (dec) {mp[a] = stoi(k)*mult+1; dec = false;} else {mp[a] = stoi(k)*mult;}}
                        else if (k != "") {if (dec) {mp[a] += stoi(k)*mult+1; dec = false;} else {mp[a] += stoi(k)*mult;}}
                    }
                }
                }
            }
            string f = "";
            for (auto entry : mp) {// Stores the elements with their corresponding amounts into a concatenated string
                f += entry.first;
                if (entry.second > 1) {f+= to_string(entry.second);}
            }
            return f;
            }
            
            void checkAtoms() {
                if (formula.length()>0) {
                    int imax = contains.size();
                    vector<string> symbols{};
                    for (int i=0; i<imax; i++) {
                        Component c = contains.at(i);
                        if (c.getAmount()>1) {
                            if (c.getElement() == "D" || c.getElement() == "T") {symbols.push_back("H" + to_string(int(c.getAmount())));}
                            else {symbols.push_back(c.getElement()+to_string(int(c.getAmount())));}}
                        else {
                            if (c.getElement() == "D" || c.getElement() == "T") {symbols.push_back("H");}
                            else {symbols.push_back(c.getElement());}}
                    }
                    sort(symbols.begin(), symbols.end());
                    string f = countAtoms(formula);
                    while(f.find(" ") != -1) {f.replace(f.find(" "), 1, "");}
                    string lst = "";
                    for (int j=0; j<imax; j++) {
                        lst += symbols.at(j);
                    }
                    bool matches = f == lst;
                    // Weird case where D or T starts first in the formula
                    if (f.length()>1 && f[0] == 'H' && int(f[1])>=65 && int(f[1])<=71) {string specCase = f.substr(1, f.length()-1) + f[0]; matches = specCase == lst;}

                    if(matches){
                        // cout << "    " << name << "\t" << "Okay with " << imax << " element"; 
                        //if (imax>1) {cout << "s";} cout << endl;
                    }
                    else {
                        cout << "    " << name << " " << formula << "\t" << "formula does not match atom amounts" << endl;
                        cout << f << " " << lst << endl;
                    }
                }
                else {
                    cout << "    " << name << "\t" << "No formula given" << endl;
                }
            }
            void checkFractions() {
                int imax = contains.size();
                double sum = 0.0;
                for (int i=0; i<imax; i++) {
                    Component c = contains.at(i);
                    string s = c.getElement();
                    double d = c.getAmount();
                    sum += d;
                }
                
                bool good = sum>0.99999 && sum<1.00001;
                if(good) {
                    cout << "\t" << "Fracs add to 1" << endl;
                }
                else {
                    // string ref = references.at(0);
                    cout << "\t" << name << "\t" << sum << " is not ok " << type << endl;
                }
            }
            void display() {
                cout << "\t" << "Material" << endl;
                cout << "\t\t" << "Name:  " << name << endl;
                if (formula != "") {cout << "\t\t" << "Formula:  " << formula << endl;}
                cout << "\t\t" << "Density:  " << density << endl;
                if (abbreviations.size() > 0) {
                    cout << "\t\t" << "Abbreviation:  ";
                    for (int i=0; i<abbreviations.size(); i++) {cout << abbreviations.at(i) << " ";}
                    cout << endl;
                }
                if (alias != "") {cout << "\t\t" << "Alias:  " << alias << endl;}
                if (aliases.size() > 0) {
                    cout << "\t\t" << "Aliases:  ";
                    for (int i=0; i<aliases.size(); i++) {cout << aliases.at(i) << "  ";}
                    cout << endl;
                    }
                if (comment != "") {cout << "\t\t" << "Comment:  " << comment << endl;}
                if (comments.size() > 0) {
                    cout << "\t\t" << "Comments:" << endl;
                    for (int i=0; i<comment.size(); i++) {cout << "\t\t  " << comment.at(i) << endl;}
                }
                if (contains.size()>0) {
                    cout << "\t\t" << "Components (" << type << "):" << endl;
                    for (int i=0; i<contains.size(); i++) {
                        Component c = contains.at(i);
                        cout << "\t\t   " << c.getElement();
                        cout << "\t" << c.getAmount();
                        if (c.getMassNum()>0) {cout << "   " << "MassNumber: " << c.getMassNum();}
                        cout << endl;
                    }
                }
                if (reference != "") {cout << "\t\t" << "Reference:  " << reference << endl;}
                if (references.size() > 0) {
                    cout << "\t\t" << "References:" << endl;
                    for (int i=0; i<references.size(); i++) {cout << "\t\t   " << references.at(i) << endl;}
                }
                if (source != "") {cout << "\t\t" << "Source:  " << source << endl;}
                contact.display();
            }
    };

    string dbName;
    vector<Material> materials;
    string reference;
    vector<string> comments;

    vector<Material> matVec{};
    vector<Component> compVec{};

    public:
        // Constructor:
        Database() {
            dbName = string();
            materials = vector<Material>();
            reference = string();
            comments = vector<string>();
        }

        // Set Methods:
           void setDB(string a) {dbName = a;}
           void setMats(vector<Material> a) {materials = a;}
           void setRef(string a) {reference = a;}
           void setComments(vector<string> a) {comments = a;}

        // Get Methods:
        string getDB() {return dbName;}
        string getRef() {return reference;}
        vector<string> getComments() {return comments;}
        int getMats () {return materials.size();}
        Material getMat(int i) {return materials[i];}

        void addMat(Material material) {
            materials.push_back(material);
        }

        bool build(const std::string& path, std::ostream& cerr){
            mass.build("C:/Users/k12jsti/source/repos/materialsdatabase/wasp/waspmaterial/materials/NISTmasses.json", cerr);
            std::ifstream input(path);
            DataObject::SP json_ptr;
            {
                JSONObjectParser generator(json_ptr, input, std::cerr, nullptr);
                if (0 != generator.parse()) {
                    return false;
                }
            }
            DataObject* json = (json_ptr.get());
            return build_database(json, cerr);
        }
        bool build_database(DataObject* materialDB, std::ostream& cerr){
            wasp_require(materialDB != nullptr); // throws an exception if contract is violated
            auto itr = materialDB->find("MaterialDatabase");
            if (itr == materialDB->end()) {
                cerr << "Unable to find MaterialDatabase!" << endl;
                return false;
            }
            if (!itr-> second.is_object()){
                cerr << "MaterialDatabase is not an object and is in unrecognized format." << endl;
                return false;
            }
            DataObject* db = itr -> second.to_object();
            
            itr = db->find("Name");
            setDB(itr->second.to_string());
            itr = db->find("Reference");
            setRef(itr->second.to_string());
            
            itr = db->find("Comments");
            vector<string> commVec{};
            if (itr != db->end()) {
                DataArray* comms = itr->second.to_array();
                for (auto citr = comms->begin(); citr!=comms->end(); citr++) {
                    auto c = *citr;
                    commVec.push_back(c.to_string());
                }
                setComments(commVec);
            }

            itr = db->find("Materials");
            if (itr == db->end()) {
                cerr << "Unable to find Materials array!" << endl;
                return false;
            }
            else if (!itr-> second.is_array()){
                cerr << "Materials is not an array!" << endl;
                return false;
            }
            DataArray* materials = itr->second.to_array();
            return build_materials(materials, cerr);
        }
        bool build_materials(DataArray* materialsArray, std::ostream& cerr){
            wasp_require(materialsArray != nullptr);
            matVec = {};
            bool success = true;
            for (auto itr= materialsArray->begin(); itr != materialsArray->end(); itr++) {
                Value& material = *itr;
                if (!material.is_object()) {
                    cerr << "Material is not an object and is in an unrecognized format." << endl;
                    success = false;
                }
                else {
                    success &= build_material(material.to_object(), cerr);
                }
            }
            setMats(matVec);
            return success;
        }
        bool build_material(DataObject* material, std::ostream& cerr){
            wasp_require(material != nullptr);
            Material m;
            // Could we switch to a for loop and still have specific error messages to save runtime?

            auto itr = material->find("Name");
            if (itr == material->end()) {
                cerr << "Unable to find material name!" << std::endl;
                return false;
            }
            else if (!itr->second.is_string()) {cerr << "Name is not a string." << endl;}
            m.setName(itr->second.to_string());

            itr = material->find("Density");
            if (itr == material->end()) {
                cerr << "Unable to find material density!" << std::endl;
                return false;
            }
            else if (!itr->second.is_double() && !itr->second.is_int()) {cerr << m.getName() << " " << "Density is not a double." << endl;}
            m.setDensity(itr->second.to_double());

            itr = material->find("Type");
            if (itr == material->end()) {
                cerr << "Unable to find material type for " << m.getName() << "!" << endl;
            }
            else if (!itr->second.is_string()) {cerr << "Type is not a string." << endl;}
            else {m.setType(itr->second.to_string());
                m.setNative(m.getType());
                if (m.getType() != "Weight Fractions") {
                    itr = material->find("Formula");
                    m.setFormula(itr->second.to_string());
                }
            }

            itr = material->find("Aliases");
            if (itr != material->end() && itr->second.is_string()) {m.setAlias(itr->second.to_string());}
            else if (itr != material->end() && itr->second.is_array()) {
                DataArray* ali = itr->second.to_array();
                vector<string> aliVec{};
                for (auto aitr = ali->begin(); aitr != ali->end(); aitr++) {
                     auto al = *aitr;
                     aliVec.push_back(al.to_string());
                }
                m.setAliases(aliVec);
            }

            itr = material->find("Abbreviations");
            if (itr != material->end() && itr->second.is_array()) {
                DataArray* abb = itr->second.to_array();
                vector<string> abbVec{};
                for (auto aitr = abb->begin(); aitr != abb->end(); aitr++) {
                     auto a = *aitr;
                     abbVec.push_back(a.to_string());
                }
                m.setAbbreviations(abbVec);
            }

            itr = material->find("Comment");
            if (itr != material->end() && itr->second.is_string()) {m.setComment(itr->second.to_string());}
            itr = material->find("Comments");
            if (itr != material->end() && itr->second.is_string()) {m.setComment(itr->second.to_string());}
            else if (itr != material->end() && itr->second.is_array()) {
                DataArray* coms = itr->second.to_array();
                vector<string> comVec{};
                for (auto citr = coms->begin(); citr != coms->end(); citr++) {
                     auto co = *citr;
                     comVec.push_back(co.to_string());
                }
                m.setComments(comVec);
            }
            
            itr = material->find("References");
            if (itr != material->end() && itr->second.is_string()) {m.setRef(itr->second.to_string());}
            itr = material->find("References");
            if (itr != material->end() && itr->second.is_array()) {
                DataArray* refs = itr->second.to_array();
                vector<string> refVec{};
            for (auto ritr = refs->begin(); ritr != refs->end(); ritr++) {
                     auto r = *ritr;
                     refVec.push_back(r.to_string());
                }
            m.setRefs(refVec);
            }

            itr = material->find("Source");
            if (itr != material->end()) {
                 m.setSource(itr->second.to_string());
            }

            itr = material->find("Contact");
            if (itr != material->end()) {
                DataObject* contact = itr->second.to_object();
                // build_contact:
                wasp_require(contact != nullptr);
                Contact con;
                auto itr = contact->find("Name");
                if (itr != contact->end()) {con.setName(itr->second.to_string());}
                itr = contact->find("Email");
                if (itr != contact->end()) {con.setEmail(itr->second.to_string());}
                itr = contact->find("Phone");
                if (itr != contact->end()) {con.setPhone(itr->second.to_string());}
                m.setContact(con);
            }
            

            itr = material->find("Contains");
            if (itr == material->end()) {
                cerr << "Unable to find material components!" << endl;
            }
            else if (!itr->second.is_array()) {cerr << "Contains is not an array." << endl;}
            else {
                DataArray* components = itr->second.to_array();
                if (!build_material_components(components, m, cerr)) {
                    cerr << "No components!";
                    return false;
                }
                m.setElements(compVec);
                m.setNativeComps(compVec);
            }

            // Building vector of element+amt
            vector<string> symbols{};
            int apm = 0;
            for (int i=0; i<m.getElements().size(); i++) {
                Component c = m.getElements().at(i);
                symbols.push_back(c.getElement());
                // get numbers from formula
                // if (c.getAmount()>1) {symbols.push_back(c.getElement()); apm += int(c.getAmount());}
                // else {symbols.push_back(c.getElement()); apm += 1;}
            }
            m.setSymb(symbols);
            // m.setAmtSum(apm);

            matVec.push_back(m);
            // if (m.getType() == "Weight Fractions") {m.getInputFormat("ORIGEN", "Weight Fractions", "Isotopic", dbName); m.checkFractions(); cout << endl;}
            //if (m.getType() == "Weight Fractions") {m.convert("Weight Fractions", true); m.checkFractions(); m.convert("Native", false); cout << endl;}
            // if (m.getType() == "Weight Fractions") {m.convert("Atom Fractions", true); m.checkFractions(); m.convert("Weight Fractions", false); m.checkFractions(); cout << "Native" << endl; m.convert("Native", false); cout << endl;}
            // if (m.getType() == "Atom Fractions") {m.convert("Atom Fractions", true); m.checkFractions(); m.convert("Weight Fractions", true); m.checkFractions(); m.convert("Weight Fractions", false); m.convert("Atom Fractions", false); m.checkFractions(); m.convert("Native", false); cout << endl;}
            if (m.getType() == "Atom Fractions") {m.convert("Atom Fractions", true); m.checkFractions(); m.convert("Atom Fractions", false); m.checkFractions(); m.convert("Native", false);
            //m.convert("Weight Fractions", true); m.checkFractions(); m.convert("Weight Fractions", false); m.checkFractions(); m.convert("Native", false); 
            cout << endl;}
            // if (m.getType() == "Atom Fractions") {m.convert("Weight Fractions", true); m.convert("Atom Fractions", true); m.getInputFormat("MAVRIC/KENO", "Weight Fractions", "Elemental", dbName); m.checkFractions(); cout << endl;}
            // if (m.getType() == "Chemical Formula") {m.checkAtoms();}
            // if (m.getName() == "Acetone") {
                // m.getInputFormat("MAVRIC/KENO", "Weight Fractions", "Elemental", dbName); m.convert("Native", false);
                // m.getInputFormat("ORIGEN", "Weight Fractions", "Elemental", dbName); m.convert("Native", false);
                // m.getInputFormat("MCNP", "Weight Fractions", "Elemental", dbName); m.convert("Native", false);
                // m.getInputFormat("Generic", "Weight Fractions", "Elemental", dbName);
            //}
            return true;
        }
        
        bool build_material_components(DataArray* components, Material m, std::ostream& cerr) {
            compVec = {};
            for (auto itr = components->begin(); itr != components->end(); itr++) {
                Value& comp = *itr;
                bool success = true;
                if (!comp.is_object()) {
                    cerr << "Component is not an object and is in an unrecognized format." << endl;
                    // success = false;
                }
                else {
                    success &= build_component(comp.to_object(), m, cerr);
                }
            }
            return true;
        }
        
        bool build_component(DataObject* comp, Material m, std::ostream& cerr) {
            wasp_require(comp != nullptr);
            Component c;
            // Applies to all 3 formats (is there a reason we would need to differentiate between the two fraction types?)
            
            auto itr = comp->find("Element");
            c.setElement(itr->second.to_string());
            itr = comp->find("MassNumber");
            if (itr != comp->end()) {c.setMassNum(itr->second.to_int()); c.setIso(true);}
            // Weight Fractions
            if (m.getType() == "Weight Fractions") {
                itr = comp->find("Fraction");
                c.setAmount((long double) itr->second.to_double());
                c.setAtom(false);
            }
            // Atom Fractions
            else if (m.getType() == "Atom Fractions") {
                itr = comp->find("Fraction");
                c.setAmount((long double) itr->second.to_double());
                c.setAtom(false);
            }
            // Chemical Formula
            else if (m.getType() == "Chemical Formula") {
                itr = comp->find("Atoms");
                c.setAmount((long double) itr->second.to_double());
                c.setAtom(true);
            }
            else {
                cerr << "Unable to find material composition in the form of fractions or a chemical formula!" << endl;
                return false;
            }
            compVec.push_back(c);
            return true;
        }

        void check() {
            cout << endl << endl << "Checking..." << endl;
            int matNum = materials.size();

            for (int i=0;i<matNum;i++) {
                Material material = materials[i];
                material.check();
            }
        }

        void display(bool verbose) { // Displays the approved files
            // cout << "Database " << getDB() << endl;
            int matNum = getMats();
            cout << "Database " << getDB() << " with " << matNum << " materials" << endl << endl;
            if(matNum>0){
                if(verbose){
                    for(int i=0; i<matNum; i++){
                        Material material = materials.at(i);
                        material.display();
                    }
                }
                else {
                Material material = materials.at(0);
                material.display();
                cout << endl << "     ..." << endl << endl;
                material = materials.at(matNum-1);
                material.display();
                }
            }
            cout << endl;
            cout << "    Reference:  " << endl << "        " << reference << endl;
            if (comments.size()>0) {
                cout << "    Comments:" << endl;
                for (int j=0; j<comments.size(); j++) {cout << "        " << comments.at(j) << endl;}
                cout << endl;
                }
        }
    };

const string Database::Material::NAME = "Name";
const string Database::Material::FORMULA = "Formula";
const string Database::Material::DENSITY = "Density";
const string Database::Material::COMMENT = "Comment";
const string Database::Material::ELEMENTS = "Elements";
const string Database::Material::REFERENCES = "References";
const string Database::Material::ABBREVIATIONS = "Abbreviations";
const string Database::Material::ALIASES = "Aliases";
const string Database::Material::SOURCE = "Source";
const string Database::Material::CONTACT = "Contact";

const string Database::Component::ELEMENT = "Element";
const string Database::Component::WEIGHTFRACTION = "WeightFraction";
const string Database::Component::ATOMFRACTION = "AtomFraction";

const string Database::Contact::NAME = "Name";
const string Database::Contact::PHONE = "Phone";
const string Database::Contact::EMAIL = "Email";