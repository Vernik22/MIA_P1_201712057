#include <iostream>
#include "mkdisk.h"


using namespace std;

int main(){

    string comando = "";
    mkdisk *disco1 = new mkdisk();
    while(comando != "salir"){
        cout<<"\n------------------------------Ingrese un comando------------------------------\n\n";
        comando = "";
        getline(cin, comando);
        if(comando != ""&&comando!= "salir"){
        cout<<endl;
        disco1->setSPath("/home/vernik/Escritorio/Disco1.dk");
        disco1->setSize(5);
        disco1->setFit("F");
        disco1->setUnit("m");

            disco1->ejecutarComandoMkdisk(disco1);
        }

    }


    return 0;
}
