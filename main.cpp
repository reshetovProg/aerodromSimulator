
#include "param.h"

//#define test

struct Aeroplane {
    string id;
    string type;
};

struct Aerodrom {
    int angarCount;
    int placeCount;

    Aeroplane** angar = nullptr;
    Aeroplane* place = nullptr;
};


int main()
{
    srand(time(NULL));
    generateAeroplaneFile(PATH_AEROPLANE_FILE, COUNT_AEROPLANE);
    generateAerodromFile(PATH_AERODROM_FILE, COUNT_AERODROM);

    //вывод файлов
#ifdef test
    string* arrayAeroplane = readFile(PATH_AEROPLANE_FILE);
    string* arrayAerodrom = readFile(PATH_AERODROM_FILE);

    cout << "aeroplanes" << endl;
    for (int i = 0; i < COUNT_AEROPLANE; i++) {
        cout << arrayAeroplane[i] << endl;
    }

    cout << "aerodroms" << endl;
    for (int i = 0; i < COUNT_AERODROM; i++) {
        cout << arrayAerodrom[i] << endl;
    }

#endif
    
}
