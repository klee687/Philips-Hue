#ifndef signin_h
#define signin_h

#include <iostream>

#include <Wt/WDialog>
#include <Wt/WLabel>
#include <Wt/WBreak>
#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WText>
#include <Wt/WComboBox>


/**
 * class: signIn
 * Creates a dialog box and allow the user to sign in
 */
class signIn : public Wt::WDialog{

	public:
		void showDialog();
		void onClick(WDialog *log);
		void invalid();
		void setPointer(Wt::WText *display, Wt::WComboBox *bridgeSelect);
		void bridgeListUpdate(std::string email);

	private:

		Wt::WComboBox *bridgeList;

		Wt::WDialog *dialog; /**< Wt::WDialog creates signin dialog box*/

		std::string *email; /**< String saves the email of the user signing in*/

		Wt::WText *displayEmail; /**< Wt::WText used to display the email on main page*/

		Wt::WText *invalidMessage; /**< Wt::WText Message displayed if incorrect data entered*/

		Wt::WLabel *uName; /**< Wt::WLabel Displays "username" on screen*/
		Wt::WLineEdit *user; /**< Wt::WLineEdit allows user to enter there username*/

		Wt::WLabel *pass; /**< Wt::WLabel Displays "password" on screen*/
		Wt::WLineEdit *password; /**< Wt::WLineEdit allows user to enter there password*/

		Wt::WPushButton *signInButton; /**< Wt::WPushButton Allows the user to sign in with above data*/
		Wt::WPushButton *signUpButton; /**< Wt::WPushButton Allows the user to go to the sign up page*/

};

#endif
