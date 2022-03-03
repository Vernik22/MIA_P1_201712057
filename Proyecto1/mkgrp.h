#ifndef MKGRP_H
#define MKGRP_H
#include "mount.h"
#include <vector>
#include "estructuras.h"


class mkgrp
{
    public:
        mkgrp();
        virtual ~mkgrp();
        void ejecutarMkgrp(mkgrp *grupo,mount []);
        vector<string> split(string str, char pattern);
        void returnDatosPart(MBR mbrTemp, string pathD,string nombrePart,int &tamPart, int &iniPart);
        void modificarArchivo(string pathDisco, string nombrePart, string nombreG);

        //para el comando RMGRP, eliminar un grupo
        void ejecutarRmgrp(mkgrp *grupo,mount []);
        void modificarArchivoRM(string pathDisco, string nombrePart, string nombreG);
        int cantidadBloquesUsar(string data);
        string retFecha();


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
        string name;
        User datosUsuario;
};

#endif // MKGRP_H
