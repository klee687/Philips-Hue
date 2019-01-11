#include <iostream>
#include <string>

#include <Wt/WApplication>
#include <Wt/WServer>
#include <Wt/Http/Response>
#include <Wt/WApplication>
#include <Wt/Http/Client>
#include <Wt/Http/Message>
#include <Wt/WPushButton>

#include "HueEmulatorApplication.h"

using namespace std;
using namespace Wt;

/*! 
 *  \brief     'create application' class.
 *  \details   This class creates the application.
 *  \author    Gary Tohill		gtohill@uwo.ca
 *  \version   Stage 2
 *  \date      2017, November 10
 */
WApplication *createApplication(const WEnvironment &env) {

    /*!
     * You could read information from the environment to decide whether
     * the user has permission to start a new application
     */
    return new HueEmulatorApplication(env);
}

/*! 
 *  \brief     'main' class.
 *  \details   This class runs all the other classes.
 *  \author    Gary Tohill		gtohill@uwo.ca
 *  \version   Stage 2
 *  \date      2017, November 10
 */
int main(int argc, char **argv) {

    try {
        Wt::WServer server(argc, argv, WTHTTP_CONFIGURATION);
        server.addEntryPoint(Wt::Application, &createApplication, "/");
        server.run();
    } catch (Wt::WServer::Exception &e) {
        std::cerr << e.what() << std::endl;
    } catch (std::exception &e) {
        std::cerr << "exception: " << e.what() << std::endl;
    }

}
