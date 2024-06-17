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

class report
{
private:
    Shared shared;
    Disk disk;
    Mount mount;
    // FileManager filemanager;
public:
    Report();
    void generar(vector <string> context, Mount m);
};

#endif
