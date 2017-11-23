#include "stdafx.h"
/*****************************
CS 323 Project, Part 2
Written in C++

Ash, Tyler
Cuevas, Fernando
Nguyen, Timothy

Read a text file and trace using given parsing table.
Determine if the file is accepted
if not accepted, produce error messages explaining why
*******************************/

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stack>


using namespace std;

bool traceCFG(string word, string table[][31]);
void printStack(string readVal, stack<string> pStack);
int getRow(string popVal);
int getCol(string readVal);
string readFile(ifstream& inFile, string fileName);

//void readRawData();
vector<string> splitSpace(string text);
bool isReserve(string token);
bool parse(string readVal, stack<string>& pStack, int& i, bool& isAcc, string table[][31]);

int main() {
	// Predictive Parsing Table
	// invalid path if starts with 'e', gives error number
	// empty string "" represents lambda
	string table[21][31] = {
		"program B ; var C begin F end.", "e0", "e1", "e2", "e3", "e4", "e5", "e6", "e7", "e8", "e9", "e10", "e11", "e12", "e13", "e14", "e15", "e16", "e17", "e18", "e19", "e20", "e21", "e22", "e23", "e24", "e25", "e26", "e27", "e28", "e29",
		"e30", "e31", "e32", "e33", "e34", "e35", "e36", "e37", "e38", "e39", "e40", "e41", "e42", "e43", "e44", "e45", "e46", "P Q", "P Q", "P Q", "P Q", "e47", "e48", "e49", "e50", "e51", "e52", "e53", "e54", "e55", "e56",
		"e57", "e58", "e59", "e60", "e61", "e62", "e63", "e64", "e65", "e66", "e67", "e68", "e69", "e70", "e71", "e72", "e73", "D : E ;", "D : E ;", "D : E ;", "D : E ;", "e74", "e75", "e76", "e77", "e78", "e79", "e80", "e81", "e82", "e83",
		"e84", "e85", "e86", "e87", "e88", "e89", "e90", "e91", "e92", "e93", "e94", "e95", "e96", "e97", "e98", "e99", "e100", "B Z", "B Z", "B Z", "B Z", "e101", "e102", "e103", "e104", "e105", "e106", "e107", "e108", "e109", "e110",
		"e111", "e112", "e113", "e114", "integer", "e115", "e116", "e117", "e118", "e119", "e120", "e121", "e122", "e123", "e124", "e125", "e126", "e127", "e128", "e129", "e130", "e131", "e132", "e133", "e134", "e135", "e136", "e137", "e138", "e139", "e140",
		"e141", "e142", "e143", "", "e144", "G F", "e145", "e146", "e147", "e148", "e149", "e150", "e151", "e152", "e153", "e154", "e155", "G F", "G F", "G F", "G F", "e156", "e157", "e158", "e159", "e160", "e161", "e162", "e163", "e164", "e165",
		"e166", "e167", "e168", "e169", "e170", "H", "e171", "e172", "e173", "e174", "e175", "e176", "e177", "e178", "e179", "e180", "e181", "I", "I", "I", "I", "e182", "e183", "e184", "e185", "e186", "e187", "e188", "e189", "e190", "e191",
		"e192", "e193", "e194", "e195", "e196", "print ( B ) ;", "e197", "e198", "e199", "e200", "e201", "e202", "e203", "e204", "e205", "e206", "e207", "e208", "e209", "e210", "e211", "e212", "e213", "e214", "e215", "e216", "e217", "e218", "e219", "e220", "e221",
		"e222", "e223", "e224", "e225", "e226", "e227", "e228", "e229", "e230", "e231", "e232", "e233", "e234", "e235", "e236", "e237", "e238", "B = J ;", "B = J ;", "B = J ;", "B = J ;", "e239", "e240", "e241", "e242", "e243", "e244", "e245", "e246", "e247", "e248",
		"e249", "e250", "e251", "e252", "e253", "e254", "e255", "e256", "e257", "K R", "e258", "e259", "K R", "K R", "e260", "e261", "e262", "K R", "K R", "K R", "K R", "K R", "K R", "K R", "K R", "K R", "K R", "K R", "K R", "K R", "K R",
		"e263", "e264", "e265", "e266", "e267", "e268", "e269", "e270", "e271", "L S", "e272", "e273", "L S", "L S", "e274", "e275", "e276", "L S", "L S", "L S", "L S", "L S", "L S", "L S", "L S", "L S", "L S", "L S", "L S", "L S", "L S",
		"e277", "e278", "e279", "e280", "e281", "e282", "e283", "e284", "e285", "( J )", "e286", "e287", "M", "M", "e288", "e289", "e290", "B", "B", "B", "B", "M", "M", "M", "M", "M", "M", "M", "M", "M", "M",
		"e291", "e292", "e293", "e294", "e295", "e296", "e297", "e298", "e299", "e300", "e301", "e302", "N O T", "N O T", "e303", "e304", "N O T", "e305", "e306", "e307", "e308", "N O T", "N O T", "N O T", "N O T", "N O T", "N O T", "N O T", "N O T", "N O T", "N O T",
		"e319", "e320", "e321", "e322", "e323", "e324", "e325", "e326", "e327", "e328", "e329", "e330", "+", "-", "e331", "e332", "e333", "e334", "e335", "e336", "e337", "", "", "", "", "", "", "", "", "", "",
		"e338", "e339", "e340", "e341", "e342", "e343", "e344", "e345", "e346", "e347", "e348", "e349", "e350", "e351", "e352", "e353", "e354", "e355", "e356", "e357", "e358", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
		"e359", "e360", "e361", "e362", "e363", "e364", "e365", "e366", "e367", "e368", "e369", "e370", "e371", "e372", "e373", "e374", "e375", "p", "q", "r", "s", "e376", "e377", "e378", "e379", "e380", "e381", "e382", "e383", "e384", "e385",
		"e386", "e387", "e388", "e389", "e390", "e391", "", "", "", "e392", "", "", "", "", "", "", "e393", "P Q", "P Q", "P Q", "P Q", "O Q", "O Q", "O Q", "O Q", "O Q", "O Q", "O Q", "O Q", "O Q", "O Q",
		"e394", "e395", "e396", "e397", "e398", "e399", "", "e400", "e401", "e402", "", "e403", "+ K R", "- K R", "e404", "e405", "e406", "e407", "e408", "e409", "e410", "e411", "e412", "e413", "e414", "e415", "e416", "e417", "e418", "e419", "e420",
		"e421", "e422", "e423", "e424", "e425", "e426", "e427", "e428", "e429", "e430", "", "e431", "", "", "* L S", "/ L S", "e432", "e433", "e434", "e435", "e436", "e437", "e438", "e439", "e440", "e441", "e442", "e443", "e444", "e445", "e446",
		"e447", "e448", "e449", "e450", "e451", "e452", "", "e454", "e455", "e456", "e457", "e458", "", "", "", "", "e463", "e464", "e465", "e466", "e467", "0 T", "1 T", "2 T", "3 T", "4 T", "5 T", "6 T", "7 T", "8 T", "9 T",
		"e468", "e469", "e470", "e471", "e472", "e473", "e474", "", ", B Z", "e475", "e476", "e477", "e478", "e479", "e480", "e481", "e482", "e483", "e484", "e485", "e486", "e487", "e488", "e489", "e490", "e491", "e492", "e493", "e494", "e495", "e496"
	};

	cout << "Trace word from given CFG" << "\n\n";

	// read from text file
	cout << "Enter file name: ";
	string fileName = "";
	getline(cin, fileName);
	ifstream inFile;
	string text = readFile(inFile, fileName.c_str());
	bool isAcc = traceCFG(text, table);
	if (isAcc) cout << "text is accepted" << endl;
	else cout << "text is not accepted" << endl;

	// user input
	//char continue_ = 'y';
	//while (tolower(continue_) == 'y') {
	//    string word = "";                       // get word from user
	//    cout << "Enter a postfix expression: ";
	//    getline(cin, word);

	//    bool isAcc = traceCFG(word, table);

	//    cout << "Continue(y/n)? ";         // ask user to continue
	//    cin >> continue_;
	//    cin.ignore();
	//    cout << endl;
	//}
}

