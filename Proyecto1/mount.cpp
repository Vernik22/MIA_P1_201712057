#include "mount.h"
#include <iostream>

mount::mount()
{
    //ctor
}

mount::~mount()
{
    //dtor
}

//estructura del id: *57+ Numero + Letra :: ejemplo  ids= 571A ; 571B ; 571C, 572A
// Numero sera el mismo para particiones en el mismo disco y letra diferente para particiones en el mismo disco

void mount::ejecutarComandoMount(mount *part, vector<mount> &paMoun)
{
    cout<<"\n************Ejecutar Mount************\n"<<endl;
    bool vError= true;

    for(int i =0; i< paMoun.size(); i++)
    {
        if(paMoun[i].getName()==part->getName()&&paMoun[i].getPath() == part->getPath())
        {
            cout<<"ERROR: Ya esta montada la particion solicitada"<<endl;
            vError = false;
            break;
        }

    }

    if(vError)
    {

        part->setId(generarId(part,paMoun));
        mount nueva;
        nueva.setName(part->getName());
        nueva.setPath(part->getPath());
        nueva.setId(part->getId());
        paMoun.push_back(nueva); //comando para agregar al vector
    }


}

string mount::generarId(mount *part,vector<mount> &paMoun)
{
    //en ascii A = 65 -- Z = 90 ;; 1 = 49 -- 9 = 57
    //Mismo path cambia la letra, diferente path cambia el numero
    //concatena char con string
    char l = 65;
    int n = 1;
    string idTemp = "57";

    /* esto funciona para crear los ids e ir concatenando las letras y los numeros
    char f = 65;
    string h = "57";
    h+=f;
    char g = 49;
    g++; //para aumentar en 1 al char osea 1 a 2 en ascii
    h += g;
    cout<<h<<endl;


        char n[2] = {' ',49};
        string idTemp = "57";
        idTemp += n;
        cout<<idTemp<<endl;

        string idTemp = "57";
        int n =3;
        idTemp = idTemp + std::to_string(n);
        cout<<idTemp<<endl;
    */

    for(int i =0; i< paMoun.size(); i++)
    {
        if(paMoun[i].getPath()==part->getPath())
        {
            l++;
        }else{
            n++;
        }


    }
    idTemp = idTemp + std::to_string(n);
    idTemp += l;
    return idTemp;

}
