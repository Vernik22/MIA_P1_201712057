#include "mkdisk.h"
#include <iostream>
#include <time.h>
#include <string.h>
#include <cstdlib>
#include <sys/stat.h>

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
    dirExist(disco);
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

void mkdisk::dirExist(mkdisk *disco)
{

    //verificar que el directorio existe
    vector<string> resultados;
    resultados = split(disco->path, '/');
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
    if (stat(disco->path.c_str(), &st) == 0)
    {
        printf("Existe el archivo en: %s\n", disco->path.c_str());
    }else if (stat(newpath.c_str(), &st) == 0)
    {
        printf("Existe el directorio: %s\n",newpath.c_str());
    }
    else
    {
        printf("No Existe el directorio: %s\n", disco->path.c_str());
        string comando = "mkdir -p " + newpath;
        system(comando.c_str());
        printf("Se creo el dir en: %s \n", disco->path.c_str());
    }
}

vector<string> mkdisk::split(string str, char pattern)
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
