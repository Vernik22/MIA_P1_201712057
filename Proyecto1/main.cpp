#include <iostream>
#include "mkdisk.h"
#include "rmdisk.h"


using namespace std;

int main(){

    string comando = "";
    mkdisk *disco1 = new mkdisk();
    rmdisk *rm = new rmdisk();
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
        }

    }


    return 0;
}
