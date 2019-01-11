/*! 
 *  \brief     'global variable' class.
 *  \details   This class holds many variables used across different methods. It also initialized some functions.
 *  \author    Gary Tohill		gtohill@uwo.ca
 *  \author    Nicholas Cheng		ncheng29@uwo.ca
 *  \version   Stage 2
 *  \date      2017, November 10
 */

#ifndef HueEmulatorApplication_h
#define HueEmulatorApplication_h

#include <iostream>
#include <string>
#include <vector>

#include <Wt/WApplication>
#include <Wt/WApplication>
#include <Wt/WResource>
#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/Http/Client>
#include <Wt/Http/Message>
#include <Wt/WComboBox>
#include <Wt/WPushButton>


class HueEmulatorApplication: public Wt::WApplication {


	public:
		HueEmulatorApplication(const Wt::WEnvironment &env);
		void onClick();
		void setBridge();
		void editBridge();
		void removeBridge();

	private:

		std::string email;
		Wt::WText *displayEmail;
		Wt::WComboBox *bridgeSelect;
		Wt::WPushButton *editButton;
		Wt::WPushButton *removeButton;

		std::string bridgeName;
		std::string bridgeIP;
		std::string bridgePort;
		std::string bridgeLocation;
		std::string bridgeUser;
    	        std::string currentBridge;
		std::string brdgName;
  		std::string bridgeID;

		Wt::WPushButton *navLight;
		Wt::WPushButton *navGroup;
		Wt::WPushButton *navSchedule;

		Wt::WStackedWidget *middleContainer;


		void setLight();
		void setLightName();
		void getLight();
		void createGroup();
		void setGroup();
		void setGroupAttributes();
		void getGroup();
		void deleteGroup();
		void deleteSchedule();
		void setSchedule();
		void createSchedule();
		void getSchedule();
};

#endif
