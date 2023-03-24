#include "param.h"

string* readFile(string path) {
	ifstream in(path);
	string str;
	int count = 0;
	if (in.is_open()) {
		while (getline(in, str)) {
			count++;
		}
	}
	in.close();
	in.open(path);
	string* buf = new string[count];
	if (in.is_open()) {
		count = 0;
		while (getline(in, str)) {
			buf[count++] = str;
		}
	}
	in.close();
	return buf;
}

