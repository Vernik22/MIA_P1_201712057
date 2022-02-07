#ifndef MKDISK_H
#define MKDISK_H
#include <vector>
#include <string>
#include "estructuras.h"

using namespace std;
class mkdisk
{
    public:
        mkdisk();
        virtual ~mkdisk();
        void ejecutarComandoMkdisk(mkdisk *disco);
        vector<string> split(string str, char pattern);
        void dirExist(mkdisk *disco);
        bool ejecMkdisk(string nombreComando, Propiedad propiedades[], int cont);

        //metodos para las variables privadas
        void setSize(int s){
            size=s;
        }
        int getSize(){
            return size;
        }
        void setUnit(string s){
            unit=s;
        }
        string getUnit(){
            return unit;
        }
        void setSPath(string s){
            path=s;
        }
        string getPath(){
            return path;
        }
        void setFit(string s){
            fit=s;
        }
        string getFit(){
            return fit;
        }

    private:
        int size;
        string unit;
        string path;
        string fit;
};

#endif // MKDISK_H
