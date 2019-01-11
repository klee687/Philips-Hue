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

#include "groupContainer.h"

using namespace Wt;
using namespace std;

/*! 
 *  \brief     'group container' class.
 *  \details   This class generates most of the html for the group subsection, and calls almost every other method.
 *  \author    Gary Tohill		gtohill@uwo.ca
 *  \author    Nicholas Cheng		ncheng29@uwo.ca
 *  \version   Stage 3
 *  \date      2017, November 28
 */
GroupContainer::GroupContainer(string *pointBridgeIP, string *pointBridgePort, string *pointBridgeUser){

    this->addWidget(new WText("Enter Group #: "));  //< show some text
    groupNum = new WLineEdit(this);                 //< allow text input
    groupNum->setValidator(new Wt::WIntValidator(0, 200));
    this->addWidget(new WBreak);

    //!turn light off or on
    this->addWidget(new WText("1 = On | 0 = Off: "));
    on3 = new WLineEdit(this);
    on3->setValidator(new Wt::WIntValidator(0,1));
    this->addWidget(new WBreak);
    //!change saturation
    this->addWidget(new WBreak);
    this->addWidget(new WText("Saturation [0-254]: "));
    sat3 = new WLineEdit(this);
    sat3->setValidator(new Wt::WIntValidator(0,254));
    //!change hue
    this->addWidget(new WBreak);
    this->addWidget(new WText("Hue [0-10000]: "));
    hue3 = new WLineEdit(this);
    hue3->setValidator(new Wt::WIntValidator(0,10000));
    //!change bri
    this->addWidget(new WBreak);
    this->addWidget(new WText("Brightness [0-254]: "));
    bri3 = new WLineEdit(this);
    bri3->setValidator(new Wt::WIntValidator(0,254));
    //!change transition
    this->addWidget(new WBreak);
    this->addWidget(new WText("Transition Time (integer, eg: 10 = 1 second): "));
    transition3 = new WLineEdit(this);
    transition3->setValidator(new Wt::WIntValidator());
    //!change effect
    this->addWidget(new WBreak);
    this->addWidget(new WText("Effect ('none' or 'colorloop'): "));
    effect3 = new WLineEdit(this);
    effect3->setValidator(new Wt::WRegExpValidator("(none|colorloop)"));
    this->addWidget(new WBreak);

    //!submit to change light
    WPushButton *button4= new WPushButton("Set Group State", this);    //!< create a button
    button4->setMargin(5, Left);                                 //!< add 5 pixels margin

    /*!
    * Connect signals with slots- simple Wt-way
    */    
    button4->clicked().connect(std::bind([=] () {

        if (groupNum->validate() && on3->validate() && hue3->validate() && bri3->validate() && transition3->validate() && effect3->validate()){
            this->setGroup(pointBridgeIP, pointBridgePort, pointBridgeUser);
        }
        else{
            groupgetResult->setText("<b>An Error Occured : One of the values you entered in invalid</b>");
        }
    }));


    //!get status of group
    WPushButton *button2 = new WPushButton("Get Group", this);    //!<create a button
    button2->setMargin(5, Left);

    /*!
    * Connect signals with slots- simple Wt-way
    */
    button2->clicked().connect(std::bind([=] () {
        if (groupNum->validate()){
            this->getGroup(pointBridgeIP, pointBridgePort, pointBridgeUser);
        }
        else{
            groupgetResult->setText("<b>An Error Occured : One of the values you entered in invalid</b>");
        }
    }));

    /*!
     * - using an arbitrary function object (binding values with boost::bind())
     */

    //!submit to delete group
    WPushButton *button5 = new WPushButton("Delete Group", this);    //!< create a button
    button5->setMargin(5, Left);                                 //!< add 5 pixels margin

    /*!
    * Connect signals with slots- simple Wt-way
    */    
    button5->clicked().connect(std::bind([=] () {

        if (groupNum->validate()){
            this->deleteGroup(pointBridgeIP, pointBridgePort, pointBridgeUser);
        }
        else{
            groupgetResult->setText("<b>An Error Occured : One of the values you entered in invalid</b>");
        }
    }));

    //!SETTING A GROUP
    this->addWidget(new WBreak);
    //!group name
    this->addWidget(new WText("Group Name: "));
    groupName = new WLineEdit(this);
    groupName->setValidator(new Wt::WRegExpValidator("[^\"]*"));
    this->addWidget(new WBreak);

    //!group lights
    this->addWidget(new WText("Group Light Numbers: "));
    groupLightNums = new WLineEdit(this);
    groupLightNums->setValidator(new Wt::WRegExpValidator("^[0-9]{1,5}( *, *[0-9]{1,5})*$"));

    //!edit group attributes
    this->addWidget(new WBreak);
    WPushButton *button6 = new WPushButton("Set Group Attributes", this);    //!< create a button
    button6->setMargin(5, Left);                                 //!< add 5 pixels margin

    /*!
    * Connect signals with slots- simple Wt-way
    */    
    button6->clicked().connect(std::bind([=] () {
	if (groupNum->validate() && groupName->validate() && groupLightNums->validate()){
            this->setGroupAttributes(pointBridgeIP, pointBridgePort, pointBridgeUser);
        }
        else{
            groupgetResult->setText("<b>An Error Occured : One of the values you entered in invalid</b>");
        }
    }));


    //!create group
    WPushButton *button3 = new WPushButton("Create Group", this);    //!< create a button
    button3->setMargin(5, Left);                                 //!< add 5 pixels margin
    this->addWidget(new WBreak);
    /*!
    * Connect signals with slots- simple Wt-way
    */    
    button3->clicked().connect(std::bind([=] () {
	if (groupName->validate() && groupLightNums->validate()){
            this->createGroup(pointBridgeIP, pointBridgePort, pointBridgeUser);
        }
        else{
            groupgetResult->setText("<b>An Error Occured : One of the values you entered in invalid</b>");
        }
    }));
    this->addWidget(new WBreak);
    groupgetResult = new WText(this);          //!<empty text
    groupgetResult->setFocus();
    this->addWidget(new WBreak);
}

