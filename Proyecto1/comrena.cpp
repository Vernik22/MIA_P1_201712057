#include "comrena.h"
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <fstream>
#include <stdio.h>
#include <iostream>



comrena::comrena()
{
    //ctor
}

comrena::~comrena()
{
    //dtor
}

void comrena::ejecutarComandoRename(comrena *mDir,mount paMoun[])
{
    cout<<"\n************Ejecutar RENAME************\n"<<endl;


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
vector<string> comrena::split(string str, char pattern)
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

string comrena::retFecha()
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

void comrena::modificarArchivo(string pathDisco, string nombrePart, comrena *mDir)
{
    vector<string> rutaArchivo;
    rutaArchivo = split(mDir->getPath(), '/');

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

        Inodo inodoAnterior;
        Inodo inodoTemp;
        fseek(arch,superBloque.s_inode_start,SEEK_SET);
        fread(&inodoTemp,sizeof(Inodo),1,arch);

        inodoAnterior = inodoTemp;


        bool existeCarpeta = false;

        int carp = rutaArchivo.size();


        for(int i= 1; i< carp ; i++)
        {
            existeCarpeta = false;

            if(inodoTemp.i_type == '0')
            {
                for(int j = 0; j<15; j++)
                {
                    if(j<12)
                    {
                        if(inodoTemp.i_block[j] != -1 )
                        {
                            BCarpeta carpetaComprobar;
                            fseek(arch,superBloque.s_block_start +(inodoTemp.i_block[j]*sizeof(BCarpeta)),SEEK_SET);
                            fread(&carpetaComprobar,sizeof(BCarpeta),1,arch);

                            for(int k = 0; k<4; k++)
                            {
                                if(rutaArchivo[i] == carpetaComprobar.b_content[k].b_name)
                                {
                                    inodoAnterior = inodoTemp;
                                    fseek(arch,superBloque.s_inode_start+(carpetaComprobar.b_content[k].b_inodo * sizeof(Inodo)),SEEK_SET);
                                    fread(&inodoTemp,sizeof(Inodo),1,arch);
                                    existeCarpeta= true;
                                    j = 20;
                                    break;


                                }
                            }


                        }

                    }
                    else if(j==12)
                    {
                        if(inodoTemp.i_block[j] != -1 )
                        {
                            //cuerpo += "struct"+std::to_string(contador)+" [shape=record,label=\"<f0> Bloque Apuntador"+ std::to_string(contador)+"| ";
                            BApun apuntadores;
                            fseek(arch,superBloque.s_block_start +(inodoTemp.i_block[j]*sizeof(BCarpeta)),SEEK_SET);
                            fread(&apuntadores,sizeof(BApun),1,arch);


                            for(int k=0; k<4; k++)
                            {
                                if(apuntadores.b_apuntadores [k].b_inodo != -1)
                                {
                                    BCarpeta carpetaComprobar;
                                    fseek(arch,superBloque.s_block_start +(apuntadores.b_apuntadores [k].b_inodo*sizeof(BCarpeta)),SEEK_SET);
                                    fread(&carpetaComprobar,sizeof(BCarpeta),1,arch);

                                    for(int l = 0; l<4; l++)
                                    {
                                        if(rutaArchivo[i] == carpetaComprobar.b_content[l].b_name)
                                        {
                                            inodoAnterior = inodoTemp;
                                            fseek(arch,superBloque.s_inode_start+(carpetaComprobar.b_content[l].b_inodo * sizeof(Inodo)),SEEK_SET);
                                            fread(&inodoTemp,sizeof(Inodo),1,arch);
                                            existeCarpeta= true;
                                            j = 20;
                                            break;


                                        }
                                    }

                                }
                            }




                        }

                    }
                    else if(j==13)
                    {
                        if(inodoTemp.i_block[j] != -1 )
                        {
                            // cuerpo += "struct"+std::to_string(contador)+" [shape=record,label=\"<f0> Bloque Apuntador"+ std::to_string(contador)+"| ";
                            BApun apuntadores;
                            fseek(arch,superBloque.s_block_start +(inodoTemp.i_block[j]*sizeof(BCarpeta)),SEEK_SET);
                            fread(&apuntadores,sizeof(BApun),1,arch);


                            for(int k=0; k<4; k++)
                            {
                                if(apuntadores.b_apuntadores [k].b_inodo != -1)
                                {
                                    BApun apun1;
                                    fseek(arch,superBloque.s_block_start +(apuntadores.b_apuntadores [k].b_inodo*sizeof(BCarpeta)),SEEK_SET);
                                    fread(&apun1,sizeof(BApun),1,arch);
                                    for(int l=0; l<4; l++)
                                    {
                                        if(apun1.b_apuntadores [l].b_inodo != -1)
                                        {
                                            BCarpeta carpetaComprobar;
                                            fseek(arch,superBloque.s_block_start +(apuntadores.b_apuntadores [k].b_inodo*sizeof(BCarpeta)),SEEK_SET);
                                            fread(&carpetaComprobar,sizeof(BCarpeta),1,arch);

                                            for(int n = 0; n<4; n++)
                                            {
                                                if(rutaArchivo[i] == carpetaComprobar.b_content[n].b_name)
                                                {
                                                    inodoAnterior = inodoTemp;
                                                    fseek(arch,superBloque.s_inode_start+(carpetaComprobar.b_content[n].b_inodo * sizeof(Inodo)),SEEK_SET);
                                                    fread(&inodoTemp,sizeof(Inodo),1,arch);
                                                    existeCarpeta= true;
                                                    j = 20;
                                                    break;


                                                }
                                            }
                                        }
                                    }


                                }

                            }




                        }

                    }
                    else if(j==14)
                    {
                        if(inodoTemp.i_block[j] != -1 )
                        {
                            //cuerpo += "struct"+std::to_string(contador)+" [shape=record,label=\"<f0> Bloque Apuntador"+ std::to_string(contador)+"| ";
                            BApun apuntadores;
                            fseek(arch,superBloque.s_block_start +(inodoTemp.i_block[j]*sizeof(BCarpeta)),SEEK_SET);
                            fread(&apuntadores,sizeof(BApun),1,arch);


                            for(int k=0; k<4; k++)
                            {
                                if(apuntadores.b_apuntadores [k].b_inodo != -1)
                                {
                                    BApun apun1;
                                    fseek(arch,superBloque.s_block_start +(apuntadores.b_apuntadores[k].b_inodo*sizeof(BCarpeta)),SEEK_SET);
                                    fread(&apun1,sizeof(BApun),1,arch);
                                    for(int l=0; l<4; l++)
                                    {
                                        if(apun1.b_apuntadores [l].b_inodo != -1)
                                        {
                                            BApun apun2;
                                            fseek(arch,superBloque.s_block_start +(apun1.b_apuntadores[l].b_inodo*sizeof(BCarpeta)),SEEK_SET);
                                            fread(&apun2,sizeof(BApun),1,arch);

                                            for(int n=0; n<4; n++)
                                            {
                                                if(apun2.b_apuntadores [n].b_inodo != -1)
                                                {
                                                    BCarpeta carpetaComprobar;
                                                    fseek(arch,superBloque.s_block_start +(apuntadores.b_apuntadores [k].b_inodo*sizeof(BCarpeta)),SEEK_SET);
                                                    fread(&carpetaComprobar,sizeof(BCarpeta),1,arch);

                                                    for(int m = 0; m<4; m++)
                                                    {
                                                        if(rutaArchivo[i] == carpetaComprobar.b_content[m].b_name)
                                                        {
                                                            inodoAnterior = inodoTemp;
                                                            fseek(arch,superBloque.s_inode_start+(carpetaComprobar.b_content[m].b_inodo * sizeof(Inodo)),SEEK_SET);
                                                            fread(&inodoTemp,sizeof(Inodo),1,arch);
                                                            existeCarpeta= true;
                                                            j = 20;
                                                            break;


                                                        }
                                                    }

                                                }
                                            }

                                        }
                                    }


                                }

                            }


                        }

                    }
                }

            }
            else
            {

                existeCarpeta = true;


            }
            if(!existeCarpeta)
            {
                cout<<"ERROR: no se encontro la ruta "<<endl;
                break;

            }

        }

        //rename
        if(existeCarpeta )
        {

            for(int j = 0; j<15; j++)
            {
                if(j<12)
                {
                    if(inodoAnterior.i_block[j] != -1 )
                    {
                        BCarpeta carpetaComprobar;
                        fseek(arch,superBloque.s_block_start +(inodoAnterior.i_block[j]*sizeof(BCarpeta)),SEEK_SET);
                        fread(&carpetaComprobar,sizeof(BCarpeta),1,arch);

                        for(int k = 0; k<4; k++)
                        {
                            if(rutaArchivo[carp - 1] == carpetaComprobar.b_content[k].b_name)
                            {

                                string vacio = mDir->getName();
                                strcpy(carpetaComprobar.b_content[k].b_name,vacio.c_str());

                                fseek(arch,superBloque.s_block_start +(inodoAnterior.i_block[j]*sizeof(BCarpeta)),SEEK_SET);
                                fwrite(&carpetaComprobar,sizeof(BCarpeta),1,arch);
                                cout<<"Se renombro con exito: "<<rutaArchivo[carp -1] <<" a: "<< mDir->getName()<<endl;
                                j = 20;
                                break;


                            }
                            else if(mDir->getName() == carpetaComprobar.b_content[k].b_name)
                            {
                                cout<<"ERROR: el nombre ya existe"<<endl;
                                j = 20;
                                break;

                            }
                        }


                    }

                }
                else if(j==12)
                {
                    if(inodoAnterior.i_block[j] != -1 )
                    {
                        //cuerpo += "struct"+std::to_string(contador)+" [shape=record,label=\"<f0> Bloque Apuntador"+ std::to_string(contador)+"| ";
                        BApun apuntadores;
                        fseek(arch,superBloque.s_block_start +(inodoAnterior.i_block[j]*sizeof(BCarpeta)),SEEK_SET);
                        fread(&apuntadores,sizeof(BApun),1,arch);


                        for(int k=0; k<4; k++)
                        {
                            if(apuntadores.b_apuntadores [k].b_inodo != -1)
                            {
                                BCarpeta carpetaComprobar;
                                fseek(arch,superBloque.s_block_start +(apuntadores.b_apuntadores [k].b_inodo*sizeof(BCarpeta)),SEEK_SET);
                                fread(&carpetaComprobar,sizeof(BCarpeta),1,arch);

                                for(int l = 0; l<4; l++)
                                {
                                    if(rutaArchivo[carp -1 ] == carpetaComprobar.b_content[l].b_name)
                                    {

                                    string vacio = mDir->getName();
                                        strcpy(carpetaComprobar.b_content[l].b_name,vacio.c_str());


                                        fseek(arch,superBloque.s_block_start +(apuntadores.b_apuntadores [k].b_inodo*sizeof(BCarpeta)),SEEK_SET);
                                        fwrite(&carpetaComprobar,sizeof(BCarpeta),1,arch);
                                        cout<<"Se renombro con exito: "<<rutaArchivo[carp -1] <<" a: "<< mDir->getName()<<endl;
                                        j = 20;
                                        break;


                                    }
                                    else if(mDir->getName() == carpetaComprobar.b_content[l].b_name)
                                    {
                                        cout<<"ERROR: el nombre ya existe"<<endl;
                                        j = 20;
                                        break;

                                    }
                                }

                            }
                        }




                    }

                }
                else if(j==13)
                {
                    if(inodoAnterior.i_block[j] != -1 )
                    {
                        // cuerpo += "struct"+std::to_string(contador)+" [shape=record,label=\"<f0> Bloque Apuntador"+ std::to_string(contador)+"| ";
                        BApun apuntadores;
                        fseek(arch,superBloque.s_block_start +(inodoAnterior.i_block[j]*sizeof(BCarpeta)),SEEK_SET);
                        fread(&apuntadores,sizeof(BApun),1,arch);


                        for(int k=0; k<4; k++)
                        {
                            if(apuntadores.b_apuntadores [k].b_inodo != -1)
                            {
                                BApun apun1;
                                fseek(arch,superBloque.s_block_start +(apuntadores.b_apuntadores [k].b_inodo*sizeof(BCarpeta)),SEEK_SET);
                                fread(&apun1,sizeof(BApun),1,arch);
                                for(int l=0; l<4; l++)
                                {
                                    if(apun1.b_apuntadores [l].b_inodo != -1)
                                    {
                                        BCarpeta carpetaComprobar;
                                        fseek(arch,superBloque.s_block_start +(apuntadores.b_apuntadores [k].b_inodo*sizeof(BCarpeta)),SEEK_SET);
                                        fread(&carpetaComprobar,sizeof(BCarpeta),1,arch);

                                        for(int n = 0; n<4; n++)
                                        {
                                            if(rutaArchivo[carp - 1] == carpetaComprobar.b_content[n].b_name)
                                            {

                                            string vacio = mDir->getName();
                                                strcpy(carpetaComprobar.b_content[n].b_name,vacio.c_str());

                                                fseek(arch,superBloque.s_block_start +(apuntadores.b_apuntadores [k].b_inodo*sizeof(BCarpeta)),SEEK_SET);
                                                fwrite(&carpetaComprobar,sizeof(BCarpeta),1,arch);
                                                cout<<"Se renombro con exito: "<<rutaArchivo[carp -1] <<" a: "<< mDir->getName()<<endl;
                                                j = 20;
                                                break;


                                            }
                                            else if(mDir->getName() == carpetaComprobar.b_content[n].b_name)
                                            {
                                                cout<<"ERROR: el nombre ya existe"<<endl;
                                                j = 20;
                                                break;
                                            }
                                        }
                                    }


                                }

                            }




                        }

                    }
                }
                else if(j==14)
                {
                    if(inodoAnterior.i_block[j] != -1 )
                    {
                        //cuerpo += "struct"+std::to_string(contador)+" [shape=record,label=\"<f0> Bloque Apuntador"+ std::to_string(contador)+"| ";
                        BApun apuntadores;
                        fseek(arch,superBloque.s_block_start +(inodoAnterior.i_block[j]*sizeof(BCarpeta)),SEEK_SET);
                        fread(&apuntadores,sizeof(BApun),1,arch);


                        for(int k=0; k<4; k++)
                        {
                            if(apuntadores.b_apuntadores [k].b_inodo != -1)
                            {
                                BApun apun1;
                                fseek(arch,superBloque.s_block_start +(apuntadores.b_apuntadores[k].b_inodo*sizeof(BCarpeta)),SEEK_SET);
                                fread(&apun1,sizeof(BApun),1,arch);
                                for(int l=0; l<4; l++)
                                {
                                    if(apun1.b_apuntadores [l].b_inodo != -1)
                                    {
                                        BApun apun2;
                                        fseek(arch,superBloque.s_block_start +(apun1.b_apuntadores[l].b_inodo*sizeof(BCarpeta)),SEEK_SET);
                                        fread(&apun2,sizeof(BApun),1,arch);

                                        for(int n=0; n<4; n++)
                                        {
                                            if(apun2.b_apuntadores [n].b_inodo != -1)
                                            {
                                                BCarpeta carpetaComprobar;
                                                fseek(arch,superBloque.s_block_start +(apuntadores.b_apuntadores [k].b_inodo*sizeof(BCarpeta)),SEEK_SET);
                                                fread(&carpetaComprobar,sizeof(BCarpeta),1,arch);

                                                for(int m = 0; m<4; m++)
                                                {
                                                    if(rutaArchivo[carp-1] == carpetaComprobar.b_content[m].b_name)
                                                    {
                                                        string vacio = mDir->getName();

                                                        strcpy(carpetaComprobar.b_content[m].b_name,vacio.c_str());

                                                        fseek(arch,superBloque.s_block_start +(apuntadores.b_apuntadores [k].b_inodo*sizeof(BCarpeta)),SEEK_SET);
                                                        fwrite(&carpetaComprobar,sizeof(BCarpeta),1,arch);
                                                        cout<<"Se renombro con exito: "<<rutaArchivo[carp -1] <<" a: "<< mDir->getName()<<endl;
                                                        j = 20;
                                                        break;

                                                    }
                                                    else if(mDir->getName() == carpetaComprobar.b_content[m].b_name)
                                                    {
                                                        cout<<"ERROR: el nombre ya existe"<<endl;
                                                        j = 20;
                                                        break;
                                                    }
                                                }

                                            }
                                        }

                                    }
                                }


                            }

                        }


                    }

                }
            }


        }
        else
        {

            cout<<"ERROR: no se pudo realizar la accion de rename"<<endl;
        }

    }
    fclose(arch);
}

void comrena::returnDatosPart(MBR mbrTemp, string pathD,string nombrePart,int &tamPart, int &iniPart)
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

