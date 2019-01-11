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

#include "SceduleContainer.h"

using namespace Wt;
using namespace std;

/*! 
 *  \brief     'schedule container' class.
 *  \details   This class generates most of the html for the schedule subsection, and calls almost every other method.
 *  \author    Gary Tohill		gtohill@uwo.ca
 *  \author    Nicholas Cheng		ncheng29@uwo.ca
 *  \author    Kyunjoo Lee
 *  \version   Stage 3
 *  \date      2017, November 28
 */
ScheduleContainer::ScheduleContainer(string *pointBridgeIP, string *pointBridgePort, string *pointBridgeUser){
    //!CREATE A SCHEDULE //
    this->addWidget(new WText("Create a Schedule"));       //<'CREATE SCHEDULE' TITLE>
    this->addWidget(new WBreak);
    this->addWidget(new WText("Schedule Name: "));        //<INSERT A SCHEDULE NAME>
    scheduleName = new WLineEdit(this);
    scheduleName->setValidator(new Wt::WRegExpValidator("[^\"]*"));
    this->addWidget(new WBreak);
    this->addWidget(new WText("Schedule Description: "));    //<DESCRIPTION>
    description = new WLineEdit(this);
    description->setValidator(new Wt::WRegExpValidator("[^\"]*"));
    this->addWidget(new WBreak);

    // LET USER TO CHOOSE TO SCHEDULE A LIGHT OR A GROUP
    this->addWidget(new WText("Group #: "));
    groupNum1 = new WLineEdit(this);
    groupNum1->setValidator(new Wt::WIntValidator(0, 200));
    this->addWidget(new WBreak);
    this->addWidget(new WText("light #: "));
    lightNum1 = new WLineEdit(this);
    lightNum1->setValidator(new Wt::WIntValidator(0, 200));

    //ASK USER TO CREATE TIME FOR THEIR SCHEDULE.
    this->addWidget(new WBreak);
    this->addWidget(new WText("Create a schedule time | YYYY-MM-DDThh:mm:ss : "));
    localtime = new WLineEdit(this);
    this->addWidget(new WBreak);

    //!TURN LIGHT ON/OFF
    this->addWidget(new WText("1 = On | 0 = Off: "));
    on1 = new WLineEdit(this);
    on1->setValidator(new Wt::WIntValidator(0,1));
    this->addWidget(new WBreak);

    //!CHANGE BRIGHTNESS
    this->addWidget(new WBreak);
    this->addWidget(new WText("Brightness [0-254]: "));
    bri1 = new WLineEdit(this);
    bri1->setValidator(new Wt::WIntValidator(0,254));

    //!CHANGE TRANSITION
    this->addWidget(new WBreak);
    this->addWidget(new WText("Transition Time (integer, eg: 10 = 1 second): "));
    transition1 = new WLineEdit(this);
    transition1->setValidator(new Wt::WIntValidator());

    //!CHANGE EFFECT
    this->addWidget(new WBreak);
    this->addWidget(new WText("Effect ('none' or 'colorloop'): "));
    effect1 = new WLineEdit(this);
    effect1->setValidator(new Wt::WRegExpValidator("(none|colorloop)"));
    this->addWidget(new WBreak);

    //BUTTON FOR A CREATE SCHEDULE
    WPushButton *button9999 = new WPushButton("Create Schedule", this);    //!< create a button
    button9999->setMargin(5, Left);                                 //!< add 5 pixels margin
    this->addWidget(new WBreak);

        button9999->clicked().connect(std::bind([=] () {
        if (scheduleName->validate() && description->validate() && on1->validate() && lightNum1->validate() 
           && groupNum1->validate() && localtime->validate() && bri1->validate() && effect1->validate() 
           && transition1->validate()){
            this->createSchedule(pointBridgeIP, pointBridgePort, pointBridgeUser);
        }else{
	    schedulegetResult->setText("<b>An Error Occured : One of the values you entered in invalid</b>");
        }
    }));

    // <SET A SCHEDULE> //
    this->addWidget(new WText("Set a schedule. "));
    this->addWidget(new WBreak); 
    this->addWidget(new WText("Schedule Name: "));
    scheduleName1 = new WLineEdit(this);
    scheduleName1->setValidator(new Wt::WRegExpValidator("[^\"]*"));
    this->addWidget(new WBreak);

    // LET USER TO CHOOSE TO SCHEDULE A LIGHT OR A GROUP
    this->addWidget(new WText("Group #: "));
    groupNum2 = new WLineEdit(this);
    groupNum2->setValidator(new Wt::WIntValidator(0, 200));
    this->addWidget(new WBreak);
    this->addWidget(new WText("light #: "));
    lightNum2 = new WLineEdit(this);
    lightNum2->setValidator(new Wt::WIntValidator(0, 200));

    // LET USER TO CHOOSE TO CHOOSE WHICH SCHEDULE THEY WANT TO MODIFY.
    this->addWidget(new WText("schedule number: "));
    scheduleNum = new WLineEdit(this);
    scheduleNum->setValidator(new Wt::WIntValidator(0, 100));
    this->addWidget(new WBreak);

    this->addWidget(new WText("Schedule Description: "));
    description1 = new WLineEdit(this);
    description1->setValidator(new Wt::WRegExpValidator("[^\"]*"));
    this->addWidget(new WBreak);

    //ASK USER TO SET TIME FOR THEIR SCHEDULE.
    this->addWidget(new WText("Set a schedule time | YYYY-MM-DDThh:mm:ss : "));
    localtime1 = new WLineEdit(this);    
    this->addWidget(new WBreak);

    //TURN LIGHT ON/OFF
    this->addWidget(new WText("1 = On | 0 = Off: "));
    on2 = new WLineEdit(this);
    on2->setValidator(new Wt::WIntValidator(0,1));
    this->addWidget(new WBreak);

    //CHANGE BRIGHTNESS
    this->addWidget(new WBreak);
    this->addWidget(new WText("Brightness [0-254]: "));
    bri2 = new WLineEdit(this);
    bri2->setValidator(new Wt::WIntValidator(0,254));

    //CHANGE TRANSITION
    this->addWidget(new WBreak);
    this->addWidget(new WText("Transition Time (integer, eg: 10 = 1 second): "));
    transition2 = new WLineEdit(this);
    transition2->setValidator(new Wt::WIntValidator());

    //CHANGE EFFECT
    this->addWidget(new WBreak);
    this->addWidget(new WText("Effect ('none' or 'colorloop'): "));
    effect2 = new WLineEdit(this);
    effect2->setValidator(new Wt::WRegExpValidator("(none|colorloop)"));
    this->addWidget(new WBreak);

    //BUTTON TO SUBMIT THE 'CHANGE SCHEDULE'
    WPushButton *button9 = new WPushButton("Submit",this);
    button9-> setMargin(5,Left);
    this->addWidget (new WBreak);
    this->addWidget (new WBreak);
    button9->clicked().connect(std::bind([=] () {
        if (effect2->validate() && transition2->validate() && bri2->validate() && on2->validate() && localtime1->validate() && description1->validate() && scheduleNum->validate() && scheduleName1->validate() && groupNum2->validate() && lightNum2->validate()){
            this->setSchedule(pointBridgeIP, pointBridgePort, pointBridgeUser);
        }else{
            schedulegetResult->setText("<b>An Error Occured : One of the values you entered in invalid</b>");
        }
    }));
    this->addWidget(new WBreak);

    // <GET A SCHEDULE>
    this->addWidget(new WBreak);
    this->addWidget(new WText("Get Schedule #: ")); //ASK USER TO SEE WHAT SCHEDULE THEY WANT TO GET
    scheduleNum1 = new WLineEdit(this);
    scheduleNum1->setValidator(new Wt::WIntValidator(0, 200));
    this->addWidget(new WBreak);

    //'GET A SCHEDULE' SUBMISSION BUTTON
    WPushButton *button1010 = new WPushButton("Submit", this);
    button1010->setMargin(5, Left);
    this->addWidget(new WBreak);
    this->addWidget(new WBreak);
    button1010->clicked().connect(std::bind([=] () {
        if (scheduleNum1->validate()){
            this->getSchedule(pointBridgeIP, pointBridgePort, pointBridgeUser);
        }else{
            schedulegetResult->setText("<b>An Error Occured : One of the values you entered in invalid</b>");
        }
    }));

    // <DELETE A SCHEDULE>
    this->addWidget(new WBreak);
    this->addWidget(new WText("Delete a schedule. "));

    this->addWidget(new WBreak);
    this->addWidget(new WText("schedule number: ")); //ASK USER TO SEE WHAT SCHEDULE THEY WANT TO DELETE
    scheduleNum2 = new WLineEdit(this);
    scheduleNum2->setValidator(new Wt::WIntValidator(0, 200));
    this->addWidget(new WBreak);

    //'DELETE A SCHEDULE' SUBMISSION BUTTON
    WPushButton *button111 = new WPushButton("Delete Schedule", this);
    button111->setMargin(5, Left);
    button111->clicked().connect(std::bind([=] () {
        if (scheduleNum2->validate()){
            this->deleteSchedule(pointBridgeIP, pointBridgePort, pointBridgeUser);
        }else{
            schedulegetResult->setText("<b>An Error Occured : One of the values you entered in invalid</b>");
        }
    }));

    schedulegetResult = new WText(this);
    schedulegetResult->setFocus();
}

