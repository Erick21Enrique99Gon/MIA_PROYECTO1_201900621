#include "../lib/filemanager.h"

#include <iostream>
#include "string"

using namespace std;

FileManager ::FileManager() {}

void FileManager::leerInodes(Structs::Inodes *inode,string path,int apuntador,Structs::Superblock super){
    FILE *rfile = fopen(path.c_str(), "rb");
    fseek(rfile, super.s_inode_start + apuntador * sizeof(Structs::Inodes), SEEK_SET);
    fread(inode, sizeof(Structs::Inodes), 1, rfile);
    fclose(rfile);
}

void FileManager::leerBloqueApuntadores(Structs::Pointerblock *bloqueApuntdadorSimple,string path,int apuntador,Structs::Superblock super){
    FILE *rfile = fopen(path.c_str(), "rb");
    fseek(rfile, super.s_block_start + apuntador * sizeof(Structs::Pointerblock ), SEEK_SET);
    fread(bloqueApuntdadorSimple, sizeof(Structs::Pointerblock), 1, rfile);
    fclose(rfile);
}

void FileManager::leerBloqueArchivos(Structs::Fileblock *bloqueArchivo,string path,int apuntador,Structs::Superblock super){
    FILE *rfile = fopen(path.c_str(), "rb");
    fseek(rfile, super.s_block_start + apuntador * sizeof(Structs::Fileblock ), SEEK_SET);
    fread(bloqueArchivo, sizeof(Structs::Fileblock), 1, rfile);
    fclose(rfile);
}

void FileManager::leerBloqueFolder(Structs::Folderblock *bloqueFolder,string path,int apuntador,Structs::Superblock super){
    FILE *rfile = fopen(path.c_str(), "rb");
    fseek(rfile, super.s_block_start + apuntador * sizeof(Structs::Fileblock ), SEEK_SET);
    fread(bloqueFolder, sizeof(Structs::Fileblock), 1, rfile);
    fclose(rfile);
}

void FileManager::escribirBloqueArchivos(Structs::Fileblock *bloqueArchivo, string path, int apuntador, Structs::Superblock super)
{
    FILE *rfile = fopen(path.c_str(), "rb+");
    fseek(rfile, super.s_block_start + apuntador * sizeof(Structs::Fileblock ), SEEK_SET);
    fwrite(bloqueArchivo,sizeof(Structs::Fileblock ),1,rfile);
    fclose(rfile);
}

void FileManager::escribirInodos(Structs::Inodes *inode, string path, int apuntador, Structs::Superblock super)
{
    FILE *rfile = fopen(path.c_str(), "rb+");
    fseek(rfile, super.s_inode_start + apuntador * sizeof(Structs::Inodes ), SEEK_SET);
    fwrite(inode,sizeof(Structs::Inodes ),1,rfile);
    fclose(rfile);
}

void FileManager::borrarStructInodo(int size,string path, int apuntador, Structs::Superblock super){
    FILE *rfile = fopen(path.c_str(), "rb+");
    fseek(rfile, super.s_inode_start + apuntador * size, SEEK_SET);
    for(int i =0;i<size;i++){
        fwrite("\0",1,1,rfile);
    }
    fclose(rfile);
}

void FileManager::borrarStructBloque(int size,string path, int apuntador, Structs::Superblock super){
    FILE *rfile = fopen(path.c_str(), "rb+");
    fseek(rfile, super.s_block_start + apuntador * size, SEEK_SET);
    for(int i =0;i<size;i++){
        fwrite("\0",1,1,rfile);
    }
    fclose(rfile);
}

int FileManager::getfree(Structs::Superblock spr, string pth, string t)
{
    char ch = 'x';
    FILE *file = fopen(pth.c_str(), "rb");
    if (t == "BI")
    {
        fseek(file, spr.s_bm_inode_start, SEEK_SET);
        for (int i = 0; i < spr.s_inodes_count; i++)
        {
            fread(&ch, sizeof(ch), 1, file);
            if (ch == '0')
            {
                fclose(file);
                return i;
            }
        }
    }
    else
    {
        fseek(file, spr.s_bm_block_start, SEEK_SET);
        for (int i = 0; i < spr.s_blocks_count; i++)
        {
            fread(&ch, sizeof(ch), 1, file);
            if (ch == '0')
            {
                fclose(file);
                return i;
            }
        }
    }
    fclose(file);
    return -1;
}

