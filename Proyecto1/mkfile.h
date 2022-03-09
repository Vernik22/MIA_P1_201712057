#ifndef MKFILE_H
#define MKFILE_H
#include "mount.h"
#include <vector>
#include "estructuras.h"


class mkfile
{
    public:
        mkfile();
        virtual ~mkfile();
        void ejecutarComandoMkfile(mkfile *mfile,mount []);
        vector<string> split(string str, char pattern);
        void returnDatosPart(MBR mbrTemp, string pathD,string nombrePart,int &tamPart, int &iniPart);
        void modificarArchivo(string pathDisco, string nombrePart,mkfile *mfile);
        int cantidadBloquesUsar(int data);
        int cantidadBloquesUsarString(string data);

        string retFecha();

        void setPath(string s){
            path=s;
        }
        string getPath(){
            return path;
        }
        void setCont(string s){
            contenido=s;
        }
        string getCont(){
            return contenido;
        }
        void setSize(int s){
            tamano=s;
        }
        int getSize(){
            return tamano;
        }
        void setR(bool s){
            comR=s;
        }
        bool getR(){
            return comR;
        }
        void setBoolCont(bool s){
            conte=s;
        }
        bool getBoolCont(){
            return conte;
        }
        void setDatosUsu(User s){
            datosUsuario=s;
        }
        User getDatosUsu(){
            return datosUsuario;
        }

    private:
        string path;
        string contenido;
        bool conte;
        int tamano;
        bool comR;
        User datosUsuario;

};

#endif // MKFILE_H
