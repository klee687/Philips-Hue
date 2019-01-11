#ifndef AddBridge_h
#define AddBridge_h

#include <iostream>
#include <Wt/Http/Client>
#include <Wt/Http/Message>
#include <Wt/WDialog>
#include <Wt/WLabel>
#include <Wt/WBreak>
#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WText>
#include <Wt/WComboBox>

class AddBridge : public Wt::WDialog{

	public:
		void showDialog(std::string email, Wt::WComboBox *bridgeSelect);
		void onClick(Wt::WDialog *log);
		void addHandle(boost::system::error_code err, const Wt::Http::Message& response);
		bool inputChecker();
	
	private:

		std::string name;
		std::string ipAddress;
		std::string port;
		std::string location;
		std::string user;
		std::string newBridgeId;

		std::string userEmail;

		Wt::WComboBox * bridgeList;

		Wt::WText *invalidMessage; /**< Wt::WText Message displayed if incorrect data entered*/

		Wt::WLabel *bName; /**< Wt::WLabel Displays "first name" on screen*/ 
		Wt::WLineEdit *bridgeName; /**< Wt::WLineEdit allows user to enter there first name*/

		Wt::WLabel *bIP; /**< Wt::WLabel Displays "last name" on screen*/
		Wt::WLineEdit *bridgeIP; /**< Wt::WLineEdit allows user to enter there last name*/

		Wt::WLabel *bPort; /**< Wt::WLabel Displays "username" on screen*/
		Wt::WLineEdit *bridgePort; /**< Wt::WLineEdit allows user to enter there username*/

		Wt::WLabel *bLocation; /**< Wt::WLabel Displays "password" on screen*/
		Wt::WLineEdit *bridgeLocation; /**< Wt::WLineEdit allows user to enter there password*/

		Wt::WLabel *bUser; /**< Wt::WLabel Displays "confirm password" on screen*/
		Wt::WLineEdit *bridgeUser; /**< Wt::WLineEdit allows user to enter there confirm password*/

		Wt::WPushButton *createBridge; /**< Wt::WPushButton Allows the user to sign up with above data*/
		Wt::WPushButton *cancel; /**< Wt::WPushButton Allows the user to go to the sign in page*/
};

#endif