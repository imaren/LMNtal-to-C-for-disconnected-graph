#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

int main(void)
{
    ifstream ifs("in.txt");
    string str;

    if (ifs.fail()) {
        cerr << "Failed to open file." << endl;
        return -1;
    }


    vector<double> rule1,rule2,rule3,rule4,rule5,rule6,rule7,
    total,utry,usf,time;

    while (getline(ifs, str)) {
        str[101]='\0';
        // cout << "while loop\n";
        // cout << "str= " << str << endl;
        istringstream iss(str);
        if(str.find("ruleA")!=string::npos){
            while(true){
                double v; iss >> v;
                if (iss)
                rule1.push_back(v);
                else {
                    if (iss.eof()) break;
                    iss.clear();
                    iss.ignore();
                }
            }
        }else if(str.find("ruleB")!=string::npos){
            while(true){
                double v; iss >> v;
                if (iss)
                rule2.push_back(v);
                else {
                    if (iss.eof()) break;
                    iss.clear();
                    iss.ignore();
                }
            }
        }else if(str.find("ruleC")!=string::npos){
            while(true){
                double v; iss >> v;
                if (iss)
                rule3.push_back(v);
                else {
                    if (iss.eof()) break;
                    iss.clear();
                    iss.ignore();
                }
            }
        }else if(str.find("ruleD")!=string::npos){
            while(true){
                double v; iss >> v;
                if (iss)
                rule4.push_back(v);
                else {
                    if (iss.eof()) break;
                    iss.clear();
                    iss.ignore();
                }
            }
        }else if(str.find("ruleE")!=string::npos){
            while(true){
                double v; iss >> v;
                if (iss)
                rule5.push_back(v);
                else {
                    if (iss.eof()) break;
                    iss.clear();
                    iss.ignore();
                }
            }
        }else if(str.find("ruleF")!=string::npos){
            while(true){
                double v; iss >> v;
                if (iss)
                rule6.push_back(v);
                else {
                    if (iss.eof()) break;
                    iss.clear();
                    iss.ignore();
                }
            }
        }else if(str.find("ruleG")!=string::npos){
            while(true){
                double v; iss >> v;
                if (iss)
                rule7.push_back(v);
                else {
                    if (iss.eof()) break;
                    iss.clear();
                    iss.ignore();
                }
            }
        }else if(str.find("trial")!=string::npos){
            while(true){
                double v; iss >> v;
                if (iss)
                utry.push_back(v);
                else {
                    if (iss.eof()) break;
                    iss.clear();
                    iss.ignore();
                }
            }
        }else if(str.find("suc")!=string::npos){
            while(true){
                double v; iss >> v;
                if (iss)
                usf.push_back(v);
                else {
                    if (iss.eof()) break;
                    iss.clear();
                    iss.ignore();
                }
            }
        }else if(str.find("Total")!=string::npos){
            while(true){
                double v; iss >> v;
                if (iss){
                total.push_back(v);
                }
                else {
                    if (iss.eof()) break;
                    iss.clear();
                    iss.ignore();
                }
            }
        }else if(str.find("uniq time")!=string::npos){
            while(true){
                double v; iss >> v;
                if (iss){
                time.push_back(v);
                }
                else {
                    if (iss.eof()) break;
                    iss.clear();
                    iss.ignore();
                }
            }
        }else if(str.find("intern time")!=string::npos){
            while(true){
                double v; iss >> v;
                if (iss){
                time.push_back(v);
                }
                else {
                    if (iss.eof()) break;
                    iss.clear();
                    iss.ignore();
                }
            }
        }else if(str.find("tostring time")!=string::npos){
            while(true){
                double v; iss >> v;
                if (iss){
                time.push_back(v);
                }
                else {
                    if (iss.eof()) break;
                    iss.clear();
                    iss.ignore();
                }
            }
        }else if(str.find("history time")!=string::npos){
            while(true){
                double v; iss >> v;
                if (iss){
                time.push_back(v);
                }
                else {
                    if (iss.eof()) break;
                    iss.clear();
                    iss.ignore();
                }
            }
        }
    }

    // //ルール1本
    cout <<
    rule1[1] << "," << total[2] << "," <<
    rule1[2] << "," << total[3] << "," << 
    rule1[3] << "," << total[4] << "," <<
    rule1[4] << "," << total[5] << "," << 
    total[0] << "," << total[1] <<endl;

    //時間だけトータル
    // cout <<
    // rule1[1] << "," <<
    // rule1[2] << "," << 
    // rule1[3] << "," << 
    // rule1[4] << "," << total[5] << "," <<  "\n";

    //ルール2本
    // cout <<
    // rule1[1] << "," << rule2[1] << "," << total[2] << "," <<
    // rule1[2] << "," << rule2[2] << "," <<total[3] << "," << 
    // rule1[3] << "," << rule2[3] << "," <<total[4] << "," <<
    // rule1[4] << "," << rule2[4] << "," <<total[5] << "," << 
    // total[0] << "," << total[1] <<endl;


    //ルール7本
    // cout <<
    // rule1[1] << "," << rule2[1] << "," <<rule3[1] << "," <<rule4[1] << "," <<
    // rule5[1] << "," << rule6[1] << "," <<rule7[1] << "," <<total[2] << "," <<
    // rule1[2] << "," << rule2[2] << "," <<rule3[2] << "," <<rule4[2] << "," <<
    // rule5[2] << "," << rule6[2] << "," <<rule7[2] << "," <<total[3] << "," <<
    // rule1[3] << "," << rule2[3] << "," <<rule3[3] << "," <<rule4[3] << "," <<
    // rule5[3] << "," << rule6[3] << "," <<rule7[3] << "," <<total[4] << "," <<
    // rule1[4] << "," << rule2[4] << "," <<rule3[4] << "," <<rule4[4] << "," <<
    // rule5[4] << "," << rule6[4] << "," <<rule7[4] << "," <<total[5] << "," <<
    // total[0] << "," <<total[1] << endl;

    return 0;
}