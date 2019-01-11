/*! 
 *  \class     EditBridge
 *  \file      EditBridge.cpp        
 *  \brief     Allows the user to edit a bridge
 *  \details   The class allows the editing of bridges, the class edits the bridge from the user data
 *  \author    Gary Tohill      gtohill@uwo.ca
 *  \version   Stage 3
 *  \date      2017, November 28
 */
#include <cstdio>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <boost/filesystem.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#include <Wt/WDialog>
#include <Wt/WLabel>
#include <Wt/WBreak>
#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WText>
#include <Wt/Json/Object>
#include <Wt/Json/Parser>
#include <Wt/WTemplate>
#include <Wt/Json/Object>
#include <Wt/Json/Parser>


#include "hashFunction.h"
#include "EditBridge.h"

using namespace Wt;
using namespace std;

/*! 
 *  \fn        inputChecker
 *  \brief     Checks the input the user entered
 *  \details   Checks the ip address and the port values to see if they are valued before adding it
 *  \author    Vaughan Robertson      vrobert6@uwo.ca
 *  \version   Stage 3
 *  \date      2017, November 28
 */

bool EditBridge::inputChecker(string ipAddress, string port){ 

    bool result = true;

    //Checks to see if the entered IP address is valid or not

    std::vector<std::string> ipSplit;

    boost::split(ipSplit, ipAddress, boost::is_any_of("."), boost::token_compress_on);

    if(ipSplit.size() != 4){

        invalidMessage->setText("The IP address entered is invalid");
        result = false;
    }
    else{

        if(ipSplit[0].size() > 3 or ipSplit[1].size() > 3 or ipSplit[2].size() > 3 or ipSplit[3].size() > 3){

            invalidMessage->setText("The IP address entered is invalid");
            result = false;
        }
        else{

            for(int x = 0; x <=3; x++){

                try{

                    stoi(ipSplit[x]);
                }
                catch(invalid_argument e){

                    invalidMessage->setText("The IP address entered is invalid");
                    result = false;
                }
            }
        }
    }

    try{

        int portNumber = stoi(port);

        if(portNumber > 65535){

            invalidMessage->setText("The port number entered is invalid");
            result = false;
        }


    }
    catch(invalid_argument e){

        invalidMessage->setText("The port number entered is invalid");
        result = false;
    }


    return result;
}


/*!
 *  \fn        onClick
 *  \brief     Function called when button is clicked
 *  \details   Once the button is clicked this function checks with the bridge and edits all the values in variables
 *  \author    Vaughan Robertson      vrobert6@uwo.ca
 *  \author    Gary Tohill      gtohill@uwo.ca
 *  \version   Stage 3
 *  \date      2017, November 28
 */

