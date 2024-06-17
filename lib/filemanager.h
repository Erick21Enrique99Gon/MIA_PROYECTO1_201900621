#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include <bits/stdc++.h>

#include "../lib/shared.h"
#include "../lib/structs.h"
#include "../lib/mount.h"

using namespace std;

class FileManager
{
private:
    Mount mount;
    Shared shares;
public:
    FileManager();
    int getfree(Structs::Superblock spb,string pth,string tipo);
};

#endif