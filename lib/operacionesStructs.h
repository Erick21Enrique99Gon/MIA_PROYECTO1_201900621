#ifndef OPERACCIONESSTRUCTS_H
#define OPERACCIONESSTRUCTS_H

#include <string>
#include <vector>
#include "../lib/operacionesStructs.h"
#include "../lib/shared.h"
#include "../lib/scanner.h"
#include "../lib/structs.h"
#include "../lib/mount.h"

using namespace std;

class operacionesStructs
{
private:
    scanner scan;
    Shared shared;
public:
    operacionesStructs();
    string obtenerContenidoFileBlocks(Mount mount,string pathInterno,string id,string path, Structs::Inodes inodo);
    Structs::Inodes obtenerInodo(Mount mount,string pathInterno,string id,string path, Structs::Inodes inodo);
};
#endif