// Table-Driven Predictive Parser Algorithm
// print stack after every match
bool traceCFG(string text, string table[][31]) {

	vector<string> tokens = splitSpace(text);

	stack<string> pStack;
	pStack.push("end.");        // starting conditions
	pStack.push("A");

	int i = 0;
	string readVal = tokens[i];
	cout << '\t' << "match:" << '\t' << "stack:" << endl;    // stack header

	bool isAcc = false;
	while (!pStack.empty()) {
		string readVal = "";
		bool traceDone = false;
		if (i >= tokens.size()) {
			cout << "input not complete?" << endl;
			return false;
		}
		// if current string is reserved word, parse entire string
		if (isReserve(tokens[i])) {
			readVal = tokens[i];
			traceDone = parse(readVal, pStack, i, isAcc, table);
		}
		// if current string is not reserved word, parse string one character at a time
		else {
			int j = 0;
			while (j <  tokens[i].size()) {
				readVal = "";
				readVal += tokens[i][j];
				traceDone = parse(readVal, pStack, j, isAcc, table);
			}
			++i;
		}
		if (traceDone == true) return isAcc;
	}
	return isAcc;    // if trace doesn't complete
}

// returns if trace is complete
bool parse(string readVal, stack<string>& pStack, int& i, bool& isAcc, string table[][31]) {

	string popVal = pStack.top();
	pStack.pop();

	// if no match, add table values to stack
	if (popVal != readVal) {
		int row = getRow(popVal);
		int col = getCol(readVal);
		if (row == -1 || col == -1) {
			cout << readVal << " is not part of language" << endl    // get entire string?
				<< "Range of tableVal is invalid. " << endl;
			isAcc = false;
			return true;
		}
		else {
			string tableVal = table[row][col];
			if (tableVal.length() > 0 && tableVal[0] == 'e') {
				cout << "Error: " << tableVal.substr(1, tableVal.back()) << endl
					<< "Pop value: " << popVal << endl
					<< "Read value: " << readVal << endl;
				isAcc = false;
				return true;
			}
			// push table values to stack in reverse order
			vector<string> tableVals = splitSpace(tableVal);
			for (int i = tableVals.size() - 1; i >= 0; --i)
				pStack.push(tableVals[i]);
		}
	}
	// if match, print stack and read next value
	else {
		printStack(readVal, pStack);
		if (popVal == "end.") {
			if (pStack.empty()) {
				cout << "File is accepted" << endl;
				isAcc = true;
				return true;
			}
			else {
				cout << "reached \"end.\" but stack is not empty" << endl;
				isAcc = false;
				return false;
			}
		}
		++i;
	}
}

