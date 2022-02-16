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

    private:
};

#endif // MANEJADOR_H
