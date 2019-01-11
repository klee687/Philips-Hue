#include <iostream>
#include <vector>
#include <string>
#include <bitset>

#include "hashFunction.h"

using namespace std;


/**
 * hashFunction constructor
 * Takes in a string and converts it into a hashed value for safe storage
 * param preHash the original string
 */

hashFunction::hashFunction(string preHash){

	for (auto &&c: preHash) { //Loops through each character of the string and converts to ascii

		string Binary = bitset<8>((int)c).to_string();

		addString += Binary;
		length++;

	}

		string Binary = bitset<160>(addString).to_string();

		for (unsigned i = 0; i < Binary.length(); i += 4) { //Places the values into an array
		tempValues.push_back(Binary.substr(i, 8));
	}

	for(int x = 0; x < tempValues.size(); x=x+2){ //XORs the different array elements together

		bitset<8> tempBit1 = bitset<8>(tempValues[x]);
		bitset<8> tempBit2 = bitset<8>(tempValues[x+1]);

		finalValues.push_back((tempBit1 ^ tempBit2).to_string());
	}

	for(int x = 0; x < finalValues.size(); x++){ //Switches the values around in the array

		swap(finalValues[x], finalValues[((10*x/2)) % finalValues.size()]);
	}

	for(int x = 0; x < finalValues.size(); x++){ //Puts the values into a string

		hashedString += finalValues[x];
	}

		

		
	}

/**
 * hashFunction getHash
 * Returns the hash string obtained from the function
 */

string hashFunction::getHash(){

		return hashedString;
	}
