#ifndef MKFS_H
#define MKFS_H
#include "mount.h"


class mkfs
{
    public:
        mkfs();
        virtual ~mkfs();
        void ejecutarComandoMkfs(mkfs *fs, mount []);
        bool existeIdMount(string idB,mount []);
        void returnDatosPart(MBR mbrTemp, string pathD,string nombrePart, int &tamPart, int &iniPart);
        void crearRaiz(string path, int inicioPart);
        void inicializarBitmaps(string path,int partStart,SupB superBlock);

        void setId(string s){
            idMount = s;
        }
        string getId(){
            return idMount;
        }
        void setType(string s){
            type = s;
        }
        string getType(){
            return type;
        }
        void setFs(string s){
            fsT = s;
        }
        string getFs(){
            return fsT;
        }

    private:
        string idMount;
        string type;
        string fsT;
};

#endif // MKFS_H
