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
        void ejecutarRepDisk(reporte *rep,mount []);
        void ejecutarRepMBR(reporte *rep,mount []);
        void ejecutarRepInodo(reporte *rep,mount []);
        void ejecutarRepBloque(reporte *rep,mount []);
        void ejecutarRepTree(reporte *rep,mount []);
        void ejecutarRepFile(reporte *rep,mount []);
        void ejecutarRepLs(reporte *rep,mount []);

        vector<string> split(string str, char pattern);
        void dirExist(reporte *disco);
        string pathConComillas(string pathDestino);


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
