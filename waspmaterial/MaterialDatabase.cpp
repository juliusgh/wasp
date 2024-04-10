#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <stack>
#include <array>
#include <cstdlib>
#include <sstream>
#include <stdexcept>
#include <functional>
#include <algorithm>
#include "ctype.h"
#include "waspjson\JSONObjectParser.hpp"
#include "waspcore\Object.h"
using namespace std;
using namespace wasp;

class Component {
    string elem;
    double amount;
    bool atom;
    static const string ELEMENT;
    static const string WEIGHTFRACTION;
    static const string ATOMFRACTION;

    public:
        // Constructor:
        Component() {
            elem = string();
            amount = 0.0;
            atom = true;
        }

        // Set Methods:
        void setElement(string a) {elem = a;}
        void setAmount(double a) {amount = a;}
        void setAtom(bool a) {atom = a;}

        // Get Methods:
        string getElement() {return elem;}
        double getAmount() {return amount;}
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

class Database {
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
            Contact getContact() {return contact;};

            void check() {
                int cmax = contains.size();
                if (cmax>0) {
                    Component c = contains.at(0);
                    bool atom = c.getAtom();
                    if(atom){checkAtoms();}
                    // else {checkFractions();}
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
            // Use LinkedHashmap to store
            // elements in insertion order
            map<string, int> mp;
            bool cut = false;
            int mult = 1;
            vector<int> cords;
            for (int i = 0; i < str.length(); i++) {
                int count = 0;
                char c = str.at(i);
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
                    a = string(1, str.at(i));
                }
                // '(' or ')'
                if (a == "(") {
                    cords.push_back(i);
                }
                else if (a == ")" && i != str.length()-1) {
                    if (isdigit(str[i+1])) {
                        int start = cords.back();
                        cords.pop_back();
                        mult = int(str[i+1]-48);
                        for (int r=start; r<i; r++) {
                            string s = string(1, str[r]);
                            if (s.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ") != string::npos) {
                                mp[s] *= mult;
                            }
                        }
                        cut = false;
                        i++;
                        }
                    }
                if (a.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ") != string::npos) {
                    if (a=="D") {a="H";}
                    else if (a=="T") {a="T";}
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
                            int k = stoi(b);
                            if (mp.find(a) == mp.end()) {mp[a] = k*mult;}
                            else {mp[a] = (10*mp[a]/mult + k)*mult;}
                            count = 1;
                        }
                        else {
                            i = j - 1;
                            //if (mp[a] == 0) {mp[a] = 1*mult;}
                            break;
                        }
                    }
                    // cout << mp[a] << " " << a << " ";
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
                    // vector<int> amts{};
                    for (int i=0; i<imax; i++) {
                        Component c = contains.at(i);
                        if (c.getAmount()>1) {symbols.push_back(c.getElement()+to_string(int(c.getAmount())));}
                        else {symbols.push_back(c.getElement());}
                        // amts.push_back(round(c.getAmount()));
                    }
                    sort(symbols.begin(), symbols.end());
                    string f = countAtoms(formula);
                    string lst = "";
                    for (int j=0; j<imax; j++) {
                        lst += symbols.at(j);
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
                    cout << "\t" << name << "\t" << sum << endl;
                }
                else {
                    // string ref = references.at(0);
                    cout << "\t" << name << "\t" << sum << endl;
                }
            }
            void display() {
                cout << "\t" << "Material" << endl;
                cout << "\t\t" << "Name:  " << name << endl;
                if (formula != "") {cout << "\t\t" << "Formula:  " << formula << endl;}
                cout << "\t\t" << "Density:  " << density << endl;
                if (abbreviations.size() > 0) {
                    cout << "\t\t" << "Abbreviation:  ";
                    for (int i=0; i<abbreviations.size(); i++) {cout << abbreviations.at(i) << " " << endl;}
                }
                if (alias != "") {cout << "\t\t" << "Alias:  " << alias << endl;}
                if (aliases.size() > 0) {
                    cout << "\t\t" << "Aliases:  ";
                    for (int i=0; i<aliases.size(); i++) {cout << aliases.at(i) << "  " << endl;}
                    }
                if (comment != "") {cout << "\t\t" << "Comment:  " << comment << endl;}
                if (comments.size() > 0) {
                    cout << "\t\t" << "Comments:" << endl;
                    for (int i=0; i<comment.size(); i++) {cout << "\t\t  " << comment.at(i) << endl;}
                }
                if (contains.size()>0) {
                    cout << "\t\t" << "Components:" << endl;
                    for (int i=0; i<contains.size(); i++) {
                        Component c = contains.at(i);
                        cout << "\t\t   " << c.getElement() << "\t" << c.getAmount() << endl;
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
            
            Database d;
            itr = db->find("Name");
            d.setDB(itr->second.to_string());
            itr = db->find("Reference");
            d.setRef(itr->second.to_string());
            
            itr = db->find("Comments");
            vector<string> commVec{};
            if (itr != db->end()) {
                DataArray* comms = itr->second.to_array();
                for (auto citr = comms->begin(); citr!=comms->end(); citr++) {
                    auto c = *citr;
                    commVec.push_back(c.to_string());
                }
                d.setComments(commVec);
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
            return build_materials(materials, d, cerr);
        }
        bool build_materials(DataArray* materialsArray, Database d, std::ostream& cerr){
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
            d.setMats(matVec);
            // d.display(false);
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
                build_contact(contact, cerr);
            }
            

            itr = material->find("Contains");
            if (itr == material->end()) {
                cerr << "Unable to find material components!" << endl;
                // return false;
            }
            else if (!itr->second.is_array()) {cerr << "Contains is not an array." << endl;}
            else {
                DataArray* components = itr->second.to_array();
                if (!build_material_components(components, m, cerr)) {
                    cerr << "No components!";
                    return false;
                }
            }
            matVec.push_back(m);
            return true;
        }
        bool build_contact(DataObject* contact, std::ostream& cerr) {
            wasp_require(contact != nullptr);
            Contact con;
            auto itr = contact->find("Name");
            if (itr != contact->end()) {con.setName(itr->second.to_string());}
            itr = contact->find("Email");
            if (itr != contact->end()) {con.setEmail(itr->second.to_string());}
            itr = contact->find("Phone");
            if (itr != contact->end()) {con.setPhone(itr->second.to_string());}
            // con.display();
            return true;
        }
        bool build_material_components(DataArray* components, Material m, std::ostream& cerr) {
            compVec = {};
            for (auto itr = components->begin(); itr != components->end(); itr++) {
                Value& comp = *itr;
                // cout << comp.to_string() << endl;
                bool success = true;
                if (!comp.is_object()) {
                    cerr << "Component is not an object and is in an unrecognized format." << endl;
                    // success = false;
                }
                else {
                    success &= build_component(comp.to_object(), m, cerr);
                }
            }
            m.setElements(compVec);
            m.display();
            m.check();
            return true;
        }
        
        bool build_component(DataObject* comp, Material m, std::ostream& cerr) {
            wasp_require(comp != nullptr);
            Component c;
            // Applies to all 3 formats (is there a reason we would need to differentiate between the two fraction types?)
            
            auto itr = comp->find("Element");
            c.setElement(itr->second.to_string());
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

        void check() { // Checks if the files in the directory meet the criteria for MatData json files
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

const string Component::ELEMENT = "Element";
const string Component::WEIGHTFRACTION = "WeightFraction";
const string Component::ATOMFRACTION = "AtomFraction";

const string Contact::NAME = "Name";
const string Contact::PHONE = "Phone";
const string Contact::EMAIL = "Email";


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