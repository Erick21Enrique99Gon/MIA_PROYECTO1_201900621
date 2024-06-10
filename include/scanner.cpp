#include "../lib/scanner.h"
#include "../lib/disco.h"

#include <iostream>
#include <stdlib.h>
#include <locale.h>
#include <fstream>
#include <vector>
#include <cstdlib>

using namespace std;

Disk disco;

scanner::scanner(){}

void Clear(){
    cout << "\x1B[2J\x1B[H";
}

void scanner::start(){
    system("clear");
    cout << "------Ingrese Un Comando----------" << endl;
    cout << "------exit para salir ------------" << endl;
    cout << ">>";
    while (true)
    {
        string texto;
        getline(cin,texto);
        Clear();
        if (compare(texto,"exit"))
        {
            break;
        }
        string tk = token(texto);
        cout << "Token: " << tk << endl;
        vector<string> tokens = split_tokens(texto);
        cout << "Tokens: ";
        for (string &i : tokens)
        {
            cout << i << " ";
        }
        cout <<"end\n" << endl;
        functions(tk,tokens);
        cout << "\nPresione enter para continuar...." << endl;
        getline(cin,texto);
        Clear();
        cout << "------Ingrese Un Comando----------" << endl;
        cout << "------exit para salir ------------" << endl;
        cout << ">>";
    }
}

void scanner::errores(string operacion, string mensaje){
    
    cout << "\033[1;41m Error\033"<< "\033[0;31m(" + operacion + ")~~> \033[0m"<< mensaje << endl;
}

void scanner::respuesta(string operacion, string mensaje){
    
    cout << "\033[0;42m(" + operacion + ")~~> \033[0m"<< mensaje << endl;
}

string scanner::upper(string a){
    string b = "";
    for (char &i : a)
    {
        b += toupper(i);
    }
    return b;
}

bool scanner::compare(string a, string b){
    if( upper(a) == upper(b)){
        return true;
    }
    return false;
}

string scanner::token(string text){
    string tkn = "";
    bool terminar = false;
    for(char &c : text){
        if (terminar)
        {
            if (c == ' ' || c == '-')
            {
                break;
            }
            tkn += c;
        }
        else if(c != ' ' && !terminar){
            if (c == '#')
            {
                tkn = text;
                break;
            }else{
                tkn += c;
                terminar = true;
            }
        }
    }
    return tkn;
}

vector<string> scanner::split_tokens(string text){
    vector<string> tokens;
    if (text.empty())
    {
        return tokens;
    }
    text.push_back(' ');
    string token = "";
    int estado = 0;
    for(char &c: text){
        if (estado ==0 && c=='-')
        {
            estado = 1;

        }else if(estado==0 && c=='#'){
            continue;
        }else if(estado!=0){
            if (estado == 1)
            {
                if(c=='='){
                    estado = 2;
                }else if(c == ' '){
                    continue;
                }
            }else if(estado == 2){
                if (c=='\"')
                {
                    estado = 3;
                }else{
                    estado = 4;
                }
                
            }else if(estado == 3){
                if (c=='\"')
                {
                    estado = 4;
                }
            }else if (estado==4 && c=='\"')
            {
                tokens.clear();
                continue;
            }else if (estado ==4 && c==' ')
            {
                estado = 0;
                tokens.push_back(token);
                token = "";
                continue;
            }
            token+=c;
        }
    }
    return tokens;
}

void scanner::functions(string token, vector<string> tks)
{   
    if (compare(token, "MKDISK"))
    {
        cout << "FUNCION MKDISK" << endl;
        disco.mkdisk(tks); 
    }
    // else if(compare(token, "RMDISK")){
    //     cout << "FUNCION RMDISK" << endl;
    //     //disco.rmdisk(tks);
    // }else if(compare(token, "FDISK")){
    //     cout << "FUNCION FDISK" << endl;
    //     disco.fdisk(tks);
    // }else if(compare(token, "MOUNT")){
    //     cout << "FUNCION MOUNT" << endl;
    //     mount.mount(tks);
    // }else if(compare(token, "UNMOUNT")){
    //     cout << "FUNCION *UNMOUNT" << endl;
    //     //mount.unmount(tks);
    // }else if(compare(token, "MKFS")){
    //     cout << "FUNCION MKFS" << endl;
    //     //FileSystem fileSystem = FileSystem(mount);
    //     //fileSystem.mkfs(tks);

    // }else if(compare(token, "LOGIN")){
    //     cout << "FUNCION LOGIN" << endl;
    //     if(logued){
    //         //shared.handler("LOGIN", " ya existe una sesion abierta");
    //         return;
    //     }
    //     //logued = user.login(tks,mount);

    // }else if(compare(token, "LOGOUT")){
    //     cout << "FUNCION LOGOUT" << endl;
    //     if(!logued){
    //     //    shared.handler("LOGOUT", " debe de iniciar sesion primero");
    //         return;
    //     }
    //     //logued = user.logout();

    // }else if(compare(token, "MKGRP")){
    //     if(!logued){
    //     //    shared.handler("MKGRP", " debe de iniciar sesion primero");
    //         return;
    //     }
    //     cout << "FUNCION MKGRP" << endl;
    //     //user.grp(tks,"MK");

    // }else if(compare(token, "RMGRP")){
    //     if(!logued){
    //     //    shared.handler("RMGRP", " debe de iniciar sesion primero");
    //         return;
    //     }
    //     cout << "FUNCION RMGRP" << endl;
    //     //user.grp(tks,"RM");

    // }else if(compare(token, "MKUSR")){
    //     if(!logued){
    //     //    shared.handler("MKUSR", " debe de iniciar sesion primero");
    //         return;
    //     }
    //     cout << "FUNCION MKUSR" << endl;
    //     //user.usr(tks,"MK");

    // }else if(compare(token, "RMUSR")){
    //     if(!logued){
    //     //    shared.handler("RMUSR", " debe de iniciar sesion primero");
    //         return;
    //     }
    //     cout << "FUNCION RMUSR" << endl;
    //     //user.usr(tks,"RM");

    // }else if(compare(token, "MKDIR")){
    //     if(!logued){
    //     //    shared.handler("MKDIR", " debe de iniciar sesion primero");
    //         return;
    //     }
    //     string p;
    //     cout << "FUNCION MKDIR" << endl;
    //     //Structs::Partition partition = mount.getmount(user.logged.id, &p);
    //     //filemanager.mkdir(tks, partition, p);
    // }else if(compare(token, "REP")){
    //     cout << "FUNCION REPORTES" << endl;
    //     //report.generar(tks, mount);
    // }else if(compare(token, "EXEC")){
    //     cout << "FUNCION EXEC" << endl;
    //     funcion_excec(tks);
    // }else if(compare(token.substr(0,1),"#")){
    //     respuesta("COMENTARIO",token);
    // }
    else{
        cout << "El comando ingresado no se reconoce en el sistema \""+token+"\"" << endl;
    //     errores("SYSTEM","El comando ingresado no se reconoce en el sistema \""+token+"\"");
    }
}