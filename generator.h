#pragma once

//создание строки самолет для файла
string createAeroplane(string type1, string type2, float k, int count);
//создание файла с самолетами
void generateAeroplaneFile(string, int);
//создание строки аэродром для файла
string createAerodrom(int count);
//создание файла с аэродромами
void generateAerodromFile(string path, int count);