void printStack(string readVal, stack<string> pStack) {
	cout << '\t' << readVal << '\t';
	vector<string> content;
	while (!pStack.empty()) {
		content.push_back(pStack.top());
		pStack.pop();
	}
	for (int i = content.size() - 1; i >= 0; i--)
		cout << content[i] << ' ';
	cout << endl;
}

// return true if token is reserved word
bool isReserve(string token) {
	string reserved[6] = { "program", "var", "begin", "end.", "integer", "print" };
	for (int i = 0; i < 6; ++i)
		if (token == reserved[i])
			return true;
	return false;
}

// seperate string every time there's whitespace, return vector of strings
vector<string> splitSpace(string text) {
	vector<string> tokens;
	string word = "";
	for (int i = 0; i < text.length(); ++i) {
		if (!isspace(text[i]))
			word += text[i];
		else {
			tokens.push_back(word);
			word = "";
		}
	}
	if (word.size() > 0)
		tokens.push_back(word);
	return tokens;
}

// return position of terminal in 2d array
int getRow(string popVal) {
	if (popVal == "A")      return 0;
	else if (popVal == "B") return 1;
	else if (popVal == "C") return 2;
	else if (popVal == "D") return 3;
	else if (popVal == "E") return 4;
	else if (popVal == "F") return 5;
	else if (popVal == "G") return 6;
	else if (popVal == "H") return 7;
	else if (popVal == "I") return 8;
	else if (popVal == "J") return 9;
	else if (popVal == "K") return 10;
	else if (popVal == "L") return 11;
	else if (popVal == "M") return 12;
	else if (popVal == "N") return 13;
	else if (popVal == "O") return 14;
	else if (popVal == "P") return 15;
	else if (popVal == "Q") return 16;
	else if (popVal == "R") return 17;
	else if (popVal == "S") return 18;
	else if (popVal == "T") return 19;
	else if (popVal == "Z") return 20;
	else                    return -1;
}

// return position of non-terminal in 2d array
int getCol(string readVal) {
	if (readVal == "program")   return 0;
	else if (readVal == "var")       return 1;
	else if (readVal == "begin")     return 2;
	else if (readVal == "end.")      return 3;
	else if (readVal == "integer")   return 4;
	else if (readVal == "print")     return 5;
	else if (readVal == ";")         return 6;
	else if (readVal == ":")         return 7;
	else if (readVal == ",")         return 8;
	else if (readVal == "(")         return 9;
	else if (readVal == ")")         return 10;
	else if (readVal == "=")         return 11;
	else if (readVal == "+")         return 12;
	else if (readVal == "-")         return 13;
	else if (readVal == "*")         return 14;
	else if (readVal == "/")         return 15;
	else if (readVal == "")         return 16;
	else if (readVal == "p")         return 17;
	else if (readVal == "q")         return 18;
	else if (readVal == "r")         return 19;
	else if (readVal == "s")         return 20;
	else if (readVal == "0")         return 21;
	else if (readVal == "1")         return 22;
	else if (readVal == "2")         return 23;
	else if (readVal == "3")         return 24;
	else if (readVal == "4")         return 25;
	else if (readVal == "5")         return 26;
	else if (readVal == "6")         return 27;
	else if (readVal == "7")         return 28;
	else if (readVal == "8")         return 29;
	else if (readVal == "9")         return 30;
	else                             return -1;
}

// save text from file as a single string
string readFile(ifstream& inFile, string fileName) {
	string text;
	inFile.open(fileName.c_str());
	if (!inFile)
		cout << "Unable to open \"" << fileName << "\"" << endl;
	else {
		char c;
		while (inFile.get(c))
			text += c;
	}
	return text;
}
