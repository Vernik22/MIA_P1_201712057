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

            }
            else  if(mbrTemp.mbr_partition_1.part_status == '0') //la particion1 esta disponible
            {
                //verifica el tipo de ajuste del disco
                if(Dfit=='F') //Primer ajuste
                {
                    int esp1 = 0;
                    int startEsp1 = 0;
                    int startEsp2 = 0;
                    int startEsp3 = 0;
                    //verificar si hay mas particiones ocupadas
                    if(mbrTemp.mbr_partition_2.part_status == '1')
                    {
                        startEsp1=mbrTemp.mbr_partition_2.part_start;
                    }
                    if(mbrTemp.mbr_partition_3.part_status == '1')
                    {
                        startEsp2=mbrTemp.mbr_partition_3.part_start;
                    }
                    if(mbrTemp.mbr_partition_4.part_status == '1')
                    {
                        startEsp3=mbrTemp.mbr_partition_4.part_start;
                    }

                    while(startPart < mbrTemp.mbr_tamano){

                    }

                    int peq = masPequeño(startEsp1,startEsp2,startEsp3,0);
                    if(aComp == esp1)
                            {
                                if(esp1>=tamanoParticion)
                                {

                                    //seteando los valores que llevara la nueva particion
                                    particionTemp.part_status = '1';//particion ocupada
                                    particionTemp.part_type = 'P';
                                    strcpy(particionTemp.part_name,disco->getName().c_str());
                                    particionTemp.part_size= tamanoParticion;
                                    particionTemp.part_start=startEspAsig1;
                                    //copiando particion creada
                                    mbrTemp.mbr_partition_1 =particionTemp;
                                    //toca modificar el mbr original en el archivo por el nuevo mbr con los datos de la particion creada
                                    fseek(arch,0,SEEK_SET);
                                    fwrite(&mbrTemp,sizeof(MBR),1,arch); //se sobreescribe el mbr
                                    fclose(arch);
                                    cout<<"Se Creo la particion Correctamente"<<endl;


                                }
                                else
                                {
                                    cout<<"No hay espacio suficiente en el disco"<<endl;
                                }
                            }
                            else if(aComp == esp2)
                            {
                                if(esp2>=tamanoParticion)
                                {

                                    //seteando los valores que llevara la nueva particion
                                    particionTemp.part_status = '1';//particion ocupada
                                    particionTemp.part_type = 'P';
                                    strcpy(particionTemp.part_name,disco->getName().c_str());
                                    particionTemp.part_size= tamanoParticion;
                                    particionTemp.part_start=startEspAsig2;
                                    //copiando particion creada
                                    mbrTemp.mbr_partition_1 =particionTemp;
                                    //toca modificar el mbr original en el archivo por el nuevo mbr con los datos de la particion creada
                                    fseek(arch,0,SEEK_SET);
                                    fwrite(&mbrTemp,sizeof(MBR),1,arch); //se sobreescribe el mbr
                                    fclose(arch);
                                    cout<<"Se Creo la particion Correctamente"<<endl;


                                }
                                else
                                {
                                    cout<<"No hay espacio suficiente en el disco"<<endl;
                                }
                            }
                            else if(aComp == esp3)
                            {
                                if(esp3>=tamanoParticion)
                                {

                                    //seteando los valores que llevara la nueva particion
                                    particionTemp.part_status = '1';//particion ocupada
                                    particionTemp.part_type = 'P';
                                    strcpy(particionTemp.part_name,disco->getName().c_str());
                                    particionTemp.part_size= tamanoParticion;
                                    particionTemp.part_start=startEspAsig3;
                                    //copiando particion creada
                                    mbrTemp.mbr_partition_1 =particionTemp;
                                    //toca modificar el mbr original en el archivo por el nuevo mbr con los datos de la particion creada
                                    fseek(arch,0,SEEK_SET);
                                    fwrite(&mbrTemp,sizeof(MBR),1,arch); //se sobreescribe el mbr
                                    fclose(arch);
                                    cout<<"Se Creo la particion Correctamente"<<endl;


                                }
                                else
                                {
                                    cout<<"No hay espacio suficiente en el disco"<<endl;
                                }
                            }

                }
                else if (Dfit=='W')  //Peor ajuste
                {
                    int esp1 = 0;
                    int startEspAsig1 = 0;
                    int startEsp1 = 0;
                    int esp2 = 0;
                    int startEspAsig2 = 0;
                    int startEsp2 = 0;
                    int esp3 = 0;
                    int startEspAsig3 = 0;
                    int startEsp3 = 0;
                    int esp4 = 0;
                    int startEsp4 = 0;
                    //verificar si hay mas particiones ocupadas
                    if(mbrTemp.mbr_partition_2.part_status == '1')
                    {
                        startEsp1=mbrTemp.mbr_partition_2.part_start;
                    }
                    if(mbrTemp.mbr_partition_3.part_status == '1')
                    {
                        startEsp2=mbrTemp.mbr_partition_3.part_start;
                        //esp2=espacioLibre(startPart,mbrTemp.mbr_partition_3.part_start);
                    }
                    if(mbrTemp.mbr_partition_4.part_status == '1')
                    {
                        startEsp3=mbrTemp.mbr_partition_4.part_start;
                        //esp3=espacioLibre(startPart,mbrTemp.mbr_partition_4.part_start);
                    }

                    //revisa cual start es mas pequeño y quiere decir que esta primero en el disco
                    if(startEsp1< startEsp2&& startEsp1< startEsp3 && startEsp1!=0)
                    {
                        esp1=espacioLibre(startPart,startEsp1);
                        startEspAsig1 = startPart;
                        startPart=(startEsp1 + mbrTemp.mbr_partition_2.part_size);

                        if(startEsp2< startEsp3)
                        {
                            //startEsp3 es el mas grande
                            //veo el espacio entre startEsp2 y startEsp1
                            esp2=espacioLibre(startPart,startEsp2);
                            startEspAsig2 = startPart;
                            startPart=(startEsp2 + mbrTemp.mbr_partition_3.part_size);
                            //veo el espacio entre startEsp3 y starEsp2
                            esp3=espacioLibre(startPart,startEsp3);
                            startEspAsig3 = startPart;
                            startPart=(startEsp1 + mbrTemp.mbr_partition_4.part_size);
                            startEsp4 = startPart;
                            //si queda espacio al final del disco despues de startEsp3
                            esp4 = espacioLibre(startPart,mbrTemp.mbr_tamano);

                            //ver cual espacio es mas grande
                            int aComp = masGrande(esp1, esp2, esp3, esp4);
                            if(aComp == esp1)
                            {
                                if(esp1>=tamanoParticion)
                                {

                                    //seteando los valores que llevara la nueva particion
                                    particionTemp.part_status = '1';//particion ocupada
                                    particionTemp.part_type = 'P';
                                    strcpy(particionTemp.part_name,disco->getName().c_str());
                                    particionTemp.part_size= tamanoParticion;
                                    particionTemp.part_start=startEspAsig1;
                                    //copiando particion creada
                                    mbrTemp.mbr_partition_1 =particionTemp;
                                    //toca modificar el mbr original en el archivo por el nuevo mbr con los datos de la particion creada
                                    fseek(arch,0,SEEK_SET);
                                    fwrite(&mbrTemp,sizeof(MBR),1,arch); //se sobreescribe el mbr
                                    fclose(arch);
                                    cout<<"Se Creo la particion Correctamente"<<endl;


                                }
                                else
                                {
                                    cout<<"No hay espacio suficiente en el disco"<<endl;
                                }
                            }
                            else if(aComp == esp2)
                            {
                                if(esp2>=tamanoParticion)
                                {

                                    //seteando los valores que llevara la nueva particion
                                    particionTemp.part_status = '1';//particion ocupada
                                    particionTemp.part_type = 'P';
                                    strcpy(particionTemp.part_name,disco->getName().c_str());
                                    particionTemp.part_size= tamanoParticion;
                                    particionTemp.part_start=startEspAsig2;
                                    //copiando particion creada
                                    mbrTemp.mbr_partition_1 =particionTemp;
                                    //toca modificar el mbr original en el archivo por el nuevo mbr con los datos de la particion creada
                                    fseek(arch,0,SEEK_SET);
                                    fwrite(&mbrTemp,sizeof(MBR),1,arch); //se sobreescribe el mbr
                                    fclose(arch);
                                    cout<<"Se Creo la particion Correctamente"<<endl;


                                }
                                else
                                {
                                    cout<<"No hay espacio suficiente en el disco"<<endl;
                                }
                            }
                            else if(aComp == esp3)
                            {
                                if(esp3>=tamanoParticion)
                                {

                                    //seteando los valores que llevara la nueva particion
                                    particionTemp.part_status = '1';//particion ocupada
                                    particionTemp.part_type = 'P';
                                    strcpy(particionTemp.part_name,disco->getName().c_str());
                                    particionTemp.part_size= tamanoParticion;
                                    particionTemp.part_start=startEspAsig3;
                                    //copiando particion creada
                                    mbrTemp.mbr_partition_1 =particionTemp;
                                    //toca modificar el mbr original en el archivo por el nuevo mbr con los datos de la particion creada
                                    fseek(arch,0,SEEK_SET);
                                    fwrite(&mbrTemp,sizeof(MBR),1,arch); //se sobreescribe el mbr
                                    fclose(arch);
                                    cout<<"Se Creo la particion Correctamente"<<endl;


                                }
                                else
                                {
                                    cout<<"No hay espacio suficiente en el disco"<<endl;
                                }
                            }
                            else
                            {
                                if(esp4>=tamanoParticion)
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

                                }
                                else
                                {
                                    cout<<"No hay espacio suficiente en el disco"<<endl;
                                }
                            }

                        }
                        else   //startEsp3< startEsp2
                        {
                            //startEsp2 es el mas grande
                            //veo el espacio entre startEsp3 y startEsp1
                            esp3=espacioLibre(startPart,startEsp3);
                            startEspAsig3 = startPart;
                            startPart=(startEsp3 + mbrTemp.mbr_partition_4.part_size);
                            //veo el espacio entre startEsp3 y starEsp2
                            esp2=espacioLibre(startPart,startEsp2);
                            startEspAsig2 = startPart;
                            startPart=(startEsp2 + mbrTemp.mbr_partition_3.part_size);
                            startEsp4 = startPart;
                            //si queda espacio al final del disco despues de startEsp3
                            esp4 = espacioLibre(startPart,mbrTemp.mbr_tamano);

                            //ver cual espacio es mas grande
                            int aComp = masGrande(esp1, esp2, esp3, esp4);
                            if(aComp == esp1)
                            {
                                if(esp1>=tamanoParticion)
                                {

                                    //seteando los valores que llevara la nueva particion
                                    particionTemp.part_status = '1';//particion ocupada
                                    particionTemp.part_type = 'P';
                                    strcpy(particionTemp.part_name,disco->getName().c_str());
                                    particionTemp.part_size= tamanoParticion;
                                    particionTemp.part_start=startEspAsig1;
                                    //copiando particion creada
                                    mbrTemp.mbr_partition_1 =particionTemp;
                                    //toca modificar el mbr original en el archivo por el nuevo mbr con los datos de la particion creada
                                    fseek(arch,0,SEEK_SET);
                                    fwrite(&mbrTemp,sizeof(MBR),1,arch); //se sobreescribe el mbr
                                    fclose(arch);
                                    cout<<"Se Creo la particion Correctamente"<<endl;


                                }
                                else
                                {
                                    cout<<"No hay espacio suficiente en el disco"<<endl;
                                }
                            }
                            else if(aComp == esp2)
                            {
                                if(esp2>=tamanoParticion)
                                {

                                    //seteando los valores que llevara la nueva particion
                                    particionTemp.part_status = '1';//particion ocupada
                                    particionTemp.part_type = 'P';
                                    strcpy(particionTemp.part_name,disco->getName().c_str());
                                    particionTemp.part_size= tamanoParticion;
                                    particionTemp.part_start=startEspAsig2;
                                    //copiando particion creada
                                    mbrTemp.mbr_partition_1 =particionTemp;
                                    //toca modificar el mbr original en el archivo por el nuevo mbr con los datos de la particion creada
                                    fseek(arch,0,SEEK_SET);
                                    fwrite(&mbrTemp,sizeof(MBR),1,arch); //se sobreescribe el mbr
                                    fclose(arch);
                                    cout<<"Se Creo la particion Correctamente"<<endl;


                                }
                                else
                                {
                                    cout<<"No hay espacio suficiente en el disco"<<endl;
                                }
                            }
                            else if(aComp == esp3)
                            {
                                if(esp3>=tamanoParticion)
                                {

                                    //seteando los valores que llevara la nueva particion
                                    particionTemp.part_status = '1';//particion ocupada
                                    particionTemp.part_type = 'P';
                                    strcpy(particionTemp.part_name,disco->getName().c_str());
                                    particionTemp.part_size= tamanoParticion;
                                    particionTemp.part_start=startEspAsig3;
                                    //copiando particion creada
                                    mbrTemp.mbr_partition_1 =particionTemp;
                                    //toca modificar el mbr original en el archivo por el nuevo mbr con los datos de la particion creada
                                    fseek(arch,0,SEEK_SET);
                                    fwrite(&mbrTemp,sizeof(MBR),1,arch); //se sobreescribe el mbr
                                    fclose(arch);
                                    cout<<"Se Creo la particion Correctamente"<<endl;


                                }
                                else
                                {
                                    cout<<"No hay espacio suficiente en el disco"<<endl;
                                }
                            }
                            else
                            {
                                if(esp4>=tamanoParticion)
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

                                }
                                else
                                {
                                    cout<<"No hay espacio suficiente en el disco"<<endl;
                                }
                            }

                        }
                    }
                    else if(startEsp2< startEsp1&& startEsp2< startEsp3 &&  startEsp2!=0 )
                    {

                        esp2=espacioLibre(startPart,startEsp2);
                        startEspAsig2 = startPart;
                        startPart=(startEsp2 + mbrTemp.mbr_partition_3.part_size);

                        if(startEsp1< startEsp3)
                        {
                            //startEsp3 es el mas grande
                            //veo el espacio entre startEsp2 y startEsp1
                            esp1=espacioLibre(startPart,startEsp1);
                            startEspAsig1 = startPart;
                            startPart=(startEsp1 + mbrTemp.mbr_partition_2.part_size);
                            //veo el espacio entre startEsp3 y starEsp2
                            esp3=espacioLibre(startPart,startEsp3);
                            startEspAsig3 = startPart;
                            startPart=(startEsp3 + mbrTemp.mbr_partition_4.part_size);
                            startEsp4 = startPart;
                            //si queda espacio al final del disco despues de startEsp3
                            esp4 = espacioLibre(startPart,mbrTemp.mbr_tamano);

                            //ver cual espacio es mas grande
                            int aComp = masGrande(esp1, esp2, esp3, esp4);
                            if(aComp == esp1)
                            {
                                if(esp1>=tamanoParticion)
                                {

                                    //seteando los valores que llevara la nueva particion
                                    particionTemp.part_status = '1';//particion ocupada
                                    particionTemp.part_type = 'P';
                                    strcpy(particionTemp.part_name,disco->getName().c_str());
                                    particionTemp.part_size= tamanoParticion;
                                    particionTemp.part_start=startEspAsig1;
                                    //copiando particion creada
                                    mbrTemp.mbr_partition_1 =particionTemp;
                                    //toca modificar el mbr original en el archivo por el nuevo mbr con los datos de la particion creada
                                    fseek(arch,0,SEEK_SET);
                                    fwrite(&mbrTemp,sizeof(MBR),1,arch); //se sobreescribe el mbr
                                    fclose(arch);
                                    cout<<"Se Creo la particion Correctamente"<<endl;


                                }
                                else
                                {
                                    cout<<"No hay espacio suficiente en el disco"<<endl;
                                }
                            }
                            else if(aComp == esp2)
                            {
                                if(esp2>=tamanoParticion)
                                {

                                    //seteando los valores que llevara la nueva particion
                                    particionTemp.part_status = '1';//particion ocupada
                                    particionTemp.part_type = 'P';
                                    strcpy(particionTemp.part_name,disco->getName().c_str());
                                    particionTemp.part_size= tamanoParticion;
                                    particionTemp.part_start=startEspAsig2;
                                    //copiando particion creada
                                    mbrTemp.mbr_partition_1 =particionTemp;
                                    //toca modificar el mbr original en el archivo por el nuevo mbr con los datos de la particion creada
                                    fseek(arch,0,SEEK_SET);
                                    fwrite(&mbrTemp,sizeof(MBR),1,arch); //se sobreescribe el mbr
                                    fclose(arch);
                                    cout<<"Se Creo la particion Correctamente"<<endl;


                                }
                                else
                                {
                                    cout<<"No hay espacio suficiente en el disco"<<endl;
                                }
                            }
                            else if(aComp == esp3)
                            {
                                if(esp3>=tamanoParticion)
                                {

                                    //seteando los valores que llevara la nueva particion
                                    particionTemp.part_status = '1';//particion ocupada
                                    particionTemp.part_type = 'P';
                                    strcpy(particionTemp.part_name,disco->getName().c_str());
                                    particionTemp.part_size= tamanoParticion;
                                    particionTemp.part_start=startEspAsig3;
                                    //copiando particion creada
                                    mbrTemp.mbr_partition_1 =particionTemp;
                                    //toca modificar el mbr original en el archivo por el nuevo mbr con los datos de la particion creada
                                    fseek(arch,0,SEEK_SET);
                                    fwrite(&mbrTemp,sizeof(MBR),1,arch); //se sobreescribe el mbr
                                    fclose(arch);
                                    cout<<"Se Creo la particion Correctamente"<<endl;


                                }
                                else
                                {
                                    cout<<"No hay espacio suficiente en el disco"<<endl;
                                }
                            }
                            else
                            {
                                if(esp4>=tamanoParticion)
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

                                }
                                else
                                {
                                    cout<<"No hay espacio suficiente en el disco"<<endl;
                                }
                            }

                        }
                        else   //startEsp3< startEsp1
                        {
                            //startEsp1 es el mas grande
                            //veo el espacio entre startEsp3 y startEsp1
                            esp3=espacioLibre(startPart,startEsp3);
                            startEspAsig3 = startPart;
                            startPart=(startEsp3 + mbrTemp.mbr_partition_4.part_size);
                            //veo el espacio entre startEsp3 y starEsp2
                            esp1=espacioLibre(startPart,startEsp1);
                            startEspAsig1 = startPart;
                            startPart=(startEsp1 + mbrTemp.mbr_partition_2.part_size);
                            startEsp4 = startPart;
                            //si queda espacio al final del disco despues de startEsp3
                            esp4 = espacioLibre(startPart,mbrTemp.mbr_tamano);

                            //ver cual espacio es mas grande
                            int aComp = masGrande(esp1, esp2, esp3, esp4);
                            if(aComp == esp1)
                            {
                                if(esp1>=tamanoParticion)
                                {

                                    //seteando los valores que llevara la nueva particion
                                    particionTemp.part_status = '1';//particion ocupada
                                    particionTemp.part_type = 'P';
                                    strcpy(particionTemp.part_name,disco->getName().c_str());
                                    particionTemp.part_size= tamanoParticion;
                                    particionTemp.part_start=startEspAsig1;
                                    //copiando particion creada
                                    mbrTemp.mbr_partition_1 =particionTemp;
                                    //toca modificar el mbr original en el archivo por el nuevo mbr con los datos de la particion creada
                                    fseek(arch,0,SEEK_SET);
                                    fwrite(&mbrTemp,sizeof(MBR),1,arch); //se sobreescribe el mbr
                                    fclose(arch);
                                    cout<<"Se Creo la particion Correctamente"<<endl;


                                }
                                else
                                {
                                    cout<<"No hay espacio suficiente en el disco"<<endl;
                                }
                            }
                            else if(aComp == esp2)
                            {
                                if(esp2>=tamanoParticion)
                                {

                                    //seteando los valores que llevara la nueva particion
                                    particionTemp.part_status = '1';//particion ocupada
                                    particionTemp.part_type = 'P';
                                    strcpy(particionTemp.part_name,disco->getName().c_str());
                                    particionTemp.part_size= tamanoParticion;
                                    particionTemp.part_start=startEspAsig2;
                                    //copiando particion creada
                                    mbrTemp.mbr_partition_1 =particionTemp;
                                    //toca modificar el mbr original en el archivo por el nuevo mbr con los datos de la particion creada
                                    fseek(arch,0,SEEK_SET);
                                    fwrite(&mbrTemp,sizeof(MBR),1,arch); //se sobreescribe el mbr
                                    fclose(arch);
                                    cout<<"Se Creo la particion Correctamente"<<endl;


                                }
                                else
                                {
                                    cout<<"No hay espacio suficiente en el disco"<<endl;
                                }
                            }
                            else if(aComp == esp3)
                            {
                                if(esp3>=tamanoParticion)
                                {

                                    //seteando los valores que llevara la nueva particion
                                    particionTemp.part_status = '1';//particion ocupada
                                    particionTemp.part_type = 'P';
                                    strcpy(particionTemp.part_name,disco->getName().c_str());
                                    particionTemp.part_size= tamanoParticion;
                                    particionTemp.part_start=startEspAsig3;
                                    //copiando particion creada
                                    mbrTemp.mbr_partition_1 =particionTemp;
                                    //toca modificar el mbr original en el archivo por el nuevo mbr con los datos de la particion creada
                                    fseek(arch,0,SEEK_SET);
                                    fwrite(&mbrTemp,sizeof(MBR),1,arch); //se sobreescribe el mbr
                                    fclose(arch);
                                    cout<<"Se Creo la particion Correctamente"<<endl;


                                }
                                else
                                {
                                    cout<<"No hay espacio suficiente en el disco"<<endl;
                                }
                            }
                            else
                            {
                                if(esp4>=tamanoParticion)
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

                                }
                                else
                                {
                                    cout<<"No hay espacio suficiente en el disco"<<endl;
                                }
                            }

                        }
                    }
                    else if(startEsp3< startEsp2&& startEsp3< startEsp1 &&  startEsp3!=0)
                    {
                        esp3=espacioLibre(startPart,startEsp3);
                        startEspAsig3 = startPart;
                        startPart=(startEsp3 + mbrTemp.mbr_partition_4.part_size);

                        if(startEsp1< startEsp2)
                        {
                            //startEsp3 es el mas grande
                            //veo el espacio entre startEsp2 y startEsp1
                            esp1=espacioLibre(startPart,startEsp1);
                            startEspAsig1 = startPart;
                            startPart=(startEsp1 + mbrTemp.mbr_partition_2.part_size);
                            //veo el espacio entre startEsp3 y starEsp2
                            esp2=espacioLibre(startPart,startEsp2);
                            startEspAsig2 = startPart;
                            startPart=(startEsp2 + mbrTemp.mbr_partition_3.part_size);
                            startEsp4 = startPart;
                            //si queda espacio al final del disco despues de startEsp3
                            esp4 = espacioLibre(startPart,mbrTemp.mbr_tamano);

                            //ver cual espacio es mas grande
                            int aComp = masGrande(esp1, esp2, esp3, esp4);
                            if(aComp == esp1)
                            {
                                if(esp1>=tamanoParticion)
                                {

                                    //seteando los valores que llevara la nueva particion
                                    particionTemp.part_status = '1';//particion ocupada
                                    particionTemp.part_type = 'P';
                                    strcpy(particionTemp.part_name,disco->getName().c_str());
                                    particionTemp.part_size= tamanoParticion;
                                    particionTemp.part_start=startEspAsig1;
                                    //copiando particion creada
                                    mbrTemp.mbr_partition_1 =particionTemp;
                                    //toca modificar el mbr original en el archivo por el nuevo mbr con los datos de la particion creada
                                    fseek(arch,0,SEEK_SET);
                                    fwrite(&mbrTemp,sizeof(MBR),1,arch); //se sobreescribe el mbr
                                    fclose(arch);
                                    cout<<"Se Creo la particion Correctamente"<<endl;


                                }
                                else
                                {
                                    cout<<"No hay espacio suficiente en el disco"<<endl;
                                }
                            }
                            else if(aComp == esp2)
                            {
                                if(esp2>=tamanoParticion)
                                {

                                    //seteando los valores que llevara la nueva particion
                                    particionTemp.part_status = '1';//particion ocupada
                                    particionTemp.part_type = 'P';
                                    strcpy(particionTemp.part_name,disco->getName().c_str());
                                    particionTemp.part_size= tamanoParticion;
                                    particionTemp.part_start=startEspAsig2;
                                    //copiando particion creada
                                    mbrTemp.mbr_partition_1 =particionTemp;
                                    //toca modificar el mbr original en el archivo por el nuevo mbr con los datos de la particion creada
                                    fseek(arch,0,SEEK_SET);
                                    fwrite(&mbrTemp,sizeof(MBR),1,arch); //se sobreescribe el mbr
                                    fclose(arch);
                                    cout<<"Se Creo la particion Correctamente"<<endl;


                                }
                                else
                                {
                                    cout<<"No hay espacio suficiente en el disco"<<endl;
                                }
                            }
                            else if(aComp == esp3)
                            {
                                if(esp3>=tamanoParticion)
                                {

                                    //seteando los valores que llevara la nueva particion
                                    particionTemp.part_status = '1';//particion ocupada
                                    particionTemp.part_type = 'P';
                                    strcpy(particionTemp.part_name,disco->getName().c_str());
                                    particionTemp.part_size= tamanoParticion;
                                    particionTemp.part_start=startEspAsig3;
                                    //copiando particion creada
                                    mbrTemp.mbr_partition_1 =particionTemp;
                                    //toca modificar el mbr original en el archivo por el nuevo mbr con los datos de la particion creada
                                    fseek(arch,0,SEEK_SET);
                                    fwrite(&mbrTemp,sizeof(MBR),1,arch); //se sobreescribe el mbr
                                    fclose(arch);
                                    cout<<"Se Creo la particion Correctamente"<<endl;


                                }
                                else
                                {
                                    cout<<"No hay espacio suficiente en el disco"<<endl;
                                }
                            }
                            else
                            {
                                if(esp4>=tamanoParticion)
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

                                }
                                else
                                {
                                    cout<<"No hay espacio suficiente en el disco"<<endl;
                                }
                            }

                        }
                        else   //startEsp2< startEsp1
                        {
                            //startEsp1 es el mas grande
                            //veo el espacio entre startEsp3 y startEsp1
                            esp2=espacioLibre(startPart,startEsp2);
                            startEspAsig2 = startPart;
                            startPart=(startEsp2 + mbrTemp.mbr_partition_3.part_size);
                            //veo el espacio entre startEsp3 y starEsp2
                            esp1=espacioLibre(startPart,startEsp1);
                            startEspAsig1 = startPart;
                            startPart=(startEsp1 + mbrTemp.mbr_partition_2.part_size);
                            startEsp4 = startPart;
                            //si queda espacio al final del disco despues de startEsp3
                            esp4 = espacioLibre(startPart,mbrTemp.mbr_tamano);

                            //ver cual espacio es mas grande
                            int aComp = masGrande(esp1, esp2, esp3, esp4);
                            if(aComp == esp1)
                            {
                                if(esp1>=tamanoParticion)
                                {

                                    //seteando los valores que llevara la nueva particion
                                    particionTemp.part_status = '1';//particion ocupada
                                    particionTemp.part_type = 'P';
                                    strcpy(particionTemp.part_name,disco->getName().c_str());
                                    particionTemp.part_size= tamanoParticion;
                                    particionTemp.part_start=startEspAsig1;
                                    //copiando particion creada
                                    mbrTemp.mbr_partition_1 =particionTemp;
                                    //toca modificar el mbr original en el archivo por el nuevo mbr con los datos de la particion creada
                                    fseek(arch,0,SEEK_SET);
                                    fwrite(&mbrTemp,sizeof(MBR),1,arch); //se sobreescribe el mbr
                                    fclose(arch);
                                    cout<<"Se Creo la particion Correctamente"<<endl;


                                }
                                else
                                {
                                    cout<<"No hay espacio suficiente en el disco"<<endl;
                                }
                            }
                            else if(aComp == esp2)
                            {
                                if(esp2>=tamanoParticion)
                                {

                                    //seteando los valores que llevara la nueva particion
                                    particionTemp.part_status = '1';//particion ocupada
                                    particionTemp.part_type = 'P';
                                    strcpy(particionTemp.part_name,disco->getName().c_str());
                                    particionTemp.part_size= tamanoParticion;
                                    particionTemp.part_start=startEspAsig2;
                                    //copiando particion creada
                                    mbrTemp.mbr_partition_1 =particionTemp;
                                    //toca modificar el mbr original en el archivo por el nuevo mbr con los datos de la particion creada
                                    fseek(arch,0,SEEK_SET);
                                    fwrite(&mbrTemp,sizeof(MBR),1,arch); //se sobreescribe el mbr
                                    fclose(arch);
                                    cout<<"Se Creo la particion Correctamente"<<endl;


                                }
                                else
                                {
                                    cout<<"No hay espacio suficiente en el disco"<<endl;
                                }
                            }
                            else if(aComp == esp3)
                            {
                                if(esp3>=tamanoParticion)
                                {

                                    //seteando los valores que llevara la nueva particion
                                    particionTemp.part_status = '1';//particion ocupada
                                    particionTemp.part_type = 'P';
                                    strcpy(particionTemp.part_name,disco->getName().c_str());
                                    particionTemp.part_size= tamanoParticion;
                                    particionTemp.part_start=startEspAsig3;
                                    //copiando particion creada
                                    mbrTemp.mbr_partition_1 =particionTemp;
                                    //toca modificar el mbr original en el archivo por el nuevo mbr con los datos de la particion creada
                                    fseek(arch,0,SEEK_SET);
                                    fwrite(&mbrTemp,sizeof(MBR),1,arch); //se sobreescribe el mbr
                                    fclose(arch);
                                    cout<<"Se Creo la particion Correctamente"<<endl;


                                }
                                else
                                {
                                    cout<<"No hay espacio suficiente en el disco"<<endl;
                                }
                            }
                            else
                            {
                                if(esp4>=tamanoParticion)
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

                                }
                                else
                                {
                                    cout<<"No hay espacio suficiente en el disco"<<endl;
                                }
                            }

                        }
                    }

                }
                else   //Aqui quiere decir que tiene el B ajuste (Mejor ajuste)
                {
                    int esp1 = 0;
                    int startEspAsig1 = 0;
                    int startEsp1 = 0;
                    int esp2 = 0;
                    int startEspAsig2 = 0;
                    int startEsp2 = 0;
                    int esp3 = 0;
                    int startEspAsig3 = 0;
                    int startEsp3 = 0;
                    int esp4 = 0;
                    int startEsp4 = 0;
                    //verificar si hay mas particiones ocupadas
                    if(mbrTemp.mbr_partition_2.part_status == '1')
                    {
                        startEsp1=mbrTemp.mbr_partition_2.part_start;
                    }
                    if(mbrTemp.mbr_partition_3.part_status == '1')
                    {
                        startEsp2=mbrTemp.mbr_partition_3.part_start;
                        //esp2=espacioLibre(startPart,mbrTemp.mbr_partition_3.part_start);
                    }
                    if(mbrTemp.mbr_partition_4.part_status == '1')
                    {
                        startEsp3=mbrTemp.mbr_partition_4.part_start;
                        //esp3=espacioLibre(startPart,mbrTemp.mbr_partition_4.part_start);
                    }

                    //revisa cual start es mas pequeño y quiere decir que esta primero en el disco
                    if(startEsp1< startEsp2&& startEsp1< startEsp3 && startEsp1!=0)
                    {
                        esp1=espacioLibre(startPart,startEsp1);
                        startEspAsig1 = startPart;
                        startPart=(startEsp1 + mbrTemp.mbr_partition_2.part_size);

                        if(startEsp2< startEsp3)
                        {
                            //startEsp3 es el mas grande
                            //veo el espacio entre startEsp2 y startEsp1
                            esp2=espacioLibre(startPart,startEsp2);
                            startEspAsig2 = startPart;
                            startPart=(startEsp2 + mbrTemp.mbr_partition_3.part_size);
                            //veo el espacio entre startEsp3 y starEsp2
                            esp3=espacioLibre(startPart,startEsp3);
                            startEspAsig3 = startPart;
                            startPart=(startEsp1 + mbrTemp.mbr_partition_4.part_size);
                            startEsp4 = startPart;
                            //si queda espacio al final del disco despues de startEsp3
                            esp4 = espacioLibre(startPart,mbrTemp.mbr_tamano);

                            //ver cual espacio es mas grande
                            int aComp = mejorAsign(esp1, esp2, esp3, esp4, tamanoParticion);
                            if(aComp == esp1)
                            {
                                if(esp1>=tamanoParticion)
                                {

                                    //seteando los valores que llevara la nueva particion
                                    particionTemp.part_status = '1';//particion ocupada
                                    particionTemp.part_type = 'P';
                                    strcpy(particionTemp.part_name,disco->getName().c_str());
                                    particionTemp.part_size= tamanoParticion;
                                    particionTemp.part_start=startEspAsig1;
                                    //copiando particion creada
                                    mbrTemp.mbr_partition_1 =particionTemp;
                                    //toca modificar el mbr original en el archivo por el nuevo mbr con los datos de la particion creada
                                    fseek(arch,0,SEEK_SET);
                                    fwrite(&mbrTemp,sizeof(MBR),1,arch); //se sobreescribe el mbr
                                    fclose(arch);
                                    cout<<"Se Creo la particion Correctamente"<<endl;


                                }
                                else
                                {
                                    cout<<"No hay espacio suficiente en el disco"<<endl;
                                }
                            }
                            else if(aComp == esp2)
                            {
                                if(esp2>=tamanoParticion)
                                {

                                    //seteando los valores que llevara la nueva particion
                                    particionTemp.part_status = '1';//particion ocupada
                                    particionTemp.part_type = 'P';
                                    strcpy(particionTemp.part_name,disco->getName().c_str());
                                    particionTemp.part_size= tamanoParticion;
                                    particionTemp.part_start=startEspAsig2;
                                    //copiando particion creada
                                    mbrTemp.mbr_partition_1 =particionTemp;
                                    //toca modificar el mbr original en el archivo por el nuevo mbr con los datos de la particion creada
                                    fseek(arch,0,SEEK_SET);
                                    fwrite(&mbrTemp,sizeof(MBR),1,arch); //se sobreescribe el mbr
                                    fclose(arch);
                                    cout<<"Se Creo la particion Correctamente"<<endl;


                                }
                                else
                                {
                                    cout<<"No hay espacio suficiente en el disco"<<endl;
                                }
                            }
                            else if(aComp == esp3)
                            {
                                if(esp3>=tamanoParticion)
                                {

                                    //seteando los valores que llevara la nueva particion
                                    particionTemp.part_status = '1';//particion ocupada
                                    particionTemp.part_type = 'P';
                                    strcpy(particionTemp.part_name,disco->getName().c_str());
                                    particionTemp.part_size= tamanoParticion;
                                    particionTemp.part_start=startEspAsig3;
                                    //copiando particion creada
                                    mbrTemp.mbr_partition_1 =particionTemp;
                                    //toca modificar el mbr original en el archivo por el nuevo mbr con los datos de la particion creada
                                    fseek(arch,0,SEEK_SET);
                                    fwrite(&mbrTemp,sizeof(MBR),1,arch); //se sobreescribe el mbr
                                    fclose(arch);
                                    cout<<"Se Creo la particion Correctamente"<<endl;


                                }
                                else
                                {
                                    cout<<"No hay espacio suficiente en el disco"<<endl;
                                }
                            }
                            else
                            {
                                if(esp4>=tamanoParticion)
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

                                }
                                else
                                {
                                    cout<<"No hay espacio suficiente en el disco"<<endl;
                                }
                            }

                        }
                        else   //startEsp3< startEsp2
                        {
                            //startEsp2 es el mas grande
                            //veo el espacio entre startEsp3 y startEsp1
                            esp3=espacioLibre(startPart,startEsp3);
                            startEspAsig3 = startPart;
                            startPart=(startEsp3 + mbrTemp.mbr_partition_4.part_size);
                            //veo el espacio entre startEsp3 y starEsp2
                            esp2=espacioLibre(startPart,startEsp2);
                            startEspAsig2 = startPart;
                            startPart=(startEsp2 + mbrTemp.mbr_partition_3.part_size);
                            startEsp4 = startPart;
                            //si queda espacio al final del disco despues de startEsp3
                            esp4 = espacioLibre(startPart,mbrTemp.mbr_tamano);

                            //ver cual espacio es mas grande
                            int aComp = mejorAsign(esp1, esp2, esp3, esp4, tamanoParticion);
                            if(aComp == esp1)
                            {
                                if(esp1>=tamanoParticion)
                                {

                                    //seteando los valores que llevara la nueva particion
                                    particionTemp.part_status = '1';//particion ocupada
                                    particionTemp.part_type = 'P';
                                    strcpy(particionTemp.part_name,disco->getName().c_str());
                                    particionTemp.part_size= tamanoParticion;
                                    particionTemp.part_start=startEspAsig1;
                                    //copiando particion creada
                                    mbrTemp.mbr_partition_1 =particionTemp;
                                    //toca modificar el mbr original en el archivo por el nuevo mbr con los datos de la particion creada
                                    fseek(arch,0,SEEK_SET);
                                    fwrite(&mbrTemp,sizeof(MBR),1,arch); //se sobreescribe el mbr
                                    fclose(arch);
                                    cout<<"Se Creo la particion Correctamente"<<endl;


                                }
                                else
                                {
                                    cout<<"No hay espacio suficiente en el disco"<<endl;
                                }
                            }
                            else if(aComp == esp2)
                            {
                                if(esp2>=tamanoParticion)
                                {

                                    //seteando los valores que llevara la nueva particion
                                    particionTemp.part_status = '1';//particion ocupada
                                    particionTemp.part_type = 'P';
                                    strcpy(particionTemp.part_name,disco->getName().c_str());
                                    particionTemp.part_size= tamanoParticion;
                                    particionTemp.part_start=startEspAsig2;
                                    //copiando particion creada
                                    mbrTemp.mbr_partition_1 =particionTemp;
                                    //toca modificar el mbr original en el archivo por el nuevo mbr con los datos de la particion creada
                                    fseek(arch,0,SEEK_SET);
                                    fwrite(&mbrTemp,sizeof(MBR),1,arch); //se sobreescribe el mbr
                                    fclose(arch);
                                    cout<<"Se Creo la particion Correctamente"<<endl;


                                }
                                else
                                {
                                    cout<<"No hay espacio suficiente en el disco"<<endl;
                                }
                            }
                            else if(aComp == esp3)
                            {
                                if(esp3>=tamanoParticion)
                                {

                                    //seteando los valores que llevara la nueva particion
                                    particionTemp.part_status = '1';//particion ocupada
                                    particionTemp.part_type = 'P';
                                    strcpy(particionTemp.part_name,disco->getName().c_str());
                                    particionTemp.part_size= tamanoParticion;
                                    particionTemp.part_start=startEspAsig3;
                                    //copiando particion creada
                                    mbrTemp.mbr_partition_1 =particionTemp;
                                    //toca modificar el mbr original en el archivo por el nuevo mbr con los datos de la particion creada
                                    fseek(arch,0,SEEK_SET);
                                    fwrite(&mbrTemp,sizeof(MBR),1,arch); //se sobreescribe el mbr
                                    fclose(arch);
                                    cout<<"Se Creo la particion Correctamente"<<endl;


                                }
                                else
                                {
                                    cout<<"No hay espacio suficiente en el disco"<<endl;
                                }
                            }
                            else
                            {
                                if(esp4>=tamanoParticion)
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

                                }
                                else
                                {
                                    cout<<"No hay espacio suficiente en el disco"<<endl;
                                }
                            }

                        }
                    }
                    else if(startEsp2< startEsp1&& startEsp2< startEsp3 &&  startEsp2!=0 )
                    {

                        esp2=espacioLibre(startPart,startEsp2);
                        startEspAsig2 = startPart;
                        startPart=(startEsp2 + mbrTemp.mbr_partition_3.part_size);

                        if(startEsp1< startEsp3)
                        {
                            //startEsp3 es el mas grande
                            //veo el espacio entre startEsp2 y startEsp1
                            esp1=espacioLibre(startPart,startEsp1);
                            startEspAsig1 = startPart;
                            startPart=(startEsp1 + mbrTemp.mbr_partition_2.part_size);
                            //veo el espacio entre startEsp3 y starEsp2
                            esp3=espacioLibre(startPart,startEsp3);
                            startEspAsig3 = startPart;
                            startPart=(startEsp3 + mbrTemp.mbr_partition_4.part_size);
                            startEsp4 = startPart;
                            //si queda espacio al final del disco despues de startEsp3
                            esp4 = espacioLibre(startPart,mbrTemp.mbr_tamano);

                            //ver cual espacio es mas grande
                            int aComp = mejorAsign(esp1, esp2, esp3, esp4, tamanoParticion);
                            if(aComp == esp1)
                            {
                                if(esp1>=tamanoParticion)
                                {

                                    //seteando los valores que llevara la nueva particion
                                    particionTemp.part_status = '1';//particion ocupada
                                    particionTemp.part_type = 'P';
                                    strcpy(particionTemp.part_name,disco->getName().c_str());
                                    particionTemp.part_size= tamanoParticion;
                                    particionTemp.part_start=startEspAsig1;
                                    //copiando particion creada
                                    mbrTemp.mbr_partition_1 =particionTemp;
                                    //toca modificar el mbr original en el archivo por el nuevo mbr con los datos de la particion creada
                                    fseek(arch,0,SEEK_SET);
                                    fwrite(&mbrTemp,sizeof(MBR),1,arch); //se sobreescribe el mbr
                                    fclose(arch);
                                    cout<<"Se Creo la particion Correctamente"<<endl;


                                }
                                else
                                {
                                    cout<<"No hay espacio suficiente en el disco"<<endl;
                                }
                            }
                            else if(aComp == esp2)
                            {
                                if(esp2>=tamanoParticion)
                                {

                                    //seteando los valores que llevara la nueva particion
                                    particionTemp.part_status = '1';//particion ocupada
                                    particionTemp.part_type = 'P';
                                    strcpy(particionTemp.part_name,disco->getName().c_str());
                                    particionTemp.part_size= tamanoParticion;
                                    particionTemp.part_start=startEspAsig2;
                                    //copiando particion creada
                                    mbrTemp.mbr_partition_1 =particionTemp;
                                    //toca modificar el mbr original en el archivo por el nuevo mbr con los datos de la particion creada
                                    fseek(arch,0,SEEK_SET);
                                    fwrite(&mbrTemp,sizeof(MBR),1,arch); //se sobreescribe el mbr
                                    fclose(arch);
                                    cout<<"Se Creo la particion Correctamente"<<endl;


                                }
                                else
                                {
                                    cout<<"No hay espacio suficiente en el disco"<<endl;
                                }
                            }
                            else if(aComp == esp3)
                            {
                                if(esp3>=tamanoParticion)
                                {

                                    //seteando los valores que llevara la nueva particion
                                    particionTemp.part_status = '1';//particion ocupada
                                    particionTemp.part_type = 'P';
                                    strcpy(particionTemp.part_name,disco->getName().c_str());
                                    particionTemp.part_size= tamanoParticion;
                                    particionTemp.part_start=startEspAsig3;
                                    //copiando particion creada
                                    mbrTemp.mbr_partition_1 =particionTemp;
                                    //toca modificar el mbr original en el archivo por el nuevo mbr con los datos de la particion creada
                                    fseek(arch,0,SEEK_SET);
                                    fwrite(&mbrTemp,sizeof(MBR),1,arch); //se sobreescribe el mbr
                                    fclose(arch);
                                    cout<<"Se Creo la particion Correctamente"<<endl;


                                }
                                else
                                {
                                    cout<<"No hay espacio suficiente en el disco"<<endl;
                                }
                            }
                            else
                            {
                                if(esp4>=tamanoParticion)
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

                                }
                                else
                                {
                                    cout<<"No hay espacio suficiente en el disco"<<endl;
                                }
                            }

                        }
                        else   //startEsp3< startEsp1
                        {
                            //startEsp1 es el mas grande
                            //veo el espacio entre startEsp3 y startEsp1
                            esp3=espacioLibre(startPart,startEsp3);
                            startEspAsig3 = startPart;
                            startPart=(startEsp3 + mbrTemp.mbr_partition_4.part_size);
                            //veo el espacio entre startEsp3 y starEsp2
                            esp1=espacioLibre(startPart,startEsp1);
                            startEspAsig1 = startPart;
                            startPart=(startEsp1 + mbrTemp.mbr_partition_2.part_size);
                            startEsp4 = startPart;
                            //si queda espacio al final del disco despues de startEsp3
                            esp4 = espacioLibre(startPart,mbrTemp.mbr_tamano);

                            //ver cual espacio es mas grande
                            int aComp = mejorAsign(esp1, esp2, esp3, esp4,tamanoParticion);
                            if(aComp == esp1)
                            {
                                if(esp1>=tamanoParticion)
                                {

                                    //seteando los valores que llevara la nueva particion
                                    particionTemp.part_status = '1';//particion ocupada
                                    particionTemp.part_type = 'P';
                                    strcpy(particionTemp.part_name,disco->getName().c_str());
                                    particionTemp.part_size= tamanoParticion;
                                    particionTemp.part_start=startEspAsig1;
                                    //copiando particion creada
                                    mbrTemp.mbr_partition_1 =particionTemp;
                                    //toca modificar el mbr original en el archivo por el nuevo mbr con los datos de la particion creada
                                    fseek(arch,0,SEEK_SET);
                                    fwrite(&mbrTemp,sizeof(MBR),1,arch); //se sobreescribe el mbr
                                    fclose(arch);
                                    cout<<"Se Creo la particion Correctamente"<<endl;


                                }
                                else
                                {
                                    cout<<"No hay espacio suficiente en el disco"<<endl;
                                }
                            }
                            else if(aComp == esp2)
                            {
                                if(esp2>=tamanoParticion)
                                {

                                    //seteando los valores que llevara la nueva particion
                                    particionTemp.part_status = '1';//particion ocupada
                                    particionTemp.part_type = 'P';
                                    strcpy(particionTemp.part_name,disco->getName().c_str());
                                    particionTemp.part_size= tamanoParticion;
                                    particionTemp.part_start=startEspAsig2;
                                    //copiando particion creada
                                    mbrTemp.mbr_partition_1 =particionTemp;
                                    //toca modificar el mbr original en el archivo por el nuevo mbr con los datos de la particion creada
                                    fseek(arch,0,SEEK_SET);
                                    fwrite(&mbrTemp,sizeof(MBR),1,arch); //se sobreescribe el mbr
                                    fclose(arch);
                                    cout<<"Se Creo la particion Correctamente"<<endl;


                                }
                                else
                                {
                                    cout<<"No hay espacio suficiente en el disco"<<endl;
                                }
                            }
                            else if(aComp == esp3)
                            {
                                if(esp3>=tamanoParticion)
                                {

                                    //seteando los valores que llevara la nueva particion
                                    particionTemp.part_status = '1';//particion ocupada
                                    particionTemp.part_type = 'P';
                                    strcpy(particionTemp.part_name,disco->getName().c_str());
                                    particionTemp.part_size= tamanoParticion;
                                    particionTemp.part_start=startEspAsig3;
                                    //copiando particion creada
                                    mbrTemp.mbr_partition_1 =particionTemp;
                                    //toca modificar el mbr original en el archivo por el nuevo mbr con los datos de la particion creada
                                    fseek(arch,0,SEEK_SET);
                                    fwrite(&mbrTemp,sizeof(MBR),1,arch); //se sobreescribe el mbr
                                    fclose(arch);
                                    cout<<"Se Creo la particion Correctamente"<<endl;


                                }
                                else
                                {
                                    cout<<"No hay espacio suficiente en el disco"<<endl;
                                }
                            }
                            else
                            {
                                if(esp4>=tamanoParticion)
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

                                }
                                else
                                {
                                    cout<<"No hay espacio suficiente en el disco"<<endl;
                                }
                            }

                        }
                    }
                    else if(startEsp3< startEsp2&& startEsp3< startEsp1 &&  startEsp3!=0)
                    {
                        esp3=espacioLibre(startPart,startEsp3);
                        startEspAsig3 = startPart;
                        startPart=(startEsp3 + mbrTemp.mbr_partition_4.part_size);

                        if(startEsp1< startEsp2)
                        {
                            //startEsp3 es el mas grande
                            //veo el espacio entre startEsp2 y startEsp1
                            esp1=espacioLibre(startPart,startEsp1);
                            startEspAsig1 = startPart;
                            startPart=(startEsp1 + mbrTemp.mbr_partition_2.part_size);
                            //veo el espacio entre startEsp3 y starEsp2
                            esp2=espacioLibre(startPart,startEsp2);
                            startEspAsig2 = startPart;
                            startPart=(startEsp2 + mbrTemp.mbr_partition_3.part_size);
                            startEsp4 = startPart;
                            //si queda espacio al final del disco despues de startEsp3
                            esp4 = espacioLibre(startPart,mbrTemp.mbr_tamano);

                            //ver cual espacio es mas grande
                            int aComp = mejorAsign(esp1, esp2, esp3, esp4, tamanoParticion);
                            if(aComp == esp1)
                            {
                                if(esp1>=tamanoParticion)
                                {

                                    //seteando los valores que llevara la nueva particion
                                    particionTemp.part_status = '1';//particion ocupada
                                    particionTemp.part_type = 'P';
                                    strcpy(particionTemp.part_name,disco->getName().c_str());
                                    particionTemp.part_size= tamanoParticion;
                                    particionTemp.part_start=startEspAsig1;
                                    //copiando particion creada
                                    mbrTemp.mbr_partition_1 =particionTemp;
                                    //toca modificar el mbr original en el archivo por el nuevo mbr con los datos de la particion creada
                                    fseek(arch,0,SEEK_SET);
                                    fwrite(&mbrTemp,sizeof(MBR),1,arch); //se sobreescribe el mbr
                                    fclose(arch);
                                    cout<<"Se Creo la particion Correctamente"<<endl;


                                }
                                else
                                {
                                    cout<<"No hay espacio suficiente en el disco"<<endl;
                                }
                            }
                            else if(aComp == esp2)
                            {
                                if(esp2>=tamanoParticion)
                                {

                                    //seteando los valores que llevara la nueva particion
                                    particionTemp.part_status = '1';//particion ocupada
                                    particionTemp.part_type = 'P';
                                    strcpy(particionTemp.part_name,disco->getName().c_str());
                                    particionTemp.part_size= tamanoParticion;
                                    particionTemp.part_start=startEspAsig2;
                                    //copiando particion creada
                                    mbrTemp.mbr_partition_1 =particionTemp;
                                    //toca modificar el mbr original en el archivo por el nuevo mbr con los datos de la particion creada
                                    fseek(arch,0,SEEK_SET);
                                    fwrite(&mbrTemp,sizeof(MBR),1,arch); //se sobreescribe el mbr
                                    fclose(arch);
                                    cout<<"Se Creo la particion Correctamente"<<endl;


                                }
                                else
                                {
                                    cout<<"No hay espacio suficiente en el disco"<<endl;
                                }
                            }
                            else if(aComp == esp3)
                            {
                                if(esp3>=tamanoParticion)
                                {

                                    //seteando los valores que llevara la nueva particion
                                    particionTemp.part_status = '1';//particion ocupada
                                    particionTemp.part_type = 'P';
                                    strcpy(particionTemp.part_name,disco->getName().c_str());
                                    particionTemp.part_size= tamanoParticion;
                                    particionTemp.part_start=startEspAsig3;
                                    //copiando particion creada
                                    mbrTemp.mbr_partition_1 =particionTemp;
                                    //toca modificar el mbr original en el archivo por el nuevo mbr con los datos de la particion creada
                                    fseek(arch,0,SEEK_SET);
                                    fwrite(&mbrTemp,sizeof(MBR),1,arch); //se sobreescribe el mbr
                                    fclose(arch);
                                    cout<<"Se Creo la particion Correctamente"<<endl;


                                }
                                else
                                {
                                    cout<<"No hay espacio suficiente en el disco"<<endl;
                                }
                            }
                            else
                            {
                                if(esp4>=tamanoParticion)
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

                                }
                                else
                                {
                                    cout<<"No hay espacio suficiente en el disco"<<endl;
                                }
                            }

                        }
                        else   //startEsp2< startEsp1
                        {
                            //startEsp1 es el mas grande
                            //veo el espacio entre startEsp3 y startEsp1
                            esp2=espacioLibre(startPart,startEsp2);
                            startEspAsig2 = startPart;
                            startPart=(startEsp2 + mbrTemp.mbr_partition_3.part_size);
                            //veo el espacio entre startEsp3 y starEsp2
                            esp1=espacioLibre(startPart,startEsp1);
                            startEspAsig1 = startPart;
                            startPart=(startEsp1 + mbrTemp.mbr_partition_2.part_size);
                            startEsp4 = startPart;
                            //si queda espacio al final del disco despues de startEsp3
                            esp4 = espacioLibre(startPart,mbrTemp.mbr_tamano);

                            //ver cual espacio es mas grande
                            int aComp = mejorAsign(esp1, esp2, esp3, esp4,tamanoParticion);
                            if(aComp == esp1)
                            {
                                if(esp1>=tamanoParticion)
                                {

                                    //seteando los valores que llevara la nueva particion
                                    particionTemp.part_status = '1';//particion ocupada
                                    particionTemp.part_type = 'P';
                                    strcpy(particionTemp.part_name,disco->getName().c_str());
                                    particionTemp.part_size= tamanoParticion;
                                    particionTemp.part_start=startEspAsig1;
                                    //copiando particion creada
                                    mbrTemp.mbr_partition_1 =particionTemp;
                                    //toca modificar el mbr original en el archivo por el nuevo mbr con los datos de la particion creada
                                    fseek(arch,0,SEEK_SET);
                                    fwrite(&mbrTemp,sizeof(MBR),1,arch); //se sobreescribe el mbr
                                    fclose(arch);
                                    cout<<"Se Creo la particion Correctamente"<<endl;


                                }
                                else
                                {
                                    cout<<"No hay espacio suficiente en el disco"<<endl;
                                }
                            }
                            else if(aComp == esp2)
                            {
                                if(esp2>=tamanoParticion)
                                {

                                    //seteando los valores que llevara la nueva particion
                                    particionTemp.part_status = '1';//particion ocupada
                                    particionTemp.part_type = 'P';
                                    strcpy(particionTemp.part_name,disco->getName().c_str());
                                    particionTemp.part_size= tamanoParticion;
                                    particionTemp.part_start=startEspAsig2;
                                    //copiando particion creada
                                    mbrTemp.mbr_partition_1 =particionTemp;
                                    //toca modificar el mbr original en el archivo por el nuevo mbr con los datos de la particion creada
                                    fseek(arch,0,SEEK_SET);
                                    fwrite(&mbrTemp,sizeof(MBR),1,arch); //se sobreescribe el mbr
                                    fclose(arch);
                                    cout<<"Se Creo la particion Correctamente"<<endl;


                                }
                                else
                                {
                                    cout<<"No hay espacio suficiente en el disco"<<endl;
                                }
                            }
                            else if(aComp == esp3)
                            {
                                if(esp3>=tamanoParticion)
                                {

                                    //seteando los valores que llevara la nueva particion
                                    particionTemp.part_status = '1';//particion ocupada
                                    particionTemp.part_type = 'P';
                                    strcpy(particionTemp.part_name,disco->getName().c_str());
                                    particionTemp.part_size= tamanoParticion;
                                    particionTemp.part_start=startEspAsig3;
                                    //copiando particion creada
                                    mbrTemp.mbr_partition_1 =particionTemp;
                                    //toca modificar el mbr original en el archivo por el nuevo mbr con los datos de la particion creada
                                    fseek(arch,0,SEEK_SET);
                                    fwrite(&mbrTemp,sizeof(MBR),1,arch); //se sobreescribe el mbr
                                    fclose(arch);
                                    cout<<"Se Creo la particion Correctamente"<<endl;


                                }
                                else
                                {
                                    cout<<"No hay espacio suficiente en el disco"<<endl;
                                }
                            }
                            else
                            {
                                if(esp4>=tamanoParticion)
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

                                }
                                else
                                {
                                    cout<<"No hay espacio suficiente en el disco"<<endl;
                                }
                            }

                        }
                    }
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

    imprimirDatosDisco(disco->getPath());
}


