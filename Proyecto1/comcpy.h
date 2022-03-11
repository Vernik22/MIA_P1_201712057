#ifndef COMCPY_H
#define COMCPY_H
#include "mount.h"
#include <vector>
#include "estructuras.h"


class comcpy
{
    public:
        comcpy();
        virtual ~comcpy();
        void ejecutarComandoCopy(comcpy *cmCp,mount []);
        vector<string> split(string str, char pattern);
        void returnDatosPart(MBR mbrTemp, string pathD,string nombrePart,int &tamPart, int &iniPart);
        void modificarArchivo(string pathDisco, string nombrePart,comcpy *cmCp);
        bool existeIdMount(string idB,mount []);

        string retFecha();



        void setPath(string s){
            path=s;
        }
        string getPath(){
            return path;
        }
        void setDestino(string s){
            destino=s;
        }
        string getDestino(){
            return destino;
        }

        void setDatosUsu(User s){
            datosUsuario=s;
        }
        User getDatosUsu(){
            return datosUsuario;
        }

    private:
        string path;
        string destino;
        User datosUsuario;
};

#endif // COMCPY_H