void EditBridge::onClick(WDialog *log){
    string IP;
    string brPort;
    string brLocation;
    string brUser;

    //variables from dialog box
    string brName = bridgeName->text().toUTF8();
    string ip = bridgeIP->text().toUTF8();
    string port = bridgePort->text().toUTF8();
    string location = bridgeLocation->text().toUTF8();
    string user = bridgeUser->text().toUTF8();

    if(inputChecker(ip, port)){

        Wt::Json::Object userContent;

        //string data;
        if(boost::filesystem::exists("./users/"+userEmail+".json")) {
            //get data from exisiting file
            ifstream file1("./users/" + userEmail + ".json");
            std::istreambuf_iterator<char> eos1;
            std::string s1(std::istreambuf_iterator<char>(file1), eos1);

            Json::parse(s1, userContent);

            //file.close();

            //find bridge using current bridge
            int x = 1;
            string num;
            string bridgeNum;

            for (; x <= 50; x++) {

                ostringstream ss;
                ss << x;
                num = ss.str();

                if (userContent.contains("Bridge" + num)) {
                    bridgeNum = "Bridge" + num;
                    Json::Object bridge = userContent.get("Bridge" + num);

                    string name = bridge.get("name").orIfNull("name");

                    if (currentBridge == name) {
                        //foundName = name;
                        IP = bridge.get("ip").orIfNull("ip");
                        brPort = bridge.get("port").orIfNull("port");
                        brLocation = bridge.get("location").orIfNull("location");
                        brUser = bridge.get("user").orIfNull("user");

                        x = 51;
                    }
                }

            }

            //file.close();

            //cout << "Bridge " << bridgeNum << endl;
            string updateBridgeList[6];
            updateBridgeList[0] = bridgeNum;

            if (currentBridge != brName) {
                updateBridgeList[1] = brName;
            } else {
                updateBridgeList[1] = currentBridge;
            }

            if (IP != ip) {
                updateBridgeList[2] = ip;
            } else {
                updateBridgeList[2] = IP;
            }

            if (brPort != port) {
                updateBridgeList[3] = port;
            } else {
                updateBridgeList[3] = brPort;
            }

            if (brLocation != location) {
                updateBridgeList[4] = location;
            } else {
                updateBridgeList[4] = brLocation;
            }

            if (brUser != user) {
                updateBridgeList[5] = user;
            } else {
                updateBridgeList[5] = brUser;
            }

            //print contents of update list
            for (int i = 0; i < 6; i++) {
                cout << "Bridge: " << updateBridgeList[i] << endl;
            }
            file1.close();
            cout <<"User Email " << userEmail << endl;
            //remove file
            string sDirectory ="/home/gary/CLionProjects/team07/users/" ;
            //chdir(sDirectory.c_str());
            string json =".json";
            string filename = sDirectory+userEmail+json;
            remove(filename.c_str());

            //get data from json file and reenter it. then append updated json to file
            string addData1;
            string pass = userContent.get("password");
            string fName = userContent.get("Firstname");
            string lName = userContent.get("Lastname");
            addData1 = "{"
                              "\n\t\"password\": \"" + pass + "\"" + ","
                              "\n\t\"Firstname\": " + "\"" + fName + "\"" + ","
                              "\n\t\"Lastname\": " + "\"" + lName + "\"\n"
                              "}";
            ofstream out1("./users/" + userEmail + ".json");
            out1 << addData1;
            out1.close();


            int y = 1;
            for (; y <= 50; y++) {
                string uIP;
                string uPort;
                string uLocation;
                string uUser;

                //cout << "Append Update"<< endl;
                ostringstream ss;
                ss << y;
                num = ss.str();
                //find the bridge with current bridge name

                if(userContent.contains("Bridge"+num)){

                    ifstream file2("./users/"+userEmail+".json");

                    std::istreambuf_iterator<char> eos2;
                    std::string s2(std::istreambuf_iterator<char>(file2), eos2);
                    cout << "S2: " << s2 <<endl;

                    file2.close();
                    bridgeNum = "Bridge" + num;
                    Json::Object bridge = userContent.get("Bridge" + num);

                    string name = bridge.get("name").orIfNull("name");
                    //cout <<"currentBridge " << currentBridge <<" name " << name <<endl;

                    if (currentBridge != name) {
                        //foundName = name;
                        uIP = bridge.get("ip").orIfNull("ip");
                        uPort = bridge.get("port").orIfNull("port");
                        uLocation = bridge.get("location").orIfNull("location");
                        uUser = bridge.get("user").orIfNull("user");
                        cout <<"uIP " << uIP<< endl;

                        s2.pop_back();
                        s2.pop_back();

                        string addData2 = "\n\t\"Bridge"+num+"\": {"

                                "\n\t\t\"name\": \""+name+"\""+","
                                          "\n\t\t\"ip\": \""+uIP+"\""+","
                                          "\n\t\t\"port\": \""+uPort+"\""+","
                                          "\n\t\t\"location\": \""+uLocation+"\""+","
                                          "\n\t\t\"user\": \""+uUser+"\""
                                          "\n\t}\n"
                                          "},";

                        s2 = s2+",";

                        s2 = s2 + addData2;
                        ofstream out2("./users/" + userEmail + ".json");
                        out2 << s2;
                        out2.close();
                    }
                }
            }

            ifstream file3("./users/"+userEmail+".json");
            std::istreambuf_iterator<char> eos3;
            std::string s3(std::istreambuf_iterator<char>(file3), eos3);
            cout << "S3 " << s3 << endl;
            file3.close();

            s3.pop_back();
            s3.pop_back();

            string finalData = "\n\t\""+bridgName+"\": {"

                    "\n\t\t\"name\": \""+updateBridgeList[1]+"\""+","
                                       "\n\t\t\"ip\": \""+updateBridgeList[2]+"\""+","
                                       "\n\t\t\"port\": \""+updateBridgeList[3]+"\""+","
                                       "\n\t\t\"location\": \""+updateBridgeList[4]+"\""+","
                                       "\n\t\t\"user\": \""+updateBridgeList[5]+"\""
                                       "\n\t}\n"
                                       "}";

            int index = bridgeList->findText(currentBridge);
            bridgeList->setItemText(index, brName);
            bridgeList->refresh();
            s3 = s3+",";

            s3 = s3 + finalData;
            cout<< "S4 " << s3 << endl;

            ofstream out3("./users/"+userEmail+".json");
            out3 << s3;

            out3.close();

            log->accept();
            }

        }
    }

