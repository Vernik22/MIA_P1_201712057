#include "chmod.h"

chmod::chmod()
{
    //ctor
}

chmod::~chmod()
{
    //dtor
}

void chmod::ejecutarComandoChmod(chmod *permisos,mount paMoun[]){
    cout<<"\n************Ejecutar CHMOD************\n"<<endl;

    if(getDatosUsu().nombreUsuario == "root")
    {
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
        modificarArchivo(pathDisco, nombrePart,permisos);

    }
    else
    {
        cout<<"Usuario incorrecto, no es el usuario root"<<endl;
    }

}

vector<string> chmod::split(string str, char pattern)
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

void chmod::modificarArchivo(string pathDisco, string nombrePart, chmod *permisos)
{


}
