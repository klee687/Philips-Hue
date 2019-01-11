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

#include "ChangeLights.h"

using namespace Wt;
using namespace std;

/*! 
 *  \brief     'change lights' class.
 *  \details   This class generates most of the html for the lights subsection, and calls almost every other method.
 *  \author    Gary Tohill		gtohill@uwo.ca
 *  \author    Nicholas Cheng		ncheng29@uwo.ca
 *  \version   Stage 3
 *  \date      2017, November 28
 */
ChangeLights::ChangeLights(string *pointBridgeIP, string *pointBridgePort, string *pointBridgeUser){

    this->addWidget(new WBreak);
    //!BEGIN LIGHTS
    //!enter light to change
    this->addWidget(new WBreak);
    this->addWidget(new WText("Enter Light #: "));  //< show some text
    lightNum = new WLineEdit(this);                 //< allow text input
    lightNum->setValidator(new Wt::WIntValidator(0, 200));
    this->addWidget(new WBreak);

    //!turn light off or on
    this->addWidget(new WText("1 = On | 0 = Off: "));
    on = new WLineEdit(this);
    on->setValidator(new Wt::WIntValidator(0,1));
    this->addWidget(new WBreak);
    //!change saturation
    this->addWidget(new WBreak);
    this->addWidget(new WText("Saturation [0-254]: "));
    sat = new WLineEdit(this);
    sat->setValidator(new Wt::WIntValidator(0,254));
    //!change hue
    this->addWidget(new WBreak);
    this->addWidget(new WText("Hue [0-10000]: "));
    hue = new WLineEdit(this);
    hue->setValidator(new Wt::WIntValidator(0,10000));
    //!change bri
    this->addWidget(new WBreak);
    this->addWidget(new WText("Brightness [0-254]: "));
    bri = new WLineEdit(this);
    bri->setValidator(new Wt::WIntValidator(0,254));
    //!change transition
    this->addWidget(new WBreak);
    this->addWidget(new WText("Transition Time (integer, eg: 10 = 1 second): "));
    transition = new WLineEdit(this);
    transition->setValidator(new Wt::WIntValidator());
    //!change effect
    this->addWidget(new WBreak);
    this->addWidget(new WText("Effect ('none' or 'colorloop'): "));
    effect = new WLineEdit(this);
    effect->setValidator(new Wt::WRegExpValidator("(none|colorloop)"));
    this->addWidget(new WBreak);
    //!get of light
    
    WPushButton *button0 = new WPushButton("Get All Lights", this);    //!<create a button
    button0->setMargin(5, Left);

    /*!
    * Connect signals with slots- simple Wt-way
    */
    button0->clicked().connect(std::bind([=] () {
        this->getAllLights(pointBridgeIP, pointBridgePort, pointBridgeUser);
    }));

    WPushButton *button1 = new WPushButton("Get Light", this);    //!<create a button
    button1->setMargin(5, Left);

    /*!
    * Connect signals with slots- simple Wt-way
    */
    button1->clicked().connect(std::bind([=] () {
        if (lightNum->validate()){
            this->getLight(pointBridgeIP, pointBridgePort, pointBridgeUser);
        }
        else{
            lightgetResult->setText("<b>An Error Occured : One of the values you entered in invalid</b>");
        }
    }));

    //!submit to change light
    button = new WPushButton("Set Light State", this);    //!< create a button
    button->setMargin(5, Left);                                 //!< add 5 pixels margin

    button->clicked().connect(std::bind([=] () {

        if (lightNum->validate() && on->validate() && 
            sat->validate() && hue->validate() && bri->validate() 
            && effect->validate() && transition->validate()){

            this->setLight(pointBridgeIP, pointBridgePort, pointBridgeUser);
        }

        else{

            lightgetResult->setText("<b>An Error Occured : One of the values you entered in invalid</b>");
        }
    }));

    this->addWidget(new WBreak);  
    this->addWidget(new WText("Enter Light Name: "));  //< show some text
    lightName = new WLineEdit(this);                 //< allow text input
    lightName->setValidator(new Wt::WRegExpValidator("[^\"]*"));

    WPushButton *button7 = new WPushButton("Set Light Name", this);    //!<create a button
    button7->setMargin(5, Left);
    /*!
    * Connect signals with slots- simple Wt-way
    */
    button7->clicked().connect(std::bind([=] () {
        if (lightNum->validate() && lightName->validate()){
            this->setLightName(pointBridgeIP, pointBridgePort, pointBridgeUser);
        }
        else{
            lightgetResult->setText("<b>An Error Occured : One of the values you entered in invalid</b>");
        }
    }));

    this->addWidget(new WBreak);

    //light result
    lightgetResult = new WText(this);          //!<empty text
    lightgetResult->setFocus();
    this->addWidget(new WBreak);

}

