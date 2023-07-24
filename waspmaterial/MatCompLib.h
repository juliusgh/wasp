#include <iostream>
#include <fstream>
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
// namespace fs = std::filesystem;


class MaterialCompositionLib {
    string path; //Parent directory
    vector<Database> databases;
    Masses nistMasses;
    vector<string> dbFilenames{};
    vector<string> filesPaths;

    public:
        MaterialCompositionLib() {
            path = "C:/Users/k12jsti/source/repos/materialsdatabase/wasp/waspmaterial/materials";
            databases = vector<Database>();
            Masses *p = &nistMasses; p = nullptr;
            for (int i=0; i<databases.size(); i++) {
                Database d = databases.at(i);
                dbFilenames.push_back(d.getDB());
            }
            filesPaths = vector<string>();
        }

        void setDatabases(vector<Database> a) {databases = a;}

        string getPath() {return path;}
        Masses getMasses() {return nistMasses;}

        void getFilesList(string filePath,string extension) {
            WIN32_FIND_DATA fileInfo;
            HANDLE hFind;   
            string fullPath = filePath;
            hFind = FindFirstFile(fullPath.c_str(), &fileInfo);
            if (hFind != INVALID_HANDLE_VALUE){
                filesPaths.push_back(filePath+fileInfo.cFileName);
                while (FindNextFile(hFind, &fileInfo) != 0){
                    filesPaths.push_back(filePath+fileInfo.cFileName);
                }
            }
            // cout << "size" << filesPaths.size() << endl;
        }

        bool extract_files(const std::string& folderPath) {
            struct stat sb;
            // for (const auto& entry : std::filesystem::directory_iterator(path)) {
            //     std::filesystem::path outfilename = entry.path();
            //     string outfilename_str = outfilename.string();
            //     const char* path = outfilename_str.c_str();
            //     if (stat(path, &sb) == 0 && !(sb.st_mode & S_IFDIR))
            //         cout << path << std::endl;
            // }
        }
        
        bool build(const std::string& matRelPath, const std::string& massRelPath, std::ostream& cerr){
            string optfileName ="";        
            string inputFolderPath =""; 
            string extension = "*.json*";
            // getFilesList(inputFolderPath,extension);
            // vector<string>::const_iterator it = filesPaths.begin();
            // cout << "size" << filesPaths.size() << endl;
            for (auto it = filesPaths.begin(); it != filesPaths.end(); it++) {
                // auto frame = imread(*it);//read file names
                //     //doyourwork here ( frame );
                // sprintf(buf, "%s/Out/%d.jpg", optfileName.c_str(),it->c_str());
                // imwrite(buf,frame);
            }
            std::cout <<"Data path is: " << massRelPath << std::endl;
            if (!nistMasses.build(massRelPath, cerr)) {return false;}
            vector <string> paths = {
                "C:/Users/k12jsti/source/repos/materialsdatabase/wasp/waspmaterial/materials/NIST_126_table2.json",
                "C:/Users/k12jsti/source/repos/materialsdatabase/wasp/waspmaterial/materials/PNNL-15870r2_eff.json"
            };
            // for (int i=0; i<paths.size(); i++) { // replace filespaths w/ paths
            // //     Database db = databases.at(i);
            //     // cout << i << endl;
            //     if (paths.at(i).find(".json") == -1) {continue;}
            //     Database db;
            //     if (!db.build(paths.at(i), cerr) || paths.at(i).find("masses") != -1) {continue;}
            //     // else if (!db.build(paths.at(i), cerr)) {return false;}  // Should only need to return false if there's no mat comp jsons and no masses json
            //     databases.push_back(db);
            // }
            Database db;
            string matPath = wasp::dir_name(__FILE__) + matRelPath;
            if (!db.build(matPath, cerr)) {return false;}
            
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
            cout << "Pick some databases: " << endl;
            char inpDB[50]; string val;
            cin.getline(inpDB, 20, ',');
            cout << inpDB << endl; // Split DB names into elements in an array and sort
            Database db;
            for(int d=0; d<databases.size(); d++){
                Database dbtemp = databases.at(d); // Make accessible to use multiple databases simultaneously
                if (d>0) {db.setDB(db.getDB()+", "+dbtemp.getDB());}
                else {db.setDB(dbtemp.getDB());}
                    // auto v = dbtemp.getMats();
                    // for (auto &val: dbtemp.getMats()) {v.push_back(val);}
                    // db.setMats(dbtemp.getMats());
                cout << db.getDB() << endl;
            }
            db = databases.at(0);
            
                vector<string> commands = {"0: display masses", "1: display material", "2: search", "3: advanced search", "4: change material", "5: check fractional compositions", "6: create a nuclear code"};
                vector <string> searchName{};
                string search;
                string name;
                //int srchidx = -1;
                double dens = -1.0;
                
                string code; string type; string calcType;
                int idx;

                cout << "Materials:" << endl;
                for (int m=1; m<db.getMats()+1; m++) {cout << db.getMat(m-1).getName() << "     "; if (m%4==0) {cout << endl << endl;}}
                cout << endl << "(Out of " << db.getMats() << ") " << endl;
                cin >> idx; idx --; cout << "Material #" << idx+1 << ": " << db.getMat(idx).getName() << endl;
                int hello;
                cout << "Enter -1 to exit" << endl << "What do you want to do?" << endl << "Some commands are: ";
                for (int a=0; a<commands.size(); a++) {
                    cout << commands.at(a) << "   ";
                    if (a%3 == 0) {cout << endl;}
                }
                cout << endl;
                
                for (;cin >> hello && hello != -1;) {
                switch (hello) {
                case 0:
                    nistMasses.display(false);
                    break;
                
                case 1:
                    db.getMat(idx).display();
                    break;
                
                case 2: //Search
                    cin >> search; // if sorted, might be able to use a binary search
                    for (int b=0; b<db.getMats(); b++) {
                        name = db.getMat(b).getName();
                        if (name.find(search) != string::npos) {
                            //searchName.push_back(name);
                            cout << b << ": " << name << endl;
                        }
                    }
                    hello = 4;
                
                case 3: // Advanced Search
                    // Identifiers: Name, Density, Formula
                    // cout << "Material #:"; cin >> srchidx; cout << srchidx << endl;
                    // if (srchidx > 0) {cout << srchidx << ": " << db.getMat(srchidx-1).getName() << endl;}
                    cout << "Name: "; cin >> search; cout << search << endl;
                    cout << "Density: "; cin >> dens; cout << dens << endl;
                    for (int c=0; c<db.getMats(); c++) {
                        name = db.getMat(c).getName();
                        double density = db.getMat(c).getDensity();
                        if (name.find(search) != string::npos || dens == density) {
                            cout << c << ": " << name << endl;
                        }
                    }
                    hello = 4;

                case 4: // Change Mat
                    cout << "Change material to: ";
                    cin >> idx;
                    cout << db.getMat(idx).getName() << endl;
                    break;

                case 5: // checkFracs -> check()
                    db.getMat(idx).checkFractions();
                    break;
                
                case 6: // Transport code
                    cin >> code >> type >> calcType;
                    db.getMat(idx).getInputFormat(code, type, calcType, db.getDB());
                    
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
