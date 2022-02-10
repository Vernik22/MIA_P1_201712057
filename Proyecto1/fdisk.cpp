#include "fdisk.h"
#include <iostream>
#include <string.h>
#include <sys/stat.h>
#include <algorithm>
#include <stdio.h>

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
//Verificar que no exista el nombre en las particiones principales

    Particion parts[4] ;

    //Ver el tipo de particion a crear si es Primaria, Extendida o Logica
    if(disco->getType()=="p"||disco->getType()=="P")
    {
        FILE *arch;
        arch = fopen(disco->getPath().c_str(), "rb+"); // se abre el archivo del disco en modo lectura
        if (arch != NULL)
        {
            fseek(arch,0,SEEK_SET);
            fread(&mbrTemp, sizeof(MBR),1,arch); //mbrTemp guarda el MBR del disco a formatear
            char Dfit = mbrTemp.dsk_fit;

            if(mbrTemp.mbr_partition_1.part_name != disco->getName()&& mbrTemp.mbr_partition_2.part_name != disco->getName()&& mbrTemp.mbr_partition_3.part_name != disco->getName()&& mbrTemp.mbr_partition_4.part_name != disco->getName())
            {
                if(hayEspacio(tamanoParticion,mbrTemp.mbr_tamano))
                {
                    //hay particiones disponibles
                    if(mbrTemp.mbr_partition_1.part_status == '0'|| mbrTemp.mbr_partition_2.part_status == '0'|| mbrTemp.mbr_partition_3.part_status == '0'|| mbrTemp.mbr_partition_4.part_status == '0')
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
                        else
                        {
                            parts[0]= mbrTemp.mbr_partition_1;
                            parts[1]= mbrTemp.mbr_partition_2;
                            parts[2]= mbrTemp.mbr_partition_3;
                            parts[3]= mbrTemp.mbr_partition_4;
                            int partEnd = 0;

                            for(int i= 0; i<4; i++)
                            {
                                if(parts[i].part_status == '1')
                                {

                                    partEnd = parts[i].part_start;
                                    int espL = espacioLibre(startPart,partEnd);
                                    if(hayEspacio(tamanoParticion,espL))
                                    {
                                        //seteando los valores que llevara la nueva particion
                                        particionTemp.part_status = '1';//particion ocupada
                                        particionTemp.part_type = 'P';
                                        strcpy(particionTemp.part_name,disco->getName().c_str());
                                        particionTemp.part_size= tamanoParticion;
                                        particionTemp.part_start=startPart;
                                        break;
                                    }
                                    else
                                    {
                                        startPart = parts[i].part_start + parts[i].part_size;
                                    }
                                }
                                else
                                {
                                    int espL = espacioLibre(startPart,mbrTemp.mbr_tamano);
                                    if(hayEspacio(tamanoParticion,espL))

                                    {
                                        //seteando los valores que llevara la nueva particion
                                        particionTemp.part_status = '1';//particion ocupada
                                        particionTemp.part_type = 'P';
                                        strcpy(particionTemp.part_name,disco->getName().c_str());
                                        particionTemp.part_size= tamanoParticion;
                                        particionTemp.part_start=startPart;
                                        break;

                                    }
                                    else
                                    {
                                        cout<<"ERROR: No hay espacio suficiente en el disco para crear la particion"<<endl;
                                    }

                                }

                            }

                            for(int i =0; i<4; i++)
                            {

                                if(parts[i].part_status == '0')
                                {
                                    parts[i] =particionTemp;


                                    break;
                                }
                            }
                            mbrTemp.mbr_partition_1= parts[0];
                            mbrTemp.mbr_partition_2= parts[1];
                            mbrTemp.mbr_partition_3= parts[2];
                            mbrTemp.mbr_partition_4= parts[3];
                            //toca modificar el mbr original en el archivo por el nuevo mbr con los datos de la particion creada
                            fseek(arch,0,SEEK_SET);
                            fwrite(&mbrTemp,sizeof(MBR),1,arch); //se sobreescribe el mbr
                            fclose(arch);
                            cout<<"Se Creo la particion Correctamente"<<endl;

                        }
                    }
                    else
                    {
                        cout<<"ERROR: Ya no se pueden crear mas particiones primarias; hay 4 particiones creadas"<<endl;
                    }

                }
                else
                {
                    cout<<"ERROR: No hay espacio suficiente en el disco para crear la particion"<<endl;
                }
            }
            else
            {
                cout<<"ERROR: El nombre de la particion ya existe, intente de nuevo"<<endl;
            }

        }
        else
        {
            cout<<"ERROR: Disco no encontrado en la ruta: "<<disco->getPath()<<endl;
        }
    }
    else if(disco->getType()=="e"||disco->getType()=="E")
    {
        FILE *arch;
        arch = fopen(disco->getPath().c_str(), "rb+"); // se abre el archivo del disco en modo lectura
        if (arch != NULL)
        {
            fseek(arch,0,SEEK_SET);
            fread(&mbrTemp, sizeof(MBR),1,arch); //mbrTemp guarda el MBR del disco a formatear
            char Dfit = mbrTemp.dsk_fit;
            if(mbrTemp.mbr_partition_1.part_name != disco->getName()&& mbrTemp.mbr_partition_2.part_name != disco->getName()&& mbrTemp.mbr_partition_3.part_name != disco->getName()&& mbrTemp.mbr_partition_4.part_name != disco->getName())
            {
                if(hayEspacio(tamanoParticion,mbrTemp.mbr_tamano))
                {
                    //hay particiones disponibles
                    if(mbrTemp.mbr_partition_1.part_status == '0'|| mbrTemp.mbr_partition_2.part_status == '0'|| mbrTemp.mbr_partition_3.part_status == '0'|| mbrTemp.mbr_partition_4.part_status == '0')
                    {
                        if(mbrTemp.mbr_partition_1.part_type != 'E'&& mbrTemp.mbr_partition_2.part_type != 'E'&& mbrTemp.mbr_partition_3.part_type != 'E'&& mbrTemp.mbr_partition_4.part_type != 'E')
                        {
                            //Creo un EBR vacio para colocarlo en la particion
                            EBR vacia;
                            vacia.part_fit= '-';
                            vacia.part_name[0] = '\0';
                            vacia.part_next = -1;
                            vacia.part_size = -1;
                            vacia.part_start = -1;
                            vacia.part_status = '0';
                            //si hay espacio suficiente en el disco entra
                            //verificar si hay particiones disponibles
                            //caso base si no hay ninguna particion ocupada
                            if(mbrTemp.mbr_partition_1.part_status == '0'&& mbrTemp.mbr_partition_2.part_status == '0'&& mbrTemp.mbr_partition_3.part_status == '0'&& mbrTemp.mbr_partition_4.part_status == '0')
                            {
                                //seteando los valores que llevara la nueva particion
                                particionTemp.part_status = '1';//particion ocupada
                                particionTemp.part_type = 'E';
                                strcpy(particionTemp.part_name,disco->getName().c_str());
                                particionTemp.part_size= tamanoParticion;
                                particionTemp.part_start=startPart;
                                //copiando particion creada
                                mbrTemp.mbr_partition_1 =particionTemp;
                                //toca modificar el mbr original en el archivo por el nuevo mbr con los datos de la particion creada
                                fseek(arch,0,SEEK_SET);
                                fwrite(&mbrTemp,sizeof(MBR),1,arch); //se sobreescribe el mbr
                                //escribir el EBR dentro de la particion Extendida creada
                                fseek(arch,startPart,SEEK_SET);
                                fwrite(&vacia,sizeof(EBR),1,arch); //se sobreescribe el mbr
                                fclose(arch);
                                cout<<"Se Creo la particion Correctamente"<<endl;

                            }
                            else
                            {
                                parts[0]= mbrTemp.mbr_partition_1;
                                parts[1]= mbrTemp.mbr_partition_2;
                                parts[2]= mbrTemp.mbr_partition_3;
                                parts[3]= mbrTemp.mbr_partition_4;
                                int partEnd = 0;

                                for(int i= 0; i<4; i++)
                                {
                                    if(parts[i].part_status == '1')
                                    {

                                        partEnd = parts[i].part_start;
                                        int espL = espacioLibre(startPart,partEnd);
                                        if(hayEspacio(tamanoParticion,espL))
                                        {
                                            //seteando los valores que llevara la nueva particion
                                            particionTemp.part_status = '1';//particion ocupada
                                            particionTemp.part_type = 'E';
                                            strcpy(particionTemp.part_name,disco->getName().c_str());
                                            particionTemp.part_size= tamanoParticion;
                                            particionTemp.part_start=startPart;
                                            break;
                                        }
                                        else
                                        {
                                            startPart = parts[i].part_start + parts[i].part_size;
                                        }
                                    }
                                    else
                                    {
                                        int espL = espacioLibre(startPart,mbrTemp.mbr_tamano);
                                        if(hayEspacio(tamanoParticion,espL))

                                        {
                                            //seteando los valores que llevara la nueva particion
                                            particionTemp.part_status = '1';//particion ocupada
                                            particionTemp.part_type = 'E';
                                            strcpy(particionTemp.part_name,disco->getName().c_str());
                                            particionTemp.part_size= tamanoParticion;
                                            particionTemp.part_start=startPart;
                                            break;

                                        }
                                        else
                                        {
                                            cout<<"ERROR: No hay espacio suficiente en el disco para crear la particion"<<endl;
                                        }

                                    }

                                }

                                for(int i =0; i<4; i++)
                                {

                                    if(parts[i].part_status == '0')
                                    {
                                        //Busca una particion vacia entre las 4 disponibles
                                        parts[i] =particionTemp;
                                        startPart=particionTemp.part_start;

                                        break;
                                    }
                                }
                                mbrTemp.mbr_partition_1= parts[0];
                                mbrTemp.mbr_partition_2= parts[1];
                                mbrTemp.mbr_partition_3= parts[2];
                                mbrTemp.mbr_partition_4= parts[3];
                                //toca modificar el mbr original en el archivo por el nuevo mbr con los datos de la particion creada
                                fseek(arch,0,SEEK_SET);
                                fwrite(&mbrTemp,sizeof(MBR),1,arch); //se sobreescribe el mbr
                                //Escribir el EBR vacio dentro de la particion Extendida
                                fseek(arch,startPart,SEEK_SET);
                                fwrite(&vacia,sizeof(EBR),1,arch);
                                fclose(arch);
                                cout<<"Se Creo la particion Correctamente"<<endl;

                            }
                        }
                        else
                        {
                            cout<<"ERROR: Ya existe una particion Extendida"<<endl;
                        }
                    }
                    else
                    {
                        cout<<"ERROR: Ya no se pueden crear mas particiones; hay 4 particiones creadas"<<endl;
                    }

                }
                else
                {
                    cout<<"ERROR: No hay espacio suficiente en el disco para crear la particion"<<endl;
                }
            }
            else
            {
                cout<<"ERROR: El nombre de la particion ya existe, intente de nuevo"<<endl;
            }

        }
        else
        {
            cout<<"ERROR: Disco no encontrado en la ruta: "<<disco->getPath()<<endl;
        }
    }
    else if(disco->getType()=="l"||disco->getType()=="L")
    {
        FILE *arch;
        arch = fopen(disco->getPath().c_str(), "rb+"); // se abre el archivo del disco en modo lectura
        if (arch != NULL)
        {

            fseek(arch,0,SEEK_SET);
            fread(&mbrTemp, sizeof(MBR),1,arch); //mbrTemp guarda el MBR del disco a formatear
            char Dfit = mbrTemp.dsk_fit;
            if(mbrTemp.mbr_partition_1.part_name != disco->getName()&& mbrTemp.mbr_partition_2.part_name != disco->getName()&& mbrTemp.mbr_partition_3.part_name != disco->getName()&& mbrTemp.mbr_partition_4.part_name != disco->getName())
            {

                if(mbrTemp.mbr_partition_1.part_type == 'E'|| mbrTemp.mbr_partition_2.part_type == 'E'|| mbrTemp.mbr_partition_3.part_type == 'E'|| mbrTemp.mbr_partition_4.part_type == 'E')
                {
                    parts[0]= mbrTemp.mbr_partition_1;
                    parts[1]= mbrTemp.mbr_partition_2;
                    parts[2]= mbrTemp.mbr_partition_3;
                    parts[3]= mbrTemp.mbr_partition_4;
                    EBR ebrExTemp;
                    EBR ebrExTemp2;
                    int logPartStart = sizeof(EBR);
                    int exPartStart = 0;
                    int tamParticionEx = 0;
                    for(int i=0; i<4; i++)
                    {
                        if(parts[i].part_type == 'E')
                        {
                            tamParticionEx = parts[i].part_size;
                            logPartStart += parts[i].part_start;
                            exPartStart = parts[i].part_start;
                            fseek(arch,parts[i].part_start,SEEK_SET);
                            fread(&ebrExTemp, sizeof(EBR),1,arch); //partExTemp guarda el EBR de la particion E
                            break;
                        }
                    }
                    ebrExTemp2 = ebrExTemp;
                    //verificar que no existe el nombre de la particion a crear
                    EBR ebr;
                    ebr=ebrExTemp;
                    bool hayNombre= false;
                    while(ebr.part_next != -1)
                    {
                        if(ebr.part_name == disco->getName())
                        {
                            hayNombre=true;
                        }
                        fseek(arch, ebr.part_next, SEEK_SET);
                        fread(&ebr, sizeof(EBR), 1, arch);
                    }


                    if(!hayNombre)
                    {
                        if(hayEspacio(tamanoParticion,tamParticionEx))
                        {

                            //si hay espacio suficiente en el disco entra
                            //verificar si hay particiones disponibles
                            //caso base si no hay ninguna particion ocupada
                            int tamTemp = sizeof(EBR);
                            EBR vacia;
                            vacia.part_fit= '-';
                            vacia.part_name[0] = '\0';
                            vacia.part_next = -1;
                            vacia.part_size = -1;
                            vacia.part_start = -1;
                            vacia.part_status = '0';
                            while(tamTemp<tamParticionEx)
                            {

                                if(ebrExTemp.part_status == '0'&& ebrExTemp.part_next == -1)
                                {
                                    ebrExTemp.part_fit= particionTemp.part_fit;
                                    strcpy(ebrExTemp.part_name,disco->getName().c_str());
                                    ebrExTemp.part_next = logPartStart + tamanoParticion;
                                    ebrExTemp.part_size = tamanoParticion;
                                    ebrExTemp.part_start = logPartStart;
                                    ebrExTemp.part_status = '1';

                                    fseek(arch,exPartStart,SEEK_SET);
                                    fwrite(&ebrExTemp,sizeof(EBR),1,arch);
                                    //Creo un ebr vacio en el next de la que acabo de crear
                                    fseek(arch,ebrExTemp.part_next,SEEK_SET);
                                    fwrite(&vacia,sizeof(EBR),1,arch);
                                    fclose(arch);
                                    cout<<"Se Creo la particion Correctamente"<<endl;
                                    break;

                                }
                                else if(ebrExTemp.part_status == '0'&& ebrExTemp.part_next != -1)
                                {
                                    int spac = espacioLibre(ebrExTemp2.part_start,ebrExTemp.part_next);
                                    if(hayEspacio(tamanoParticion,spac))
                                    {
                                        ebrExTemp.part_fit= particionTemp.part_fit;
                                        strcpy(ebrExTemp.part_name,disco->getName().c_str());
                                        ebrExTemp.part_next = logPartStart + tamanoParticion;
                                        ebrExTemp.part_size = tamanoParticion;
                                        ebrExTemp.part_start = logPartStart;
                                        ebrExTemp.part_status = '1';

                                        fseek(arch,exPartStart,SEEK_SET);
                                        fwrite(&ebrExTemp,sizeof(EBR),1,arch);
                                        //Creo un ebr vacio en el next de la que acabo de crear
                                        fseek(arch,ebrExTemp.part_next,SEEK_SET);
                                        fwrite(&vacia,sizeof(EBR),1,arch);
                                        fclose(arch);
                                        cout<<"Se Creo la particion Correctamente"<<endl;
                                        break;

                                    }
                                    else
                                    {
                                        ebrExTemp2=ebrExTemp;
                                        fseek(arch,ebrExTemp.part_next,SEEK_SET);
                                        fread(&ebrExTemp, sizeof(EBR),1,arch);
                                        tamTemp+= ebrExTemp2.part_next;
                                        logPartStart= ebrExTemp2.part_next+sizeof(EBR);
                                        exPartStart = ebrExTemp2.part_next;
                                    }

                                }/*
                        else if (ebrExTemp.part_status == '1'&& ebrExTemp.part_next == -1)
                        {


                        }*/
                                else if(ebrExTemp.part_status == '1'&& ebrExTemp.part_next != -1)
                                {
                                    ebrExTemp2=ebrExTemp;
                                    fseek(arch,ebrExTemp.part_next,SEEK_SET);
                                    fread(&ebrExTemp, sizeof(EBR),1,arch);
                                    tamTemp+= ebrExTemp2.part_next;
                                    logPartStart= ebrExTemp2.part_next+sizeof(EBR);
                                    exPartStart = ebrExTemp2.part_next;
                                }


                            }
                        }
                        else
                        {
                            cout<<"ERROR: No hay espacio suficiente en la particion E para crear la particion L"<<endl;
                        }
                    }
                    else
                    {
                        cout<<"ERROR: El nombre de la particion ya existe, intente de nuevo"<<endl;

                    }



                }
                else
                {
                    cout<<"ERROR: NO hay particion Extendida"<<endl;
                }
            }
            else
            {
                cout<<"ERROR: El nombre de la particion ya existe, intente de nuevo"<<endl;
            }


        }
        else
        {
            cout<<"ERROR: Disco no encontrado en la ruta: "<<disco->getPath()<<endl;
        }

    }

