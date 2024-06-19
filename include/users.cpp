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

        string txt;
        txt += filemanager.obtenerContenidoFileBlocks(mount,"/user.txt",id,path,raiz);
        cout<<txt<<endl;
        vector<string> vctr = getElements(txt, '\n');
        for (string line:vctr) {
            if (line[2] == 'U' || line[2] == 'u') {
                vector<string> in = getElements(line, ',');
                if (shared.compare(in[3], usuario) && shared.compare(in[4], password)) {
                    shared.response("LOGIN", "logueado correctamente");
                    logged.id = id;
                    logged.user = usuario;
                    logged.password = password;
                    logged.uid = stoi(in[0]);
                    return true;
                }
            }
        }
        throw runtime_error("no hay credenciales similares");
    }
    catch(const std::exception& e)
    {
        shared.handler("LOGIN", e.what());
        return false;
    }
    
}

bool Users::logout(){
    shared.response("LOGOUT", "hasta luego " + logged.user);
    logged = User();
    return false;
}

vector<string> Users::getElements(string txt, char c) {
    vector<string> v;
    string line;
    if (c == ',') {
        txt.push_back(',');
    }
    for (char &x: txt) {
        if (x == c) {
            v.push_back(line);
            line = "";
            continue;
        }
        line += x;
    }

    if (v.empty()) {
        throw runtime_error("no hay archivo txt");
    }
    return v;
}

void Users::grp(vector<string> context, string action) {
    vector<string> required = {"name"};
    string name;

    for (int i = 0; i < context.size(); i++) {
        string current = context.at(i);
        string id_ = current.substr(0, current.find("="));
        current.erase(0, id_.length() + 1);
        if (current.substr(0, 1) == "\"") {
            current = current.substr(1, current.length() - 2);
        }

        if (shared.compare(id_, "name")) {
            if (count(required.begin(), required.end(), id_)) {
                auto itr = find(required.begin(), required.end(), id_);
                required.erase(itr);
                name = current;
            }
        }
    }

    if (required.size() != 0) {
        shared.handler(action + "GRP", "requiere ciertos parámetros obligatorios");
        return;
    }
    if (shared.compare(action, "MK")) {
        mkgrp(name);
    } else {
        rmgrp(name);
    }
}

void Users::mkgrp(string n) {
    try {
        if (!(shared.compare(logged.user, "root"))) {
            throw runtime_error("no se puede realizar la acción sin el usuario root");
        }
        string path;
        Structs::Partition partition = mount.getmount(logged.id, &path);

        Structs::Superblock super;
        FILE *rfile = fopen(path.c_str(), "rb+");
        fseek(rfile, partition.part_start, SEEK_SET);
        fread(&super, sizeof(Structs::Superblock), 1, rfile);

        Structs::Inodes raiz;
        fseek(rfile, super.s_inode_start, SEEK_SET);
        fread(&raiz, sizeof(Structs::Inodes), 1, rfile);
        fclose(rfile);

        string txt;
        txt += filemanager.obtenerContenidoFileBlocks(mount,"/user.txt",logged.id,path,raiz);
        cout<<txt<<endl;
        vector<string> vctr = getElements(txt, '\n');
        int c = 0;
        for (string line:vctr) {
            if ((line[2] == 'G' || line[2] == 'g')) {
                c++;
                vector<string> in = getElements(line, ',');
                if (in[2] == n) {
                    if (line[0] == '0') {

                    } else {
                        throw runtime_error("el grupo ya existe");
                    }
                }
            }
        }
        txt += to_string(c + 1) + ",G," + n + "\n";
        cout<<txt<<endl;
        cout << raiz.i_size <<endl;
        Structs::Inodes inodoHijo = filemanager.escribirContenidoFileBlock(mount,"/user.txt",logged.id,path,raiz,txt);
        if (inodoHijo.i_uid!=-1)
        {
            raiz = inodoHijo;
        }
        
        filemanager.escribirInodos(&raiz,path,0,super);
        shared.response("MKGRP", "grupo creado correctamente");
    }
    catch (exception &e) {
        shared.handler("MKGRP", e.what());
    }
}