void FileManager::putTaken(Structs::Superblock spr, string pth, string t, int apuntador)
{
    char ch = '1';
    FILE *file = fopen(pth.c_str(), "rb+");
    if (t == "BI")
    {
        fseek(file, spr.s_bm_inode_start+sizeof(ch)*apuntador, SEEK_SET);
        fread(&ch, sizeof(ch), 1, file);
    }
    else
    {
        fseek(file, spr.s_bm_block_start+sizeof(ch)*apuntador, SEEK_SET);
        fwrite(&ch, sizeof(ch), 1, file);
    }
    fclose(file);
}

void FileManager::borrarBM(Structs::Superblock spr, string pth, string t, int apuntador)
{
    char ch = '0';
    FILE *file = fopen(pth.c_str(), "rb+");
    if (t == "BI")
    {
        fseek(file, spr.s_bm_inode_start+sizeof(ch)*apuntador, SEEK_SET);
        fread(&ch, sizeof(ch), 1, file);
    }
    else
    {
        fseek(file, spr.s_bm_block_start+sizeof(ch)*apuntador, SEEK_SET);
        fwrite(&ch, sizeof(ch), 1, file);
    }
    fclose(file);
}

string FileManager::obtenerContenidoFileBlocks(Mount mount, string pathInterno, string id, string path, Structs::Inodes inodo)
{
    try
    {
        string path;
        Structs::Partition partition = mount.getmount(id, &path);
        Structs::Superblock super;
        FILE *rfile = fopen(path.c_str(), "rb");
        fseek(rfile, partition.part_start, SEEK_SET);
        fread(&super, sizeof(Structs::Superblock), 1, rfile);
        fclose(rfile);

        Structs::Inodes inodoFile = obtenerInodo(mount, pathInterno, id, path, inodo);
        int contador = 0;
        if(inodoFile.i_uid==-1){
            throw runtime_error("no existe direccion");
        }
        string contenido;
        for (int apuntador:inodoFile.i_block)
        {
            cout << contador << endl;
            cout << apuntador << endl;
            if (apuntador != -1)
            {
                if (contador < 12)
                {
                    Structs::Fileblock bloquefile;
                    leerBloqueArchivos(&bloquefile,path,apuntador,super);
                    contenido += bloquefile.b_content;
                    cout << "contadorFileBlocks" << endl;
                    cout << contador << endl;
                }
                else if (contador == 12)
                {   
                    Structs::Pointerblock bloqueApuntdadorSimple;
                    cout << "contador2" << endl;
                }
                else if (contador == 13)
                {
                    cout << "contador3" << endl;
                }
                else if (contador == 14)
                {
                    cout << "contador4" << endl;
                }
            }
            contador++;
        }
        return contenido;
    }
    catch (const std::exception &e)
    {
        scan.errores("Obtencion contenido de file blocks", e.what());
        string a;
        return a;
    }
}

