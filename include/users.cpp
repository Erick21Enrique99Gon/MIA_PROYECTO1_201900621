#include "../lib/users.h"
#include "../lib/operacionesStructs.h"

#include <iostream>
#include "string"

using namespace std;
operacionesStructs operacionesstructs;

Users::Users(){}

bool Users::login(vector <string> context, Mount m){
    mount = m;
    vector<string> required = {"usr", "pass", "id","pwd"};
    string id="";
    string usuario="";
    string password="";

    for(auto current:context)
    {
        string _id = shared.lower(current.substr(0,current.find("=")));
        current.erase(0,_id.length()+1);
        if(current.substr(0, 1)=="\""){
            current = current.substr(1,current.length()-2);
        }
        if(shared.compare(_id,"usr")){
            if(count(required.begin(), required.end(), _id)){
                auto itr = find(required.begin(), required.end(), _id);
                required.erase(itr);
                usuario = current;
            }
        }
        else if(shared.compare(_id,"pass")){
            if(count(required.begin(), required.end(), _id)){
                auto itr = find(required.begin(), required.end(), _id);
                required.erase(itr);
                auto itr2 = find(required.begin(), required.end(), "pwd");
                required.erase(itr2);
                password = current;
            }
        }
        else if(shared.compare(_id,"pwd")){
            if(count(required.begin(), required.end(), _id)){
                auto itr = find(required.begin(), required.end(), _id);
                required.erase(itr);
                auto itr2 = find(required.begin(), required.end(), "pass");
                required.erase(itr2);
                password = current;
            }
        }
        else if(shared.compare(_id,"id")){
            if(count(required.begin(), required.end(), _id)){
                auto itr = find(required.begin(), required.end(), _id);
                required.erase(itr);
                id = current;
            }
        }else{
            scan.errores("LOGIN","No se reconoce el parametro "+_id);
        }
    }

    if(!required.empty()){
        string faltantes = "";
        for(string faltante:required)
        {
            faltantes += faltante+" ";
        }
        faltantes = "Faltan parametros obligatorios para completar la acción: " + faltantes;
        scan.errores("LOGIN",faltantes);
        return false;
    }else{
        login(usuario, password, id);
        return true;
    }
}

bool Users::login(string usuario,string password,string id){
    try
    {
        string path;
        Structs::Partition partition = mount.getmount(id,&path);

        Structs::Superblock super;
        FILE *rfile = fopen(path.c_str(), "rb");
        fseek(rfile, partition.part_start, SEEK_SET);
        fread(&super, sizeof(Structs::Superblock), 1, rfile);

        Structs::Inodes raiz;
        fseek(rfile, super.s_inode_start, SEEK_SET);
        fread(&raiz, sizeof(Structs::Inodes), 1, rfile);
        fclose(rfile);

        

        return true;
    }
    catch(const std::exception& e)
    {
        shared.handler("LOGIN", e.what());
        return false;
    }
    
}