void Users::rmgrp(string n) {
    try {
        if (!(shared.compare(logged.user, "root"))) {
            throw runtime_error("no se puede realizar la acción sin el usuario root");
        }
        string path;
        Structs::Partition partition = mount.getmount(logged.id, &path);

        Structs::Superblock super;
        FILE *rfile = fopen(path.c_str(), "rb+");
        fseek(rfile, partition.part_start, SEEK_SET);
        fread(&super, sizeof(Structs::Superblock), 1, rfile);


        Structs::Inodes raiz;
        fseek(rfile, super.s_inode_start, SEEK_SET);
        fread(&raiz, sizeof(Structs::Inodes), 1, rfile);
        fclose(rfile);

        string txt;
        txt += filemanager.obtenerContenidoFileBlocks(mount,"/user.txt",logged.id,path,raiz);
        cout<<txt<<endl;

        bool exist = false;
        vector<string> vctr = getElements(txt, '\n');
        txt = "";

        for (string line:vctr) {
            if ((line[2] == 'G' || line[2] == 'g') && line[0] != '0') {
                vector<string> in = getElements(line, ',');
                if (in[2] == n) {
                    exist = true;
                    txt += to_string(0) + ",G," + in[2] + "\n";
                    continue;
                }
            }
            txt += line + "\n";
        }
        if (!exist) {
            throw runtime_error("el grupo no existe");
        }
        cout<<txt<<endl;
        Structs::Inodes inodoHijo = filemanager.editarArchivo(mount,"/user.txt",logged.id,path,raiz,txt);
        inodoHijo = filemanager.escribirContenidoFileBlock(mount,"/user.txt",logged.id,path,raiz,txt);
        if (inodoHijo.i_uid!=-1)
        {
            raiz = inodoHijo;
        }
        shared.response("RMGRP", "grupo eliminado correctamente");
    }
    catch (exception &e) {
        shared.handler("RMGRP", e.what());
    }
}

void Users::usr(vector<string> context, string action) {
    vector<string> required;
    if (shared.compare(action, "MK")) {
        required = {"usr", "pwd", "grp","pass"};
    } else {
        required = {"usr"};
    }
    string usr;
    string pwd;
    string grp;

    for (int i = 0; i < context.size(); i++) {
        string current = context.at(i);
        string id_ = current.substr(0, current.find("="));
        current.erase(0, id_.length() + 1);
        if (current.substr(0, 1) == "\"") {
            current = current.substr(1, current.length() - 2);
        }

        if (shared.compare(id_, "usr")) {
            if (count(required.begin(), required.end(), id_)) {
                auto itr = find(required.begin(), required.end(), id_);
                required.erase(itr);
                usr = current;
            }
        } else if (shared.compare(id_, "pwd")) {
            if (count(required.begin(), required.end(), id_)) {
                auto itr = find(required.begin(), required.end(), id_);
                required.erase(itr);
                auto itr2 = find(required.begin(), required.end(), "pass");
                required.erase(itr2);
                pwd = current;
            }
        }else if (shared.compare(id_, "pass")) {
            if (count(required.begin(), required.end(), id_)) {
                auto itr = find(required.begin(), required.end(), id_);
                required.erase(itr);
                auto itr2 = find(required.begin(), required.end(), "pwd");
                required.erase(itr2);
                pwd = current;
            }
        } else if (shared.compare(id_, "grp")) {
            if (count(required.begin(), required.end(), id_)) {
                auto itr = find(required.begin(), required.end(), id_);
                required.erase(itr);
                grp = current;
            }
        }
    }
    for (string falta: required)
    {
        cout<< falta<<endl;
    }
    
    if (required.size() != 0) {
        shared.handler(action + "GRP", "requiere ciertos parámetros obligatorios");
        return;
    }
    if (shared.compare(action, "MK")) {
        mkusr(usr, pwd, grp);
    } else {
        rmusr(usr);
    }
}

