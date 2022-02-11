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

void unmount::ejecutarComandoUnmount(unmount *part, vector<mount>& v)
{
    cout<<"\n************Ejecutar Unmount************\n"<<endl;
    bool vError = false;
    int siVe = v.size();
    int indexP=0;
    for(int i =0; i<siVe; i++)
    {
        if(part->getId() == v[i].getId()){
            vError = false;
            cout<<v[i].getId()<<endl;
            indexP = i;
            cout<<"Particion desmontada con exito!"<<endl;
            break;
        }
        vError=true;

    }

    if(vError)
    {
        cout<<"ERROR: El ID de la particion no existe, intente de nuevo"<<endl;
    }else{
        v.erase(v.begin()+indexP);
            cout<<v[indexP].getId()<<endl;
    }

}