/*! 
 *  \brief     'get light' class.
 *  \details   This class gets a user specified light from the server.
 *  \author    Gary Tohill		gtohill@uwo.ca
 *  \author    Nicholas Cheng		ncheng29@uwo.ca
 *  \version   Stage 3
 *  \date      2017, November 28
 */
void ChangeLights::getLight(string *pointBridgeIP, string *pointBridgePort, string *pointBridgeUser) {
    string light_num = lightNum->text().toUTF8();
    auto *client = new Http::Client(this);
    client->setTimeout(15);
    client->setMaximumResponseSize(10 * 1024);

    string http = "http://";
    string ipAddress = *pointBridgeIP+":";
    string port = *pointBridgePort+"/";
    string api = "api/";
    string user = *pointBridgeUser+"/";
    string lights = "lights/";
    string lightNumber = lightNum->text().toUTF8();

    string url = http+ipAddress+port+api+user+lights+lightNumber;

    if(!light_num.empty()){
        client->done().connect(boost::bind(&ChangeLights::handle,this, _1,_2));
        if(client->get(url))
        {
       	    WApplication::instance()->deferRendering();
	    WApplication::instance()->deferRendering();
        }else{
		lightgetResult->setText("<b>An Error Occured : Unable to get light.</b>");
	}
    }else{
        lightgetResult->setText("<b>An Error Occured : Light number not specified.</b>");
    }
}

/*! 
 *  \brief     'get all lights' class.
 *  \details   This class returns all light.
 *  \author    Gary Tohill		gtohill@uwo.ca
 *  \author    Nicholas Cheng		ncheng29@uwo.ca
 *  \version   Stage 3
 *  \date      2017, November 28
 */
void ChangeLights::getAllLights(string *pointBridgeIP, string *pointBridgePort, string *pointBridgeUser) {
    auto *client = new Http::Client(this);
    client->setTimeout(15);
    client->setMaximumResponseSize(10 * 1024);

    string http = "http://";
    string ipAddress = *pointBridgeIP+":";
    string port = *pointBridgePort+"/";
    string api = "api/";
    string user = *pointBridgeUser+"/";
    string lights = "lights/";

    string url = http+ipAddress+port+api+user+lights;

    client->done().connect(boost::bind(&ChangeLights::handle,this, _1,_2));
    if(client->get(url))
    {
       	WApplication::instance()->deferRendering();
	WApplication::instance()->deferRendering();
    }else{
	lightgetResult->setText("<b>An Error Occured : Unable to get light.</b>");
    }

}

/*! 
 *  \brief     'set light' class.
 *  \details   This class sets a light based on user input.
 *  \author    Gary Tohill		gtohill@uwo.ca
 *  \author    Nicholas Cheng		ncheng29@uwo.ca
 *  \version   Stage 3
 *  \date      2017, November 28
 */
