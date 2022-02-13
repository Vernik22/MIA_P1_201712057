#include "mkfs.h"
#include <iostream>
#include <string.h>
#include <cstdlib>

mkfs::mkfs()
{
    //ctor
}

mkfs::~mkfs()
{
    //dtor
}

void mkfs::ejecutarComandoMkfs(mkfs *fs, mount paMoun[])
{
    cout<<"\n************Ejecutar MKFS************\n"<<endl;

    if(existeIdMount(fs->getId(),paMoun))
    {
        time_t rawtime;
        struct tm *timeinfo;
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        string fecha = asctime(timeinfo);
        _mTime fcreacion;
        strcpy(fcreacion.mbr_fecha_creacion,fecha.c_str());
        MBR mbrTemp;
        FILE *arch;
        string pathD = "";
        string nombreParticion = "";
        for(int i =0; i<100; i++)
        {
            if(fs->getId() == paMoun[i].getId())
            {
                pathD = paMoun[i].getPath();
                nombreParticion = paMoun[i].getName();
                break;

            }

        }
        arch = fopen(pathD.c_str(),"rb+");
        if(arch != NULL)
        {
            fseek(arch,0,SEEK_SET);
            fread(&mbrTemp,sizeof(MBR),1,arch);
            int tamParticion, iniPart;
            returnDatosPart(mbrTemp, pathD,nombreParticion,tamParticion, iniPart);

            SupB superBloque;
            Inodo carpRaiz;
            BCarpeta bCarpetaRaiz;
            Inodo archivUsers;
            Content contenidoUsuarios; //nombre e inodo del BCarpeta
            //BApun apuntadorCarpeta;
            BArchivo archivoUsersTxt; //contenido del archivo users.txt
            Bitacora bitacora;


            int numero_bloques_por_inodo = 12 + 16 + (16*16) + (16*16*16);
            int capacidad_carpeta = numero_bloques_por_inodo * 4 - 2;
            int capacidad_archivo = numero_bloques_por_inodo * 64;

            //tamaño_particion = sizeof(SupB)+ n +n*sizeof(Bitacora) + 3*n+n*sizeof(Inodo) + 3*n*sizeof(BArchivo) numero_estructuras =floor(n);

            //No estructuras

            //Bitmaps

        }
        else
        {
            cout<<"Error, no se puede acceder al disco"<<endl;
        }




    }
    else
    {
        cout<<"ERROR: El ID de la particion no existe, o no esta montada"<<endl;
    }

}

bool mkfs::existeIdMount(string idB,mount paMoun[])
{
    for(int i =0; i<100; i++)
    {
        if(idB == paMoun[i].getId())
        {
            return true;

        }

    }
    return false;
}

void mkfs::returnDatosPart(MBR mbrTemp, string pathD,string nombrePart,int &tamPart, int &iniPart)
{
    Particion parts[4];
    parts[0] = mbrTemp.mbr_partition_1;
    parts[1] = mbrTemp.mbr_partition_2;
    parts[2] = mbrTemp.mbr_partition_3;
    parts[3] = mbrTemp.mbr_partition_4;
    bool noEncontrada = true;

    if(mbrTemp.mbr_partition_1.part_name == nombrePart|| mbrTemp.mbr_partition_2.part_name == nombrePart|| mbrTemp.mbr_partition_3.part_name == nombrePart|| mbrTemp.mbr_partition_4.part_name == nombrePart)
    {
        for(int i = 0; i<4; i++)
        {
            if(parts[i].part_name == nombrePart)
            {
                tamPart = parts[i].part_size;
                iniPart = parts[i].part_start;
                noEncontrada = false;
                break;
            }
        }
    }
    if(noEncontrada)
    {
        EBR ebrExTemp;
        FILE *arch;
        arch = fopen(pathD.c_str(), "rb+");
        for(int i=0; i<4; i++) // encuentra la particion extendida para revisar dentro de sus particiones logicas
        {
            if(parts[i].part_type == 'E')
            {
                fseek(arch,parts[i].part_start,SEEK_SET);
                fread(&ebrExTemp, sizeof(EBR),1,arch); //partExTemp guarda el EBR de la particion E
                break;
            }
        }
        while(ebrExTemp.part_next != -1) //revisa hasta la ultima particion logica o hasta que la encuentre
        {
            if(ebrExTemp.part_name == nombrePart)
            {
                tamPart = ebrExTemp.part_size;
                iniPart = ebrExTemp.part_start;
                break;
            }
            fseek(arch, ebrExTemp.part_next, SEEK_SET);
            fread(&ebrExTemp, sizeof(EBR), 1, arch);
        }

        fclose(arch);
    }
}
