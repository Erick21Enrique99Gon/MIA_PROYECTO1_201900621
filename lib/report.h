#ifndef REPORT_H
#define REPORT_H

#include <string>
#include <bits/stdc++.h>

#include "../lib/scanner.h"
#include "../lib/shared.h"
#include "../lib/structs.h"
#include "../lib/disco.h"
// #include "../lib/filemanager.h"
#include "../lib/mount.h"

using namespace std;

class Report
{
private:
    Shared shared;
    Disk disk;
    Mount mount;
    // FileManager filemanager;
public:
    Report();
    void generar(vector <string> context, Mount m);
    void mbr (string p, string id);
    void dks (string p,string id);
    void bminode(string p, string id);
    void bmblock(string p, string id);
    void sb(string p, string id);
    void journaling(string p, string id);
};

#endif
