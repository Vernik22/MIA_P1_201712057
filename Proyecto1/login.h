#ifndef LOGIN_H
#define LOGIN_H
#include "mount.h"

class login
{
    public:
        login();
        virtual ~login();
        void ejecutarComandoLogin(login *usuario,mount []);

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
