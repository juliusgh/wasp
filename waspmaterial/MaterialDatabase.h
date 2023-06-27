#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <stack>
#include <valarray>
#include <cstdlib>
#include <sstream>
#include <stdexcept>
#include <functional>
#include <algorithm>
#include "ctype.h"
#include "waspjson\JSONObjectParser.hpp"
#include "waspcore\Object.h"
#include "waspmaterial/MassDatabase.h"
using namespace std;
using namespace wasp;


// int findDeno(double val, int cycles = 10, double precision = 5e-4){
//     int sign = val > 0 ? 1 : -1;
//     val *= sign;
//     double new_val, whole;
//     double decimal_part = val - (int)val;
//     int counter = 0;
//     valarray<double> vec_1{double((int) val), 1}, vec_2{1,0}, temp;
    
//     while(decimal_part > precision & counter < cycles){
//         new_val = 1 / decimal_part;
//         whole = (int) new_val;
//         temp = vec_1;
//         vec_1 = whole * vec_1 + vec_2;
//         vec_2 = temp;
//         decimal_part = new_val - whole;
//         counter += 1;
//     }
//     // cout<<"x: "<< val <<"\tFraction: " << sign * vec_1[0]<<'/'<< vec_1[1]<<endl;
//     return vec_1[1];
// }

Masses mass;
class Database {
    class Component {
        string elem;
        double amount;
        int massNum;
        bool atom;
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
            }

            // Set Methods:
            void setElement(string a) {elem = a;}
            void setAmount(double a) {amount = a;}
            void setMassNum(int a) {massNum = a;}
            void setAtom(bool a) {atom = a;}