Structs::Inodes FileManager::obtenerInodo(Mount mount, string pathInterno, string id, string path, Structs::Inodes inodo)
{
    try
    {
        string path;
        Structs::Partition partition = mount.getmount(id, &path);
        Structs::Superblock super;
        FILE *rfile = fopen(path.c_str(), "rb");
        fseek(rfile, partition.part_start, SEEK_SET);
        fread(&super, sizeof(Structs::Superblock), 1, rfile);
        fclose(rfile);

        int contador = 0;
        pathInterno.erase(0, 1);
        string carpetaInterna = pathInterno.substr(0, pathInterno.find("/"));
        cout << carpetaInterna << endl;
        pathInterno.erase(0, carpetaInterna.length());
        cout << pathInterno << endl;
        Structs::Inodes inodoApuntador;
        // for(string s: carpetasInternas){
        //     cout << s<<endl;
        // }
        for (int apuntador : inodo.i_block)
        {

            cout << "contador" << endl;
            cout << contador << endl;
            cout << "apuntador" << endl;
            cout << apuntador << endl;
            if (apuntador != -1)
            {
                if (contador < 12)
                {
                    Structs::Folderblock bloquefile;
                    FILE *rfile = fopen(path.c_str(), "rb");
                    fseek(rfile, super.s_block_start + apuntador * sizeof(Structs::Folderblock), SEEK_SET);
                    fread(&bloquefile, sizeof(Structs::Folderblock), 1, rfile);
                    fclose(rfile);
                    inodoApuntador = obtenerInodoFileBlock(mount, pathInterno, id, path, bloquefile,carpetaInterna);
                    cout << "contadorInodo" << endl;
                }
                else if (contador == 12)
                {
                    cout << "contador2" << endl;
                }
                else if (contador == 13)
                {
                    cout << "contador3" << endl;
                }
                else if (contador == 14)
                {
                    cout << "contador4" << endl;
                }
            }
            contador++;
        }
        return inodoApuntador;
    }
    catch (const std::exception &e)
    {
        scan.errores("Obtencion Inodo", e.what());
        Structs::Inodes error;
        return error;
    }
}

Structs::Inodes FileManager::obtenerInodoFileBlock(Mount mount, string pathInterno, string id, string path, Structs::Folderblock fileblock,string carpetaInterna)
{
    try
    {
        string path;
        Structs::Partition partition = mount.getmount(id, &path);

        Structs::Superblock super;
        FILE *rfile = fopen(path.c_str(), "rb");
        fseek(rfile, partition.part_start, SEEK_SET);
        fread(&super, sizeof(Structs::Superblock), 1, rfile);

        Structs::Inodes error;
        cout<<"carpetaInterna" <<endl;
        cout<<carpetaInterna <<endl;
        cout<< pathInterno<<endl;
        if (fileblock.b_content[2].b_inodo != -1)
        {
            if ((fileblock.b_content[2].b_name ==carpetaInterna)&&("" == pathInterno))
            {   
                cout<< "fileblock.b_content[2].b_inodo"<<endl;
                cout<< fileblock.b_content[2].b_inodo<<endl;
                Structs::Inodes inodoEnFileBlock;
                fseek(rfile, super.s_inode_start + sizeof(Structs::Inodes) * fileblock.b_content[2].b_inodo, SEEK_SET);
                fread(&inodoEnFileBlock, sizeof(Structs::Inodes), 1, rfile);
                fclose(rfile);
                cout<< inodoEnFileBlock.i_uid<<endl;
                return inodoEnFileBlock;
            }else if (fileblock.b_content[2].b_name == carpetaInterna)
            {
                Structs::Inodes inodoEnFileBlock;
                fseek(rfile, super.s_inode_start + sizeof(Structs::Inodes) * fileblock.b_content[2].b_inodo, SEEK_SET);
                fread(&inodoEnFileBlock, sizeof(Structs::Inodes), 1, rfile);
                fclose(rfile);
                return obtenerInodo(mount, pathInterno, id, path, inodoEnFileBlock);
            }
        }
        if (fileblock.b_content[3].b_inodo != -1)
        {
            if (fileblock.b_content[3].b_name == carpetaInterna &&("" == pathInterno))
            {
                Structs::Inodes inodoEnFileBlock;
                fseek(rfile, super.s_inode_start + sizeof(Structs::Inodes) * fileblock.b_content[3].b_inodo, SEEK_SET);
                fread(&inodoEnFileBlock, sizeof(Structs::Inodes), 1, rfile);
                fclose(rfile);
                return inodoEnFileBlock;
            }else if (fileblock.b_content[3].b_name == carpetaInterna)
            {
                Structs::Inodes inodoEnFileBlock;
                fseek(rfile, super.s_inode_start + sizeof(Structs::Inodes) * fileblock.b_content[3].b_inodo, SEEK_SET);
                fread(&inodoEnFileBlock, sizeof(Structs::Inodes), 1, rfile);
                fclose(rfile);
                return obtenerInodo(mount, pathInterno, id, path, inodoEnFileBlock);
            }
            
        }
        return error;
    }
    catch (const std::exception &e)
    {
        scan.errores("Obtencion Inodo", e.what());
        Structs::Inodes error;
        return error;
    }
}

