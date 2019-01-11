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

class ChangeLights: public Wt::WContainerWidget{
	
	public:
		ChangeLights(std::string *pointBridgeIP, std::string *pointBridgePort, std::string *pointBridgeUser);
		void setLightName(std::string *pointBridgeIP, std::string *pointBridgePort, std::string *pointBridgeUser);
		void getLight(std::string *pointBridgeIP, std::string *pointBridgePort, std::string *pointBridgeUser);
		void getAllLights(std::string *pointBridgeIP, std::string *pointBridgePort, std::string *pointBridgeUser);
		void setLight(std::string *pointBridgeIP, std::string *pointBridgePort, std::string *pointBridgeUser);
		void handle();
		void handle(boost::system::error_code err, const Wt::Http::Message &response);

	private:
	    Wt::WLineEdit *on;
		Wt::WLineEdit *sat;
		Wt::WLineEdit *hue;
		Wt::WLineEdit *bri;
		Wt::WLineEdit *transition;
		Wt::WLineEdit *effect;
		Wt::WLineEdit *lightNum;
		Wt::WPushButton *button;
		Wt::WLineEdit *lightName;
		Wt::WText *lightgetResult;

};
