#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <cstdlib>
#include <sys/stat.h>
#include "waspjson\JSONObjectParser.hpp"
#include "waspcore\Object.h"
#include "waspmaterial/MaterialDatabase.h"
// #include "waspmaterial/MassDatabase.h"
using namespace std;
using namespace wasp;


class MaterialCompositionLib {
    string path; //Parent directory
    vector<Database> databases;
    Masses nistMasses;
    vector<string> dbFilenames{};

    public:
        MaterialCompositionLib() {
            path = string();
            databases = vector<Database>();
            Masses *p = &nistMasses; p = nullptr;
            for (int i=0; i<databases.size(); i++) {
                Database d = databases.at(i);
                dbFilenames.push_back(d.getDB());
            }
        }
        string getPath() {return path;}
        Masses getMasses() {return nistMasses;}

        bool extract_files(const std::string& folderPath) {
            // for (const auto &entry : filesystem::directory_iterator(folderPath)) {
            //     cout << entry.path() << endl;
            // }
            struct stat sb;
            auto path = folderPath;
            // for (const auto& entry : std::filesystem::directory_iterator(path)) {
            //     std::filesystem::folderPath outfilename = entry.path();
            //     string outfilename_str = outfilename.string();
            //     const char* path = outfilename_str.c_str();

            //      if (stat(path, &sb) == 0 && !(sb.st_mode & S_IFDIR)) {cout << path << endl;}
            // }
        }
        
        bool build(const std::string& matPath, const std::string& massPath, std::ostream& cerr){
            if (!nistMasses.build(massPath, cerr)) {return false;}
            // nistMasses.display(false);
            // for (int i=0; i<databases.size(); i++) {
            //     Database db = databases.at(i);
            Database db;
            if (!db.build(matPath, cerr)) {return false;}
            databases.push_back(db);
            //}
            
            // db.display(true);
            // Database::Material mat = db.getMats().at(0);
            // mat.getInputFormat("ORIGEN", "Weight Fractions");
            // main();
            return true;
        }

        vector<string> getDBFilenames() {return dbFilenames;}

        Database getDatabase(string name) {
            auto idx = find(dbFilenames.begin(), dbFilenames.end(), name);
            if (idx != dbFilenames.end()) {
                return databases.at(idx - dbFilenames.begin()+1);
            }
        }


        int main() {
            Database db = databases.at(0);
            vector<string> commands = {"0: display masses", "1: display material", "2: search", "3: change material", "4: check fractional compositions", "5: create a nuclear code"};
            vector <string> searchName{};
            string code; string type; string search;
            int idx;
            cout << "Materials:" << endl;
            for (int m=1; m<db.getMats()+1; m++) {cout << db.getMat(m-1).getName() << "     "; if (m%4==0) {cout << endl << endl;}}
            cout << endl << "(Out of " << db.getMats() << ") " << endl;
            cin >> idx; idx --; cout << "Material #" << idx+1 << endl;
            int hello;
            cout << "Enter -1 to exit" << endl << "What do you want to do?" << endl << "Some commands are: ";
            for (int a=0; a<commands.size(); a++) {
                cout << commands.at(a) << " ";
            }
            cout << endl;
            //cin >> hello;
            for (;cin >> hello && hello != -1;) {
            switch (hello) {
            case 0:
                nistMasses.display(false);
                break;
            
            case 1:
                db.getMat(idx).display();
                break;
            
            case 2:
                cin >> search; // if sorted, might be able to use a binary search
                for (int b=0; b<db.getMats(); b++) {
                    string name = db.getMat(b).getName();
                    if (name.find(search) != string::npos) {
                        searchName.push_back(name);
                        cout << b << ": " << name << endl;
                    }
                hello = 3;
                }
            
            case 3:
                cout << "Change material to: ";
                cin >> idx;
                cout << db.getMat(idx).getName() << endl;
                break;

            case 4:
                db.getMat(idx).checkFractions();
                break;
            
            case 5:
                cin >> code >> type;
                db.getMat(idx).getInputFormat(code, type, db.getDB());
                
                break;

            default:
                cout << "Escaping ...";
                break;
            }
            //cout << idx;
            }
            return 0;
        }

};