/*! 
 *  \brief     'create group' class.
 *  \details   This class creates a group
 *  \author    Nicholas Cheng		ncheng29@uwo.ca
 *  \version   Stage 3
 *  \date      2017, November 28
 */
void GroupContainer::createGroup(string *pointBridgeIP, string *pointBridgePort, string *pointBridgeUser) {
    string group_name = groupName->text().toUTF8();
    string group_light_nums = groupLightNums->text().toUTF8();

    Http::Client client;//!< = new Http::Client(this);
    Http::Message message = Http::Message();

    string http = "http://";
    string ipAddress = *pointBridgeIP+":";
    string port = *pointBridgePort+"/";
    string api = "api/";
    string user = *pointBridgeUser+"/";
    string groups = "groups/";

    string url = http+ipAddress+port+api+user+groups;
    message.addHeader("Content-type","application/json");

    //!check contents of params
  
    int comma = 0; //! if it contains at least 1 attribute, also indicates usage of comma
    stringstream ssSendData;
    ssSendData <<"{";
    //!change attributes in this section
    if(!group_name.empty()){
        ssSendData << "name: ";
        ssSendData << group_name;
        comma = 1;
    }
    if(!group_light_nums.empty()){
	if (comma == 1){
    	    ssSendData << ", ";
    }
        ssSendData << "lights: [";
        ssSendData << group_light_nums;
        ssSendData << "]";
        comma = 1;
    }
    //!change attributes in this section
    ssSendData <<"}";
    message.addBodyText(ssSendData.str());
    if(client.post(url, message) && comma == 1)
    {
        groupgetResult->setText("<b>Group has been created.</b>");
    }else{
        groupgetResult->setText("<b>Group has not been created.</b>");	
    }

}

/*! 
 *  \brief     'get group' class.
 *  \details   This class gets the specifed group from the server.
 *  \author    Gary Tohill		gtohill@uwo.ca
 *  \author    Nicholas Cheng		ncheng29@uwo.ca
 *  \version   Stage 3
 *  \date      2017, November 28
 */
