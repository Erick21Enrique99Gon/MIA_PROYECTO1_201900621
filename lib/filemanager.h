#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include <bits/stdc++.h>

#include "../lib/shared.h"
#include "../lib/structs.h"
#include "../lib/mount.h"
#include "../lib/scanner.h"

using namespace std;

class FileManager
{
private:
    Mount mount;
    Shared shares;
    scanner scan;
public:
    FileManager();
    int getfree(Structs::Superblock spb,string pth,string tipo);
    void putTaken(Structs::Superblock spr, string pth, string t, int apuntador);
    string obtenerContenidoFileBlocks(Mount mount,string pathInterno,string id,string path, Structs::Inodes inodo);
    Structs::Inodes obtenerInodo(Mount mount,string pathInterno,string id,string path, Structs::Inodes inodo);
    Structs::Inodes obtenerInodoFileBlock(Mount mount,string pathInterno,string id,string path, Structs::Folderblock fileblock,string carpetaInterna);
    void leerInodes(Structs::Inodes *inode,string path,int apuntador,Structs::Superblock super);
    void leerBloqueApuntadores(Structs::Pointerblock *bloqueApuntdadorSimple,string path,int apuntador,Structs::Superblock super);
    void leerBloqueArchivos(Structs::Fileblock *bloqueArchivo,string path,int apuntador,Structs::Superblock super);
    void leerBloqueFolder(Structs::Folderblock *bloqueFolder,string path,int apuntador,Structs::Superblock super);
    Structs::Inodes escribirContenidoFileBlock(Mount mount, string pathInterno, string id, string path, Structs::Inodes inodo,string content);
    Structs::Inodes buscarBloqueFileInode(Mount mount, string pathInterno, string id, string path, Structs::Folderblock bloqueFolder,string carpetaInterna, string content);
    Structs::Inodes escribirInodeFile(Mount mount,string pathInterno,string id,string path, Structs::Inodes inodo, string content);
    void escribirBloqueArchivos(Structs::Fileblock *bloqueArchivo,string path,int apuntador,Structs::Superblock super);
    void escribirInodos(Structs::Inodes *inode, string path, int apuntador, Structs::Superblock super);
    void borrarBM(Structs::Superblock spr, string pth, string t, int apuntador);
    Structs::Inodes editarArchivo(Mount mount,string pathInterno,string id,string path, Structs::Inodes inodo, string content);
    // void borrarContenidoFileInode(Mount mount,string pathInterno,string id,string path, Structs::Inodes inodo);
    void borrarStructInodo(int size,string path, int apuntador, Structs::Superblock super);
    void borrarStructBloque(int size,string path, int apuntador, Structs::Superblock super);
    Structs::Inodes buscarBloqueFileInodeBorrar(Mount mount, string pathInterno, string id, string path, Structs::Folderblock bloqueFolder,string carpetaInterna, string content);
    Structs::Inodes editarInodeBloqueFile(Mount mount, string pathInterno, string id, string path,  Structs::Inodes inodo,string content);
};

#endif