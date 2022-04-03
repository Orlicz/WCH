#pragma once
/*
Web-Class-Helper Commands Module Header File 1.1.0
This source code file is under MIT License.
Copyright (c) 2022 Class Tools Develop Team
Contributors: jsh-jsh ren-yc
*/
#define VARIABLES_H
#include <bits/stdc++.h>
#include <windows.h>
#include <conio.h>
#include <direct.h>
#include "Apis.hpp"
#include "Commands.hpp"
#include "File-process.hpp"
#include "Functions.hpp"
using namespace std;

#define WCH_ERRNO_OUT_OF_RANGE "OOR"
#define WCH_ERRNO_NETWORK_FAILURE "NF"
#define WCH_ERRNO_CLOCK_OPERATION "CO"

struct WCH_Time {
    int Year;
    int Month;
    int Day;
    int Hour;
    int Minute;
    int Second;
    int WeekDay;
    string Name;
};
const string Weekdayname[7] = { "Sunday.txt", "Monday.txt", "Tuesday.txt", "Wednesday.txt", "Thursday.txt", "Friday.txt", "Saturday.txt" };
multimap <int, pair <int, string>> mm;
int WCH_clock_num;
bool cmd_line = true;
bool anti_idle = false;
bool mode = false;
string op;
string UserName;
ifstream fin;
ofstream fout;
