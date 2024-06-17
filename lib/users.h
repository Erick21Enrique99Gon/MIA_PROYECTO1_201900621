#ifndef ADMIN_H
#define ADMIN_H

#include <string>
#include <bits/stdc++.h>
#include "../lib/shared.h"
#include "../lib/structs.h"
#include "../lib/scanner.h"
#include "../lib/mount.h"
#include "../lib/operacionesStructs.h"
using namespace std;

class Users{
    public:
        Users();

        typedef struct _Users
        {
            string user;
            string password;
            string id;
        } User;
        
        bool login(vector <string> context, Mount m);
        bool login(string user, string password, string id);
        bool logout();
        void grp(vector <string> context, string action);
    private:
        Shared shared;
        scanner scan;
        Mount mount;
};

#endif