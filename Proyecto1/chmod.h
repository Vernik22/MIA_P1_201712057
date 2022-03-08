#ifndef CHMOD_H
#define CHMOD_H
#include "mount.h"
#include <vector>
#include "estructuras.h"



class chmod
{
    public:
        chmod();
        virtual ~chmod();
        void ejecutarComandoChmod(chmod *permisos,mount []);
        vector<string> split(string str, char pattern);
        void returnDatosPart(MBR mbrTemp, string pathD,string nombrePart,int &tamPart, int &iniPart);
        void modificarArchivo(string pathDisco, string nombrePart,chmod *permisos);

        string retFecha();


        void setPath(string s){
            path=s;
        }
        string getPath(){
            return path;
        }
        void setUgo(int s){
            ugo=s;
        }
        int getUgo(){
            return ugo;
        }
        void setR(bool s){
            comR=s;
        }
        bool getR(){
            return comR;
        }
        void setDatosUsu(User s){
            datosUsuario=s;
        }
        User getDatosUsu(){
            return datosUsuario;
        }

    private:
        string path;
        int ugo;
        bool comR;
        User datosUsuario;
};

#endif // CHMOD_H
