#include <iostream>
#include <vector>
#include "manejador.h"
#include "mount.h"



using namespace std;

int main()
{

    string comando = "";
    manejador *leer = new manejador();

    //vector<mount> ParticionesMontadas;
    mount ParticionesMontadas[100];//creo el array para las particiones que estan montadas
    mount nueva;// creo una clase mount para inicializar las posiciones del array en vacias
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
        //cin>>comando;
        getline(cin, comando);
        if(comando != ""&&comando!= "salir")
        {
            cout<<endl;
            leer->leerTexto(comando,ParticionesMontadas);



        }

    }



    return 0;
}
