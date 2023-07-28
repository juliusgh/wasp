#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <sys/stat.h>
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
            path = wasp::dir_name(__FILE__) + "/materials";
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


        /** Experimental methods trying to read in files from a folder directory
         * The input generation tool intends to identify the relevant JSON files within a folder directory and build the provided information into objects.
        */
        // void getFilesList(string filePath,string extension) {
        //     WIN32_FIND_DATA fileInfo;
        //     HANDLE hFind;   
        //     string fullPath = filePath;
        //     hFind = FindFirstFile(fullPath.c_str(), &fileInfo);
        //     if (hFind != INVALID_HANDLE_VALUE){
        //         filesPaths.push_back(filePath+fileInfo.cFileName);
        //         while (FindNextFile(hFind, &fileInfo) != 0){
        //             filesPaths.push_back(filePath+fileInfo.cFileName);
        //         }
        //     }
            // cout << "size" << filesPaths.size() << endl;
        // }

        // bool extract_files(const std::string& folderPath) {
        //     struct stat sb;
            // for (const auto& entry : std::filesystem::directory_iterator(path)) {
            //     std::filesystem::path outfilename = entry.path();
            //     string outfilename_str = outfilename.string();
            //     const char* path = outfilename_str.c_str();
            //     if (stat(path, &sb) == 0 && !(sb.st_mode & S_IFDIR))
            //         cout << path << std::endl;
            // }
        // }
        
        bool build(const std::string& matRelPath, const std::string& massRelPath, std::ostream& cerr){
            string optfileName ="";        
            string inputFolderPath =""; 
            string extension = "*.json*";
            if (!nistMasses.build(massRelPath, cerr)) {return false;}
            
            // Attempting to load and access multiple databases for FakeGUI tests

            // vector<string> paths = {
            //     "C:/Users/k12jsti/source/repos/materialsdatabase/wasp/waspmaterial/materials/NIST_126_table2.json",
            //     "C:/Users/k12jsti/source/repos/materialsdatabase/wasp/waspmaterial/materials/PNNL-15870r2_eff.json"
            // };
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
            if (!db.build(matRelPath, cerr)) {return false;}
            databases.push_back(db);
            return true;
        }

        vector<string> getDBFilenames() {return dbFilenames;}

        Database getDatabase(string name) {
            auto idx = find(dbFilenames.begin(), dbFilenames.end(), name);
            if (idx != dbFilenames.end()) {
                return databases.at(idx - dbFilenames.begin()+1);
            }
        }

};
