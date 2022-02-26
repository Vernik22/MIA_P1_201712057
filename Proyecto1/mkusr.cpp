#include "mkusr.h"
#include <iostream>

using namespace std;

mkusr::mkusr()
{
    //ctor
}

mkusr::~mkusr()
{
    //dtor
}


void mkusr::ejecutarMkusr(mkusr *usua,mount paMoun[])
{
    cout<<"\n************Ejecutar MKUSR************\n"<<endl;

    if(getDatosUsu().nombreUsuario == "root")
    {
        string pathDisco;
        string nombrePart;
        for(int i =0; i<100; i++)
        {
            if(getDatosUsu().idPartMontada == paMoun[i].getId())
            {
                pathDisco = paMoun[i].getPath();
                nombrePart = paMoun[i].getName();
                break;

            }

        }
        modificarArchivo(pathDisco, nombrePart,usua->getUsuario(), usua->getPassword(), usua->getGrupo());

    }
    else
    {
        cout<<"Usuario incorrecto, no es el usuario root"<<endl;
    }
}

void mkusr::modificarArchivo(string pathDisco, string nombrePart, string nombreUser, string contrasena, string grupo)
{
    if(nombreUser.size()<11)
    {
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
                    char str[64];
                    fseek(arch,(superBlock.s_blockAr_start+(apuntadoresArch.b_pointers[i]-1)*sizeof(BArchivo)),SEEK_SET);
                    fread(&texto,sizeof(BArchivo),1,arch);
                    //str = texto.b_content;
                    userstxt+=texto.b_content;

                }
                else
                {
                    break;
                }

            }


            int idGrupo = 1;
            bool existeElGrupo = false;
            vector<string> listaUsuarios = split(userstxt,'\n');
            int cont = listaUsuarios.size();
            for(int i=0; i<cont-1; i++)
            {
                vector<string> linea = split(listaUsuarios[i],',');
                if(linea[1]=="U"&&linea[0]!="0"&&linea[0]!="")
                {

                    if(linea[3]==nombreUser)
                    {
                        cout<<"Error el usuario ya existe: "<<nombreUser<<endl;
                        existeElGrupo=true;
                        break;
                    }
                    idGrupo ++;

                }

            }


            if(!existeElGrupo)
            {
                Inodo inodoTemp;
                fseek(arch, superBlock.s_inode_start+sizeof(Inodo),SEEK_SET);
                fread(&inodoTemp,sizeof(Inodo),1,arch);

                int contador = 1;
                int noBloques = 0;
                for(int i = 1; i< inodoTemp.i_size; i++)
                {
                    if(contador == 64)
                    {
                        noBloques += 1;
                        contador = 0;
                    }
                    contador ++;
                }
                if(inodoTemp.i_size%64 != 0)
                {
                    noBloques += 1;
                }

                int bitsTotales = inodoTemp.i_size;
                int contad = nombreUser.size();
                contad += 5;
                inodoTemp.i_size += contad;

                fseek(arch, superBlock.s_inode_start+sizeof(Inodo),SEEK_SET);
                fwrite(&inodoTemp,sizeof(Inodo),1,arch);



                string nuevoCont ="";
                nuevoCont += std::to_string(idGrupo);
                nuevoCont += ",U,";
                nuevoCont +=grupo;
                nuevoCont +=",";
                nuevoCont += nombreUser;
                nuevoCont +=",";
                nuevoCont += contrasena;
                nuevoCont +="\n?";
                //cout<<nuevoCont[0]<<endl;
                vector<string> nuevaC = split(userstxt,'?');
                cout<<nuevaC[0]<<nuevaC[0].size()<<endl;
                nuevaC[0] += nuevoCont;
                if(noBloques==1)
                {
                    if(nuevaC[0].size()<64)
                    {
                        strcpy(texto.b_content,nuevaC[0].c_str());
                        fseek(arch, superBlock.s_blockAr_start,SEEK_SET);
                        fwrite(&texto,sizeof(BArchivo),1,arch);
                        cout<<"Se creo el Grupo exitosamente: " <<nombreG<<endl;

                    }
                    else
                    {
                        string nuevaCadena = nuevaC[0];
                        string textEnBlock = "";
                        int bitCont = 0;
                        for(int i = 0; i< nuevaC[0].size(); i++)
                        {
                            if(bitCont<63)
                            {
                                textEnBlock += nuevaCadena[i];
                            }
                            else
                            {
                                bitCont = -1;
                                strcpy(texto.b_content,textEnBlock.c_str());
                                fseek(arch, superBlock.s_blockAr_start,SEEK_SET);
                                fwrite(&texto,sizeof(BArchivo),1,arch);
                                textEnBlock = "";
                                i--;

                            }
                            bitCont ++;
                        }


                        bitCont = 1;
                        for(int i = 0; i< superBlock.s_inodes_count; i++)
                        {
                            fseek(arch,(superBlock.s_blockAr_start+(bitCont-1)*sizeof(BArchivo)), SEEK_SET);
                            fread(&texto,sizeof(BArchivo),1,arch);
                            if(texto.b_content[0]=='-')
                            {
                                strcpy(texto.b_content,textEnBlock.c_str());
                                fseek(arch, (superBlock.s_blockAr_start+(bitCont-1)*sizeof(BArchivo)),SEEK_SET);
                                fwrite(&texto,sizeof(BArchivo),1,arch);
                                apuntadoresArch.b_pointers[1]=bitCont;
                                fseek(arch,(superBlock.s_blockAp_start+sizeof(BApun)),SEEK_SET);
                                fwrite(&apuntadoresArch,sizeof(BApun),1,arch);
                                cout<<"Se creo el Grupo exitosamente: " <<nombreG<<endl;
                                break;

                            }

                            bitCont++;

                        }
                        int llenar = 1;
                        fseek(arch, superBlock.s_bm_blockAr_start+sizeof(llenar),SEEK_SET);

                        fwrite(&llenar, sizeof(llenar),1,arch);



                    }
                }
                else
                {
                    //cuando hay mas de 1 bloque
                    fseek(arch,(superBlock.s_blockAp_start+sizeof(BApun)),SEEK_SET);
                    fread(&apuntadoresArch,sizeof(BApun),1,arch);

                    string nuevaCadena = "";
                    for(int i =0; i<16; i++)
                    {
                        //cout<<"wl apuntador es: "<<apuntadoresArch.b_pointers[i]<<endl;
                        if(apuntadoresArch.b_pointers[i]==-1)
                        {

                            //cont++;
                            fseek(arch,(superBlock.s_blockAr_start+(apuntadoresArch.b_pointers[i-1]-1)*sizeof(BArchivo)),SEEK_SET);
                            fread(&texto,sizeof(BArchivo),1,arch);
                            nuevaCadena=texto.b_content;

                            nuevaC = split(nuevaCadena,'?');

                            nuevaC[0]+=nuevoCont;

                            if(nuevaC[0].size()<64)
                            {
                                strcpy(texto.b_content,nuevaC[0].c_str());
                                fseek(arch,(superBlock.s_blockAr_start+(apuntadoresArch.b_pointers[i-1]-1)*sizeof(BArchivo)),SEEK_SET);
                                fwrite(&texto,sizeof(BArchivo),1,arch);
                                cout<<"Se creo el Grupo exitosamente: " <<nombreG<<endl;
                            }
                            else
                            {
                                //REvisar esta algo mal
                                nuevaCadena = nuevaC[0];
                                string textEnBlock = "";
                                int bitCont = 0;
                                for(int j = 0; j< nuevaC[0].size(); j++)
                                {
                                    if(bitCont<63)
                                    {
                                        textEnBlock += nuevaCadena[j];
                                    }
                                    else
                                    {
                                        bitCont = -1;
                                        strcpy(texto.b_content,textEnBlock.c_str());
                                        fseek(arch,(superBlock.s_blockAr_start+(apuntadoresArch.b_pointers[i-1]-1)*sizeof(BArchivo)),SEEK_SET);
                                        fwrite(&texto,sizeof(BArchivo),1,arch);
                                        textEnBlock = "";
                                        j--;
                                    }
                                    bitCont ++;
                                }

                                bitCont = 1;
                                for(int j = 0; j< superBlock.s_inodes_count; j++)
                                {
                                    fseek(arch,(superBlock.s_blockAr_start+(bitCont-1)*sizeof(BArchivo)), SEEK_SET);
                                    fread(&texto,sizeof(BArchivo),1,arch);
                                    if(texto.b_content[0]=='-')
                                    {
                                        strcpy(texto.b_content,textEnBlock.c_str());
                                        fseek(arch, (superBlock.s_blockAr_start+(bitCont-1)*sizeof(BArchivo)),SEEK_SET);
                                        fwrite(&texto,sizeof(BArchivo),1,arch);
                                        apuntadoresArch.b_pointers[i]=bitCont;
                                        fseek(arch,(superBlock.s_blockAp_start+sizeof(BApun)),SEEK_SET);
                                        fwrite(&apuntadoresArch,sizeof(BApun),1,arch);
                                        cout<<"Se creo el Grupo exitosamente: " <<nombreG<<endl;
                                        break;

                                    }
                                    bitCont++;

                                }

                                int llenar = 1;
                                int comp = 1;
                                for(int j = 0; j< superBlock.s_inodes_count; j++)
                                {
                                    fseek(arch,(superBlock.s_bm_blockAr_start+(j)*sizeof(llenar)),SEEK_SET);

                                    fread(&comp, sizeof(llenar),1,arch);

                                    if(comp==0)
                                    {

                                        fseek(arch,(superBlock.s_bm_blockAr_start+(j)*sizeof(llenar)),SEEK_SET);
                                        fwrite(&llenar, sizeof(llenar),1,arch);
                                    }
                                }

                            }
                            break;

                        }
                    }

                }

            }


            fclose(arch);
        }

    }
    else
    {
        cout<<"ERROR, el nombre es mayor a 10 letras"<<endl;
    }

}

void mkusr::ejecutarRmusr(mkusr *usua,mount paMoun[])
{
    cout<<"\n************Ejecutar RMUSR************\n"<<endl;

    if(getDatosUsu().nombreUsuario == "root")
    {
        string pathDisco;
        string nombrePart;
        for(int i =0; i<100; i++)
        {
            if(getDatosUsu().idPartMontada == paMoun[i].getId())
            {
                pathDisco = paMoun[i].getPath();
                nombrePart = paMoun[i].getName();
                break;

            }

        }
        modificarArchivoRM(pathDisco, nombrePart,usua->getUsuario());

    }
    else
    {
        cout<<"Usuario incorrecto, no es el usuario root"<<endl;
    }

}

void mkusr::modificarArchivoRM(string pathDisco, string nombrePart, string nombreUser){


}

vector<string> mkusr::split(string str, char pattern)
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