void GroupContainer::getGroup(string *pointBridgeIP, string *pointBridgePort, string *pointBridgeUser) {
    string group_num = groupNum->text().toUTF8();
    auto *client = new Http::Client(this);
    client->setTimeout(15);
    client->setMaximumResponseSize(10 * 1024);

    string http = "http://";
    string ipAddress = *pointBridgeIP+":";
    string port = *pointBridgePort+"/";
    string api = "api/";
    string user = *pointBridgeUser+"/";
    string groups = "groups/";

    string url = http+ipAddress+port+api+user+groups+group_num; //<!gets a groups

    //!I think this always causes an error?
    if (!group_num.empty()){
        client->done().connect(boost::bind(&GroupContainer::handle,this, _1,_2));
        if(client->get(url))
        {
            WApplication::instance()->deferRendering();
	    WApplication::instance()->deferRendering();
        }else{
	    groupgetResult->setText("<b>An Error Occured : Unable to get group.</b>");
	}
    }else{
        groupgetResult->setText("<b>An Error Occured : No group number specified.</b>");
    }
}

/*! 
 *  \brief     'set group' class.
 *  \details   This class sets attributes of a group state. Groups are slightly different than lights:
 *  \details   their state does not get modified if they are off. a call like
 *  \details   {on: true, bri: 100} will turn all off-lights ON, but only change the brightness
 *  \details   of existing lights. This makes groups a little weird....
 *  \author    Nicholas Cheng		ncheng29@uwo.ca
 *  \version   Stage 3
 *  \date      2017, November 28
 */
void GroupContainer::setGroup(string *pointBridgeIP, string *pointBridgePort, string *pointBridgeUser) {
    string group_num = groupNum->text().toUTF8();
    string light_on_off = on3->text().toUTF8();
    string hue1 = hue3->text().toUTF8();
    string sat1 = sat3->text().toUTF8();
    string bri1 = bri3->text().toUTF8();
    string transition1 = transition3->text().toUTF8();
    string effect1 = effect3->text().toUTF8();

    Http::Client client;//!< = new Http::Client(this);
    Http::Message message = Http::Message();

    string http = "http://";
    string ipAddress = *pointBridgeIP+":";
    string port = *pointBridgePort+"/";
    string api = "api/";
    string user = *pointBridgeUser+"/";
    string groups = "groups/";
    string action = "/action";

    string url = http+ipAddress+port+api+user+groups+group_num+action;
    message.addHeader("Content-type","application/json");

    //!check contents of params
    if (!group_num.empty()){
	int comma = 0; //!1 if it contains at least 1 attribute, also indicates usage of comma
	stringstream ssSendData;
	ssSendData <<"{";
	//!change attributes in this section
	if(!light_on_off.empty()){
	    ssSendData << "on:";
	    if (light_on_off == "1"){
	        ssSendData << "true";
	    }else{
	        ssSendData << "false";
	    }
	    comma = 1;
	}
	if(!hue1.empty()){
	    if (comma == 1){
		ssSendData << ", ";
	    }
	    ssSendData << "hue:";
	    ssSendData << hue1;
	    comma = 1;
	}
	if(!sat1.empty()){
	    if (comma == 1){
		ssSendData << ", ";
	    }
	    ssSendData << "sat:";
	    ssSendData << sat1;
	    comma = 1;
	}
	if(!bri1.empty()){
	    if (comma == 1){
		ssSendData << ", ";
	    }
	    ssSendData << "bri:";
	    ssSendData << bri1;
	    comma = 1;
	}
	if(!transition1.empty()){
	    if (comma == 1){
		ssSendData << ", ";
	    }
	    ssSendData << "transitiontime:";
	    ssSendData << transition1;
	    comma = 1;
	}
	if(!effect1.empty() && (effect1 == "none" || effect1 == "colorloop")){
	    if (comma == 1){
		ssSendData << ", ";
	    }
	    ssSendData << "effect:";
	    ssSendData << effect1;
	    comma = 1;
	}
	//!end attributes section
	ssSendData <<"}";
	message.addBodyText(ssSendData.str());
        if(client.put(url, message) && comma == 1)
        {
            groupgetResult->setText("<b>Group state has been changed</b>");
        }else{
	    groupgetResult->setText("<b>Please enter valid, non-empty values for at least one attribute</b>");	
	}
    }else{
        groupgetResult->setText("<b>An Error Occured : Group has not been changed</b>");
    }
}