            // Get Methods:
            string getElement() {return elem;}
            double getAmount() {return amount;}
            int getMassNum() {return massNum;}
            bool getAtom() {return atom;}

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
        string formula;
        string source;
        vector<Component> contains;
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
        // int amtSum;

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
                density = 0.0;
                symb = vector<string>();
                // amtSum = 0;
            }

            // Set Methods:
            void setName(string a) {name = a;}
            void setType(string a) {type = a;}
            void setFormula(string a) {formula = a;}
            void setDensity(double a) {density = a;}
            void setComment (string a) {comment = a;}
            void setComments (vector<string> a) {comments = a;}
            void setElements (vector<Component> a) {contains = a;}
            void setRef (string a) {reference = a;}
            void setRefs (vector<string> a) {references = a;}
            void setAbbreviations (vector<string> a) {abbreviations = a;}
            void setAlias (string a) {alias = a;}
            void setAliases (vector<string> a) {aliases = a;}
            void setSource (string a) {source = a;}
            void setContact (Contact a) {contact = a;}
            void setSymb (vector<string> a) {symb = a;}
            // void setAmtSum (int a) {amtSum = a;}

            // Get Methods:
            string getName() {return name;}
            string getType() {return type;}
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
            // int getAmts() {return amtSum;}

            void convert(string style) { // if faster to use switch statement, come back and use enum+map to use on strings
                if (type != style && (type != "Chemical Formula" || style != "Atom Per Molecule")) {
                    vector<double> atomMasses {};
                    cout << name << "  " << type << " to " << style << endl;

                    // 1) Weight Fractions to Atom Fractions
                    if (type == "Weight Fractions" && style == "Atom Fractions") {
                        // cout << type << " to " << style << endl;
                        double total = 0;
                        for (int i=0; i<contains.size(); i++) {
                            Component ci = contains.at(i);
                            for (int m=0; m<mass.getElems(); m++) {
                                if (ci.getElement() == mass.getElem(m).getSymbol()) {
                                    total += ci.getAmount() / mass.getElem(m).getMass(); 
                                    atomMasses.push_back(ci.getAmount() / mass.getElem(m).getMass());
                                    break;
                                }
                            }
                        }
                        for (int j=0; j<contains.size(); j++) {
                            contains.at(j).setAmount(atomMasses.at(j)/total);
                            Component c = contains.at(j);
                            cout << c.getElement() << " " << contains.at(j).getAmount() << endl;
                        }
                    }

                    // 2) Atom Fractions to Weight Fractions
                    else if (type == "Atom Fractions" && style == "Weight Fractions") {
                        // cout << type << " to " << style << endl;
                        double total = 0;
                        for (int i=0; i<contains.size(); i++) {
                            Component ci = contains.at(i);
                            for (int m=0; m<mass.getElems(); m++) {
                                if (ci.getElement() == mass.getElem(m).getSymbol()) {
                                    total += ci.getAmount() * mass.getElem(m).getMass(); 
                                    atomMasses.push_back(ci.getAmount() * mass.getElem(m).getMass());
                                    break;
                                }
                            }
                        }
                        for (int j=0; j<contains.size(); j++) {
                            contains.at(j).setAmount(atomMasses.at(j)/total);
                            Component c = contains.at(j);
                            // cout << c.getElement() << " " << contains.at(j).getAmount() << endl;
                        }
                    }
                    
                    // 3) Chem Formula to Weight Fractions
                    else if (type == "Chemical Formula" && style == "Weight Fractions") {
                        // cout << type << " to " << style << endl;
                        double total = 0;
                        for (int i=0; i<contains.size(); i++) {
                            Component ci = contains.at(i);
                            for (int m=0; m<mass.getElems(); m++) {
                                auto e = mass.getElem(m);
                                if (ci.getElement() == e.getSymbol()) {
                                    if (ci.getMassNum()>0){
                                    for (int p=0; p<e.getIsotopes().size(); p++){
                                        if (ci.getMassNum() == e.getIsotopes().at(p).getMassNum()) {total += ci.getAmount() * e.getIsotopes().at(p).getMass();
                                        }
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
                    
                    // 4) Atom Fractions to APM
                    else if (type == "Atom Fractions" && style == "Atoms Per Molecule") {
                        cout << type << " to " << style << endl;
                        cout << name << endl;
                        cout << formula << endl;
                        double min = contains.at(0).getAmount();
                        for (int i=0; i<contains.size(); i++) {
                            double mAtom = contains.at(i).getAmount();
                            if (min > mAtom) {min = mAtom;}
                        }
                        for (int j=0; j<contains.size(); j++) {
                            contains.at(j).setAmount(contains.at(j).getAmount()/min);
                        }
                        int mult = 1;
                        int len = contains.at(0).getElement().length();
                        int start = formula.find(contains.at(0).getElement());
                        if (start != string::npos) {
                                if (start != -1 && (isdigit(formula[start+len]) && formula[start+len] != char(contains.at(0).getAmount()))) {
                                    double digit = stod(string(1, formula[start+len]));
                                    if (isdigit(formula[start+len+1])) {
                                        digit = 10*digit + int(formula[start+len+1])-48;
                                        if (isdigit(formula[start+len+2])) {digit = 10*digit + int(formula[start+len+2])-48;}
                                    }
                                    // cout << digit << " " << contains.at(0).getAmount() << endl;
                                    mult = digit/contains.at(0).getAmount();
                                    // cout << mult << endl;
                                }
                        }
                        for (int k=0; k<contains.size(); k++) {
                            contains.at(k).setAmount(mult*contains.at(k).getAmount());
                            Component ci = contains.at(k);
                            cout << ci.getElement() << " " << ci.getAmount() << endl;
                        }
                    }
                }
                setType(style);
            }
            
            // Either weight fractions or apm
            // void getdataStyle() {}
            
            // Add a selection between the 4 code types
            void getInputFormat(string code, string dataStyle, string dbName) {
                // convert(dataStyle);
                int aNum = 1;
                if (code == "MAVRIC/KENO") {
                    // Add index
                    cout << "'    " << dbName << endl;
                    // cout << "'    " << name << ", " << formula << ", " << density << " g/cm^3" << endl;
                    for (int k=0; k<comments.size(); k++) {
                        cout << "'    " << comments.at(k) << endl;
                    }
                    if (comment != "") {cout << "'    " << comment << endl;}

                    cout << "     atom" << name.substr(0, 12) << "  1  " << density << " " << contains.size();
                    for (int i=0; i<contains.size(); i++) {
                        Component c = contains.at(i);
                        for (int m=0; m<mass.getElems(); m++) {
                            if (c.getElement() == mass.getElem(m).getSymbol()) {aNum = mass.getElem(m).getAtomNum(); break;}
                        }
                        cout << endl << "         " << aNum*1000+c.getMassNum() << "   " << c.getAmount();
                        // other 2 numbers stay constant (density && temp)?
                    }
                    cout << "   " << "end" << endl << endl;

                    // standard comp (mass density)
                    // standard comp (atom density)
                    
                }
                else if (code == "ORIGEN") {
                    string unit;
                    // specify units used
                    if (type == "Weight Fractions") {unit="gram";}
                    else if (type == "Atom Fractions") {unit="mole";}
                    cout << "% 1 " << unit << " of " << name << " using " << type << endl;

                    unit += "s";
                    cout << "mat {" << endl;
                    cout << "\t" << "iso= [ ";
                    
                    // Convert to atoms per molecule?
                    for (int i=0; i<contains.size(); i++) {
                        Component c = contains.at(i);
                        cout << c.getElement() << "=" << c.getAmount() << " ";
                    }
                    cout << "]" << endl << "\t" << "units=";
                    
                    cout << unit << endl << "}" << endl << endl;
                }
                else if (code == "MCNP") {
                    int id = 1;
                    cin >> id;  // Should this be index #, or is this determined by the user?
                    cout << "c    " << dbName << endl;
                    cout << "c    " << name << ", " << density << " g/cm^3" << endl;
                    for (int k=0; k<comments.size(); k++) {
                        cout << "c    " << comments.at(k) << endl;
                    }
                    if (comment != "") {cout << "c    " << comment << endl;}
                    for (int i=0; i<contains.size(); i++) {
                        Component c = contains.at(i);
                        if (i==0) {cout << "  m" << id << " ";}
                        // Find atomic number from element
                        else {
                            cout << "     ";
                        }
                        for (int m=0; m<mass.getElems(); m++) {
                            if (c.getElement() == mass.getElem(m).getSymbol()) {aNum = mass.getElem(m).getAtomNum(); break;}
                        }
                        cout << aNum*1000+c.getMassNum() << "    ";
                        if (type == "Weight Fractions") {cout << '-';}
                        cout << c.getAmount() << endl;
                    }
                    cout << "c "; check(); cout << endl << endl;
                }
                else { // Generic
                    cout << "#     " << dbName << endl;
                    cout << "#     " << name << ", " << density << " g/cm^3" << endl;
                    for (int i=0; i<contains.size(); i++) {
                        Component c = contains.at(i);
                        for (int m=0; m<mass.getElems(); m++) {
                            if (c.getElement() == mass.getElem(m).getSymbol()) {aNum = mass.getElem(m).getAtomNum(); break;}
                        }
                        cout << "       " << aNum << "   " << c.getElement() << "   " << c.getMassNum() << "\t";
                        if (type == "Weight Fractions") {cout << '-';}
                        cout << c.getAmount() << endl;
                    }
                    if (type == "Weight Fractions") {cout << "c "; check();}
                    cout << "c     Generic " << type << endl << endl;
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
            // Use LinkedHashmap to store elements in insertion order
            map<string, int> mp;
            bool cut = false; bool isStr = false;
            int mult = 1;
            // vector<int> cords;
            stack<int> stack;
            while (str.find("sub>") != string::npos) {
                if (str.find("<sub>x") != string::npos) {str.replace(str.find("<sub>x"), 6, "");}
                if (str.find("<sub>") != string::npos) {str.replace(str.find("<sub>"), 5, "");}
                if (str.find("</sub>") != string::npos) {str.replace(str.find("</sub>"), 6, "");}
            }

            for (int i = 0; i < str.length(); i++) {
                int count = 0;
                char c = str[i];
                string a = string(1, c);
                if (!cut) {mult = 1;}
    
                if (a.find_first_of("0123456789") != string::npos) {
                    cut = true;
                    int z = i;
                    mult = 0;
                    while (z < str.length()-2 && str[z] >= '0' && str[z] <= '9') {
                        mult = stoi(to_string(mult)+str[z]);
                        z++;
                    }
                    // cout << mult;
                    i = z;
                    a = string(1, str[i]);
                }
                if (a == ".") {i +=2; a = string(1, str[i]);}
                
                if (a == "(" && str.find(')') != str.substr(i, str.length()-2).length()-1) {
                    int end = str.find(')');
                    stack.push(i);
                    if (isdigit(str[end+1])) {
                        int start = stack.top()+1;
                        stack.pop();
                        mult = int(str[end+1]-48);
                        // cout << start << " " << mult << endl;
                        for (int r=start; r<end; r++) {
                            string s = string(1, str[r]);
                            // cout << s << endl;
                            if (s.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ") != string::npos) {
                                count = 0;
                                for (int t = r + 1; t < end; t++) {
                                    char q = str.at(t);
                                    string st = string(1, q);
                                    if (st.find_first_of("abcdefghijklmnopqrstuvwxyz") != string::npos) {
                                        s += st;
                                        if (mp.find(s) == mp.end()) {mp[s] = 0;}
                                        else {mp[s] += 1*mult;}
                                        count = 1;
                                    }
                                    else if (st.find_first_of("0123456789") != string::npos) {
                                        if (s=="D" || s=="T") {s="H";}
                                        int k = stoi(st);
                                        if (mp.find(s) == mp.end()) {mp[s] = k*mult;}
                                        else if (isStr) {mp[s] = (10*mp[s]/mult + k)*mult;}
                                        else {mp[s] += k*mult;}
                                        //cout << s << mp[s] << endl;
                                        count = 1;
                                        isStr = true;
                                    }
                                    else {
                                        r = t - 1;
                                        // if (mp[s] == 0) {mp[s] = 1*mult;}
                                        isStr = false;
                                        // cout << s << mp[s] << endl;
                                        break;
                                    }
                                }
                                if (count == 0 || mp[s] == 0) {
                                    if (mp.find(s) == mp.end()) {mp[s] = 1*mult;}
                                    else {mp[s] += 1*mult;}
                                }
                                else if (r+1 == end) {mp[s] += 1*mult;}
                            }
                            }
                        cut = false;
                        i = end+1;
                        }
                        continue;
                }
                // else if (a == ")" && i != str.length()-1) {
                    
                //     }
                else if (a.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ") != string::npos) {
                    for (int j = i + 1; j < str.length(); j++) {
                        char d = str.at(j);
                        string b = string(1, d);
                        if (b.find_first_of("abcdefghijklmnopqrstuvwxyz") != string::npos) {
                            a += b;
                            if (mp.find(a) == mp.end()) {mp[a] = 0;}
                            else {mp[a] += 1*mult;}
                            count = 1;
                        }
                        else if (b.find_first_of("0123456789") != string::npos) {
                            if (a=="D" || a=="T") {a="H";}
                            int k = stoi(b);
                            if (mp.find(a) == mp.end()) {mp[a] = k*mult;}
                            else if (isStr) {mp[a] = (10*mp[a]/mult + k)*mult;}
                            else {mp[a] += k*mult;}
                            count = 1;
                            isStr = true;
                        }
                        else {
                            i = j - 1;
                            //if (mp[a] == 0) {mp[a] = 1*mult;}
                            isStr = false;
                            break;
                        }
                    }
                    if (count == 0) {
                        if (mp.find(a) == mp.end()) {mp[a] = 1*mult;}
                        else {mp[a] += 1*mult;}
                    }
                }
            }
            string f = "";
            for (auto entry : mp) {
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
                        if (c.getAmount()>1) {symbols.push_back(c.getElement()+to_string(int(c.getAmount())));}
                        else {symbols.push_back(c.getElement());}
                    }
                    sort(symbols.begin(), symbols.end());
                    string f = countAtoms(formula);
                    string lst = "";
                    for (int j=0; j<imax; j++) {
                        if ((symbols.at(j)[0] == 'D' || symbols.at(j)[0] == 'T') && isdigit(symbols.at(j)[1])) {lst += "H" + symbols.at(j).substr(1, symbols.at(j).length()-1);}
                        else {lst += symbols.at(j);}
                    }
                    bool matches = f == lst;

                    if(matches){
                        // cout << "    " << name << "\t" << "Okay with " << imax << " element"; 
                        // if (imax>1) {cout << "s";} cout << endl;
                    }
                    else {
                        //string ref = references[0];
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
                    cout << "\t" << "Weights add to 1" << endl;
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
    //        void setPath(string a) {path = a;}
    //        void setName(string a) {name = a;}
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
            mass.build("C:/Users/k12jsti/source/repos/materialsdatabase/NISTmasses.json", cerr);
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
                //build_contact(contact, m, cerr);
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
            // m.getInputFormat("MAVRIC/KENO", "Weight Fractions", dbName);
            // if (m.getType() == "Chemical Formula")
            // {m.convert("Weight Fractions"); cout << endl; m.checkFractions();}
            if (m.getType() == "Chemical Formula") {m.checkAtoms();}
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
            if (itr != comp->end()) {c.setMassNum(itr->second.to_int());}
            // Weight Fractions
            if (m.getType() == "Weight Fractions") {
                itr = comp->find("Fraction");
                c.setAmount(itr->second.to_double());
                c.setAtom(false);
            }
            // Atom Fractions
            else if (m.getType() == "Atom Fractions") {
                itr = comp->find("Fraction");
                c.setAmount(itr->second.to_double());
                c.setAtom(false);
            }
            // Chemical Formula
            else if (m.getType() == "Chemical Formula") {
                itr = comp->find("Atoms");
                c.setAmount(itr->second.to_double());
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


 // string makeFormMap(map<string, int> form){
            //     string result = "";
            //     for (auto& it : form) {
            //         if (it.first != "n") {result += it.first;}
            //         if (it.second > 1) {
            //             result += to_string(it.second);
            //         }
            //     }
            //     return result;
            // }
            // string countAtoms(string s){
            //     map<string, int> m;
            //     stack<map<string, int> > st;
            //     int i = 0;
            //     int n = s.size();
            //     while (i < n) {
            //         char c = s[i];
            //         i++;
            //         if (c == '(') {
            //             st.push(m);
            //             m = map<string, int>();
            //         }
            //         else if (c == ')') {
            //             int val = 0;
            //             while (i < n && s[i] >= '0' && s[i] <= '9') {
            //                 val = val * 10 + (s[i] - '0');
            //                 i++;
            //             }
            //             map<string, int> temp = st.top();
            //             st.pop();
            //             for (auto& it : m) {
            //                 it.second *= val;
            //                 temp[it.first] += it.second;
            //             }
            //             m = temp;
            //         }  
            //         else {
            //             string name = "";
            //             int val = 0;
            //             name += c;
            //             while (i < n && s[i] >= 'a' && s[i] <= 'z') {
            //                 name += s[i];
            //                 i++;
            //             }
            //             while (i < n && s[i] >= '0' && s[i] <= '9') {
            //                 val = val * 10 + (s[i] - '0');
            //                 i++;
            //             }
            //             val = val == 0 ? 1 : val;
            //             m[name] += val;
            //         }
            //     }
            //     return makeFormMap(m);
            // }