Structs::Inodes FileManager::escribirContenidoFileBlock(Mount mount, string pathInterno, string id, string path, Structs::Inodes inodo, string content)
{
    try
    {
        string path;
        Structs::Partition partition = mount.getmount(id, &path);
        Structs::Superblock super;
        FILE *rfile = fopen(path.c_str(), "rb");
        fseek(rfile, partition.part_start, SEEK_SET);
        fread(&super, sizeof(Structs::Superblock), 1, rfile);
        fclose(rfile);

        int contador = 0;
        pathInterno.erase(0, 1);
        string carpetaInterna = pathInterno.substr(0, pathInterno.find("/"));
        cout << carpetaInterna << endl;
        pathInterno.erase(0, carpetaInterna.length());
        cout << pathInterno << endl;

        for (int apuntador : inodo.i_block)
        {
            cout << "contador" << endl;
            cout << contador << endl;
            cout << "apuntador" << endl;
            cout << apuntador << endl;
            if (apuntador != -1)
            {
                if (contador < 12)
                {
                    Structs:: Inodes InodeHijo;
                    Structs::Folderblock bloqueFolder;
                    leerBloqueFolder(&bloqueFolder,path,apuntador,super);
                    InodeHijo = buscarBloqueFileInode(mount,pathInterno,id,path,bloqueFolder,carpetaInterna,content);
                    if (InodeHijo.i_uid !=-1)
                    {
                        cout << "contadorInodo" << endl;
                        inodo.i_size = inodo.i_size + InodeHijo.i_size;
                    }
                    
                }
                else if (contador == 12)
                {
                    cout << "contador2" << endl;
                }
                else if (contador == 13)
                {
                    cout << "contador3" << endl;
                }
                else if (contador == 14)
                {
                    cout << "contador4" << endl;
                }
            }
            contador++;
        }
        return inodo;
    }
    catch(const std::exception& e)
    {
        scan.errores("Escritura en file blocks", e.what());
        return inodo;
    }
    
}

