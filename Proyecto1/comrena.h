#ifndef COMRENA_H
#define COMRENA_H
#include "mount.h"
#include <vector>
#include "estructuras.h"


class comrena
{
    public:
        comrena();
        virtual ~comrena();
        void ejecutarComandoRename(comrena *cmRen,mount []);
        vector<string> split(string str, char pattern);
        void returnDatosPart(MBR mbrTemp, string pathD,string nombrePart,int &tamPart, int &iniPart);
        void modificarArchivo(string pathDisco, string nombrePart,comrena *cmRen);

        string retFecha();



        void setPath(string s){
            path=s;
        }
        string getPath(){
            return path;
        }
        void setName(string s){
            name=s;
        }
        string getName(){
            return name;
        }

        void setDatosUsu(User s){
            datosUsuario=s;
        }
        User getDatosUsu(){
            return datosUsuario;
        }

    private:
        string path;
        string name;
        User datosUsuario;
};

#endif // COMRENA_H
