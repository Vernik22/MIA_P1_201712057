#ifndef LOGIN_H
#define LOGIN_H
#include "mount.h"
#include <vector>

class login
{
    public:
        login();
        virtual ~login();
        bool ejecutarComandoLogin(login *usuario,mount []);
        bool existeIdMount (string idP,mount []);
        vector<string> split(string str, char pattern);
        void returnDatosPart(MBR mbrTemp, string pathD,string nombrePart,int &tamPart, int &iniPart);
        bool logout();

        void setUsuario(string s){
            usuario = s;
        }
        string getUsuario(){
            return usuario;
        }
        void setPassword(string s){
            pass = s;
        }
        string getPassword(){
            return pass;
        }
        void setIdMount(string s){
            idMount = s;
        }
        string getIdMount(){
            return idMount;
        }

    private:
        string usuario;
        string pass;
        string idMount;
};

#endif // LOGIN_H
