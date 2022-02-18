#ifndef MANEJADOR_H
#define MANEJADOR_H
#include <vector>
#include "estructuras.h"
#include "mount.h"


class manejador
{
    public:
        manejador();
        virtual ~manejador();
        void leerTexto(string data, mount []);
        void listaComandosValidos(vector<Comando> &listaComandos, mount []);
        vector<string> split(string str, char pattern);

        void setHayInicioSesion(bool s){
            hayInicioSesion = s;
        }
        bool getHayInicioSesion(){
            return hayInicioSesion;
        }
        void setUsuarioAct(string s){
            usuarioActivo = s;
        }
        string getUsuarioAct(){
            return usuarioActivo;
        }
    private:
        bool hayInicioSesion;
        string usuarioActivo;
};

#endif // MANEJADOR_H
