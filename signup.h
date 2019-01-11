
#ifndef signup_h
#define signup_h

#include <Wt/WDialog>
#include <Wt/WLabel>
#include <Wt/WBreak>
#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WText>

class signUp : public Wt::WDialog{

	public:
		void showDialog();
		void onClick(Wt::WDialog *log);
		void valid();
	
	private:

		Wt::WText *invalidMessage; /**< Wt::WText Message displayed if incorrect data entered*/

		Wt::WLabel *fname; /**< Wt::WLabel Displays "first name" on screen*/ 
		Wt::WLineEdit *firstName; /**< Wt::WLineEdit allows user to enter there first name*/

		Wt::WLabel *lname; /**< Wt::WLabel Displays "last name" on screen*/
		Wt::WLineEdit *lastName; /**< Wt::WLineEdit allows user to enter there last name*/

		Wt::WLabel *uName; /**< Wt::WLabel Displays "username" on screen*/
		Wt::WLineEdit *username; /**< Wt::WLineEdit allows user to enter there username*/

		Wt::WLabel *pass; /**< Wt::WLabel Displays "password" on screen*/
		Wt::WLineEdit *password; /**< Wt::WLineEdit allows user to enter there password*/

		Wt::WLabel *Cpass; /**< Wt::WLabel Displays "confirm password" on screen*/
		Wt::WLineEdit *confirmpass; /**< Wt::WLineEdit allows user to enter there confirm password*/

		Wt::WPushButton *signup; /**< Wt::WPushButton Allows the user to sign up with above data*/
		Wt::WPushButton *signin; /**< Wt::WPushButton Allows the user to go to the sign in page*/
};

#endif