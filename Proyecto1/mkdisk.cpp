#include "mkdisk.h"
#include <iostream>
#include <time.h>
#include <string.h>
#include <cstdlib>

using namespace std;
mkdisk::mkdisk()
{
    //ctor
}

mkdisk::~mkdisk()
{
    //dtor
}

void mkdisk::ejecutarComandoMkdisk(mkdisk *disco)
{
    cout<<"************Ejecutar MKDISK************\n"<<endl;
    MBR mbrDisco;
    FILE *arch;
    arch=fopen(disco->getPath().c_str(),"wb");
    char buffer[1024];

    //simular un kb
    if(disco->getUnit()=="k"||disco->getUnit()=="K")
    {
        //se asigna el tamaño a mbr en bytes
        mbrDisco.mbr_tamano = disco->getSize() * 1024;
        //se llena la variable buffer de ceros para que no este en null
        for(int i = 0; i<1024; i++)
        {
            buffer[i]='\0';
        }
        //creo el disco y lo lleno con el buffer para que quede lleno de 0
        for(int i=0; i<disco->getSize(); i++)
        {
            fwrite(&buffer,1024,1,arch);
        }
        fclose(arch);
    }
    else if(disco->getUnit()=="m"||disco->getUnit()=="M")   // si son megas entra aqui
    {
        //se asigna el tamaño al mbr en bytes
        mbrDisco.mbr_tamano = disco->getSize()*1024*1024;
        //se llena la variable buffer de ceros para que no este en null
        for(int i = 0; i<1024; i++)
        {
            buffer[i]='\0';
        }
        //creo el disco y lo lleno con el buffer para que quede lleno de 0
        for(int i=0; i<(disco->getSize()*1024); i++)
        {
            fwrite(&buffer,1024,1,arch);
        }
        fclose(arch);
    }

    //hora actual para el mbr
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    string fecha = asctime(timeinfo);
    _mTime fcreacion;
    strcpy(fcreacion.mbr_fecha_creacion,fecha.c_str());
    mbrDisco.mbr_fecha_creacion = fcreacion;
    srand (time(NULL)); // para crear numeros aleatorios de verdad
    mbrDisco.mbr_dsk_signature = (rand()%200); //numero random para darle etiqueta al disco
    //si el fit viene vacio se pone el primer ajuste
    if(disco->getFit().empty()==true){
        mbrDisco.dsk_fit = 'F'; // se toma el primer ajuste
    }else {
        //se asigna el fit que viene
        if(disco->getFit()=="W"){
            mbrDisco.dsk_fit='W';
        }else if(disco->getFit()=="B"){
            mbrDisco.dsk_fit='B';
        }else{
            mbrDisco.dsk_fit='F';
        }
    }

    Particion vacia; //creo una particion vacia para asignarala al mbr
    vacia.part_fit='-';
    vacia.part_name[0]='\0';
    vacia.part_size=-1;
    vacia.part_start=-1;
    vacia.part_status ='0';//status inactivo
    vacia.part_type='-';

    //asigno esta particion a las disponibles en el mbr
    mbrDisco.mbr_partition_1 = vacia;
    mbrDisco.mbr_partition_2 = vacia;
    mbrDisco.mbr_partition_3 = vacia;
    mbrDisco.mbr_partition_4 = vacia;

    //Agrego el mbr creado al disco que creé
    //se abre en modo de escritura mixto que permite actualizar un fichero sin borrar el contenido anterior
    arch = fopen(disco->getPath().c_str(),"rb+");
    if(arch != NULL){
        fseek(arch,0,SEEK_SET);
        fwrite(&mbrDisco,sizeof(MBR),1,arch);
        cout<<"Disco creado correctametne...!!"<<endl;
    }else{
        cout<<"Error, no se puede acceder al disco, MBR no creado"<<endl;
    }
    fclose(arch);
}
