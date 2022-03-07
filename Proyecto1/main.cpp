#include <iostream>
#include <vector>
#include "manejador.h"
#include "mount.h"

#include "mkdisk.h"
#include "mkfs.h"
#include "fdisk.h"
#include "login.h"
#include "mkgrp.h"
#include "estructuras.h"
#include "reporte.h"



using namespace std;

int main()
{

    string comando = "";
    manejador *leer = new manejador();
    mkdisk *disco1 = new mkdisk();
    //rmdisk *rm = new rmdisk();
    fdisk *fd = new fdisk();
    mount *pm = new mount();
    //unmount *pu = new unmount();
    mkfs *fs = new mkfs();
    login *log = new login();
    mkgrp *gr = new mkgrp();
    reporte *rep = new reporte();
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
        comando = "";
        //getline(cin, comando);
        cin>>comando;
        if(comando != ""&&comando!= "salir")
        {
            cout<<endl;
            //leer->leerTexto(comando,ParticionesMontadas);
            disco1->setSPath("/home/vernik/Escritorio/misDiscos/Disco1.dk");
            disco1->setSize(5);
            disco1->setFit("F");
            disco1->setUnit("m");

            disco1->ejecutarComandoMkdisk(disco1);

            fd->setName("Prueba");
            fd->setPath("/home/vernik/Escritorio/misDiscos/Disco1.dk");
            fd->setSize(100);
            fd->setType("P");
            fd->setUnit("k");
            fd->setFit("BF");

            fd->ejecutarComandoFdisk(fd);
            pm->setName("Prueba");
            pm->setPath("/home/vernik/Escritorio/misDiscos/Disco1.dk");

            pm->ejecutarComandoMount(pm,ParticionesMontadas);

            //fs->setFs("3fs");
            fs->setId("571A");
            fs->setType("full");
            fs->ejecutarComandoMkfs(fs,ParticionesMontadas);

            log->setIdMount("571A");
            log->setUsuario("root");
            log->setPassword("123");
            log->ejecutarComandoLogin(log,ParticionesMontadas);

            User usuario;
            usuario.idPartMontada = "571A";
            usuario.nombreUsuario = "root";
            gr->setDatosUsu(usuario);
            gr->setName("prueba");
            gr->ejecutarMkgrp(gr,ParticionesMontadas);
            gr->setName("prueba1");
            gr->ejecutarMkgrp(gr,ParticionesMontadas);
            gr->setName("prueba2");
            gr->ejecutarMkgrp(gr,ParticionesMontadas);
            gr->setName("prueba3");
            gr->ejecutarMkgrp(gr,ParticionesMontadas);
            gr->setName("prueba4");
            gr->ejecutarMkgrp(gr,ParticionesMontadas);
            gr->setName("prueba5");
            gr->ejecutarMkgrp(gr,ParticionesMontadas);
            gr->setName("prueba6");
            gr->ejecutarMkgrp(gr,ParticionesMontadas);
            gr->setName("prueba7");
            gr->ejecutarMkgrp(gr,ParticionesMontadas);
            gr->setName("prueba8");
            gr->ejecutarMkgrp(gr,ParticionesMontadas);
            gr->setName("prueba9");
            gr->ejecutarMkgrp(gr,ParticionesMontadas);

            rep->setId("571A");
            rep->setPath("/home/vernik/Imágenes/rep/inodo.txt");
            rep->ejecutarRepBmInodo(rep, ParticionesMontadas);

            rep->setPath("/home/vernik/Imágenes/rep/bloques.txt");
            rep->ejecutarRepBmBloque(rep, ParticionesMontadas);

            rep->setPath("/home/vernik/Imágenes/rep/SuperB.pdf");
            rep->ejecutarRepSuperB(rep,ParticionesMontadas);

            rep->setPath("/home/vernik/Imágenes/rep/repDisk.pdf");
            rep->ejecutarRepDisk(rep,ParticionesMontadas);

            rep->setPath("/home/vernik/Imágenes/rep/repMbr.pdf");
            rep->ejecutarRepMBR(rep,ParticionesMontadas);

            rep->setPath("/home/vernik/Imágenes/rep/repIno.pdf");
            rep->ejecutarRepInodo(rep,ParticionesMontadas);

            rep->setPath("/home/vernik/Imágenes/rep/repBlock.pdf");
            rep->ejecutarRepBloque(rep,ParticionesMontadas);

            rep->setPath("/home/vernik/Imágenes/rep/repTree.pdf");
            rep->ejecutarRepTree(rep,ParticionesMontadas);

            rep->setRuta("/users.txt");
            rep->setPath("/home/vernik/Imágenes/rep/repFile.pdf");
            rep->ejecutarRepFile(rep,ParticionesMontadas);

            rep->setRuta("/");
            rep->setPath("/home/vernik/Imágenes/rep/repLS.pdf");
            rep->ejecutarRepLs(rep,ParticionesMontadas);

            /*
            disco1->setSPath("/home/vernik/Escritorio/Disco1.dk");
            disco1->setSize(5);
            disco1->setFit("F");
            disco1->setUnit("m");

            disco1->ejecutarComandoMkdisk(disco1);

//            rm->setPath("/home/vernik/Escritorio/Disco1.dk");

//          rm->ejecutarComandoRmdisk(rm);

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
            /*

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

            //pu->setId("571A");
            //pu->ejecutarComandoUnmount(pu,ParticionesMontadas);


            cout<< ParticionesMontadas[0].getName()<<endl;
            cout<< ParticionesMontadas[0].getId()<<endl;


            fs->setFs("3fs");
            fs->setId("571A");
            fs->setType("full");
            fs->ejecutarComandoMkfs(fs,ParticionesMontadas);*/




        }

    }

        /*
        para el comando pausa usar lo siguiente

        printf("Pause Presione una tecla para continuar... \n");
        cin.get();
        */

    return 0;
}
