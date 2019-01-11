#include <iostream>
#include <fstream>
#include <string>

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

#include "signup.h"
#include "signin.h"
#include "hashFunction.h"

using namespace Wt;
using namespace std;

/**
 * Signup onClick
 * Handles signup button press
 * param log WDialog created in the showDialog method
 */

void signUp::onClick(WDialog *log){

	//Convert the user input into varables
	hashFunction hash(password->text().toUTF8()); 
	hashFunction chash(confirmpass->text().toUTF8());

	string email = username->text().toUTF8();

	string first = firstName->text().toUTF8();

	string last = lastName->text().toUTF8();

	if(first.compare("") == 0 || last.compare("")== 0 || email.compare("")== 0 || 
		password->text().toUTF8().compare("")== 0 || confirmpass->text().toUTF8().compare("") == 0){
		//If any of the data fields are empty

		invalidMessage->setText("Fill in all of the fields");
	}

	else if(ifstream("./users/"+email+".json")){ //If the user already exists

		invalidMessage->setText("The email is already taken");
	}

	else if(hash.getHash().compare(chash.getHash()) != 0){ //If the password and confirm password don't match

		invalidMessage->setText("The passwords do not match");
	}

	else if(password->text().toUTF8().size() < 8 or password->text().toUTF8().size() > 20){
		//If the password is not between 8 and 20 characters

		invalidMessage->setText("The password has to be between 8 and 20 characters");
	}

	else{ //If all of the data is valid

		string json;

		json = "{"
			"\n\t\"password\": \""+hash.getHash()+"\""+","
			"\n\t\"Firstname\": "+"\""+first+"\""+","
			"\n\t\"Lastname\": "+"\""+last+"\"\n"
		"}";

    	ofstream out("./users/"+email+".json");

    	out << json;

    	out.close();
		
		log->accept(); //Accept the dialog box
	}

}

void signUp::valid(){

	
}

/**
 * Signup showDialog
 * Creates a Wt::Dialog and handles signing up
 */

void signUp::showDialog(){

	//Set data about the display
	WDialog *dialog = new WDialog("Sign Up");
	dialog -> resize(400, 370);
	dialog -> setAutoFocus(true);

	//Fill the dialog with the widgets for the sign in process
	fname = new WLabel("First Name",dialog->contents());
	firstName = new Wt::WLineEdit(dialog->contents());

	fname->setBuddy(firstName);

	new WBreak(dialog->contents());

	lname = new WLabel("Last Name",dialog->contents());
	lastName = new Wt::WLineEdit(dialog->contents());

	lname->setBuddy(lastName);

	new WBreak(dialog->contents());

	uName = new WLabel("Email",dialog->contents());
	username = new Wt::WLineEdit(dialog->contents());

	uName->setBuddy(username);

	new WBreak(dialog->contents());

	pass = new WLabel("Password",dialog->contents());
	password = new Wt::WLineEdit(dialog->contents());
	password->setEchoMode(WLineEdit::Password);

	pass->setBuddy(password);


	new WBreak(dialog->contents());

	Cpass = new WLabel("Confirm Password",dialog->contents());
	confirmpass = new Wt::WLineEdit(dialog->contents());
	confirmpass->setEchoMode(WLineEdit::Password);

	Cpass->setBuddy(confirmpass);

	new WBreak(dialog->contents());

	Wt::WPushButton *signup = new Wt::WPushButton("Sign Up", dialog->contents());

	new WBreak(dialog->contents());

	invalidMessage = new WText(dialog->contents());
	invalidMessage->addStyleClass("invalid");

	new WBreak(dialog->contents());
	Wt::WPushButton *signin = new Wt::WPushButton("Already have an account? Sign In", dialog->contents());


	signup->clicked().connect(std::bind([=] () { //On click of the sign up button

		this->onClick(dialog);
		
	}));

	signin->clicked().connect(std::bind([=] () { //On click of the salready have account button

		dialog->accept();
		delete this;
		
	}));
 
	dialog->finished().connect(std::bind([=] () { //What to do when the dialog is finished running

	delete dialog;
	}));

	dialog->show(); //Show the dialog box
	dialog->contents()->setStyleClass(WString("pop")); //Set the dialog css
	dialog->addStyleClass("pop-up");

}

