#ifndef SIMLOS_H
#define SIMLOS_H
#include "mount.h"
#include <vector>
#include "estructuras.h"


class simlos
{
    public:
        simlos();
        virtual ~simlos();
        void ejecutarLoss(simlos *clos,mount []);
        vector<string> split(string str, char pattern);
        void returnDatosPart(MBR mbrTemp, string pathD,string nombrePart,int &tamPart, int &iniPart);
        void modificarArchivo(string pathDisco, string nombrePart,simlos *clos);
        bool existeIdMount(string idB,mount []);

        string retFecha();

        void setId(string s){
            idPart=s;
        }
        string getId(){
            return idPart;
        }

    private:
        string idPart;
};

#endif // SIMLOS_H
