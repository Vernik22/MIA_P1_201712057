#include "login.h"
#include <iostream>


using namespace std;
login::login()
{
    //ctor
}

login::~login()
{
    //dtor
}
//En la clase manejador hay que verificar si ya hay una sesion iniciada

bool login::ejecutarComandoLogin(login *usuario, mount paMoun[])
{
    cout<<"\n************Ejecutar LOGIN************\n"<<endl;
    if(existeIdMount(usuario->getIdMount(),paMoun))
    {
        string pathDisco;
        string nombrePart;
        for(int i =0; i<100; i++)
        {
            if(usuario->getIdMount() == paMoun[i].getId())
            {
                pathDisco = paMoun[i].getPath();
                nombrePart = paMoun[i].getName();
                break;

            }

        }

        FILE *arch;
        arch = fopen(pathDisco.c_str(),"rb+");
        if(arch != NULL)
        {
            MBR mbrTemp;
            fseek(arch,0,SEEK_SET);
            fread(&mbrTemp,sizeof(MBR),1,arch);

            int tamParticion, iniPart;
            returnDatosPart(mbrTemp,pathDisco,nombrePart,tamParticion, iniPart);
            SupB superBlock;
            fseek(arch,iniPart,SEEK_SET);
            fread(&superBlock,sizeof(SupB),1,arch);

            BApun apuntadoresArch;
            fseek(arch,(superBlock.s_blockAp_start+sizeof(BApun)),SEEK_SET);
            fread(&apuntadoresArch,sizeof(BApun),1,arch);

            BArchivo texto;
            string userstxt = "";
            for(int i =0; i<16; i++)
            {
                if(apuntadoresArch.b_pointers[i]!=-1)
                {
                    //cont++;
                    fseek(arch,(superBlock.s_blockAr_start+(apuntadoresArch.b_pointers[i]-1)*sizeof(BArchivo)),SEEK_SET);
                    fread(&texto,sizeof(BArchivo),1,arch);
                    userstxt+=texto.b_content;

                }
                else
                {
                    break;
                }

            }
            //cout<<"contenido Users.txt"<<endl;
            //cout<< userstxt<<endl;
            vector<string> listaUsuarios = split(userstxt,'\n');
            int cont = listaUsuarios.size();
            for(int i=0; i<cont-1; i++){
                vector<string> linea = split(listaUsuarios[i],',');
                if(linea[1]=="U"&&linea[0]!="0"){
                    if(linea[3]==usuario->getUsuario()&&linea[4]==usuario->getPassword()){
                        cout<<"Bienvenido al sistema: "<<usuario->getUsuario()<<endl;
                        return true;
                    }
                }

            }


            fclose(arch);

        }




    }
    else
    {
        cout<<"El id no existe, la particion no esta montada"<<endl;
    }

    return false;
}

bool login::existeIdMount(string idP,mount paMoun[])
{
    for(int i =0; i<100; i++)
    {
        if(idP == paMoun[i].getId())
        {
            return true;

        }

    }
    return false;
}

vector<string> login::split(string str, char pattern)
{

    int posInit = 0;
    int posFound = 0;
    string splitted;
    vector<string> results;

    while (posFound >= 0)
    {
        posFound = str.find(pattern, posInit);
        splitted = str.substr(posInit, posFound - posInit);
        posInit = posFound + 1;
        results.push_back(splitted);
    }

    return results;
}

bool login::logout(){
    cout<<"\n************Ejecutar LOGOUT************\n"<<endl;
    cout<<"Se cerro la sesion"<<endl;
    return false;
}

void login::returnDatosPart(MBR mbrTemp, string pathD,string nombrePart,int &tamPart, int &iniPart)
{


    Particion parts[4];
    parts[0] = mbrTemp.mbr_partition_1;
    parts[1] = mbrTemp.mbr_partition_2;
    parts[2] = mbrTemp.mbr_partition_3;
    parts[3] = mbrTemp.mbr_partition_4;
    bool noEncontrada = true;

    if(mbrTemp.mbr_partition_1.part_name == nombrePart|| mbrTemp.mbr_partition_2.part_name == nombrePart|| mbrTemp.mbr_partition_3.part_name == nombrePart|| mbrTemp.mbr_partition_4.part_name == nombrePart)
    {
        for(int i = 0; i<4; i++)
        {
            if(parts[i].part_name == nombrePart)
            {
                tamPart = parts[i].part_size;
                iniPart = parts[i].part_start;
                noEncontrada = false;
                break;
            }
        }
    }
    if(noEncontrada)
    {
        EBR ebrExTemp;
        FILE *arch;
        arch = fopen(pathD.c_str(), "rb+");
        for(int i=0; i<4; i++) // encuentra la particion extendida para revisar dentro de sus particiones logicas
        {
            if(parts[i].part_type == 'E')
            {
                fseek(arch,parts[i].part_start,SEEK_SET);
                fread(&ebrExTemp, sizeof(EBR),1,arch); //partExTemp guarda el EBR de la particion E
                break;
            }
        }
        while(ebrExTemp.part_next != -1) //revisa hasta la ultima particion logica o hasta que la encuentre
        {
            if(ebrExTemp.part_name == nombrePart)
            {
                tamPart = ebrExTemp.part_size;
                iniPart = ebrExTemp.part_start;
                break;
            }
            fseek(arch, ebrExTemp.part_next, SEEK_SET);
            fread(&ebrExTemp, sizeof(EBR), 1, arch);
        }

        fclose(arch);
    }
}
