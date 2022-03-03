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
        void modificarArchivo(string pathDisco, string nombrePart, string nombreUser, string contrasena, string grupo);

        //comando para eliminar usuarios
        void ejecutarRmusr(mkusr *usua,mount []);
        void modificarArchivoRM(string pathDisco, string nombrePart, string nombreUser);
        string retFecha();

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
        void setDatosUsu(User s){
            datosUsuario=s;
        }
        User getDatosUsu(){
            return datosUsuario;
        }
        void setGrupo(string s){
            grupo = s;
        }
        string getGrupo(){
            return grupo;
        }

    private:
        string usuario;
        string password;
        string grupo;
        User datosUsuario;
};

#endif // MKUSR_H
