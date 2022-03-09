#ifndef COMMV_H
#define COMMV_H
#include "mount.h"
#include <vector>
#include "estructuras.h"


class commv
{
    public:
        commv();
        virtual ~commv();
        void ejecutarComandoMove(commv *cmMv,mount []);
        vector<string> split(string str, char pattern);
        void returnDatosPart(MBR mbrTemp, string pathD,string nombrePart,int &tamPart, int &iniPart);
        void modificarArchivo(string pathDisco, string nombrePart,commv *cmMv);

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

#endif // COMMV_H
