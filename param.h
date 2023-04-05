#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
using namespace std;

#define random(a,b) a+rand()%(b+1-a)

//генерация базы данных
#include "generator.h"

//работа с файлами
#include "file.h"

const int COUNT_AEROPLANE = 10; //кол-во самолетов
const int COUNT_AERODROM = 10; //кол-во аэропортов
const std::string PATH_AEROPLANE_FILE = "aeroplanes.csv"; //путь к бд самолеты
const std::string PATH_AERODROM_FILE = "aerodrom.csv"; //путь к бд аэродромы
