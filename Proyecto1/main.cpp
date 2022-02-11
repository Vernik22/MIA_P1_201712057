#include <iostream>
#include "mkdisk.h"
#include "rmdisk.h"
#include "fdisk.h"
#include <vector>
#include "mount.h"
#include "unmount.h"


using namespace std;

int main()
{

    string comando = "";
    mkdisk *disco1 = new mkdisk();
    rmdisk *rm = new rmdisk();
    fdisk *fd = new fdisk();
    mount *pm = new mount();
    unmount *pu = new unmount();
    //vector<mount> ParticionesMontadas;
    mount ParticionesMontadas[100];//creo el array para las particiones que estan montadas
    mount nueva;// creo una calase mount para inicializar las posiciones del array en vacias
    nueva.setName(" ");
    nueva.setPath(" ");
    nueva.setId(" ");
    for(int i = 0 ; i< 100; i++)
    {
        ParticionesMontadas[i]=nueva; // inicializo todas las posiciones del array
    }

    while(comando != "salir")
    {
        cout<<"\n------------------------------Ingrese un comando------------------------------\n\n";

        //getline(cin, comando);
        cin>>comando;
        if(comando != ""&&comando!= "salir")
        {
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

            fd->setName("Prueba");
            fd->setPath("/home/vernik/Escritorio/Disco1.dk");
            fd->setSize(2);
            fd->setType("P");
            fd->setUnit("k");
            fd->setFit("BF");

            fd->ejecutarComandoFdisk(fd);

            fd->setName("Tercera");
            fd->setPath("/home/vernik/Escritorio/Disco1.dk");
            fd->setSize(2);
            fd->setType("E");
            fd->setUnit("k");
            fd->setFit("BF");

            fd->ejecutarComandoFdisk(fd);

            fd->setName("Cuarta");
            fd->setPath("/home/vernik/Escritorio/Disco1.dk");
            fd->setSize(20);
            fd->setType("L");
            fd->setUnit("B");
            fd->setFit("BF");


            fd->ejecutarComandoFdisk(fd);

            fd->setName("Cuarta");
            fd->setPath("/home/vernik/Escritorio/Disco1.dk");
            fd->setSize(20);
            fd->setType("L");
            fd->setUnit("B");
            fd->setFit("BF");


            fd->ejecutarComandoFdisk(fd);

            fd->setName("Quinta");
            fd->setPath("/home/vernik/Escritorio/Disco1.dk");
            fd->setSize(2);
            fd->setType("P");
            fd->setUnit("k");
            fd->setFit("BF");

            fd->ejecutarComandoFdisk(fd);

            fd->setName("Tercera");
            fd->setPath("/home/vernik/Escritorio/Disco1.dk");
            fd->setDel("FAST");
            fd->eliminarParticion(fd);

            pm->setName("Prueba");
            pm->setPath("/home/vernik/Escritorio/Disco1.dk");

            pm->ejecutarComandoMount(pm,ParticionesMontadas);

            pu->setId("571A");
            pu->ejecutarComandoUnmount(pu,ParticionesMontadas);


            cout<< ParticionesMontadas[0].getName()<<endl;
            cout<< ParticionesMontadas[0].getId()<<endl;

        }

    }

        /*
        para el comando pausa usar lo siguiente

        printf("Pause Presione una tecla para continuar... \n");
        cin.get();
        */

    return 0;
}
