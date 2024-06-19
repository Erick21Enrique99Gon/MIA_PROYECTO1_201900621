#ifndef ADMIN_H
#define ADMIN_H

#include <string>
#include <bits/stdc++.h>
#include "../lib/shared.h"
#include "../lib/structs.h"
#include "../lib/scanner.h"
#include "../lib/mount.h"
#include "../lib/filemanager.h"

using namespace std;

class Users{
    public:
        Users();

        typedef struct _Users
        {
            string user;
            string password;
            string id;
            int uid;
        } User;
        vector<string> getElements(string txt, char c);
        bool login(vector <string> context, Mount m);
        bool login(string user, string password, string id);
        bool logout();
        void grp(vector <string> context, string action);
        void mkgrp(string n);
        void rmgrp(string n);
        void usr(vector<string> context, string action);
        void rmusr(string usr);
        void mkusr(string usr, string pwd, string grp);
        User logged;
    private:
        Shared shared;
        scanner scan;
        Mount mount;
        FileManager filemanager;
};

#endif