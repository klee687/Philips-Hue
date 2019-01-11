#include <iostream>
#include <string>
#include <vector>

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
#include <Wt/WBreak>
#include <Wt/WText>

class ScheduleContainer: public Wt::WContainerWidget{
	
	public:
		ScheduleContainer(std::string *pointBridgeIP, std::string *pointBridgePort, std::string *pointBridgeUser);
		void handle(boost::system::error_code err, const Wt::Http::Message &response);
		void createSchedule(std::string *pointBridgeIP, std::string *pointBridgePort, std::string *pointBridgeUser);
		void getSchedule(std::string *pointBridgeIP, std::string *pointBridgePort, std::string *pointBridgeUser);
		void setSchedule(std::string *pointBridgeIP, std::string *pointBridgePort, std::string *pointBridgeUser);
		void deleteSchedule(std::string *pointBridgeIP, std::string *pointBridgePort, std::string *pointBridgeUser);


	private:

		Wt::WLineEdit *on1;
		Wt::WLineEdit *sat1;
		Wt::WLineEdit *hue1;
		Wt::WLineEdit *bri1;
		Wt::WLineEdit *transition1;
		Wt::WLineEdit *effect1;
		Wt::WLineEdit *LorG;    
		Wt::WLineEdit *LorG1;
		Wt::WLineEdit *scheduleName;
		Wt::WLineEdit *description;
		Wt::WLineEdit *lightNum1;
		Wt::WLineEdit *groupNum1;
		Wt::WLineEdit *description1;
		Wt::WLineEdit *scheduleNum;
		Wt::WLineEdit *scheduleNum1;
		Wt::WLineEdit *scheduleNum2;
		Wt::WLineEdit *lightNum2;
		Wt::WLineEdit *groupNum2;
		Wt::WLineEdit *on2;
		Wt::WLineEdit *sat2;
		Wt::WLineEdit *hue2;
		Wt::WLineEdit *bri2;
		Wt::WLineEdit *transition2;
		Wt::WLineEdit *effect2;
		Wt::WText *schedulegetResult;
    	Wt::WLineEdit *scheduleName1;
   		Wt::WLineEdit *localtime;
   		Wt::WLineEdit *localtime1;
   		Wt::WText *lightgetResult;


};
