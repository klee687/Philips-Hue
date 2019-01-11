#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iterator>

#include <Wt/WApplication>
#include <Wt/Http/Response>
#include <Wt/WApplication>
#include <Wt/Http/Client>
#include <Wt/Http/Message>
#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WIntValidator>
#include <Wt/WComboBox>
#include <Wt/WHBoxLayout>
#include <Wt/WRegExpValidator>
#include <Wt/WPushButton>
#include <Wt/Json/Object>
#include <Wt/Json/Parser>
#include <Wt/WStackedWidget>

#include "signin.h"
#include "signup.h"
#include "hashFunction.h"
#include "AddBridge.h"
#include "EditBridge.h"
#include "RemoveBridge.h"
#include "HueEmulatorApplication.h"
#include "ChangeLights.h"
#include "groupContainer.h"
#include "SceduleContainer.h"

using namespace std;
using namespace Wt;


/*! 
 *  \brief     'HTML' class. Renders most of the html.
 *  \details   This class loads most of the web page. It also calls several get/put methods.
 *  \author    Gary Tohill      gtohill@uwo.ca
 *  \author    Nicholas Cheng       ncheng29@uwo.ca
 *  \author    Vaughan Robertson    vrobert6@uwo.ca
 *  \author    Kyungjoo Lee     klee687@uwo.ca
 *  \version   Stage 3
 *  \date      2017, November 28
 */