/*!
 *  \brief     'create schedule' class.
 *  \details   This class creates a new schedule
 *  \author    Kyungjoo Lee, Nicholas Cheng
 *  \date      2017, November 28
 */
void ScheduleContainer::createSchedule(string *pointBridgeIP, string *pointBridgePort, string *pointBridgeUser) {
    Http::Client client;//!< = new Http::Client(this);
    Http::Message message = Http::Message();

    string http = "http://";
    string ipAddress = *pointBridgeIP+":";
    string port = *pointBridgePort+"/";
    string api = "api/";
    string user = *pointBridgeUser+"/";
    string schedules = "schedules";
    string groups = "\"/groups/";
    string action = "/action\"";

    string lights = "\"/lights/";
    string state = "/state\"";
    string light_num = lightNum1->text().toUTF8();
    string group_num = groupNum1->text().toUTF8();
    string url = http+ipAddress+port+api+user+schedules;
    string time = localtime->text().toUTF8();
    string light_on_off = on1->text().toUTF8();
    string bri_ = bri1->text().toUTF8();
    string trans_ = transition1->text().toUTF8();
    string effect_ = effect1->text().toUTF8();
    
    message.addHeader("Content-type","application/json");
       
    int comma = 0;
    stringstream ssSendData;
    string schedule_name = scheduleName->text().toUTF8();
    string sche_des = description->text().toUTF8();

    ssSendData << "{";

    if(!schedule_name.empty()){
    ssSendData <<"\"name\"";
    ssSendData <<":";
    ssSendData <<"\"";
    ssSendData << schedule_name;
    ssSendData <<"\"";
    ssSendData <<",";
    comma = 1;
    }

    if(!sche_des.empty()){
    ssSendData <<"\"description\"";
    ssSendData <<":";
    ssSendData <<"\"";
    ssSendData <<sche_des;
    ssSendData <<"\"";
    ssSendData <<",";
    comma = 1;
    }

    ssSendData <<"\"command\"";
    ssSendData <<":";
    ssSendData <<"{";

    if(!group_num.empty()){
      ssSendData <<"\"address\"";
      ssSendData <<":";
      ssSendData << groups+group_num+action; 
      ssSendData <<",";
      comma = 1;

   }else if(!light_num.empty()){
      ssSendData <<"\"address\"";
      ssSendData <<":";
      ssSendData << lights+light_num+state; 
      ssSendData <<",";
      comma = 1;

   }else if(!light_num.empty() && !group_num.empty()){
     ssSendData <<"\"address\"";
     ssSendData <<":";
     ssSendData << "";
     ssSendData <<",";
     comma = 1;
   }

   else{
     ssSendData <<"\"address\"";
     ssSendData <<":";
     ssSendData << "";
     ssSendData << ",";
   }

    ssSendData <<"\"method\"";
    ssSendData <<":";
    ssSendData <<"\"PUT\"";
    ssSendData <<",";
    ssSendData <<"\"body\"";
    ssSendData <<":";
    ssSendData <<"{";
      
    if (light_on_off == "1"){
      ssSendData <<"\"on\"";
      ssSendData <<":";      
      ssSendData << "\"true\"";
      ssSendData <<",";
      comma = 1;
   }else{
      ssSendData <<"\"on\"";
      ssSendData <<":";   
      ssSendData << "\"false\"";
      ssSendData <<",";
      comma = 1;
    }

    if(!bri_.empty()){
    ssSendData <<"\"bri\"";
    ssSendData <<":";
    ssSendData <<"\"";
    ssSendData << bri_;
    ssSendData <<"\"";
    comma = 1;
    }
    else{
    ssSendData <<"\"bri\"";
    ssSendData <<":";
    ssSendData <<"\"254\"";
    comma = 1;
    }

    if(!trans_.empty()){
    ssSendData <<"\"transitiontime\"";
    ssSendData <<":";
    ssSendData <<"\"";
    ssSendData << trans_;
    ssSendData <<"\"";
    ssSendData <<",";
    comma = 1;
    }
 
    if(!effect_.empty()){
    ssSendData <<",";
    ssSendData <<"\"effect\"";
    ssSendData <<":";
    ssSendData <<"\"";
    ssSendData << effect_;
    ssSendData <<"\"";
    comma = 1;
    }
    ssSendData <<"}";
    ssSendData <<"}";
    ssSendData <<",";

    if(!time.empty()){
    ssSendData <<"\"time\"";
    ssSendData <<":";
    ssSendData <<"\"";
    ssSendData << time;
    ssSendData <<"\"";
    comma = 1;
    }
    ssSendData <<"}";

	message.addBodyText(ssSendData.str());
        comma = 1;
	if(client.post(url, message) && comma == 1)
        {	
	    schedulegetResult->setText("<b>Schedule has been changed</b>");        
       }else{
	    schedulegetResult->setText("<b>Please enter valid, non-empty values for at least one attribute</b>");	
	}
}