/*! 
 *  \fn        showDialog
 *  \brief     Function that displays the addBridge dialog
 *  \details   The interface for the addBridge function, it uses a wdialog to make the information pop up
 *  \author    Vaughan Robertson      vrobert6@uwo.ca
 *  \version   Stage 3
 *  \date      2017, November 28
 */

void EditBridge::showDialog(Wt::WComboBox *bridgeSelect,string email, string brdgName,  string curBridge, string IP, string port, string location, string user) {
    bridgeList = bridgeSelect;

    //bridgeList = bridgeSelect;
    currentBridge = curBridge;
    bridgName = brdgName;
    userEmail = email;

    //Set data about the display
    WDialog *dialog = new WDialog("Edit Bridge");
    dialog->resize(600, 600);
    dialog->setAutoFocus(true);

    //Fill the dialog with the widgets for the sign in process
    bName = new WLabel("Bridge Name", dialog->contents());
    bridgeName = new Wt::WLineEdit(dialog->contents());
    bridgeName->setText(curBridge);

    bName->setBuddy(bridgeName);

    new WBreak(dialog->contents());

    bIP = new WLabel("Bridge IP", dialog->contents());
    bridgeIP = new Wt::WLineEdit(dialog->contents());
    bridgeIP->setText(IP);

    bIP->setBuddy(bridgeIP);

    new WBreak(dialog->contents());

    bPort = new WLabel("Bridge port", dialog->contents());
    bridgePort = new Wt::WLineEdit(dialog->contents());
    bridgePort->setText(port);
    bridgePort->setInputMask("99999");

    bPort->setBuddy(bridgePort);

    new WBreak(dialog->contents());

    bLocation = new WLabel("Bridge Location", dialog->contents());
    bridgeLocation = new Wt::WLineEdit(dialog->contents());
    bridgeLocation->setText(location);

    bLocation->setBuddy(bridgeLocation);


    new WBreak(dialog->contents());

    bUser = new WLabel("Bridge User", dialog->contents());
    bridgeUser = new Wt::WLineEdit(dialog->contents());
    bridgeUser->setText(user);

    bUser->setBuddy(bridgeUser);

    new WBreak(dialog->contents());
    new WBreak(dialog->contents());

    createBridge = new Wt::WPushButton("Edit Bridge", dialog->contents());

    new WBreak(dialog->contents());
    new WBreak(dialog->contents());

    invalidMessage = new WText(dialog->contents());
    invalidMessage->addStyleClass("invalid");

    new WBreak(dialog->contents());
    new WBreak(dialog->contents());
    new WBreak(dialog->contents());

    cancel = new Wt::WPushButton("Cancel", dialog->contents());


    createBridge->clicked().connect(std::bind([=]() { //On click of the create Bridge button

        this->onClick(dialog);

    }));

    cancel->clicked().connect(std::bind([=]() { //On click of cancel dialog button

        dialog->accept();
        delete this;

    }));

    dialog->finished().connect(std::bind([=]() { //What to do when the dialog is finished running

        delete dialog;
    }));

    dialog->show(); //Show the dialog box
    dialog->contents()->setStyleClass(WString("pop")); //Set the dialog css
    dialog->addStyleClass("addBridge");
}

