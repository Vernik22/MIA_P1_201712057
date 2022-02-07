#ifndef FDISK_H
#define FDISK_H
#include <string>
#include "estructuras.h"

using namespace std;
class fdisk
{
    public:
        fdisk();
        virtual ~fdisk();
        void ejecutarComandoFdisk(fdisk *disco);
        bool hayEspacio(int tamaParticion, int tamanoDisco);
        void eliminarParticion(string path,string nombre, string tipoDelete);
        void imprimirDatosDisco(string path);

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
            return fit;
        }
        void setPath(string s){
            path=s;
        }
        string getPath(){
            return path;
        }
        void setType(string s){
            type=s;
        }
        string getType(){
            return type;
        }
        void setFit(string s){
            fit=s;
        }
        string getFit(){
            return fit;
        }
        void setDel(string s){
            del=s;
        }
        string getDel(){
            return del;
        }
        void setName(string s){
            name=s;
        }
        string getName(){
            return name;
        }
        void setAdd(int s){
            add=s;
        }
        int getAdd(){
            return add;
        }

    private:
        int size;
        string unit;
        string path;
        string type;
        string fit;
        string del;
        string name;
        int add;
};

#endif // FDISK_H
