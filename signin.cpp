/*!
 *	\class     signin 
 *	\file      signin.cpp   	
 *  \brief     Allows the user to sign into the application
 *  \details   The class allows the user to sign in to an already created account, and checks the json to confirm that the person is in the database
 *  \author    Vaughan Robertson    vrobert6@uwo.ca
 *  \version   Stage 3
 *  \date      2017, November 28
 */

#include <iostream>
#include <string>
#include <fstream>

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

#include "signin.h"
#include "signup.h"
#include "hashFunction.h"

using namespace Wt;
using namespace std;

void signIn::bridgeListUpdate(string email){

	Wt::Json::Object userContent;


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

        	string bridgeName = bridge.get("name").orIfNull("name");

        	bridgeList->addItem(bridgeName);
        }

    }

    file.close();
}


/*! 
 *	\fn 	   onClick
 *  \brief     Signin onClick
 *  \details   Handles signin button click and validates the users input
 *  \author    Vaughan Robertson      vrobert6@uwo.ca
 *  \version   Stage 3
 *  \date      2017, November 28
 */

void signIn::onClick(WDialog *log){

	string passCheck;
	string uName = user->text().toUTF8();

	hashFunction hash(password->text().toUTF8());

	transform(uName.begin(), uName.end(), uName.begin(), ::tolower);

	if(boost::filesystem::exists("./users/"+uName+".json")){ //If username already exists

		ifstream file("./users/"+uName+".json");

		std::istreambuf_iterator<char> eos;
		std::string s(std::istreambuf_iterator<char>(file), eos);

		Wt::Json::Object userContent;

		Json::parse(s, userContent);

		passCheck = userContent.get("password").orIfNull("password");

		file.close();


	 	if((hash.getHash()).compare(passCheck) == 0){ //compare with typed in password

	 		displayEmail->setText("Signed in as "+ uName +"    "); //display email on screen

	 		log->accept();
	 		this->bridgeListUpdate(uName);

	 	}

	 	else{ //If password does not match the users password

	 		this->invalid();
	 	}
	 }

	 else{ //If user does not exist

	 	this->invalid();
	 }

}

/*! 
 *	\fn 	   invalid
 *  \brief     Signin invalid
 *  \details   Handles printing an error message on invalid input
 *  \author    Vaughan Robertson      vrobert6@uwo.ca
 *  \version   Stage 3
 *  \date      2017, November 28
 */

void signIn::invalid(){

	invalidMessage->setText("The username or password that you entered was incorrect");
}

/*! 
 *	\fn 	   showDialog
 *  \brief     Signin showDialog
 *  \details   Creates a Wt::Dialog for the sign in interface and handles signing in
 *  \author    Vaughan Robertson      vrobert6@uwo.ca
 *  \version   Stage 3
 *  \date      2017, November 28
 */

void signIn::showDialog(){

	//Set data about the display
	WDialog *dialog = new WDialog("Sign in");
	dialog -> resize(400, 370);
	dialog -> setAutoFocus(true);

	//Fill the dialog with the widgets for the sign in process
	new WBreak(dialog->contents());
	uName = new WLabel("Email",dialog->contents());
	user = new Wt::WLineEdit(dialog->contents());

	uName->setBuddy(user);

	new WBreak(dialog->contents());
	new WBreak(dialog->contents());

	pass = new WLabel("Password",dialog->contents());
	password = new Wt::WLineEdit(dialog->contents());
	password->setEchoMode(WLineEdit::Password);

	pass->setBuddy(password);

	new WBreak(dialog->contents());
	new WBreak(dialog->contents());

	signInButton = new Wt::WPushButton("Sign In", dialog->contents());

	new WBreak(dialog->contents());
	new WBreak(dialog->contents());

	invalidMessage = new WText(dialog->contents());
	invalidMessage->addStyleClass("invalid");
	
	new WBreak(dialog->contents());
	new WBreak(dialog->contents());
	new WBreak(dialog->contents());	

	signUpButton = new Wt::WPushButton("Sign Up", dialog->contents());

	signInButton->clicked().connect(std::bind([=] () { //On click of the sign in button

		this->onClick(dialog);
		
	}));


	signUpButton->clicked().connect(std::bind([=] () { //On click of the sign up button

		signUp *signup = new signUp();
		signup->showDialog();
		invalidMessage->setText("");
		
	}));

	dialog->finished().connect(std::bind([=] () { //What to do when the dialog is finished running

	delete dialog;

	}));

	dialog->show(); //show the dialog box

	dialog->contents()->setStyleClass(WString("pop")); //set css styles
	dialog->addStyleClass("pop-up");

	return;

}

/**
 * Signin setPointer
 * edit pointers from the display to update information
 * param getName string of the persons display email
 * param display WText widget to display the email from the sign in on screen
 */

void signIn::setPointer(WText *display, Wt::WComboBox *bridgeSelect){

	bridgeList = bridgeSelect;

	displayEmail = display;
}


