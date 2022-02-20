#ifndef MKUSR_H
#define MKUSR_H
#include "mount.h"
#include <vector>
#include "estructuras.h"


class mkusr
{
    public:
        mkusr();
        virtual ~mkusr();
        void ejecutarMkusr(mkusr *usua,mount []);
        vector<string> split(string str, char pattern);
        void returnDatosPart(MBR mbrTemp, string pathD,string nombrePart,int &tamPart, int &iniPart);
        void modificarArchivo(string pathDisco, string nombrePart, string nombreUser);
        //comando para eliminar usuarios
        void ejecutarRmusr(mkusr *grupo,mount []);

        void setUsuario(string s){
            usuario = s;
        }
        void setPassword(string s){
            password = s;
        }
        string getUsuario(){
            return usuario;
        }
        string getPassword(){
            return password;
        }
    private:
        string usuario;
        string password;
};

#endif // MKUSR_H
