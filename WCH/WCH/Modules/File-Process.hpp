#pragma once
/*
Web-Class-Helper File Process Module Header File 1.1.0
This source code file is under MIT License.
Copyright (c) 2022 Class Tools Develop Team
Contributors: jsh-jsh ren-yc
*/
#define FILE_PROCESS_H
#include <bits/stdc++.h>
#include <windows.h>
#include <conio.h>
#include <direct.h>
#include "Apis.hpp"
#include "Commands.hpp"
#include "Functions.hpp"
#include "Variables.hpp"
using namespace std;

extern const string Weekdayname[7];
extern multimap <int, pair <int, string>> mm;
extern int WCH_clock_num;
extern bool cmd_line;
extern bool anti_idle;
extern bool mode;
extern string op;
extern string UserName;
extern ifstream fin;
extern ofstream fout;
extern void WCH_SetWindowStatus(bool);

inline void in_data(string fname) {
    freopen(fname.c_str(), "r", stdin);
}

inline void out_data(string fname) {
    freopen(fname.c_str(), "w", stdout);
}
