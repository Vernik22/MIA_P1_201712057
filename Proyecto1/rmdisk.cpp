#include "rmdisk.h"
//#include <string.h>
#include <sys/stat.h>
//#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

rmdisk::rmdisk()
{
    //ctor
}

rmdisk::~rmdisk()
{
    //dtor
}

void rmdisk::ejecutarComandoRmdisk(rmdisk *disco)
{
    cout<<"************Ejecutar RMDISK************\n"<<endl;
    try
    {
        vector<string> resultados;
        resultados = split(disco->getPath(), '/');
        string pathconc = "";
        for (int i = 1; i < resultados.size(); i++)
        {
            //llenar el string con el path sin el disk.dk
            pathconc += "/\"" + resultados[i] + "\""; //le agrego comillas a los nombres por si vienen con espacios por ejemplo "mis discos"
        }
        //verificar 3 veces si el archivo existe, si el path existe, y enviar el path con comillas

        struct stat st;
        if (stat(disco->getPath().c_str(), &st) == 0)
        {
            printf("Existe el archivo en: %s\n", disco->getPath().c_str());
            string comando = "rm " + pathconc;
            system(comando.c_str());
            printf("Se borro el archivo en: %s \n", disco->getPath().c_str());
        }
        else
        {
            printf("No Existe el archivo: %s\n", disco->getPath().c_str());
        }
    }



catch(...)
{
    cout<<"Error al eliminar el disco "<<endl;
}

}

vector<string> rmdisk::split(string str, char pattern)
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