Structs::Inodes FileManager::buscarBloqueFileInode(Mount mount, string pathInterno, string id, string path, Structs::Folderblock bloqueFolder,string carpetaInterna, string content) {
    try
    {
        string path;
        Structs::Partition partition = mount.getmount(id, &path);

        Structs::Superblock super;
        FILE *rfile = fopen(path.c_str(), "rb");
        fseek(rfile, partition.part_start, SEEK_SET);
        fread(&super, sizeof(Structs::Superblock), 1, rfile);
        fclose(rfile);
        Structs::Inodes error;
        cout<<"carpetaInterna" <<endl;
        cout<< carpetaInterna <<endl;
        cout<< pathInterno<<endl;
        if (bloqueFolder.b_content[2].b_inodo != -1)
        {
            if ((bloqueFolder.b_content[2].b_name ==carpetaInterna)&&("" == pathInterno))
            {   
                cout<< "bloqueFolder.b_content[2].b_inodo aqui"<<endl;
                cout<< bloqueFolder.b_content[2].b_inodo<<endl;
                Structs::Inodes inodoEnFileBlock;
                leerInodes(&inodoEnFileBlock,path,bloqueFolder.b_content[2].b_inodo,super);
                cout<< inodoEnFileBlock.i_uid<<endl;
                inodoEnFileBlock = escribirInodeFile(mount, pathInterno, id, path, inodoEnFileBlock,content);
                escribirInodos(&inodoEnFileBlock,path,bloqueFolder.b_content[2].b_inodo,super);
                return inodoEnFileBlock;
            }else if (bloqueFolder.b_content[2].b_name == carpetaInterna)
            {
                Structs::Inodes inodoEnFileBlock;
                FILE *rfile = fopen(path.c_str(), "rb");
                fseek(rfile, super.s_inode_start + sizeof(Structs::Inodes) * bloqueFolder.b_content[2].b_inodo, SEEK_SET);
                fread(&inodoEnFileBlock, sizeof(Structs::Inodes), 1, rfile);
                fclose(rfile);
                inodoEnFileBlock  = escribirContenidoFileBlock(mount, pathInterno, id, path, inodoEnFileBlock,content);
                escribirInodos(&inodoEnFileBlock,path,bloqueFolder.b_content[2].b_inodo,super);
                return inodoEnFileBlock;
            }
        }
        if (bloqueFolder.b_content[3].b_inodo != -1)
        {
            if (bloqueFolder.b_content[3].b_name == carpetaInterna &&("" == pathInterno))
            {
                Structs::Inodes inodoEnFileBlock;
                FILE *rfile = fopen(path.c_str(), "rb");
                fseek(rfile, super.s_inode_start + sizeof(Structs::Inodes) * bloqueFolder.b_content[3].b_inodo, SEEK_SET);
                fread(&inodoEnFileBlock, sizeof(Structs::Inodes), 1, rfile);
                fclose(rfile);
                //3
                return inodoEnFileBlock;
            }else if (bloqueFolder.b_content[3].b_name == carpetaInterna)
            {
                Structs::Inodes inodoEnFileBlock;
                FILE *rfile = fopen(path.c_str(), "rb");
                fseek(rfile, super.s_inode_start + sizeof(Structs::Inodes) * bloqueFolder.b_content[3].b_inodo, SEEK_SET);
                fread(&inodoEnFileBlock, sizeof(Structs::Inodes), 1, rfile);
                fclose(rfile);
                return escribirContenidoFileBlock(mount, pathInterno, id, path, inodoEnFileBlock,content);
            }
            
        }
        return error;
    }
    catch(const std::exception& e)
    {
        scan.errores("buscarBloqueFileInode", e.what());
        Structs::Inodes error;
        return error;
    }
    
}

Structs::Inodes FileManager::escribirInodeFile(Mount mount,string pathInterno,string id,string path, Structs::Inodes inodo, string content){
    try
    {
        string path;
        Structs::Partition partition = mount.getmount(id, &path);
        Structs::Superblock super;
        FILE *rfile = fopen(path.c_str(), "rb");
        fseek(rfile, partition.part_start, SEEK_SET);
        fread(&super, sizeof(Structs::Superblock), 1, rfile);
        fclose(rfile);
        if(inodo.i_uid==-1){
            throw runtime_error("no existe direccion");
        }

        int contador = 0;
        int tamañoContent = content.length();
        int tamaño_nuevo = 0;
        vector <string> vectorContenido;
        
        for (int apuntador : inodo.i_block)
        {
            cout << "contador" << endl;
            cout << contador << endl;
            cout << "apuntador" << endl;
            cout << apuntador << endl;
            cout << content << endl;
            if(content!=""){
                tamaño_nuevo += sizeof(Structs::Fileblock);
                if (contador < 12)
                {   
                    if(apuntador == -1)
                    {
                        int apuntadorNuevo = getfree(super,path,"BB");
                        cout << apuntadorNuevo << endl;
                        Structs::Fileblock bloquefile;
                        string escritoContent = content.substr(0,sizeof(Structs::Fileblock));
                        cout << (escritoContent.length()) << endl;
                        content.erase(0,sizeof(Structs::Fileblock));
                        strcpy(bloquefile.b_content, escritoContent.c_str());
                        cout << bloquefile.b_content << endl;
                        escribirBloqueArchivos(&bloquefile,path,apuntadorNuevo,super);
                        cout << "contadorInodo -1" << endl;
                        inodo.i_block[contador] = apuntadorNuevo;
                        putTaken(super,path,"BB",apuntadorNuevo);
                    }else{
                        Structs::Fileblock bloquefile;
                        leerBloqueArchivos(&bloquefile,path,apuntador,super);
                        string escritoContent = content.substr(0,sizeof(Structs::Fileblock));
                        content.erase(0,sizeof(Structs::Fileblock));
                        strcpy(bloquefile.b_content, escritoContent.c_str());
                        cout << bloquefile.b_content << endl;
                        escribirBloqueArchivos(&bloquefile,path,apuntador,super);
                        cout << "contadorInodo DISTINTO" << endl;
                    }
                    cout << "contadorInodo" << endl;
                }
                else if (contador == 12)
                {
                    cout << "contador2" << endl;
                }
                else if (contador == 13)
                {
                    cout << "contador3" << endl;
                }
                else if (contador == 14)
                {
                    cout << "contador4" << endl;
                }
            }
            contador++;
        }
        inodo.i_size = tamaño_nuevo;
        return inodo;
    }
    catch(const std::exception& e)
    {
        scan.errores("escribirInodeFile", e.what());
        return inodo;
    }
}

