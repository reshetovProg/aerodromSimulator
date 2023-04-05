
#include "param.h"

#define show

#define test
//#define testSplit

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

    //инициализация объекта из строки данных
    void startInit(string str) {
        string* array = split(str, ';');
        id = array[0];
        type = array[1];
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

    string info() {
        return "angarCount: " + to_string(angarCount) + ", placeCount: " +to_string(placeCount);
    }

    //начальная инициализация массивов
    void startInit() {
        place = new Aeroplane*[placeCount]{nullptr};
        placeBuffer = new int[placeCount] {};

        angar = new Aeroplane ** [angarCount];
        for (int i = 0; i < angarCount; i++) {
            angar[i] = new Aeroplane*[2]{ nullptr };
        }
    }

    
    //инициализация объекта из строки данных
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
            if (place[i] != nullptr) {
                if (place[i]->id == id) {
                    aeroplane = place[i];
                    place[i] = nullptr;
                    removeFromBuffer(i);
                    break;
                }
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
        std::cout << aeroplane->info() << "просит посадки" << endl;
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

    void visual() {
        std::cout << "angar" << endl;
        for (int i = 0; i < angarCount; i++) {
            string str1 = (angar[i][0] == nullptr) ? "--" : angar[i][0]->info();
            string str2 = (angar[i][1] == nullptr) ? "--" : angar[i][1]->info();
            std::cout << "|" << str1 << "|" << str2 << "|";
        }
        cout << endl;
        std::cout << "place" << endl;
        for (int i = 0; i < placeCount; i++) {
            string str1 = (place[i] == nullptr) ? "--" : place[i]->info();
            std::cout << "|" << str1;
        }
        std::cout << endl;

    }


};


void replace(Aeroplane** &array1, Aeroplane** &array2, int index) {

    
    int size2;
    if (array2 == nullptr) 
        size2 = 0;
    else
        size2 = _msize(array2) / sizeof(array2);

    int size1 = _msize(array1) / sizeof(array1);
    cout << "replace" << size1 << ":" << size2 << endl;
    Aeroplane** buf1 = new Aeroplane*[size1 - 1];
    Aeroplane** buf2 = new Aeroplane*[size2 + 1];

    for (int i = 0; i < index; i++) {
        buf1[i] = array1[i];
    }
    for (int i = index+1; i < size1; i++) {
        buf1[i-1] = array1[i];
    }
    for (int i = 0; i < size2; i++) {
        buf2[i] = array2[i];
    }
    buf2[size2] = array1[index];
    delete[] array1;
    delete[] array2;
    if (size1 == 1) buf1 = nullptr;
    array1 = buf1;
    array2 = buf2;
}


int main()
{
    srand(time(NULL));
    setlocale(LC_ALL, "");
    generateAeroplaneFile(PATH_AEROPLANE_FILE, COUNT_AEROPLANE);
    generateAerodromFile(PATH_AERODROM_FILE, COUNT_AERODROM);

    //вывод файлов

#ifdef testSplit
    string* strArrayAeroplane = readFile(PATH_AEROPLANE_FILE);
    Aeroplane* aero = new Aeroplane;
    aero->startInit(strArrayAeroplane[0]);
    cout<<aero->info();
#endif
#ifdef test

    for (int i = 0; i < 10; i++) {
        string* strArrayAeroplane = readFile(PATH_AEROPLANE_FILE);
        string* strArrayAerodrom = readFile(PATH_AERODROM_FILE);

        //cout << "aeroplanes" << endl;
        //for (int i = 0; i < COUNT_AEROPLANE; i++) {
        //    cout << strArrayAeroplane[i] << endl;
        //}

        //cout << "aerodroms" << endl;
        //for (int i = 0; i < COUNT_AERODROM; i++) {
        //    cout << strArrayAerodrom[i] << endl;
        //}

        //создание массива объектов
        Aeroplane** arrayAeroplane = new Aeroplane * [COUNT_AEROPLANE];
        for (int i = 0; i < COUNT_AEROPLANE; i++) {
            arrayAeroplane[i] = new Aeroplane;
            arrayAeroplane[i]->startInit(strArrayAeroplane[i]);
            std::cout << arrayAeroplane[i]->info() << endl;
        }

        //создание аэропорта
        Aerodrom aerodrom;
        aerodrom.startInit(strArrayAerodrom[random(0, COUNT_AERODROM - 1)]);
        std::cout << aerodrom.info();
        cout << endl;
        //массив самолетов в аэропорту
        Aeroplane** inAeroport = nullptr;

        //симуляция перелетов
        for (int i = 0, size; i < 10; i++) {
            //выбор прилетел и отлет с вероятностью 7/1
            //true - in / false - out
            bool in_out = (random(0, 7) == 0) ? false : true;
            cout << endl << in_out << endl;
            if (in_out) {
                size = _msize(arrayAeroplane) / sizeof(arrayAeroplane[0]);
                int index = random(0, size - 1);
                if (aerodrom.income(arrayAeroplane[index])) {
                    cout << endl << "income" << arrayAeroplane[index]->info() << endl;
                    replace(arrayAeroplane, inAeroport, index);
                    aerodrom.visual();
                }

            }
            else {
                if (inAeroport != nullptr) {
                    size = _msize(inAeroport) / sizeof(inAeroport[0]);
                    int index = random(0, size-1);
                    cout << endl;
                    for (int i = 0; i < size; i++) {
                        cout << inAeroport[i]->info() << endl;
                    }
                    cout << endl << "index" << index << endl;
                    if (size != 1) {
                        aerodrom.outcome(inAeroport[index]->id);
                        cout << endl << "outcome" << inAeroport[index]->info() << endl;
                        replace(inAeroport, arrayAeroplane, index);
                        aerodrom.visual();
                    }
                    else cout << "else" << endl;
                }
                
            }

        }
    }
    

#endif



    
   // Aerodrom aero = ;
    //aero.startInit();

    
}
