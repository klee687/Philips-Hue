//
// Created by gary on 24/11/17.
//

#ifndef EditBridge_h
#define EditBridge_h

#include <iostream>

#include <Wt/WDialog>
#include <Wt/WLabel>
#include <Wt/WBreak>
#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WText>
#include <Wt/WComboBox>

using namespace std;
class EditBridge : public Wt::WDialog{

    public:
        void showDialog(Wt::WComboBox *bridgeSelect, string email, string bridgeName, string cBridge, string IP, string port, string location, string user);// Wt::WComboBox *bridgeSelect);
        void onClick(Wt::WDialog *log);
        bool inputChecker(string ipAddress, string port);


    private:

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

        string currentBridge;
        string bridgName;

};


#endif //EditBridge_h