Structs::Inodes FileManager::editarArchivo(Mount mount, string pathInterno, string id, string path, Structs::Inodes inodo, string content)
{
    try
    {
        string path;
        Structs::Partition partition = mount.getmount(id, &path);
        Structs::Superblock super;
        FILE *rfile = fopen(path.c_str(), "rb");
        fseek(rfile, partition.part_start, SEEK_SET);
        fread(&super, sizeof(Structs::Superblock), 1, rfile);
        fclose(rfile);

        int contador = 0;
        pathInterno.erase(0, 1);
        string carpetaInterna = pathInterno.substr(0, pathInterno.find("/"));
        cout << carpetaInterna << endl;
        pathInterno.erase(0, carpetaInterna.length());
        cout << pathInterno << endl;

        for (int apuntador : inodo.i_block)
        {
            cout << "contador" << endl;
            cout << contador << endl;
            cout << "apuntador" << endl;
            cout << apuntador << endl;
            if (apuntador != -1)
            {
                if (contador < 12)
                {
                    Structs:: Inodes InodeHijo;
                    Structs::Folderblock bloqueFolder;
                    leerBloqueFolder(&bloqueFolder,path,apuntador,super);
                    buscarBloqueFileInodeBorrar(mount,pathInterno,id,path,bloqueFolder,carpetaInterna,content);
                }
                else if (contador == 12)
                {
                    cout << "contador2" << endl;
                }
                else if (contador == 13)
                {
                    cout << "contador3" << endl;
                }
                else if (contador == 14)
                {
                    cout << "contador4" << endl;
                }
            }
            contador++;
        }
        return inodo;
    }
    catch(const std::exception& e)
    {
        scan.errores("Escritura en file blocks", e.what());
        return inodo;
    }
    
}

