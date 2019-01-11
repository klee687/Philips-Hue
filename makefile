

all: 
	g++ -o main main.cpp HueEmulatorApplication.cpp EditBridge.cpp RemoveBridge.cpp hashFunction.cpp signin.cpp signup.cpp AddBridge.cpp ChangeLights.cpp groupContainer.cpp SceduleContainer.cpp -lwthttp -lwt -lboost_regex -lboost_signals -lboost_system -lboost_filesystem -lboost_program_options -lboost_date_time -std=c++14