/*! 
 *  \brief     'set group attributes' class.
 *  \details   This class sets attributes of a group. Groups are slightly different than lights:
 *  \details   their state does not get modified if they are off. a call like
 *  \details   {on: true, bri: 100} will turn all off-lights ON, but only change the brightness
 *  \details   of existing lights. This makes groups a little weird....
 *  \author    Nicholas Cheng		ncheng29@uwo.ca
 *  \version   Stage 3
 *  \date      2017, November 28
 */
void GroupContainer::setGroupAttributes(string *pointBridgeIP, string *pointBridgePort, string *pointBridgeUser) {
    string group_num = groupNum->text().toUTF8();
    string name1 = groupName->text().toUTF8();
    string lights1 = groupLightNums->text().toUTF8();


    Http::Client client;//!< = new Http::Client(this);
    Http::Message message = Http::Message();

    string http = "http://";
    string ipAddress = *pointBridgeIP+":";
    string port = *pointBridgePort+"/";
    string api = "api/";
    string user = *pointBridgeUser+"/";
    string groups = "groups/";

    string url = http+ipAddress+port+api+user+groups+group_num;
    message.addHeader("Content-type","application/json");

    //!check contents of params
    if (!group_num.empty()){
	int comma = 0; //!1 if it contains at least 1 attribute, also indicates usage of comma
	stringstream ssSendData;
	ssSendData <<"{";
	if (!name1.empty()){
		ssSendData <<"name:";
		ssSendData << name1;
		comma = 1;
	}
        if(!lights1.empty()){
	    if (comma == 1){
    	        ssSendData << ", ";
            }
            ssSendData << "lights: [";
            ssSendData << lights1;
            ssSendData << "]";
            comma = 1;
        }
	ssSendData <<"}";
	message.addBodyText(ssSendData.str());
        if(client.put(url, message) && comma == 1)
        {
            groupgetResult->setText("<b>Group Attributes have been changed</b>");
        }else{
	    groupgetResult->setText("<b>Please enter valid, non-empty values for at least one attribute</b>");	
	}
    }else{
        groupgetResult->setText("<b>An Error Occured : Group has not been changed</b>");
    }
}

/*! 
 *  \brief     'delete group' class.
 *  \details   This class deletes the specified group
 *  \author    Nicholas Cheng		ncheng29@uwo.ca
 *  \version   Stage 3
 *  \date      2017, November 28
 */
void GroupContainer::deleteGroup(string *pointBridgeIP, string *pointBridgePort, string *pointBridgeUser) {
    string group_num = groupNum->text().toUTF8();

    Http::Client client;//!< = new Http::Client(this);
    Http::Message message = Http::Message();

    string http = "http://";
    string ipAddress = *pointBridgeIP+":";
    string port = *pointBridgePort+"/";
    string api = "api/";
    string user = *pointBridgeUser+"/";
    string groups = "groups/";

    string url = http+ipAddress+port+api+user+groups+group_num;
    message.addHeader("Content-type","application/json");

    if (!group_num.empty()){
	stringstream ssSendData;
	ssSendData <<"{}"; //!send nothing
	message.addBodyText(ssSendData.str());
        if(client.deleteRequest(url, message))
        {
            groupgetResult->setText("<b>Group has been deleted</b>");
        }else{
	    groupgetResult->setText("<b>Please enter valid, non-empty values for at least one attribute</b>");	
	}
    }else{
        groupgetResult->setText("<b>An Error Occured : Group has not been deleted</b>");
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
void GroupContainer::handle(boost::system::error_code err, const Http::Message& response){
    //!the parsing is a bit weird so apologies if anything goes wrong 
    WApplication::instance()->resumeRendering();
    if (!err && response.status() == 200) {
        try
        {
            groupgetResult->setText(response.body());
        }
        catch(std::exception e)
        {
            groupgetResult->setText("ERROR (group does not exist at specified address)");
        }
    }else{
         groupgetResult->setText(response.body());
    }
}