void ChangeLights::setLight(string *pointBridgeIP, string *pointBridgePort, string *pointBridgeUser) {
    string light_num = lightNum->text().toUTF8();
    string light_on_off = on->text().toUTF8();
    string hue1 = hue->text().toUTF8();
    string sat1 = sat->text().toUTF8();
    string bri1 = bri->text().toUTF8();
    string transition1 = transition->text().toUTF8();
    string effect1 = effect->text().toUTF8();

    Http::Client client;//!< = new Http::Client(this);
    Http::Message message = Http::Message();


    string http = "http://";
    string ipAddress = *pointBridgeIP+":";
    string port = *pointBridgePort+"/";
    string api = "api/";
    string user = *pointBridgeUser+"/";
    string lights = "lights/";
    string lightNumber = light_num;
    string state = "/state";

    string url = http+ipAddress+port+api+user+lights+lightNumber+state;
    message.addHeader("Content-type","application/json");

    //!check contents of params
    string flag = ""; //!<is user turn light on or off
    if (light_on_off == "1"){
        flag = "true";
    }else{
        flag = "false";
    }

    
    if (!light_num.empty()){
        int comma = 0; //!1 if it contains at least 1 attribute, also indicates usage of comma
        stringstream ssSendData;
        ssSendData <<"{";
        //!change attributes in this section
        if(!hue1.empty()){
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
        if(!light_on_off.empty() && (comma == 0)){ //!<only turn light on/off if the user doesn't enter any attributes
            ssSendData << "on:";
            ssSendData << flag;
            comma = 1;
        }
        ssSendData <<"}";
        message.addBodyText(ssSendData.str());
        if(client.put(url, message) && comma == 1)
        {
             lightgetResult->setText("<b>Light has been changed</b>");
        }else{
             lightgetResult->setText("<b>Please enter valid, non-empty values for at least one attribute</b>");  
        }
    }else{
         lightgetResult->setText("<b>An Error Occured : Light has not been changed</b>");
    }
}

/*! 
 *  \brief     'set light name' class.
 *  \details   This class sets the light name based on user input.
 *  \author    Gary Tohill      gtohill@uwo.ca
 *  \author    Nicholas Cheng       ncheng29@uwo.ca
 *  \version   Stage 3
 *  \date      2017, November 28
 */
void ChangeLights::setLightName(string *pointBridgeIP, string *pointBridgePort, string *pointBridgeUser) {
    string light_num = lightNum->text().toUTF8();
    string name1 = lightName->text().toUTF8();

    Http::Client client;//!< = new Http::Client(this);
    Http::Message message = Http::Message();

    string http = "http://";
    string ipAddress = *pointBridgeIP+":";
    string port = *pointBridgePort+"/";
    string api = "api/";
    string user = *pointBridgeUser+"/";
    string lights = "lights/";
    string lightNumber = light_num;

    string url = http+ipAddress+port+api+user+lights+lightNumber;
    message.addHeader("Content-type","application/json");

    if (!light_num.empty()){
	int comma = 0; //!1 if it contains at least 1 attribute, also indicates usage of comma
	stringstream ssSendData;
	ssSendData <<"{";
	if (!name1.empty()){
		ssSendData <<"name:";
		ssSendData << name1;
		comma = 1;
	}
	ssSendData <<"}";
	message.addBodyText(ssSendData.str());
        if(client.put(url, message) && comma == 1)
        {
            lightgetResult->setText("<b>Light has been renamed</b>");
        }else{
	    lightgetResult->setText("<b>Please enter a valid, non-empty value for the name</b>");	
	}
    }else{
        lightgetResult->setText("<b>An Error Occured : Light has not been changed</b>");
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
void ChangeLights::handle(boost::system::error_code err, const Http::Message& response){
    //!the parsing is a bit weird so apologies if anything goes wrong 
    WApplication::instance()->resumeRendering();
    if (!err && response.status() == 200) {
        try
        {
            lightgetResult->setText(response.body()); //!no checking, because it might return many lights
        }
        catch(std::exception e)
        { 
            lightgetResult->setText("ERROR (light does not exist at specified address)");
        }
    }else{
         lightgetResult->setText(response.body());
    }
}
