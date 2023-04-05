#include "param.h"

string createAeroplane(string type1, string type2, float k, int count) {
	//id;type
	string str = "";
	str += to_string(count);		
	str += ";";
	
	int random = 1 + rand() % 100;
	if (random <= 100 * k) str += type1;
	else str += type2;
	return str;
}
void generateAeroplaneFile(string path, int count) {
	ofstream out(path);
	for (int i = 0; i < count; i++) {
		out << createAeroplane("boeing","light",0.2, i+100) << endl;
	}
	out.close();
	cout << "generate " << path << " completed"<<endl;
}

string createAerodrom(int count) {
	//id;angarCount;placeCount
	string str = "";
	str += to_string(count);
	str += ";";
	str += to_string(1 + rand() % 4);
	str += ";";
	str += to_string(4 + rand() % 4);
	return str;
}

void generateAerodromFile(string path, int count) {
	ofstream out(path);
	for (int i = 0; i < count; i++) {
		out << createAerodrom(i) << endl;
	}
	out.close();
	cout << "generate " << path << " completed"<<endl;
}

