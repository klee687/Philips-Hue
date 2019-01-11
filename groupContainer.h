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

class GroupContainer: public Wt::WContainerWidget{
	
	public:
		GroupContainer(std::string *pointBridgeIP, std::string *pointBridgePort, std::string *pointBridgeUser);
		void handle(boost::system::error_code err, const Wt::Http::Message &response);
		void getGroup(std::string *pointBridgeIP, std::string *pointBridgePort, std::string *pointBridgeUser);
		void createGroup(std::string *pointBridgeIP, std::string *pointBridgePort, std::string *pointBridgeUser);
		void setGroup(std::string *pointBridgeIP, std::string *pointBridgePort, std::string *pointBridgeUser);
		void setGroupAttributes(std::string *pointBridgeIP, std::string *pointBridgePort, std::string *pointBridgeUser);
		void deleteGroup(std::string *pointBridgeIP, std::string *pointBridgePort, std::string *pointBridgeUser);

	private:
		Wt::WLineEdit *on3;
    	        Wt::WLineEdit *sat3;
   		Wt::WLineEdit *hue3;
   		Wt::WLineEdit *bri3;
   		Wt::WLineEdit *transition3;
   		Wt::WLineEdit *effect3;
   		Wt::WLineEdit *groupNum;//!for setting a group
   		Wt::WText *groupgetResult;
   		Wt::WLineEdit *groupLightNums;
   		Wt::WLineEdit *groupName;
   		Wt::WText *lightgetResult;

};