//}
    imprimirDatosDisco(disco->getPath());
}


void fdisk::eliminarParticion(fdisk *disco)
{
    cout<<"\n************Eliminar Particion************\n"<<endl;
    MBR mbrTemp;
    EBR ebrExTemp;
    EBR ebrExTemp2;
    Particion parts[4];
    FILE *arch;
    arch = fopen(disco->getPath().c_str(), "rb+"); // se abre el archivo del disco en modo lectura
    if (arch != NULL)
    {
        fseek(arch,0,SEEK_SET);
        fread(&mbrTemp, sizeof(MBR),1,arch); //mbrTemp guarda el MBR del disco
        if(mbrTemp.mbr_partition_1.part_name == disco->getName()|| mbrTemp.mbr_partition_2.part_name == disco->getName()|| mbrTemp.mbr_partition_3.part_name == disco->getName()|| mbrTemp.mbr_partition_4.part_name == disco->getName())
        {


            string tipoDel = disco->getDel();
            for_each(tipoDel.begin(), tipoDel.end(), [](char &c) //convierte el texto a minisculas
            {
                c = ::tolower(c);
            });


            if(tipoDel== "fast")
            {
                parts[0]= mbrTemp.mbr_partition_1;
                parts[1]= mbrTemp.mbr_partition_2;
                parts[2]= mbrTemp.mbr_partition_3;
                parts[3]= mbrTemp.mbr_partition_4;
                Particion vacia; //creo una particion vacia para asignarala al mbr
                vacia.part_fit='-';
                vacia.part_name[0]='\0';
                vacia.part_size=-1;
                vacia.part_start=-1;
                vacia.part_status ='0';//status inactivo
                vacia.part_type='-';

                for(int i=0; i<4; i++) //recorro las particiones para verificar cual es la Extendida
                {
                    if(parts[i].part_name == disco->getName())
                    {
                        parts[i]=vacia;

                        break;
                    }
                }
                mbrTemp.mbr_partition_1 = parts[0];
                mbrTemp.mbr_partition_2 = parts[1];
                mbrTemp.mbr_partition_3 = parts[2];
                mbrTemp.mbr_partition_4 = parts[3];
                fseek(arch,0,SEEK_SET);
                fwrite(&mbrTemp, sizeof(MBR),1,arch); //partExTemp guarda el EBR de la particion E
                fclose(arch);
                cout<<"Se elimino la particion en formato fast con exito: "<<disco->getName()<<endl;

            }
            else if(tipoDel =="full")
            {
                parts[0]= mbrTemp.mbr_partition_1;
                parts[1]= mbrTemp.mbr_partition_2;
                parts[2]= mbrTemp.mbr_partition_3;
                parts[3]= mbrTemp.mbr_partition_4;
                Particion parDel;
                Particion vacia; //creo una particion vacia para asignarala al mbr
                vacia.part_fit='-';
                vacia.part_name[0]='\0';
                vacia.part_size=-1;
                vacia.part_start=-1;
                vacia.part_status ='0';//status inactivo
                vacia.part_type='-';

                for(int i=0; i<4; i++) //recorro las particiones para verificar cual es la Extendida
                {
                    if(parts[i].part_name == disco->getName())
                    {
                        parDel = parts[i];

                        break;
                    }
                }
                char buffer[1024];


                //se llena la variable buffer de ceros para que no este en null
                for(int i = 0; i<1024; i++)
                {
                    buffer[i]='\0';
                }
                //creo el disco y lo lleno con el buffer para que quede lleno de 0
                fseek(arch,parDel.part_start,SEEK_SET);
                for(int i=parDel.part_start; i<(parDel.part_start+parDel.part_size); i+=1024)
                {
                    fwrite(&buffer,1024,1,arch);
                }


                for(int i=0; i<4; i++) //recorro las particiones para verificar cual es la Extendida
                {
                    if(parts[i].part_name == disco->getName())
                    {
                        parts[i]=vacia;

                        break;
                    }
                }


                mbrTemp.mbr_partition_1 = parts[0];
                mbrTemp.mbr_partition_2 = parts[1];
                mbrTemp.mbr_partition_3 = parts[2];
                mbrTemp.mbr_partition_4 = parts[3];
                fseek(arch,0,SEEK_SET);
                fwrite(&mbrTemp, sizeof(MBR),1,arch); //partExTemp guarda el EBR de la particion E
                fclose(arch);
                cout<<"Se elimino la particion en formato full con exito: "<<disco->getName()<<endl;

            }


        }
        else if(mbrTemp.mbr_partition_1.part_type == 'E'|| mbrTemp.mbr_partition_2.part_type == 'E'|| mbrTemp.mbr_partition_3.part_type == 'E'|| mbrTemp.mbr_partition_4.part_type == 'E')
        {
            parts[0]= mbrTemp.mbr_partition_1;
            parts[1]= mbrTemp.mbr_partition_2;
            parts[2]= mbrTemp.mbr_partition_3;
            parts[3]= mbrTemp.mbr_partition_4;

            for(int i=0; i<4; i++) //recorro las particiones para verificar cual es la Extendida
            {
                if(parts[i].part_type == 'E')
                {

                    fseek(arch,parts[i].part_start,SEEK_SET);
                    fread(&ebrExTemp, sizeof(EBR),1,arch); //partExTemp guarda el EBR de la particion E
                    break;
                }
            }

            bool hayNombre= false;
            while(ebrExTemp.part_next != -1)
            {
                if(ebrExTemp.part_name == disco->getName())
                {
                    hayNombre=true;
                    break;
                }
                ebrExTemp2 = ebrExTemp;
                fseek(arch, ebrExTemp.part_next, SEEK_SET);
                fread(&ebrExTemp, sizeof(EBR), 1, arch);
            }


            if(hayNombre)
            {
                string tipoDel = disco->getDel();
                for_each(tipoDel.begin(), tipoDel.end(), [](char &c) //convierte el texto a minisculas
                {
                    c = ::tolower(c);
                });

                if(tipoDel == "fast")
                {
                    ebrExTemp2.part_next =ebrExTemp.part_next;


                    fseek(arch,(ebrExTemp2.part_start - sizeof(EBR)),SEEK_SET);
                    fwrite(&ebrExTemp2,sizeof(EBR),1,arch);
                    fclose(arch);
                    cout<<"Se elimino la particion en formato fast con exito: "<<disco->getName()<<endl;

                }
                else if(tipoDel == "full")
                {
                    ebrExTemp2.part_next =ebrExTemp.part_next;
                    char buffer = '\0';

                    for(int i=ebrExTemp2.part_next; i<ebrExTemp.part_next; i++)
                    {
                        fwrite(&buffer,1,1,arch);
                    }

                    ebrExTemp2.part_next =ebrExTemp.part_next;
                    fseek(arch,(ebrExTemp2.part_start - sizeof(EBR)),SEEK_SET);
                    fwrite(&ebrExTemp2,sizeof(EBR),1,arch);
                    fclose(arch);

                    cout<<"Se elimino la particion en formato full con exito: "<<disco->getName()<<endl;
                }

            }
            else
            {
                cout<<"ERROR: Nombre de la particion no encotrada: "<<disco->getName()<<endl;
                fclose(arch);
            }
        }
        else
        {
            cout<<"ERROR: Nombre de la particion no encotrada: "<<disco->getName()<<endl;
            fclose(arch);
        }


    }
    else
    {
        cout<<"ERROR: Disco no encontrado en la ruta: "<<disco->getPath()<<endl;
        fclose(arch);
    }

 imprimirDatosDisco(disco->getPath());
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

int fdisk::msPeq(int u, int d, int tr, int cu)
{
    if(u<d && u < tr && u < cu && u != 0)
    {
        return u;
    }
    if(d<u && d < tr && d < cu && d != 0)
    {
        return d;
    }
    if(tr<d && tr < u && tr < cu && tr !=0)
    {
        return tr;
    }
    if(cu<d && cu < tr && cu < u && cu !=0)
    {
        return cu;
    }
}
int fdisk::mejorAsign(int uno, int dos, int tres, int cuatro, int tamParti)
{
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
    if (arch != NULL)
    {
        Particion parts[4];
        MBR mbr;
        fseek(arch, 0, SEEK_SET);
        fread(&mbr, sizeof(MBR), 1, arch);
        fclose(arch);
        parts[0]= mbr.mbr_partition_1;
        parts[1]= mbr.mbr_partition_2;
        parts[2]= mbr.mbr_partition_3;
        parts[3]= mbr.mbr_partition_4;
        cout << "\n----------DATOS DEL DISCO-----\n";
        cout << "\nDISK NAME: " << mbr.mbr_dsk_signature;
        cout << "\nDISK SIZE: " << mbr.mbr_tamano;

        for(int i =0; i<4; i++)
        {
            cout << "\n\nPARTICION : " << i+1;
            cout << "\nPARTICION STATUS : " << parts[i].part_status;
            cout << "\nPARTICION TYPE : " << parts[i].part_type;
            cout << "\nPARTICION FIT : " << parts[i].part_fit;
            cout << "\nPARTICION START : " << parts[i].part_start;
            cout << "\nPARTICION SIZE : " << parts[i].part_size;
            cout << "\nPARTICION NAME : " << parts[i].part_name<<endl;

            if(parts[i].part_type == 'E')
            {
                cout << "\n----------Particiones Logicas-----\n";
                EBR ebr;
                arch = fopen(path.c_str(), "rb+");
                fseek(arch, parts[i].part_start, SEEK_SET);
                fread(&ebr, sizeof(EBR), 1, arch);

                while(ebr.part_next != -1)
                {
                    cout << "\nPARTICION STATUS : " << ebr.part_status;
                    cout << "\nPARTICION NEXT : " << ebr.part_next;
                    cout << "\nPARTICION FIT : " << ebr.part_fit;
                    cout << "\nPARTICION START : " << ebr.part_start;
                    cout << "\nPARTICION SIZE : " << ebr.part_size;
                    cout << "\nPARTICION NAME : " << ebr.part_name<<endl;
                    fseek(arch, ebr.part_next, SEEK_SET);
                    fread(&ebr, sizeof(EBR), 1, arch);
                }
                fclose(arch);
                cout << "\n----------FIN Particiones Logicas-----\n";
            }

        }


    }


}
