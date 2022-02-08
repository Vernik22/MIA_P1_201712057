#include <iostream>
#include "mkdisk.h"
#include "rmdisk.h"
#include "fdisk.h"


using namespace std;

int main(){

    string comando = "";
    mkdisk *disco1 = new mkdisk();
    rmdisk *rm = new rmdisk();
    fdisk *fd = new fdisk();
    while(comando != "salir"){
        cout<<"\n------------------------------Ingrese un comando------------------------------\n\n";
        //getline(cin, comando);
        cin>>comando;
        if(comando != ""&&comando!= "salir"){
        cout<<endl;
        disco1->setSPath("/home/vernik/Escritorio/Disco1.dk");
        disco1->setSize(5);
        disco1->setFit("F");
        disco1->setUnit("m");

            disco1->ejecutarComandoMkdisk(disco1);
        rm->setPath("/home/vernik/Escritorio/Disco1.dk");

        rm->ejecutarComandoRmdisk(rm);

        fd->setName("Prueba");
        fd->setPath("/home/vernik/Escritorio/Disco1.dk");
        fd->setSize(2);
        fd->setType("P");
        fd->setUnit("k");
        fd->setFit("BF");

        fd->ejecutarComandoFdisk(fd);

        fd->setName("Segunda");
        fd->setPath("/home/vernik/Escritorio/Disco1.dk");
        fd->setSize(2);
        fd->setType("P");
        fd->setUnit("k");
        fd->setFit("BF");

        fd->ejecutarComandoFdisk(fd);
        }

    }


    return 0;
}