/*!
 *  \brief     'get schedule' class.
 *  \details   This class gets the specified schedule
 *  \author    Kyungjoo Lee
 *  \date      2017, November 28
 */
void ScheduleContainer::getSchedule(string *pointBridgeIP, string *pointBridgePort, string *pointBridgeUser) {
    auto *client = new Http::Client(this);
    client->setTimeout(15);
    client->setMaximumResponseSize(10 * 1024);

    string http = "http://";
    string ipAddress = *pointBridgeIP+":";
    string port = *pointBridgePort+"/";
    string api = "api/";
    string user = *pointBridgeUser+"/";
    string schedules = "schedules/";
    string schedule_num = scheduleNum1->text().toUTF8();
    string url = http+ipAddress+port+api+user+schedules+schedule_num; 

    client->done().connect(boost::bind(&ScheduleContainer::handle,this, _1,_2));
    if(client->get(url))
    {
        WApplication::instance()->deferRendering();
	WApplication::instance()->deferRendering();
    }
}

/*!
 *  \brief     'set schedule' class.
 *  \details   This class sets the specified schedule
 *  \author    Kyungjoo Lee, Nicholas Cheng
 *  \date      2017, November 28
 */
void ScheduleContainer::setSchedule(string *pointBridgeIP, string *pointBridgePort, string *pointBridgeUser) {
    Http::Client client;//!< = new Http::Client(this);
    Http::Message message = Http::Message();

    string http = "http://";
    string ipAddress = *pointBridgeIP+":";
    string port = *pointBridgePort+"/";
    string api = "api/";
    string user = *pointBridgeUser+"/";
    string schedules = "schedules/";

    string id = scheduleNum->text().toUTF8();
    string schedule_name = scheduleName1->text().toUTF8();
    string sche_des = description1->text().toUTF8();
    string time = localtime1->text().toUTF8();
    string light_on_off = on2->text().toUTF8();
    string bri_ = bri2->text().toUTF8();
    string trans_ = transition2->text().toUTF8();
    string effect_ = effect2->text().toUTF8();
    string groups = "\"/groups/";
    string action = "/action\"";
    string group_num = groupNum2->text().toUTF8();
    string lights = "\"/lights/";
    string state = "/state\"";
    string light_num = lightNum2->text().toUTF8();

    string url = http+ipAddress+port+api+user+schedules+id;
    message.addHeader("Content-type","application/json");
    int comma = 0;
    stringstream ssSendData;
   
   // ssSendData << "{\"name\": \"WHat99999aisod\"}";

    ssSendData << "{";
    if(!schedule_name.empty()){
    ssSendData <<"\"name\"";
    ssSendData <<":";
    ssSendData <<"\"";
    ssSendData << schedule_name;
    ssSendData <<"\"";
    ssSendData <<",";
    comma = 1;
    }


    if(!sche_des.empty()){
    ssSendData <<"\"description\"";
    ssSendData <<":";
    ssSendData <<"\"";
    ssSendData <<sche_des;
    ssSendData <<"\"";
    ssSendData <<",";
    comma = 1;
    }

    ssSendData <<"\"command\"";
    ssSendData <<":";
    ssSendData <<"{";

   if(!group_num.empty()){
      ssSendData <<"\"address\"";
      ssSendData <<":";
      ssSendData << groups+group_num+action; 
    ssSendData <<",";
      comma =1;
   }else if(!light_num.empty()){
    ssSendData <<"\"address\"";
    ssSendData <<":";
     ssSendData << lights+light_num+state; 
    ssSendData <<",";
     comma = 1;
   }else if(!light_num.empty() && !group_num.empty()){
     ssSendData <<"\"address\"";
     ssSendData <<":";
     ssSendData << "";
    ssSendData <<",";
     comma = 1;
   }

    ssSendData <<"\"method\"";
    ssSendData <<":";
    ssSendData <<"\"PUT\"";
    ssSendData <<",";
    ssSendData <<"\"body\"";
    ssSendData <<":";
    ssSendData <<"{";

    if (light_on_off == "1"){
        ssSendData <<"\"on\"";
        ssSendData <<":";           
	ssSendData << "\"true\"";
        comma = 1;
        ssSendData <<",";
  }else if(light_on_off == "0"){
        ssSendData <<"\"on\"";
        ssSendData <<":";        
	ssSendData << "\"false\"";
        comma = 1;
        ssSendData <<",";
	}


    if (!bri_.empty()){
    ssSendData <<"\"bri\"";
    ssSendData <<":";
    ssSendData <<"\"";
    ssSendData << bri_;
    ssSendData <<"\"";
    comma = 1;
    ssSendData <<",";
    }

    if(!trans_.empty()){
    ssSendData <<"\"transitiontime\"";
    ssSendData <<":";
    ssSendData <<"\"";
    ssSendData << trans_;
    ssSendData <<"\"";
    ssSendData <<",";    
    comma = 1;
    }

    if(!effect_.empty()){
    ssSendData <<",";
    ssSendData <<"\"effect\"";
    ssSendData <<":";
    ssSendData <<"\"";
    ssSendData << effect_;
    ssSendData <<"\"";
    comma = 1;
    }

    ssSendData <<"}";
    ssSendData <<"}";

    if(!time.empty()){
    ssSendData <<",";       
    ssSendData <<"\"time\"";
    ssSendData <<":";
    ssSendData <<"\"";
    ssSendData << time;
    ssSendData <<"\"";
    comma = 1;
    }
    ssSendData <<"}";

	message.addBodyText(ssSendData.str());
        comma = 1;
	if(client.put(url, message) && comma == 1)
        {	
	    schedulegetResult->setText("<b>Schedule has been changed</b>");
        }else{
	    schedulegetResult->setText("<b>Please enter valid, non-empty values for at least one attribute</b>");	
	}
}

