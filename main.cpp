
#include "param.h"

#define show

//#define test

string* split(string str, char s) {
    int count = 1;
    for (int i = 0; i < size(str); i++) {
        if (str[i] == s) count++;
    }
    string* buf = new string[count];
    string demo = "";
    for (int i = 0, k = 0; i < size(str); i++) {
        if (str[i] != s) {
            demo += str[i];
        }
        else {
            buf[k++] = demo;
            demo = "";
        }
    }
    buf[count - 1] = demo;
    return buf;
}

struct Aeroplane {
    string id;
    string type;

    void startInit(string str) {
        string* array = split(str, ';');
        id = stoi(array[0]);
        type = stoi(array[1]);
    }

    string info() {
        return type + " : " + id;
    }
};

struct Aerodrom {
    int angarCount;
    int placeCount;

    int id;

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

    

    void startInit(string str) {

        string* array = split(str,';');
        id = stoi(array[0]);
        angarCount = stoi(array[1]);
        placeCount = stoi(array[2]);

        place = new Aeroplane * [placeCount] {nullptr};
        placeBuffer = new int[placeCount] {};

        angar = new Aeroplane * *[angarCount];
        for (int i = 0; i < angarCount; i++) {
            angar[i] = new Aeroplane * [2]{ nullptr };
        }
    }

    //обновление очередности занятия парковочных мест после добавления самолета
    void addToBuffer(int index) {
        for (int i = 0; i < placeCount; i++) {
            if (placeBuffer[i] != 0) placeBuffer[i]++;
        }
        placeBuffer[index] = 1;
    }

    //обновление очередности занятия парковочных мест после удаления самолета
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
#ifdef show
                    cout << aeroplane->info() << " - зашел в ангар [" << i << "[0]"<<endl;
#endif
                    return true;
                }
                else if (angar[i][0]->type == "light" && angar[i][1] == nullptr) {
                    angar[i][1] = aeroplane;
#ifdef show
                    cout << aeroplane->info() << " - зашел в ангар [" << i << "[1]" << endl;
#endif
                    return true;
                }
            }
        }
        else if (aeroplane->type == "boeing") {
            //Aeroplane* buf=nullptr; //свободное место в ангаре
            int posI = -1; //номер свободного метса в ангаре
            int posN = -1; //позиция на свободном месте 0 или 1
            int firstLightPosition = -1; //позиция первого легкого самолета
            for (int i = 0; i < angarCount; i++) {
                if (angar[i][0] == nullptr && angar[i][1] == nullptr) {
                    angar[i][0] = aeroplane;
#ifdef show
                    cout << aeroplane->info() << " - зашел в ангар [" << i << "[0]" << endl;
#endif
                    return true;
                }
                if (angar[i][0] != nullptr && angar[i][1] != nullptr && firstLightPosition == -1) {
                    firstLightPosition = i;
                }
                //!!!! возможна ошибка если nullptr
                if (angar[i][0]==nullptr && posI==-1) {
                    posI = i;
                    posN = 0;
                    //buf=angar[i][1];
                }
                if (angar[i][1] == nullptr && posI == -1 && angar[i][0]->type == "light") {
                    posI = i;
                    posN = 1;
                    //buf = angar[i][0];
                }

            }
            if (posI != -1) {
                if (addToPlace(angar[posI][posN])) {
                    angar[posI][0] = aeroplane;
                    angar[posI][1] = nullptr;
                }
            }
            else {
                if (firstLightPosition != -1) {
                    int counter = 0;
                    for (int j = 0; j < placeCount; j++) {
                        if (place[j] == nullptr) {
                            counter++;
                        }
                    }
                    if (counter >= 2) {
                        addToPlace(angar[firstLightPosition][0]);
                        addToPlace(angar[firstLightPosition][1]);
                        angar[firstLightPosition][0] = aeroplane;
                        angar[firstLightPosition][1] = nullptr;
                    }
                }
                
            }
        }
#ifdef show
        cout << aeroplane->info() << "не может встать в ангар" << endl;
#endif
        return false;
    }

    //добавить самолет на парковку
    bool addToPlace(Aeroplane*& aeroplane) {
        for (int i = 0; i < placeCount; i++) {
            if (place[i] == nullptr) {
                place[i] = aeroplane;
                addToBuffer(i);
#ifdef show
                cout << aeroplane->info() << " - зашел на парковку [" << i << "]" << endl;
#endif
                return true;
            }
        }
        int maxIndex = 0;
        int max = placeBuffer[0];
        for (int i = 1; i < placeCount; i++) {
            if (max < placeBuffer[i]) {
                max = placeBuffer[i];
                maxIndex = i;
            }
        }

        if (addToAngar(place[maxIndex])) {
            removeFromPlace(place[maxIndex]->id);
            place[maxIndex] = aeroplane;
            addToBuffer(maxIndex);
#ifdef show
            cout << aeroplane->info() << " - зашел на парковку [" << maxIndex << "]" << endl;
#endif
            return true;
        }
#ifdef show
        cout << aeroplane->info() << " - нет места на парковке" << endl;
#endif
        return false;
    }

    //удалить самолет с парковки по id (возвращает удаляемый самолет)
    Aeroplane* removeFromPlace(string id) {
        
        Aeroplane* aeroplane = nullptr;
        int i = 0;
        for (; i < placeCount; i++) {
            if (place[i]->id == id) {
                aeroplane = place[i];
                place[i] = nullptr;
                removeFromBuffer(i);
                break;
            }
        }
#ifdef show
        if (aeroplane == nullptr) {
            cout << id << " не найден на парковке" << endl;
        }
        else {
            cout << aeroplane->info() << "покинул парковку с места [" << i << "]" << endl;
        }
#endif
        return aeroplane;

    }

    //удалить самолет с ангара по id (возвращает удаляемый самолет)
    Aeroplane* removeFromAngar(string id) {
        Aeroplane* aeroplane = nullptr;
        int i = 0;
        int j;
        for (; i < angarCount; i++) {
            if (angar[i][0] != nullptr && angar[i][0]->id == id) {
                aeroplane = angar[i][0];
                j = 0;
                angar[i][0] = nullptr;
                break;
            }
            if (angar[i][1] != nullptr && angar[i][1]->id == id) {
                aeroplane = angar[i][1];
                j = 1;
                angar[i][1] = nullptr;
                break;
            }
        }
#ifdef show
        if (aeroplane == nullptr) {
            cout << id << " не найден в ангаре" << endl;
        }
        else {
            cout << aeroplane->info() << "покинул ангар с места [" << i << "][" << j << "]" << endl;
        }
#endif
        return aeroplane;
    }

    //прилет самолета
    bool income(Aeroplane*& aeroplane) {
#ifdef show
        cout << aeroplane->info() << "просит посадки" << endl;
#endif
        if (aeroplane->type == "light") {
            return addToPlace(aeroplane);
        }
        if (aeroplane->type == "boeing") {
            return addToAngar(aeroplane);
        }
    }

    //отлет самолета
    Aeroplane* outcome(string id) {

        Aeroplane* aeroplane = removeFromPlace(id);
        if (aeroplane == nullptr) {
            aeroplane = removeFromAngar(id);
        }
#ifdef show
        if (aeroplane == nullptr) {
            cout << "в аэропорту не найден самолет с id: " << id << endl;
        }
        else {
            cout << aeroplane->info() << "покинул аэропорт" << endl;
        }
#endif
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



    
   // Aerodrom aero = ;
    //aero.startInit();

    
}