void Users::mkusr(string usr, string pwd, string grp) {
    try {
        if (!(shared.compare(logged.user, "root"))) {
            throw runtime_error("no se puede realizar la acción sin el usuario root");
        }
        string path;
        Structs::Partition partition = mount.getmount(logged.id, &path);

        Structs::Superblock super;
        FILE *rfile = fopen(path.c_str(), "rb+");
        fseek(rfile, partition.part_start, SEEK_SET);
        fread(&super, sizeof(Structs::Superblock), 1, rfile);

        Structs::Inodes raiz;
        fseek(rfile, super.s_inode_start, SEEK_SET);
        fread(&raiz, sizeof(Structs::Inodes), 1, rfile);
        fclose(rfile);

        string txt;
        txt += filemanager.obtenerContenidoFileBlocks(mount,"/user.txt",logged.id,path,raiz);

        vector<string> vctr = getElements(txt, '\n');
        int c = 0;
        for (string line:vctr) {
            if ((line[2] == 'U' || line[2] == 'u') && line[0] != '0') {
                c++;
                vector<string> in = getElements(line, ',');
                if (in[3] == usr) {
                    throw runtime_error("el usuario ya existe");
                }
            }
        }
        txt += to_string(c + 1) + ",U," + grp + "," + usr + "," + pwd + "\n";
        cout<<txt<<endl;
        cout << raiz.i_size <<endl;
        Structs::Inodes inodoHijo = filemanager.escribirContenidoFileBlock(mount,"/user.txt",logged.id,path,raiz,txt);
        if (inodoHijo.i_uid!=-1)
        {
            raiz = inodoHijo;
        }
        filemanager.escribirInodos(&raiz,path,0,super);
        shared.response("MKUSR", "usuario creado correctamente");
    }
    catch (exception &e) {
        shared.handler("MKUSR", e.what());
    }
}

void Users::rmusr(string usr) {
    try {
        if (!(shared.compare(logged.user, "root"))) {
            throw runtime_error("no se puede realizar la acción sin el usuario root");
        }
        string path;
        Structs::Partition partition = mount.getmount(logged.id, &path);

        Structs::Superblock super;
        FILE *rfile = fopen(path.c_str(), "rb+");
        fseek(rfile, partition.part_start, SEEK_SET);
        fread(&super, sizeof(Structs::Superblock), 1, rfile);


        Structs::Inodes raiz;
        fseek(rfile, super.s_inode_start, SEEK_SET);
        fread(&raiz, sizeof(Structs::Inodes), 1, rfile);
        fclose(rfile);

        string txt;
        txt += filemanager.obtenerContenidoFileBlocks(mount,"/user.txt",logged.id,path,raiz);
        cout<<txt<<endl;

        bool exist = false;
        vector<string> vctr = getElements(txt, '\n');
        txt = "";
        for (string line:vctr) {
            if ((line[2] == 'U' || line[2] == 'u') && line[0] != '0') {
                vector<string> in = getElements(line, ',');
                if (in[3] == usr) {
                    exist = true;
                    txt += to_string(0) + ",U," + in[2] + "," + in[3] + "," + in[4] + "\n";
                    continue;
                }
            }
            txt += line + "\n";
        }
        if (!exist) {
            throw runtime_error("el usuario no existe");
        }
        cout<<txt<<endl;
        Structs::Inodes inodoHijo = filemanager.editarArchivo(mount,"/user.txt",logged.id,path,raiz,txt);
        inodoHijo = filemanager.escribirContenidoFileBlock(mount,"/user.txt",logged.id,path,raiz,txt);
        if (inodoHijo.i_uid!=-1)
        {
            raiz = inodoHijo;
        }
        shared.response("RMGRP", "grupo eliminado correctamente");
    }
    catch (exception &e) {
        shared.handler("RMUSR", e.what());
    }
}