Structs::Inodes FileManager::buscarBloqueFileInodeBorrar(Mount mount, string pathInterno, string id, string path, Structs::Folderblock bloqueFolder,string carpetaInterna, string content){
    try{
        string path;
        Structs::Partition partition = mount.getmount(id, &path);

        Structs::Superblock super;
        FILE *rfile = fopen(path.c_str(), "rb");
        fseek(rfile, partition.part_start, SEEK_SET);
        fread(&super, sizeof(Structs::Superblock), 1, rfile);
        fclose(rfile);
        Structs::Inodes error;
        cout<<"carpetaInterna" <<endl;
        cout<< carpetaInterna <<endl;
        cout<< pathInterno<<endl;
        if (bloqueFolder.b_content[2].b_inodo != -1)
        {
            if ((bloqueFolder.b_content[2].b_name ==carpetaInterna)&&("" == pathInterno))
            {   
                cout<< "bloqueFolder.b_content[2].b_inodo aqui"<<endl;
                cout<< bloqueFolder.b_content[2].b_inodo<<endl;
                Structs::Inodes inodoEnFileBlock;
                leerInodes(&inodoEnFileBlock,path,bloqueFolder.b_content[2].b_inodo,super);
                cout<< inodoEnFileBlock.i_uid<<endl;

                inodoEnFileBlock = editarInodeBloqueFile(mount, pathInterno, id, path, inodoEnFileBlock,content);
                escribirInodos(&inodoEnFileBlock,path,bloqueFolder.b_content[2].b_inodo,super);
                return inodoEnFileBlock;

            }else if (bloqueFolder.b_content[2].b_name == carpetaInterna)
            {
                Structs::Inodes inodoEnFileBlock;
                FILE *rfile = fopen(path.c_str(), "rb");
                fseek(rfile, super.s_inode_start + sizeof(Structs::Inodes) * bloqueFolder.b_content[2].b_inodo, SEEK_SET);
                fread(&inodoEnFileBlock, sizeof(Structs::Inodes), 1, rfile);
                fclose(rfile);
                inodoEnFileBlock  = editarArchivo(mount, pathInterno, id, path, inodoEnFileBlock,content);
                escribirInodos(&inodoEnFileBlock,path,bloqueFolder.b_content[2].b_inodo,super);
                return inodoEnFileBlock;
            }
        }
        if (bloqueFolder.b_content[3].b_inodo != -1)
        {
            if (bloqueFolder.b_content[3].b_name == carpetaInterna &&("" == pathInterno))
            {
                Structs::Inodes inodoEnFileBlock;
                FILE *rfile = fopen(path.c_str(), "rb");
                fseek(rfile, super.s_inode_start + sizeof(Structs::Inodes) * bloqueFolder.b_content[3].b_inodo, SEEK_SET);
                fread(&inodoEnFileBlock, sizeof(Structs::Inodes), 1, rfile);
                fclose(rfile);
                //3
                return inodoEnFileBlock;
            }else if (bloqueFolder.b_content[3].b_name == carpetaInterna)
            {
                Structs::Inodes inodoEnFileBlock;
                FILE *rfile = fopen(path.c_str(), "rb");
                fseek(rfile, super.s_inode_start + sizeof(Structs::Inodes) * bloqueFolder.b_content[3].b_inodo, SEEK_SET);
                fread(&inodoEnFileBlock, sizeof(Structs::Inodes), 1, rfile);
                fclose(rfile);
                return escribirContenidoFileBlock(mount, pathInterno, id, path, inodoEnFileBlock,content);
            }
            
        }
        return error;
    }
    catch(const std::exception& e)
    {
        scan.errores("buscarBloqueFileInodeBorrar", e.what());
        Structs::Inodes error;
        return error;
    }
}

Structs::Inodes FileManager::editarInodeBloqueFile(Mount mount, string pathInterno, string id, string path,  Structs::Inodes inodo,string content){
try
    {
        string path;
        Structs::Partition partition = mount.getmount(id, &path);
        Structs::Superblock super;
        FILE *rfile = fopen(path.c_str(), "rb");
        fseek(rfile, partition.part_start, SEEK_SET);
        fread(&super, sizeof(Structs::Superblock), 1, rfile);
        fclose(rfile);
        if(inodo.i_uid==-1){
            throw runtime_error("no existe direccion");
        }

        int contador = 0;
        
        for (int apuntador : inodo.i_block)
        {
            cout << "contador borrado" << endl;
            cout << contador << endl;
            cout << "apuntador" << endl;
            cout << apuntador << endl;
            cout << content << endl;
            if(apuntador!=-1){
                if (contador < 12)
                {   
                    Structs::Fileblock bloquefile;
                    leerBloqueArchivos(&bloquefile,path,apuntador,super);
                    cout << bloquefile.b_content ;
                    cout << "bloquefile.b_content"<<endl ;
                    if(bloquefile.b_content != "")
                    {
                        borrarStructBloque(sizeof(Structs::Fileblock) ,path,apuntador,super);
                        borrarBM(super,path,"BB",apuntador);
                        inodo.i_block[contador] = -1;
                    }
                }
                else if (contador == 12)
                {
                    cout << "contador2" << endl;
                }
                else if (contador == 13)
                {
                    cout << "contador3" << endl;
                }
                else if (contador == 14)
                {
                    cout << "contador4" << endl;
                }
            }
            contador++;
        }
        return inodo;
    }
    catch(const std::exception& e)
    {
        scan.errores("escribirInodeFile", e.what());
        return inodo;
    }
}