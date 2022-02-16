#include "manejador.h"
#include <algorithm>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <cstdlib>
#include <cstring>

manejador::manejador()
{
    //ctor
}

manejador::~manejador()
{
    //dtor
}

void manejador::leerTexto(string data, mount DisksMount[]){
    vector<Comando> listaComandos;
    vector<string> lineasComando = split(data, '\n');
    Comando c;
    int sizeVec1 = lineasComando.size();
    for(int i = 0; i<sizeVec1 ; i++){
            string esComentario = lineasComando[i];
        if (esComentario[0] != '#')
        {
            string comando = lineasComando[i];
            // lo separo por espacios por ejemplo: mkdisk -path size
            vector<string> propiedades = split(comando, ' ');
            string nombreComando = propiedades[0];
            //lo convierto a minusculas
            std::for_each(nombreComando.begin(), nombreComando.end(), [](char &c)
                          { c = ::tolower(c); });
            c.Nombre = nombreComando;
            Propiedad propiedadesTemp[15];
            int sizeVecCom = propiedades.size();
            for(int j = 0; j<sizeVecCom; j ++){
                if(propiedades[j]=="" || propiedades[j]== " "||propiedades[j]== "#"){
                    continue;
                }else{
                    if(strstr(propiedades[j].c_str(), "=") != NULL){

                    }
                }
            }

        }else{
            cout<<"Es comentario"<<endl;
        }
    }
    listaComandosValidos(listaComandos, DisksMount);

}

void manejador::listaComandosValidos(vector<Comando> &listaComandos, mount DisksMount[]){

}


vector<string> manejador::split(string str, char pattern)
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
