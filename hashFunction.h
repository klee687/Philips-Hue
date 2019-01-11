#ifndef hashFunction_h
#define hashFunction_h

#include <iostream>
#include <vector>
#include <string>
#include <bitset>

/**
 * class: hashFunction
 * Takes in a string and converts it into a hashed value for safe storage
 */
class hashFunction{

	public:
		hashFunction(std::string preHash);
    	std::string getHash();

  	private:
  		std::string addString = ""; /**< string Original string converted into binary*/
  		int length = 0; /**< int Length of the prehash string*/
  		std::vector<std::string> tempValues; /**< string value preHash */
  		std::vector<std::string> finalValues; /**< vector<string> final values generated from the hash */

  		std::string hashedString = ""; /**< string containing the hashed string */

};

#endif