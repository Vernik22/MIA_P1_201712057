#include "unmount.h"
#include <iostream>
#include "mount.h"


unmount::unmount()
{
    //ctor
}

unmount::~unmount()
{
    //dtor
}

void unmount::ejecutarComandoUnmount(unmount *part, mount paMoun[])
{
    cout<<"\n************Ejecutar Unmount************\n"<<endl;
    bool vError = false;
    for(int i =0; i<100; i++)
    {
        if(part->getId() == paMoun[i].getId()){
            vError = false;
            cout<<"ID de la particion desmontada: "<<paMoun[i].getId()<<endl;
            mount nueva;// creo una calase mount para inicializar las posiciones del array en vacias
            nueva.setName(" ");
            nueva.setPath(" ");
            nueva.setId(" ");
            paMoun[i] = nueva; //intercambio la posicion en la que estaba el id por una vacia
            cout<<"Particion desmontada con exito!"<<endl;
            break;
        }
        vError=true;

    }

    if(vError)
    {
        cout<<"ERROR: El ID de la particion no existe, intente de nuevo"<<endl;
    }

}