HueEmulatorApplication::HueEmulatorApplication(const WEnvironment& env) : WApplication(env) {

    this->useStyleSheet("./sheet.css");
    setTitle("H u e  E m u l a t o r");
 
    Wt::WContainerWidget *contain = new WContainerWidget(root());
    contain -> setStyleClass("top-box");    
    contain -> addWidget(new WText("<h2>H u e   E m u l a t o r</h2>"));

    Wt::WContainerWidget *NavContainer = new WContainerWidget(root());
    NavContainer->setStyleClass("nav-bar");

    Wt::WContainerWidget *contain2 = new WContainerWidget(root());

    WHBoxLayout *layout = new WHBoxLayout(contain2);

    middleContainer = new WStackedWidget(); 


    string *pointBridgeIP = &bridgeIP;
    string *pointBridgePort = &bridgePort;
    string *pointBridgeUser = &bridgeID;
  
    GroupContainer *contain2_1 = new GroupContainer(pointBridgeIP, pointBridgePort, pointBridgeUser);
    ChangeLights *contain2_2 = new ChangeLights(pointBridgeIP, pointBridgePort, pointBridgeUser);
    ScheduleContainer *contain2_3 = new ScheduleContainer(pointBridgeIP, pointBridgePort, pointBridgeUser);
    WContainerWidget *contain2_0 = new WContainerWidget();


    WContainerWidget *sideContainer = new WContainerWidget();
    sideContainer-> setStyleClass("side-box1");

    contain2-> setStyleClass("middle-box");
    middleContainer-> setStyleClass("center-box");
    contain2_0-> setStyleClass("center-box");
    contain2_1-> setStyleClass("center-box");
    contain2_2-> setStyleClass("center-box");
    contain2_3-> setStyleClass("center-box");

    middleContainer->addWidget(contain2_0);
    middleContainer->addWidget(contain2_1);
    middleContainer->addWidget(contain2_2);
    middleContainer->addWidget(contain2_3);


    layout->addWidget(sideContainer);
    layout->addWidget(middleContainer);

    contain2 -> setLayout(layout);
    
    displayEmail = new WText(NavContainer);
    displayEmail -> setStyleClass("middle-box");
    NavContainer->addWidget(displayEmail);

    signIn *signin = new signIn();
    bridgeSelect = new WComboBox();
    bridgeSelect->addItem("");
    signin->setPointer(displayEmail, bridgeSelect);

    signin->showDialog();

    WPushButton *logOut = new WPushButton("Log Out", NavContainer);

    NavContainer->addWidget(new WBreak);

    NavContainer->addWidget(new WText("Your are currently connected to "));
    WText *displayServer = new WText(WApplication::makeAbsoluteUrl("/"));

    NavContainer-> addWidget(displayServer);
    NavContainer->addWidget(new WBreak);

    //!enter light to change
    NavContainer->addWidget(new WBreak);

    NavContainer->addWidget(bridgeSelect);  //add bridge WComboBox to NavContainer

    editButton = new WPushButton("Edit Current Bridge", NavContainer); //update current bridge
    editButton->setEnabled(false);

    removeButton = new WPushButton("Remove Current Bridge", NavContainer); //remove current bridge
    removeButton->setEnabled(false);

    NavContainer->addWidget(new WBreak);

    WPushButton *plusBridge = new WPushButton("Add Bridge",NavContainer); //add bridge

    logOut->clicked().connect(this,&HueEmulatorApplication::onClick);
    plusBridge->clicked().connect(this,&HueEmulatorApplication::setBridge); //add bridge button
    editButton->clicked().connect(this,&HueEmulatorApplication::editBridge); //add bridge button
    removeButton->clicked().connect(this,&HueEmulatorApplication::removeBridge); //add bridge button


    NavContainer->addWidget(new WBreak);

    navLight = new WPushButton("Light Settings", NavContainer);
    navGroup = new WPushButton("Group Settings", NavContainer);
    navSchedule = new WPushButton("Schedule Settings", NavContainer);

    navLight->setEnabled(false);
    navGroup->setEnabled(false);
    navSchedule->setEnabled(false);

    navLight->clicked().connect(std::bind([=] () {

        middleContainer->setCurrentIndex(2);
    }));

    navGroup->clicked().connect(std::bind([=] () {

        middleContainer->setCurrentIndex(1);
    }));

    navSchedule->clicked().connect(std::bind([=] () {

        middleContainer->setCurrentIndex(3);
    }));


    bridgeSelect->activated().connect(std::bind([=] () { //On click of the sign up button Combo Box

	middleContainer->setCurrentIndex(0);
        string check = bridgeSelect->valueText().toUTF8();
        currentBridge = check;


        string tempEmail = displayEmail->text().toUTF8(); //login email

        istringstream buffer(tempEmail); //input stream of String
        istream_iterator<string> temp(buffer), end;
        vector<string> tokens(temp, end);

        email = tokens[3];


        if(check.compare("") != 0){

            editButton->setEnabled(true);
            removeButton->setEnabled(true);
            navLight->setEnabled(true);
            navGroup->setEnabled(true);
            navSchedule->setEnabled(true);
            
        
            bridgeName = bridgeSelect->currentText().toUTF8();

            Json::Object userContent;

            ifstream file("./users/"+email+".json");

            std::istreambuf_iterator<char> eos;
            std::string s(std::istreambuf_iterator<char>(file), eos);

            Json::parse(s, userContent);

            int x = 1;
            string num;


            for (; x <= 50; x++){

                ostringstream ss;
                ss << x;
                num = ss.str();

                if (userContent.contains("Bridge"+num)){

                    Json::Object bridge = userContent.get("Bridge"+num);

                    string name = bridge.get("name").orIfNull("name");

                    if(bridgeName == name){
						bridgeID = bridge.get("bridgeId").orIfNull("bridgeId");
                        bridgeIP = bridge.get("ip").orIfNull("ip");
                        bridgePort = bridge.get("port").orIfNull("port");
                        bridgeLocation = bridge.get("location").orIfNull("location");
                        bridgeUser = bridge.get("user").orIfNull("user");

                        x = 51;
                    }
                }

            }

            file.close();


        }
        else{

            editButton->setEnabled(false);
            removeButton->setEnabled(false);
            navLight->setEnabled(false);
            navGroup->setEnabled(false);
            navSchedule->setEnabled(false);
        }
        
    }));

}

/*! 
 *  \brief     'setBridge' class.
 *  \details   This class redirects the user on click.
 *  \author    Vaughan Robertson      vrobert6@uwo.ca
 *  \version   Stage 2
 *  \date      2017, November 10
 */

