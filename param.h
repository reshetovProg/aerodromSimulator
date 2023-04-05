#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
using namespace std;

#define random(a,b) a+rand()%(b+1-a)

//��������� ���� ������
#include "generator.h"

//������ � �������
#include "file.h"

const int COUNT_AEROPLANE = 10; //���-�� ���������
const int COUNT_AERODROM = 10; //���-�� ����������
const std::string PATH_AEROPLANE_FILE = "aeroplanes.csv"; //���� � �� ��������
const std::string PATH_AERODROM_FILE = "aerodrom.csv"; //���� � �� ���������
