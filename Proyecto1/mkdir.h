#ifndef MKDIR_H
#define MKDIR_H
#include "mount.h"
#include <vector>
#include "estructuras.h"


class mkdir
{
    public:
        mkdir();
        virtual ~mkdir();
        void ejecutarComandoMkDir(mkdir *mDir,mount []);
        vector<string> split(string str, char pattern);
        void returnDatosPart(MBR mbrTemp, string pathD,string nombrePart,int &tamPart, int &iniPart);
        void modificarArchivo(string pathDisco, string nombrePart,mkdir *mDir);

        string retFecha();


        void setPath(string s){
            path=s;
        }
        string getPath(){
            return path;
        }
        void setP(bool s){
            comP=s;
        }
        bool getP(){
            return comP;
        }
        void setDatosUsu(User s){
            datosUsuario=s;
        }
        User getDatosUsu(){
            return datosUsuario;
        }

    private:
        string path;
        bool comP;
        User datosUsuario;
};

#endif // MKDIR_H