void HueEmulatorApplication::setBridge(){

    string tempEmail = displayEmail->text().toUTF8();

    istringstream buffer(tempEmail);
    istream_iterator<string> temp(buffer), end;
    vector<string> tokens(temp, end);

    email = tokens[3];

    AddBridge *add = new AddBridge();
    add->showDialog(email, bridgeSelect);
}

/*!
 *  \brief     'editBridge' class.
 *  \details   This class redirects the user on click.
 *  \author    Gary Tohill      gtohill@uwo.ca
 *  \version   Stage 2
 *  \date      2017, November 24
 */

void HueEmulatorApplication::editBridge(){

    //return variables
    std::string IP;
    std::string port;
    std::string location;
    std::string user;

    middleContainer->setCurrentIndex(0);

    string tempEmail = displayEmail->text().toUTF8();

    istringstream buffer(tempEmail);
    istream_iterator<string> temp(buffer), end;
    vector<string> tokens(temp, end);

    email = tokens[3];

    string cBridge = currentBridge;

    if(cBridge.compare("") != 0){

        Json::Object userContent;

        ifstream file("./users/"+email+".json");

        std::istreambuf_iterator<char> eos;
        std::string s(std::istreambuf_iterator<char>(file), eos);

        Json::parse(s, userContent);

        int x = 1;
        string num;

        for (; x <= 50; x++){

            ostringstream ss;
            ss << x;
            num = ss.str();

            if (userContent.contains("Bridge"+num)){

                Json::Object bridge = userContent.get("Bridge"+num);

                string name = bridge.get("name").orIfNull("name");

                if(cBridge == name){
                    brdgName = "Bridge"+num;
                    IP = bridge.get("ip").orIfNull("ip");
                    port = bridge.get("port").orIfNull("port");
                    location = bridge.get("location").orIfNull("location");
                    user = bridge.get("user").orIfNull("user");

                    x = 51;
                }
            }

        }

        file.close();

    }


    EditBridge *edit = new EditBridge();
    edit->showDialog(bridgeSelect, email, brdgName,cBridge, IP, port, location, user);
}

void HueEmulatorApplication::removeBridge() {
    //return variables
    std::string IP;
    std::string port;
    std::string location;
    std::string user;

    middleContainer->setCurrentIndex(0);

    string tempEmail = displayEmail->text().toUTF8();

    istringstream buffer(tempEmail);
    istream_iterator<string> temp(buffer), end;
    vector<string> tokens(temp, end);

    email = tokens[3];

    string cBridge = currentBridge;

    if(cBridge.compare("") != 0){

        Json::Object userContent;

        ifstream file("./users/"+email+".json");

        std::istreambuf_iterator<char> eos;
        std::string s(std::istreambuf_iterator<char>(file), eos);

        Json::parse(s, userContent);

        int x = 1;
        string num;

        for (; x <= 50; x++){

            ostringstream ss;
            ss << x;
            num = ss.str();

            if (userContent.contains("Bridge"+num)){

                Json::Object bridge = userContent.get("Bridge"+num);

                string name = bridge.get("name").orIfNull("name");

                if(cBridge == name){
                    brdgName = "Bridge"+num;
                    IP = bridge.get("ip").orIfNull("ip");
                    port = bridge.get("port").orIfNull("port");
                    location = bridge.get("location").orIfNull("location");
                    user = bridge.get("user").orIfNull("user");

                    x = 51;
                }
            }
 
        }

        file.close();

    }


    RemoveBridge *remove = new RemoveBridge();

    vector<WPushButton*> buttonObjects;

    buttonObjects.push_back(editButton);
    buttonObjects.push_back(removeButton);
    buttonObjects.push_back(navLight);
    buttonObjects.push_back(navGroup);
    buttonObjects.push_back(navSchedule);

    remove->showDialog(bridgeSelect, email, brdgName, cBridge, buttonObjects);//email, brdgName,cBridge, IP, port, location, user);


}
void HueEmulatorApplication::onClick(){

    WApplication::instance()->redirect("/");
}

















