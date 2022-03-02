#ifndef REPORTE_H
#define REPORTE_H
#include "mount.h"
#include <string>
#include <vector>


class reporte
{
    public:
        reporte();
        virtual ~reporte();
        bool existeIdMount (string idP,mount [], string &pathD, string &partN);
        void ejecutarRepBmInodo(reporte *rep,mount []);
        void ejecutarRepBmBloque(reporte *rep,mount []);
        void ejecutarRepSuperB(reporte *rep,mount []);
        void returnDatosPart(MBR mbrTemp, string pathD,string nombrePart,int &tamPart, int &iniPart);

        vector<string> split(string str, char pattern);
        void dirExist(reporte *disco);


        void setName(string s){
            name = s;
        }
        string getName(){
            return name;
        }
        void setPath(string s){
            path = s;
        }
        string getPath(){
            return path;
        }
        void setId(string s){
            id = s;
        }
        string getId(){
            return id;
        }
        void setRuta(string s){
            ruta = s;
        }
        string getRuta(){
            return ruta;
        }

    private:
        string name;
        string path;
        string id;
        string ruta;
};

#endif // REPORTE_H
