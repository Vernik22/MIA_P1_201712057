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
        void setUsuarioAct(User s){
            usuarioIniciado = s;
        }
        User getUsuarioAct(){
            return usuarioIniciado;
        }

    private:
        bool hayInicioSesion;
        User usuarioIniciado;
};

#endif // MANEJADOR_H