void fdisk::eliminarParticion(string path,string nombre, string tipoDelete)
{
}
void fdisk::agregarEspacioParticion()
{
}

bool fdisk::hayEspacio(int tamaParticion, int tamanoDisco)
{
    if(tamaParticion>tamanoDisco||tamaParticion<0)
    {
        return false;
    }
    return true;
}

int fdisk::masPequeño(int uno, int dos, int tres, int cuatro){
    if(uno<dos && uno < tres && uno < cuatro && uno != 0)
    {
        return uno;
    }
    if(dos<uno && dos < tres && dos < cuatro && dos != 0)
    {
        return dos;
    }
    if(tres<dos && tres < uno && tres < cuatro && tres !=0)
    {
        return tres;
    }
    if(cuatro<dos && cuatro < tres && cuatro < uno && cuatro !=0)
    {
        return cuatro;
    }
}
int fdisk::mejorAsign(int uno, int dos, int tres, int cuatro, int tamParti){
    int comp1 = 0;
    int comp2 = 0;
    int comp3 = 0;
    int comp4 = 0;

    comp1 = uno - tamParti;
    comp2 = dos - tamParti;
    comp3 = tres - tamParti;
    comp4 = cuatro - tamParti;

    if(comp1>comp2 && comp1 > comp3 && comp1 > comp4 && comp1>=0)
    {
        return uno;
    }
    if(comp2>comp1 && comp2 > comp3 && comp2 > comp4 && comp2>=0)
    {
        return dos;
    }
    if(comp3>comp2 && comp3 > comp1 && comp3 > comp4 && comp3>=0)
    {
        return tres;
    }
    if(comp4>comp2 && comp4 > comp3 && comp4 > comp1 && comp4>=0)
    {
        return cuatro;
    }
}

int fdisk::masGrande(int uno, int dos, int tres, int cuatro)
{
    if(uno>dos && uno > tres && uno > cuatro)
    {
        return uno;
    }
    if(dos>uno && dos > tres && dos > cuatro)
    {
        return dos;
    }
    if(tres>dos && tres > uno && tres > cuatro)
    {
        return tres;
    }
    if(cuatro>dos && cuatro > tres && cuatro > uno)
    {
        return cuatro;
    }
}

int fdisk::espacioLibre(int inicio, int fin)
{
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


