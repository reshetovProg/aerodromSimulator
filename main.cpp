
#include "param.h"

//#define test

struct Aeroplane {
    string id;
    string type;
};

struct Aerodrom {
    int angarCount;
    int placeCount;

    Aeroplane*** angar = nullptr;
    Aeroplane** place = nullptr;

    int* placeBuffer = nullptr;

    //начальная инициализация массивов
    void startInit() {
        place = new Aeroplane*[placeCount]{nullptr};
        placeBuffer = new int[placeCount] {};

        angar = new Aeroplane ** [angarCount];
        for (int i = 0; i < angarCount; i++) {
            angar[i] = new Aeroplane*[2]{ nullptr };
        }
    }

    //обновление очередности заняти парковочных мест после добавления самолета
    void addToBuffer(int index) {
        for (int i = 0; i < placeCount; i++) {
            if (placeBuffer[i] != 0) placeBuffer[i]++;
        }
        placeBuffer[index] = 1;
    }

    //обновление очередности заняти парковочных мест после удаления самолета
    void removeFromBuffer(int index) {  
        int n = placeBuffer[index];
        for (int i = 0; i < placeCount; i++) {
            if (placeBuffer[i] > n) placeBuffer[i]--;
        }
        placeBuffer[index] = 0;
    }

    //добавить самолет в ангар
    bool addToAngar(Aeroplane*& aeroplane) {
        if (aeroplane->type == "light") {
            for (int i = 0; i < angarCount; i++) {
                if (angar[i][0] == nullptr) {
                    angar[i][0] = aeroplane;
                    return true;
                }
                else if (angar[i][0]->type == "light" && angar[i][1] == nullptr) {
                    angar[i][1] = aeroplane;
                    return true;
                }
            }
        }
        else if (aeroplane->type == "boeing") {
            for (int i = 0; i < angarCount; i++) {
                if (angar[i][0] == nullptr && angar[i][1] == nullptr) {
                    angar[i][0] = aeroplane;
                    return true;
                }
            }
        }

        return false;
    }

    //добавить самолет на парковку
    bool addToPlace(Aeroplane*& aeroplane) {
        for (int i = 0; i < placeCount; i++) {
            if (place[i] == nullptr) {
                place[i] = aeroplane;
                addToBuffer(i);
                return true;
            }
        }
        return false;
    }

    //удалить самолет с парковки по id (возвращает удаляемый самолет)
    Aeroplane* removeFromPlace(string id) {
        
        Aeroplane* aeroplane = nullptr;       
        for (int i = 0; i < placeCount; i++) {
            if (place[i]->id == id) {
                aeroplane = place[i];
                place[i] = nullptr;
                removeFromBuffer(i);
                break;
            }
        }
        return aeroplane;

    }

};


int main()
{
    srand(time(NULL));
   // generateAeroplaneFile(PATH_AEROPLANE_FILE, COUNT_AEROPLANE);
   // generateAerodromFile(PATH_AERODROM_FILE, COUNT_AERODROM);

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

    Aerodrom aero{ 3, 5 };
    aero.startInit();

    
}