/*!
 *  \brief     'delete schedule' class.
 *  \details   This class deletes the specified schedule
 *  \author    Kyungjoo Lee
 *  \date      2017, November 28
 */
void ScheduleContainer::deleteSchedule(string *pointBridgeIP, string *pointBridgePort, string *pointBridgeUser) {
    Http::Client client;//!< = new Http::Client(this);
    Http::Message message = Http::Message();
    
    string schedule_num = scheduleNum2->text().toUTF8();
    string http = "http://";
    string ipAddress = *pointBridgeIP+":";
    string port = *pointBridgePort+"/";
    string api = "api/";
    string user = *pointBridgeUser+"/";
    string schedules = "schedules/";

    string url = http+ipAddress+port+api+user+schedules+schedule_num;
    message.addHeader("Content-type","application/json");

    if (!schedule_num.empty()){
        stringstream ssSendData;
	ssSendData <<"{}";
	message.addBodyText(ssSendData.str());
        if(client.deleteRequest(url, message))
        {
            schedulegetResult->setText("<b>Schedule has been deleted</b>");
        }else{
	    schedulegetResult->setText("<b>Please enter valid, non-empty values for at least one attribute</b>");	
	}
    }else{
        schedulegetResult->setText("<b>An Error Occured : Schedule has not been deleted</b>");
    }
}


/*! 
 *  \brief     'handle connection' class.
 *  \details   This class handles responses from the server (generally getLight() and getGroup()).
 *  \author    Gary Tohill      gtohill@uwo.ca
 *  \author    Nicholas Cheng       ncheng29@uwo.ca
 *  \author    Vaughan Robertson    vrobert6@uwo.ca
 *  \version   Stage 3
 *  \date      2017, November 28
 */
void ScheduleContainer::handle(boost::system::error_code err, const Http::Message& response)
{
    //!the parsing is a bit weird so apologies if anything goes wrong 
    WApplication::instance()->resumeRendering();
    if (!err && response.status() == 200) {
        try
        {
            schedulegetResult->setText(response.body());
        }
        catch(std::exception e)
        { 
            schedulegetResult->setText("ERROR (schedule does not exist at specified address)");
        }
    }else{
         schedulegetResult->setText(response.body());
    }
}

