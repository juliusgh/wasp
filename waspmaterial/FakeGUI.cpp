#include <iostream>
#include <string>

#include "waspmaterial\MatCompLib.h"

// This function intended to test the capabilities of the object classes as implemented into a fake GUI.
// These capabilities would be better suited as tests.
int main() {
            // cout << "Pick some databases: " << endl;
            char inpDB[50]; string val;
            Database db;
            // for(int d=0; d<databases.size(); d++){
            //     Database dbtemp = databases.at(d); // Make accessible to use multiple databases simultaneously
            //     if (d>0) {db.setDB(db.getDB()+", "+dbtemp.getDB());}
            //     else {db.setDB(dbtemp.getDB());}
            //         // auto v = dbtemp.getMats();
            //         // for (auto &val: dbtemp.getMats()) {v.push_back(val);}
            //         // db.setMats(dbtemp.getMats());
            //     cout << db.getDB() << endl;
            // }
            db = databases.at(0);
            
                vector<string> commands = {"0: display masses", "1: display material", "2: search", "3: advanced search", "4: change material", "5: check compositions", "6: create a nuclear code"};
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
                    cin >> search; // if sorted, might be able to use a binary search or more efficient search
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

                case 5: // check()
                    db.getMat(idx).check();
                    break;
                
                case 6: // Transport code input
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