#include "simlos.h"
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <fstream>
#include <stdio.h>
#include <iostream>

simlos::simlos()
{
    //ctor
}

simlos::~simlos()
{
    //dtor
}


void simlos::ejecutarLoss(simlos *mDir,mount paMoun[])
{
    cout<<"\n************Ejecutar Comando LOSS************\n"<<endl;


    string pathDisco;
    string nombrePart;
    for(int i =0; i<100; i++)
    {
        if(getDatosUsu().idPartMontada == paMoun[i].getId())
        {
            pathDisco = paMoun[i].getPath();
            nombrePart = paMoun[i].getName();
            break;

        }

    }
    modificarArchivo(pathDisco, nombrePart,mDir);


}
vector<string> simlos::split(string str, char pattern)
{
    int posInit = 0;
    int posFound = 0;
    string splitted;
    vector<string> results;

    while (posFound >= 0)
    {
        posFound = str.find(pattern, posInit);
        splitted = str.substr(posInit, posFound - posInit);
        posInit = posFound + 1;
        results.push_back(splitted);
    }

    return results;
}

string simlos::retFecha()
{

    time_t t = time(NULL);
    struct tm tiempoLocal = *localtime(&t);
    // El lugar en donde se pondrá la fecha y hora formateadas
    char fechaHora[70];
    // El formato. Mira más en https://en.cppreference.com/w/c/chrono/strftime
    char *formato = "%Y-%m-%d %H:%M:%S";
    // Intentar formatear
    int bytesEscritos =
        strftime(fechaHora, sizeof fechaHora, formato, &tiempoLocal);
    string fecha = fechaHora;
    return fecha;
}

void simlos::modificarArchivo(string pathDisco, string nombrePart, simlos *mDir)
{
    SupB superBloque;
    FILE *arch;
    arch = fopen(pathDisco.c_str(),"rb+");
    if(arch != NULL)
    {
        MBR mbrTemp;
        fseek(arch,0,SEEK_SET);
        fread(&mbrTemp,sizeof(MBR),1,arch);

        int tamParticion, iniPart;
        returnDatosPart(mbrTemp, pathDisco,nombrePart,tamParticion, iniPart);
        fseek(arch,iniPart,SEEK_SET);
        fread(&superBloque,sizeof(SupB),1,arch);


        Inodo inodoTemp;
        char buffer;
        char bufferI[sizeof(Inodo)];
        char bufferB[sizeof(BCarpeta)];

        if(superBloque.s_filesystem_type == 3){
            for(int i = 0; i<sizeof(Inodo); i++)
            {
                bufferI[i]='\0';
            }
            buffer = '\0';
            for(int i = 0; i<sizeof(BCarpeta); i++)
            {
                bufferB[i]='\0';
            }
            cout<<"Simulando falla en el bitmap de inodos "<<endl;

            fseek(arch,superBloque.s_bm_inode_start,SEEK_SET);
            for(int i = 0; i<superBloque.s_inodes_count; i++){
                fwrite(&buffer,sizeof(buffer),1,arch);
            }
            cout<<"Listo... "<<endl;

            cout<<"Simulando falla en el bitmap de bloques "<<endl;

            fseek(arch,superBloque.s_bm_block_start,SEEK_SET);
            for(int i = 0; i<superBloque.s_blocks_count; i++){
                fwrite(&buffer,sizeof(buffer),1,arch);
            }
            cout<<"Listo... "<<endl;

            cout<<"Simulando falla en los Inodos "<<endl;

            fseek(arch,superBloque.s_inode_start,SEEK_SET);
            for(int i = 0; i<superBloque.s_inodes_count; i++){
                fwrite(&bufferI,sizeof(Inodo),1,arch);
            }
            cout<<"Listo... "<<endl;

            cout<<"Simulando falla en los Bloques "<<endl;

            fseek(arch,superBloque.s_block_start,SEEK_SET);
            for(int i = 0; i<superBloque.s_blocks_count; i++){
                fwrite(&bufferB,sizeof(BCarpeta),1,arch);
            }
            cout<<"Listo... "<<endl;


        }else{

            cout<<"ERROR: El sistema de archivos es ext2 no se puede simular la falla "<<endl;
        }



    }else{
        cout<<"ERROR: particion no montada "<<endl;
    }
    fclose(arch);

}


void simlos::returnDatosPart(MBR mbrTemp, string pathD,string nombrePart,int &tamPart, int &iniPart)
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
