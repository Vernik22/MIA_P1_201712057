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
        int espacioLibre(int inicio, int fin);
        void eliminarParticion(string path,string nombre, string tipoDelete);
        void agregarEspacioParticion();
        void imprimirDatosDisco(string path);
        int masGrande(int uno, int dos, int tres, int cuatro);
        int mejorAsign(int uno, int dos, int tres, int cuatro, int tamParti);
        int msPeq(int uno, int d, int t, int c);


        void setSize(int si){
            tamano=si;
        }
        int getSize(){
            return tamano;
        }
        void setUnit(string s){
            unit=s;
        }
        string getUnit(){
            return unit;
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
        int tamano;
        string unit;
        string path;
        string type;
        string fit;
        string del;
        string name;
        int add;
};

#endif // FDISK_H
