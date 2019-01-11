/*! 
 *	\class     AddBridge
 *	\file      AddBridge.cpp     	
 *  \brief     Allows the user to add a bridge
 *  \details   The class allows adding bridges, the class also sends a test request to the bridge and adds the bridge to a json file
 *  \author    Vaughan Robertson    vrobert6@uwo.ca
 *  \author    Gary Tohill      gtohill@uwo.ca
 *  \version   Stage 3
 *  \date      2017, November 28
 */

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <boost/filesystem.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#include <Wt/Http/Client>
#include <Wt/Http/Message>
#include <Wt/WDialog>
#include <Wt/WLabel>
#include <Wt/WBreak>
#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WText>
#include <Wt/Json/Object>
#include <Wt/Json/Parser>
#include <Wt/Json/Array>
#include <Wt/WApplication>

#include "hashFunction.h"
#include "AddBridge.h"

using namespace Wt;
using namespace std;

/*! 
 *	\fn 	   inputChecker
 *  \brief     Checks the input the user entered
 *  \details   Checks the ip address and the port values to see if they are valued before adding it
 *  \author    Vaughan Robertson      vrobert6@uwo.ca
 *  \version   Stage 3
 *  \date      2017, November 28
 */

bool AddBridge::inputChecker(){

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
 *	\fn 	   onClick
 *  \brief     Function called when button is clicked
 *  \details   Once the button is clicked this function checks with the bridge and saves all the values in variables
 *  \author    Vaughan Robertson      vrobert6@uwo.ca
 *  \author    Gary Tohill      gtohill@uwo.ca
 *  \version   Stage 3
 *  \date      2017, November 28
 */

void AddBridge::onClick(WDialog *log){

	name = bridgeName->text().toUTF8();
	ipAddress = bridgeIP->text().toUTF8();
	port = bridgePort->text().toUTF8();
	location = bridgeLocation->text().toUTF8();
	user = bridgeUser->text().toUTF8();

	if (name.compare("") == 0){

		name = "newdeveloper";
	}

	auto *client1 = new Http::Client(this);
	Http::Message message = Http::Message();
    client1->setTimeout(15);
    client1->setMaximumResponseSize(10 * 1024);

	//!std::cout << "NETWORK >>> " << p_sData << std::endl;
	string http = "http://";
	string ip = ipAddress+":";
	string port1 = port+"/";
	string api = "api/";
	string p_sData = "{devicetype: "+name+"}";

	string url = http+ip+port1+api;
	cout<< "URL " << url << endl;
	message.addHeader("Content-type","application/json");
	message.addBodyText(p_sData);

	client1->done().connect(boost::bind(&AddBridge::addHandle,this, _1,_2));
	if(client1->post(url,message))
	{
		WApplication::instance()->deferRendering();
		WApplication::instance()->deferRendering();
	}

		log->accept();
	}




/*! 
 *	\fn 	   showDialog
 *  \brief     Function that displays the addBridge dialog
 *  \details   The interface for the addBridge function, it uses a wdialog to make the information pop up
 *  \author    Vaughan Robertson      vrobert6@uwo.ca
 *  \version   Stage 3
 *  \date      2017, November 28
 */

void AddBridge::showDialog(string email, WComboBox *bridgeSelect){

	bridgeList = bridgeSelect;

	userEmail = email;

	//Set data about the display
	WDialog *dialog = new WDialog("Add Bridge");
	dialog -> resize(800, 800);
	dialog -> setAutoFocus(true);


	bName = new WLabel("Bridge Name",dialog->contents());
	bridgeName = new Wt::WLineEdit(dialog->contents());

	bName->setBuddy(bridgeName);

	new WBreak(dialog->contents());

	bIP = new WLabel("Bridge IP",dialog->contents());
	bridgeIP = new Wt::WLineEdit(dialog->contents());

	bIP->setBuddy(bridgeIP);

	new WBreak(dialog->contents());

	bPort = new WLabel("Bridge port",dialog->contents());
	bridgePort = new Wt::WLineEdit(dialog->contents());

	bPort->setBuddy(bridgePort);

	new WBreak(dialog->contents());

	bLocation = new WLabel("Bridge Location",dialog->contents());
	bridgeLocation = new Wt::WLineEdit(dialog->contents());

	bLocation->setBuddy(bridgeLocation);


	new WBreak(dialog->contents());

	bUser = new WLabel("Bridge User",dialog->contents());
	bridgeUser = new Wt::WLineEdit(dialog->contents());

	bUser->setBuddy(bridgeUser);

	new WBreak(dialog->contents());
	new WBreak(dialog->contents());

	createBridge = new Wt::WPushButton("Add Bridge", dialog->contents());

	new WBreak(dialog->contents());
	new WBreak(dialog->contents());

	invalidMessage = new WText(dialog->contents());
	invalidMessage->addStyleClass("invalid");
	
	new WBreak(dialog->contents());
	new WBreak(dialog->contents());
	new WBreak(dialog->contents());





	cancel = new Wt::WPushButton("Cancel", dialog->contents());


	createBridge->clicked().connect(std::bind([=] () { //On click of the creatBridge button

		this->onClick(dialog);
		
	}));

	 cancel->clicked().connect(std::bind([=] () { //On click of cancel dialog button

	 	dialog->accept();
	 	delete this;
		
	}));
 
	dialog->finished().connect(std::bind([=] () { //What to do when the dialog is finished running

		delete dialog;
	}));

	dialog->show(); //Show the dialog box
	dialog->contents()->setStyleClass(WString("pop")); //Set the dialog css
	dialog->addStyleClass("addBridge");

}

/*! 
 *	\fn 	   addHandle
 *  \brief     Function used to handle connection with the bridge
 *  \details   Uses the data uptained from the user to make a request with the HueEmulatorAPI
 *  \author    Gary Tohill      gtohill@uwo.ca
 *  \version   Stage 3
 *  \date      2017, November 28
 */

void AddBridge::addHandle(boost::system::error_code err, const Http::Message& response) {
    //!the parsing is a bit weird so apologies if anything goes wrong
    //WApplication::instance()->resumeRendering();
    cout << "-------" << endl;
    if (response.status() == 200) {
        Wt::Json::Object bodyContent;
        Wt::Json::Object stateContent;
        WApplication::instance()->resumeRendering();
        try {
            //!I can check what 'type' of get response it is by checking the existence of all tags (eg: bodyContent.contains(name)) (lazy)

            string responseBody = response.body().erase(0, 1);
            responseBody.pop_back();
            cout << "Response " << responseBody << endl;
            Json::parse(responseBody, bodyContent);//initializing the initial json object (all info of 1 light)
            if (bodyContent.contains("success")) { //get for a light should contain these parameters

                Wt::Json::Object userBridgeName;
                userBridgeName = bodyContent.get("success"); //defining the state object    (state info of 1 light)
                string bridgeId = userBridgeName.get("username");
                newBridgeId = bridgeId;
                cout << "User bridge name " << newBridgeId << endl;

                name = bridgeName->text().toUTF8();
                int find = bridgeList->findText(name);
                if(find == -1){
                    ipAddress = bridgeIP->text().toUTF8();
                    port = bridgePort->text().toUTF8();
                    location = bridgeLocation->text().toUTF8();
                    user = bridgeUser->text().toUTF8();

                    if (inputChecker()) {

                        Wt::Json::Object userContent;

                        string data;

                        if (boost::filesystem::exists("./users/" + userEmail + ".json")) {

                            ifstream file("./users/" + userEmail + ".json");

                            std::istreambuf_iterator<char> eos;
                            std::string s(std::istreambuf_iterator<char>(file), eos);

                            Json::parse(s, userContent);

                            file.close();

                            int x = 1;
                            string num;

                            string addData;

                            for (; x <= 50; x++) {

                                ostringstream ss;
                                ss << x;
                                num = ss.str();

                                if (!userContent.contains("Bridge" + num)) {

                                    s.pop_back();
                                    s.pop_back();

                                    addData = "\n\t\"Bridge" + num + "\": {"
                                            "\n\t\t\"bridgeId\": \"" + newBridgeId + "\"" + ","
                                                      "\n\t\t\"name\": \"" + name + "\"" + ","
                                                      "\n\t\t\"ip\": \"" + ipAddress + "\"" + ","
                                                      "\n\t\t\"port\": \"" + port + "\"" + ","
                                                      "\n\t\t\"location\": \"" + location + "\"" + ","
                                                      "\n\t\t\"user\": \"" + user + "\""
                                                      "\n\t}\n"
                                                      "}";

                                    bridgeList->addItem(name);
                                    x = 51;
                                }
                            }

                            s = s + ",";
                            s = s + addData;
                            ofstream out("./users/" + userEmail + ".json");
                            out << s;
                            out.close();

                        }
                        cout << "End of onClick" << endl;


                    }

                } else { //!otherwise we got the group (probably)
                    cout << "We got the group: " << endl;

                }
            }}
            catch(std::exception e)
            {
                cout << "This may have been a parsing error. Here is the output anyways: " << endl;
                cout << response.body() << endl;
              }
        }else{
            cout << "An error has occured in the put() request." << endl;
        }
        cout << "-------" << endl;

}