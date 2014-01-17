#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

void encrypt(char alphabet[], string input, vector<unsigned int> & key, const char* outFileName);
void decrypt(char alphabet[], string input, vector<unsigned int> & key, const char* outFileName);

int main(int argc, const char* argv[])
{
	fstream file1, file2;
	file1.open(argv[1]);
	file2.open(argv[2]);
	
	// file1 contains the message to be encrypted/decrypted
	// file2 contains the key
	// file3 is desired output destination
	// last argument is either d or e to indicate decryption/encryption action
	
	if ((file1 == NULL) || (file2 == NULL)) {
		cout << "Could not open one of the files!" << endl;
		return 0;
	}
	
	if (argc < 5) {
		cout << "Not enough arguments!\n";
		return 0;
	}
	
	if (argc > 5) {
		cout << "Too many arguments!\n";
		return 0;
	}
	
	int action = 0;
	char actionIn = *argv[4];
	
	if ((actionIn == 'e') || (actionIn == 'E')) {
		// action 1 is encrypt
		action = 1;
	}
	else if ((actionIn == 'd') || (actionIn == 'D')) {
		// action 2 is decrypt
		action = 2;
	}
	
	if (action == 0) {
		cout << "Action not valid!" << endl;
		return 0;
	}
	
	char alphabet[71];
	char imp = '0';
	for (int i = 0; i < 10; i++) {
		alphabet[i] = imp;
		imp++;
	}
	imp = 'a';
	for (int i = 10; i < 36; i++) {
		alphabet[i] = imp;
		imp++;
	}
	alphabet[36] = ' ';
	alphabet[37] = '*';
	alphabet[38] = '.';
	alphabet[39] = '?';
	alphabet[40] = '!';
	alphabet[41] = '"';
	alphabet[42] = ',';
	
	imp = 'A';
	for (int i = 43; i < 69; i++) {
		alphabet[i] = imp;
		imp++;
	}
	
	alphabet[69] = '\'';
	
	string input;
	vector<string> messageLines;
	
	while (file1.good()) {
		getline(file1, input);
		messageLines.push_back(input);
	}
	
	ofstream outFile;
	outFile.open(argv[3], ios::trunc);
	outFile.close();
	
	// the following gets the chars from the input file
	// ALL INPUT FOR KEY MUST BE ON ONE LINE!
	string keyIn;
	//getline(file1, input);
	getline(file2, keyIn);
	
	// the following builds the key
	vector<unsigned int> key;
	for (unsigned int i = 0; i < keyIn.size(); i++) {
		for (unsigned int j = 0; j < 71; j++) {
			if (keyIn[i] == alphabet[j]) {
				key.push_back(j);
				break;
			}
		}
	}
	
	// the following is the code for encrypt
	if (action == 1) {
		for (unsigned int i = 0; i < messageLines.size(); i++) {
			if (messageLines[i] != "") {
				encrypt(alphabet, messageLines[i], key, argv[3]);
			}
		}
		cout << "Message Encrypted!" << endl;
	}
	
	// the following is the code for decrypt
	if (action == 2) {
		for (unsigned int i = 0; i < messageLines.size(); i++) {
			if (messageLines[i] != "") {
				decrypt(alphabet, messageLines[i], key, argv[3]);
			}
		}
		cout << "Message Decrypted!" << endl;
	}
	
	// this is the end of the program
	return 0;
}

void encrypt(char alphabet[], string input, vector<unsigned int> & key, const char* outFileName) {
	vector<unsigned int> message;
	for (unsigned int i = 0; i < input.size(); i++) {
		for (unsigned int j = 0; j < 71; j++) {
			if (input[i] == alphabet[j]) {
				message.push_back(j);
				break;
			}
		}
	}
	
	int keyIndex = -1;
	unsigned int keySize = key.size();
	unsigned int shift;
	
	ofstream outFile;
	outFile.open(outFileName, ios::app);
		
	for (unsigned int i = 0; i < message.size(); i++) {
		keyIndex++;
		keyIndex = keyIndex % keySize;
		// decrypt uses - so encrypt must use +
		shift = key[keyIndex] + message[i];
		shift = shift % 71;
		outFile << alphabet[shift];
	}
	outFile << "\n";
	outFile.close();
}

void decrypt(char alphabet[], string input, vector<unsigned int> & key, const char* outFileName) {
	vector<unsigned int> message;
	for (unsigned int i = 0; i < input.size(); i++) {
		for (unsigned int j = 0; j < 71; j++) {
			if (input[i] == alphabet[j]) {
				message.push_back(j);
				break;
			}
		}
	}
	
	int keyIndex = -1;
	int keySize = key.size();
	int shift, difference;
	
	ofstream outFile;
	outFile.open(outFileName, ios::app);
		
	for (unsigned int i = 0; i < message.size(); i++) {
		keyIndex++;
		keyIndex = keyIndex % keySize;
		// encrypt uses + so decrypt must use -
		shift = 71;
		difference = (message[i] - key[keyIndex]);
		shift += difference;
		shift = shift % 71;
		outFile << alphabet[shift];
	}
	outFile << "\n";
	outFile.close();
}
