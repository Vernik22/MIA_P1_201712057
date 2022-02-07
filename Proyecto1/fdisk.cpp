#include "fdisk.h"
#include <iostream>
#include <string.h>
#include <sys/stat.h>

fdisk::fdisk()
{
    //ctor
}

fdisk::~fdisk()
{
    //dtor
}

void fdisk::ejecutarComandoFdisk(fdisk *disco)
{
    cout<<"************Ejecutar MKDISK************\n"<<endl;
    int startPart = sizeof(MBR);
    int tamanoParticion;
    Particion particionTemp;
    MBR mbrTemp; //mbr temporal del disco al que se quiere formatear

    if(disco->getUnit()=="b"||disco->getUnit()=="B")  //si vine en bytes
    {
        tamanoParticion = disco->getSize() ;
    }
    else if(disco->getUnit()=="k"||disco->getUnit()=="K")   //si se desea en kilobytes
    {
        tamanoParticion = disco->getSize() *1024;
    }
    else if(disco->getUnit()=="m"||disco->getUnit()=="M")   //si es en megabytes
    {
        tamanoParticion = disco->getSize() * 1024 * 1024;
    }

    string ajuste = disco->getFit();
    std::for_each(ajuste.begin(), ajuste.end(), [](char &c) //convierte el texto a minisculas
    {
        c = ::tolower(c);
    });

    if(ajuste == "bf")
    {
        particionTemp.part_fit = 'B';
    }
    else if(ajuste == "ff")
    {
        particionTemp.part_fit = 'F';
    }
    else if(ajuste == "wf")
    {
        particionTemp.part_fit = 'W';
    }

    //Ver el tipo de particion a crear si es Primaria, Extendida o Logica
    if(disco->getType()=="p"||disco->getType()=="P")
    {
        FILE *arch;
        arch = fopen(disco->getPath().c_str(), "rb+"); // se abre el archivo del disco en modo lectura
        fseek(arch,0,SEEK_SET);
        fread(&mbrTemp, sizeof(MBR),1,arch); //mbrTemp guarda el MBR del disco a formatear
        if(hayEspacio(tamanoParticion,mbrTemp.mbr_tamano)){
            //si hay espacio suficiente en el disco entra
            //verificar si hay particiones disponibles

        }
    }
    else if(disco->getType()=="e"||disco->getType()=="E")
    {

    }
    else if(disco->getType()=="l"||disco->getType()=="L")
    {
    }
}

void fdisk::eliminarParticion(string path,string nombre, string tipoDelete)
{
}

bool fdisk::hayEspacio(int tamaParticion, int tamanoDisco)
{
    return true;
}

void fdisk::imprimirDatosDisco(string path)
{
}

