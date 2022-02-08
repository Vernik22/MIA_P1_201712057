#include "fdisk.h"
#include <iostream>
#include <string.h>
#include <sys/stat.h>
#include <algorithm>

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
    cout<<"************Ejecutar FDISK************\n"<<endl;
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
        tamanoParticion = disco->getSize() * 1024;
    }
    else if(disco->getUnit()=="m"||disco->getUnit()=="M")   //si es en megabytes
    {
        tamanoParticion = disco->getSize() * 1024 * 1024;
    }

    string ajuste = disco->getFit();
    for_each(ajuste.begin(), ajuste.end(), [](char &c) //convierte el texto a minisculas
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
        char Dfit = mbrTemp.dsk_fit;
        if(hayEspacio(tamanoParticion,mbrTemp.mbr_tamano))
        {
            //si hay espacio suficiente en el disco entra
            //verificar si hay particiones disponibles
            //caso base si no hay ninguna particion ocupada
            if(mbrTemp.mbr_partition_1.part_status == '0'&& mbrTemp.mbr_partition_2.part_status == '0'&& mbrTemp.mbr_partition_3.part_status == '0'&& mbrTemp.mbr_partition_4.part_status == '0')
            {
                //seteando los valores que llevara la nueva particion
                particionTemp.part_status = '1';//particion ocupada
                particionTemp.part_type = 'P';
                strcpy(particionTemp.part_name,disco->getName().c_str());
                particionTemp.part_size= tamanoParticion;
                particionTemp.part_start=startPart;
                //copiando particion creada
                mbrTemp.mbr_partition_1 =particionTemp;
                //toca modificar el mbr original en el archivo por el nuevo mbr con los datos de la particion creada
                fseek(arch,0,SEEK_SET);
                fwrite(&mbrTemp,sizeof(MBR),1,arch); //se sobreescribe el mbr
                fclose(arch);
                cout<<"Se Creo la particion Correctamente"<<endl;
                imprimirDatosDisco(disco->getPath());
            }
            else  if(mbrTemp.mbr_partition_1.part_status == '0') //la particion1 esta disponible
            {
                //verifica el tipo de ajuste del disco
                if(Dfit=='F') //Primer ajuste
                {   int esp1 = 0;
                    int startEsp1 = 0;
                    int esp2 = 0;
                    int startEsp2 = 0;
                    int esp3 = 0;
                    int startEsp3 = 0;
                    //verificar si hay mas particiones ocupadas
                    if(mbrTemp.mbr_partition_2.part_status == '1')
                    {
                        startEsp1=mbrTemp.mbr_partition_2.part_start;
                        esp1=espacioLibre(startPart,mbrTemp.mbr_partition_2.part_start);
                    }
                    if(mbrTemp.mbr_partition_3.part_status == '1')
                    {
                        startEsp2=mbrTemp.mbr_partition_3.part_start;
                        esp2=espacioLibre(startPart,mbrTemp.mbr_partition_3.part_start);
                    }
                    if(mbrTemp.mbr_partition_4.part_status == '1')
                    {
                        startEsp3=mbrTemp.mbr_partition_4.part_start;
                        esp3=espacioLibre(startPart,mbrTemp.mbr_partition_4.part_start);
                    }
                }
                else if (Dfit=='W')  //Peor ajuste
                {

                }
                else   //Aqui quiere decir que tiene el B ajuste (Mejor ajuste)
                {

                }

            }/*
            else if(mbrTemp.mbr_partition_2.part_status == '0')
            {

            }
            else if(mbrTemp.mbr_partition_3.part_status == '0')
            {

            }
            else if(mbrTemp.mbr_partition_4.part_status == '0')
            {

            }*/
            else
            {
                cout<<"No hay particiones disponibles"<<endl;
            }

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
void fdisk::agregarEspacioParticion(){
}

bool fdisk::hayEspacio(int tamaParticion, int tamanoDisco)
{
    if(tamaParticion>tamanoDisco||tamaParticion<0)
    {
        return false;
    }
    return true;
}

int fdisk::mejorAjuste();
        int peorAjuste();
        int primerAjuste();
int fdisk::espacioLibre(int inicio, int fin){
    return fin - inicio;
}


void fdisk::imprimirDatosDisco(string path)
{
    FILE *arch;
    arch = fopen(path.c_str(), "rb+");
    MBR mbr;
    fseek(arch, 0, SEEK_SET);
    fread(&mbr, sizeof(MBR), 1, arch);
    fclose(arch);
    cout << "\n----------DATOS DEL DISCO-----\n";
    cout << "\nDISK NAME: " << mbr.mbr_dsk_signature;
    cout << "\nDISK SIZE: " << mbr.mbr_tamano;
    cout << "\n\nPARTICION : " << 1;

    cout << "\nPARTICION STATUS : " << mbr.mbr_partition_1.part_status;
    cout << "\nPARTICION TYPE : " << mbr.mbr_partition_1.part_type;
    cout << "\nPARTICION FIT : " << mbr.mbr_partition_1.part_fit;
    cout << "\nPARTICION START : " << mbr.mbr_partition_1.part_start;
    cout << "\nPARTICION SIZE : " << mbr.mbr_partition_1.part_size;
    cout << "\nPARTICION NAME : " << mbr.mbr_partition_1.part_name<<endl;

    cout << "\n\nPARTICION : " << 2;
    cout << "\nPARTICION STATUS : " << mbr.mbr_partition_2.part_status;
    cout << "\nPARTICION TYPE : " << mbr.mbr_partition_2.part_type;
    cout << "\nPARTICION FIT : " << mbr.mbr_partition_2.part_fit;
    cout << "\nPARTICION START : " << mbr.mbr_partition_2.part_start;
    cout << "\nPARTICION SIZE : " << mbr.mbr_partition_2.part_size;
    cout << "\nPARTICION NAME : " << mbr.mbr_partition_2.part_name<<endl;

    cout << "\nPARTICION : " << 3;
    cout << "\nPARTICION STATUS : " << mbr.mbr_partition_3.part_status;
    cout << "\nPARTICION TYPE : " << mbr.mbr_partition_3.part_type;
    cout << "\nPARTICION FIT : " << mbr.mbr_partition_3.part_fit;
    cout << "\nPARTICION START : " << mbr.mbr_partition_3.part_start;
    cout << "\nPARTICION SIZE : " << mbr.mbr_partition_3.part_size;
    cout << "\nPARTICION NAME : " << mbr.mbr_partition_3.part_name<<endl;

    cout << "\nPARTICION : " << 4;
    cout << "\nPARTICION STATUS : " << mbr.mbr_partition_4.part_status;
    cout << "\nPARTICION TYPE : " << mbr.mbr_partition_4.part_type;
    cout << "\nPARTICION FIT : " << mbr.mbr_partition_4.part_fit;
    cout << "\nPARTICION START : " << mbr.mbr_partition_4.part_start;
    cout << "\nPARTICION SIZE : " << mbr.mbr_partition_4.part_size;
    cout << "\nPARTICION NAME : " << mbr.mbr_partition_4.part_name<<endl<<endl;
}


