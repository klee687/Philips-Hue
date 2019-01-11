To Compile: 

(1)	Change directory to Project Location.
~$  cd /Path_to_file

(2)	Type ‘make’
~$ make

(3)	You will see this line while compiling. 
g++ -o main main.cpp HueEmulatorApplication.cpp EditBridge.cpp RemoveBridge.cpp hashFunction.cpp signin.cpp signup.cpp AddBridge.cpp ChangeLights.cpp groupContainer.cpp SceduleContainer.cpp -lwthttp -lwt -lboost_regex -lboost_signals -lboost_system -lboost_filesystem -lboost_program_options -lboost_date_time -std=c++14


When the program is compiled, you will be able to run it with the instruction below. 
To Run: 
(1)	~$  ./main --http-address 0.0.0.0 --http-port 8080 --docroot=.

(2)	Follow the address given from the terminal.
~$ : [info] "wthttp: started server: http://0.0.0.0:8080"

