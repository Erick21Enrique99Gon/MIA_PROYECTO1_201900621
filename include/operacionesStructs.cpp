#include "../lib/operacionesStructs.h"
#include "../lib/disco.h"
#include "../lib/scanner.h"
#include "../lib/structs.h"

#include <vector>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <algorithm>

using namespace std;

operacionesStructs::operacionesStructs(){}

string operacionesStructs::obtenerContenidoFileBlocks(Mount mount,string pathInterno,string id,string path, Structs::Inodes inodo)
{
    try
    {
        string path;
        Structs::Partition partition = mount.getmount(id,&path);
        Structs::Superblock super;
        FILE *rfile = fopen(path.c_str(), "rb");
        fseek(rfile, partition.part_start, SEEK_SET);
        fread(&super, sizeof(Structs::Superblock), 1, rfile);

        Structs::Inodes inodoFile = obtenerInodo(mount,pathInterno, id, path, inodo);

        string a;
        return a;
    }
    catch(const std::exception& e)
    {
        scan.errores("Obtencion contenido de file blocks", e.what());
        string a;
        return  a;
    }
    
}

Structs::Inodes operacionesStructs::obtenerInodo(Mount mount,string pathInterno,string id,string path, Structs::Inodes inodo)
{
    try
    {
        string path;
        Structs::Partition partition = mount.getmount(id,&path);
        Structs::Superblock super;
        FILE *rfile = fopen(path.c_str(), "rb");
        fseek(rfile, partition.part_start, SEEK_SET);
        fread(&super, sizeof(Structs::Superblock), 1, rfile);
        int contador = 0;
        for(int apuntador:inodo.i_block){
            cout << contador<<endl;
            contador++;
        }

        Structs::Inodes a;
        return  a;
    }
    catch(const std::exception& e)
    {
        scan.errores("Obtencion Inodo", e.what());
        Structs::Inodes a;
        return  a;
    }
    
}