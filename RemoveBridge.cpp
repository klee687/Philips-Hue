/*! 
 *  \class     RemoveBridge
 *  \file      RemoveBridge.cpp        
 *  \brief     Allows the user to remove a bridge
 *  \details   The class allows the removeal bridges, the class deletes the bridge from the user data
 *  \author    Gary Tohill      gtohill@uwo.ca
 *  \version   Stage 3
 *  \date      2017, November 28
 */
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <boost/filesystem.hpp>

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
#include "RemoveBridge.h"

using namespace Wt;
using namespace std;

/*! 
 *  \fn        onClick
 *  \brief     method called after clicking the button to remove the data
 *  \details   Removes the bridge from the json file and from the selection menu
 *  \author    Gary Tohill      gtohill@uwo.ca
 *  \version   Stage 3
 *  \date      2017, November 28
 */

void RemoveBridge::onClick(WDialog *log){
    cout << "ON CLICK" << endl;
    //variables from dialog box
    string brName = bridgeName->text().toUTF8();

    Wt::Json::Object userContent;

    //string data;
    if(boost::filesystem::exists("./users/"+userEmail+".json")) {
        //get data from exisiting file
        ifstream file1("./users/" + userEmail + ".json");
        std::istreambuf_iterator<char> eos1;
        std::string s(std::istreambuf_iterator<char>(file1), eos1);

        Json::parse(s, userContent);

        //find bridge using current bridge

        string num;
        string bridgeNum;

        //get data from json file and reenter it. then append updated json to file
        string addData;
        string pass = userContent.get("password");
        string fName = userContent.get("Firstname");
        string lName = userContent.get("Lastname");
        addData = "{"
                           "\n\t\"password\": \"" + pass + "\"" + ","
                           "\n\t\"Firstname\": " + "\"" + fName + "\"" + ","
                           "\n\t\"Lastname\": " + "\"" + lName + "\"\n"
                           "} ";

        int y = 1;
        int insertValue = 1;
        string insertNum;

        for (; y <= 50; y++) {
            string uID;
            string uIP;
            string uPort;
            string uLocation;
            string uUser;

            ostringstream ss;
            ostringstream ss1;
            ss << y;
            num = ss.str();
            ss1 << insertValue;
            insertNum = ss1.str();
            //find the bridge with current bridge name

            if(userContent.contains("Bridge"+num)){

                bridgeNum = "Bridge" + num;

                Json::Object bridge = userContent.get(bridgeNum);

                string name = bridge.get("name").orIfNull("name");

                if (currentBridge != name) {
                    cout <<"bridgName " << bridgName <<" name " << bridgeNum <<endl;
                    addData.pop_back();
                    addData.pop_back();
                    addData.pop_back();
                    addData = addData+",";

                    //foundName = name;
                    uID = bridge.get("bridgeId").orIfNull("bridgeId");
                    uIP = bridge.get("ip").orIfNull("ip");
                    uPort = bridge.get("port").orIfNull("port");
                    uLocation = bridge.get("location").orIfNull("location");
                    uUser = bridge.get("user").orIfNull("user");
                    cout <<"uIP " << uIP<< endl;

                    addData  += "\n\t\"Bridge"+insertNum+"\": {"
                                    "\n\t\t\"bridgeId\": \""+uID+"\""+","
                                    "\n\t\t\"name\": \""+name+"\""+","
                                    "\n\t\t\"ip\": \""+uIP+"\""+","
                                    "\n\t\t\"port\": \""+uPort+"\""+","
                                    "\n\t\t\"location\": \""+uLocation+"\""+","
                                    "\n\t\t\"user\": \""+uUser+"\""
                                    "\n\t}\n"
                                    "\t}";
                    cout<<"ENd of bridge insert" << endl;
                }

                else{
                    cout << "currentBridge " << currentBridge << endl;
                    insertValue--;
                    int index = bridgeList->findText(currentBridge);

                    bridgeList->removeItem(index);
                    bridgeList->refresh();

                }
            }
            insertValue++;

        }
        addData.pop_back();
        addData.pop_back();
        addData+="}";

        ofstream out("./users/"+userEmail+".json");
        out << addData;
        out.close();
        cout <<" end of ON CLICK"<< endl;
        log->accept();
    }

}

/*! 
 *  \fn        showDialog
 *  \brief     Function that displays the addBridge dialog
 *  \details   The interface for the addBridge function, it uses a wdialog to make the information pop up
 *  \author    Gary Tohill      gtohill@uwo.ca
 *  \version   Stage 3
 *  \date      2017, November 28
 */

void RemoveBridge::showDialog(Wt::WComboBox *bridgeSelect,string email, string brdgName, string curBridge, vector<WPushButton*> buttons){
    //get variables

    buttonObjects = buttons;

    bridgeList = bridgeSelect;
    currentBridge = curBridge;
    bridgName = brdgName;
    userEmail = email;
    cout << "Bridge Name: " << bridgName << endl;
    //Set data about the display
    WDialog *dialog = new WDialog("Remove Bridge");
    dialog->resize(600, 600);
    dialog->setAutoFocus(true);

    //Fill the dialog with the widgets for the sign in process
    bName = new WLabel("Remove Bridge Name", dialog->contents());
    bridgeName = new Wt::WLineEdit(dialog->contents());
    bridgeName->setText(curBridge);

    bName->setBuddy(bridgeName);

    new WBreak(dialog->contents());
    createBridge = new Wt::WPushButton("Remove Bridge", dialog->contents());

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

