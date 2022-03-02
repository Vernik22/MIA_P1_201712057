#include "reporte.h"
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <cstdlib>
#include <string.h>

reporte::reporte()
{
    //ctor
}

reporte::~reporte()
{
    //dtor
}

void reporte::ejecutarRepBmInodo(reporte *rep,mount paMoun[])
{
    SupB superBloque;
    char buffer='0';
    cout<<"\n************Ejecutar REPORTE BmInodo************\n"<<endl;
    string pathDisco="";
    string partName="";
    if(existeIdMount(rep->getId(),paMoun, pathDisco, partName))
    {
        dirExist(rep);
        MBR mbrTemp;
        FILE *arch;
        arch = fopen(pathDisco.c_str(),"rb+");
        if(arch != NULL)
        {
            fseek(arch,0,SEEK_SET);
            fread(&mbrTemp,sizeof(MBR),1,arch);

            int tamParticion, iniPart;
            returnDatosPart(mbrTemp, pathDisco,partName,tamParticion, iniPart);

            fseek(arch,iniPart,SEEK_SET);
            fread(&superBloque,sizeof(SupB),1,arch);
            //fclose(arch);

            string filename(rep->getPath());
            fstream file_out;
            file_out.open(filename,std::ios_base::out);
            if(!file_out.is_open())
            {
                cout<<"no se pudo abrir el archivo"<<endl;
            }
            else
            {
                file_out<<" -------REPORTE BM INODOS------ \n"<<endl;
                string textoFinal ="";
                int contador = 1;
                for(int i =0; i<superBloque.s_inodes_count;i++){

                    if(contador<21){
                        fseek(arch,superBloque.s_bm_inode_start +(i*sizeof(buffer)),SEEK_SET);
                        fread(&buffer,sizeof(buffer),1,arch);
                        //textoFinal = textoFinal + std::to_string(buffer) + " " ;
                        textoFinal = textoFinal + buffer + " " ;
                        contador++;

                    }else{
                        file_out<<textoFinal<<endl;
                        contador = 1;
                        textoFinal="";
                    }

                }



                cout<<"Reporte creado correctamente"<<endl;

                //string comando = "dot -Tpng ima.txt -o grafo.png";
                //system(comando.c_str());
            }

            fclose(arch);


        }


    }
    else
    {
        cout<<"El id no existe, la particion no esta montada"<<endl;
    }



}

void reporte::ejecutarRepBmBloque(reporte *rep,mount paMoun[])
{
    SupB superBloque;
    char buffer='0';
    cout<<"\n************Ejecutar REPORTE BmBloques************\n"<<endl;
    string pathDisco="";
    string partName="";
    if(existeIdMount(rep->getId(),paMoun, pathDisco, partName))
    {
        dirExist(rep);
        MBR mbrTemp;
        FILE *arch;
        arch = fopen(pathDisco.c_str(),"rb+");
        if(arch != NULL)
        {
            fseek(arch,0,SEEK_SET);
            fread(&mbrTemp,sizeof(MBR),1,arch);

            int tamParticion, iniPart;
            returnDatosPart(mbrTemp, pathDisco,partName,tamParticion, iniPart);

            fseek(arch,iniPart,SEEK_SET);
            fread(&superBloque,sizeof(SupB),1,arch);
            //fclose(arch);

            string filename(rep->getPath());
            fstream file_out;
            file_out.open(filename,std::ios_base::out);
            if(!file_out.is_open())
            {
                cout<<"no se pudo abrir el archivo"<<endl;
            }
            else
            {
                file_out<<" -------REPORTE BM BLOQUES------ \n"<<endl;
                string textoFinal ="";
                int contador = 1;
                for(int i =0; i<superBloque.s_blocks_count;i++){

                    if(contador<21){
                        fseek(arch,superBloque.s_bm_block_start +(i*sizeof(buffer)),SEEK_SET);
                        fread(&buffer,sizeof(buffer),1,arch);
                        //textoFinal = textoFinal + std::to_string(buffer) + " " ;
                        textoFinal = textoFinal + buffer + " " ;
                        contador++;

                    }else{
                        file_out<<textoFinal<<endl;
                        contador = 1;
                        textoFinal="";
                    }

                }



                cout<<"Reporte creado correctamente"<<endl;

                //string comando = "dot -Tpng ima.txt -o grafo.png";
                //system(comando.c_str());
            }

            fclose(arch);


        }


    }
    else
    {
        cout<<"El id no existe, la particion no esta montada"<<endl;
    }



}

void reporte::ejecutarRepSuperB(reporte *rep,mount paMoun[])
{
    SupB superBloque;
    int buffer;
    cout<<"\n************Ejecutar REPORTE BmInodo************\n"<<endl;
    string pathDisco="";
    string partName="";
    if(existeIdMount(rep->getId(),paMoun, pathDisco, partName))
    {
        dirExist(rep);
        MBR mbrTemp;
        FILE *arch;
        arch = fopen(pathDisco.c_str(),"rb+");
        if(arch != NULL)
        {
            fseek(arch,0,SEEK_SET);
            fread(&mbrTemp,sizeof(MBR),1,arch);

            int tamParticion, iniPart;
            returnDatosPart(mbrTemp, pathDisco,partName,tamParticion, iniPart);

            fseek(arch,iniPart,SEEK_SET);
            fread(&superBloque,sizeof(SupB),1,arch);

            fclose(arch);

        }


    }
    else
    {
        cout<<"El id no existe, la particion no esta montada"<<endl;
    }



}



bool reporte::existeIdMount(string idP,mount paMoun[], string &pathD, string &partN)
{
    for(int i =0; i<100; i++)
    {
        if(idP == paMoun[i].getId())
        {
            pathD = paMoun[i].getPath();
            partN = paMoun[i].getName();
            return true;

        }

    }
    return false;
}

void reporte::returnDatosPart(MBR mbrTemp, string pathD,string nombrePart,int &tamPart, int &iniPart)
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

void reporte::dirExist(reporte *disco)
{

    //verificar que el directorio existe
    vector<string> resultados;
    resultados = split(disco->getPath(), '/');
    string newpath = "";
    string pathconc = "";
    for (int i = 1; i < resultados.size() - 1; i++)
    {                                             //llenar el string con el path sin el disk.dk
        //pathconc += "/\"" + resultados[i] + "\""; //le agrego comillas a los nombres por si vienen con espacios por ejemplo "mis discos"
        if(strstr(resultados[i].c_str(), " ")!=NULL){
           vector<string> conc = split(resultados[i], ' ');
           resultados[i]="\""+conc[0] + " "+conc[1]+"\"";
        }
        newpath += "/" + resultados[i];           //este es sin comillas para buscar el directorio no importa si vienen con espacios
    }
    //verificar 3 veces si el archivo existe, si el path existe, y enviar el path con comillas
    cout<<newpath<<endl;
    struct stat st;
    if (stat(disco->getPath().c_str(), &st) == 0)
    {
        printf("Existe el archivo en: %s\n", disco->getPath().c_str());
    }else if (stat(newpath.c_str(), &st) == 0)
    {
        printf("Existe el directorio: %s\n",newpath.c_str());
    }
    else
    {
        try{
            printf("No Existe el directorio: %s\n", disco->getPath().c_str());
            string comando = "mkdir -p " + newpath;
            system(comando.c_str());
            printf("Se creo el dir en: %s \n", disco->getPath().c_str());

        }catch(...){
            cout<<"ERROR: no se pudo crear la carpeta"<<endl;
        }

    }
}

vector<string> reporte::split(string str, char pattern)
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


