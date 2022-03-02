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

            Inodo inodoTemp;
            fseek(arch,(superBlock.s_inode_start+sizeof(Inodo)),SEEK_SET);
            fread(&inodoTemp,sizeof(Inodo),1,arch);

            BArchivo texto;
            string userstxt = "";
            for(int i =0; i<12; i++)
            {
                if(inodoTemp.i_block[i]!=-1)
                {

                    fseek(arch,(superBlock.s_block_start+(inodoTemp.i_block[i])*sizeof(BArchivo)),SEEK_SET);
                    fread(&texto,sizeof(BArchivo),1,arch);
                    userstxt+=texto.b_content;

                }
                else
                {
                    break;
                }

            }

            if(inodoTemp.i_block[12]!=-1)
            {
                //concatenar pero es bloque simple indirecto
                BApun b_apuntador;
                fseek(arch,(superBlock.s_block_start+(inodoTemp.i_block[12])*sizeof(BArchivo)),SEEK_SET);
                fread(&b_apuntador,sizeof(BApun),1,arch);

                for(int i =0; i<4; i++)
                {
                    if(b_apuntador.b_apuntadores[i].b_inodo!=-1)
                    {

                        fseek(arch,(superBlock.s_block_start+(b_apuntador.b_apuntadores[i].b_inodo)*sizeof(BArchivo)),SEEK_SET);
                        fread(&texto,sizeof(BArchivo),1,arch);
                        userstxt+=texto.b_content;

                    }
                    else
                    {
                        break;
                    }

                }


            }

            if(inodoTemp.i_block[13]!=-1)
            {
                //concatenar pero es bloque doble indirecto
                BApun b_apuntador1;
                fseek(arch,(superBlock.s_block_start+(inodoTemp.i_block[13])*sizeof(BApun)),SEEK_SET);
                fread(&b_apuntador1,sizeof(BApun),1,arch);

                for(int j = 0; j<4; j++)
                {

                    if(b_apuntador1.b_apuntadores[j].b_inodo!=-1)
                    {
                        BApun b_apuntador;
                        fseek(arch,(superBlock.s_block_start+(b_apuntador1.b_apuntadores[j].b_inodo)*sizeof(BArchivo)),SEEK_SET);
                        fread(&b_apuntador,sizeof(BApun),1,arch);

                        for(int i =0; i<4; i++)
                        {
                            if(b_apuntador.b_apuntadores[i].b_inodo!=-1)
                            {
                                //cont++;
                                fseek(arch,(superBlock.s_block_start+(b_apuntador.b_apuntadores[i].b_inodo)*sizeof(BArchivo)),SEEK_SET);
                                fread(&texto,sizeof(BArchivo),1,arch);
                                userstxt+=texto.b_content;

                            }
                            else
                            {
                                break;
                            }

                        }


                    }
                    else
                    {
                        break;
                    }
                }


            }

            if(inodoTemp.i_block[14]!=-1)
            {
                //concatenar pero es bloque triple indirecto
                BApun b_apuntador2;
                fseek(arch,(superBlock.s_block_start+(inodoTemp.i_block[14])*sizeof(BApun)),SEEK_SET);
                fread(&b_apuntador2,sizeof(BApun),1,arch);
                for(int k = 0 ; k<4; k++)
                {
                    if(b_apuntador2.b_apuntadores[k].b_inodo!=-1)
                    {
                        BApun b_apuntador1;
                        fseek(arch,(superBlock.s_block_start+(b_apuntador2.b_apuntadores[k].b_inodo)*sizeof(BApun)),SEEK_SET);
                        fread(&b_apuntador1,sizeof(BApun),1,arch);

                        for(int j = 0; j<4; j++)
                        {

                            if(b_apuntador1.b_apuntadores[j].b_inodo!=-1)
                            {
                                BApun b_apuntador;
                                fseek(arch,(superBlock.s_block_start+(b_apuntador1.b_apuntadores[j].b_inodo)*sizeof(BArchivo)),SEEK_SET);
                                fread(&b_apuntador,sizeof(BApun),1,arch);

                                for(int i =0; i<4; i++)
                                {
                                    if(b_apuntador.b_apuntadores[i].b_inodo!=-1)
                                    {
                                        //cont++;
                                        fseek(arch,(superBlock.s_block_start+(b_apuntador.b_apuntadores[i].b_inodo)*sizeof(BArchivo)),SEEK_SET);
                                        fread(&texto,sizeof(BArchivo),1,arch);
                                        userstxt+=texto.b_content;

                                    }
                                    else
                                    {
                                        break;
                                    }

                                }


                            }
                            else
                            {
                                break;
                            }
                        }

                    }
                    else
                    {
                        break;
                    }

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
                        fseek(arch, superBlock.s_block_start+inodoTemp1.i_block[0]*sizeof(BArchivo),SEEK_SET);
                        fwrite(&texto,sizeof(BArchivo),1,arch);

                        time_t rawtime;
                        struct tm *timeinfo;
                        time(&rawtime);
                        timeinfo = localtime(&rawtime);
                        string fecha = asctime(timeinfo);
                        _mTime fcreacion;
                        strcpy(fcreacion.mbr_fecha_creacion,fecha.c_str());
                        inodoTemp1.i_mtime = fcreacion;

                        fseek(arch, superBlock.s_inode_start+sizeof(Inodo),SEEK_SET);
                        fwrite(&inodoTemp1,sizeof(Inodo),1,arch);

                        cout<<"Se creo el usuario exitosamente: " <<nombreUser<<endl;

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
                                fseek(arch, superBlock.s_block_start+inodoTemp1.i_block[0]*sizeof(BArchivo),SEEK_SET);
                                fwrite(&texto,sizeof(BArchivo),1,arch);
                                textEnBlock = "";
                                i--;

                            }
                            bitCont ++;
                        }


                        strcpy(texto.b_content,textEnBlock.c_str());
                        fseek(arch, superBlock.s_block_start+(superBlock.s_first_blo*sizeof(BArchivo)),SEEK_SET);
                        fwrite(&texto,sizeof(BArchivo),1,arch);
                        inodoTemp1.i_block[1]=superBlock.s_first_blo;

                        time_t rawtime;
                        struct tm *timeinfo;
                        time(&rawtime);
                        timeinfo = localtime(&rawtime);
                        string fecha = asctime(timeinfo);
                        _mTime fcreacion;
                        strcpy(fcreacion.mbr_fecha_creacion,fecha.c_str());
                        inodoTemp1.i_mtime = fcreacion;

                        fseek(arch, superBlock.s_inode_start+sizeof(Inodo),SEEK_SET);
                        fwrite(&inodoTemp1,sizeof(Inodo),1,arch);
                        superBlock.s_first_blo = superBlock.s_first_blo + 1;
                        superBlock.s_free_blocks_count = superBlock.s_free_blocks_count - 1;
                        fseek(arch,iniPart,SEEK_SET);
                        fwrite(&superBlock,sizeof(SupB),1,arch);

                        cout<<"Se creo el usuario exitosamente: " <<nombreUser<<endl;
                        int llenar = 1;
                        int actual;
                        for(int i = 0 ; i < superBlock.s_blocks_count; i++)
                        {
                            fseek(arch, superBlock.s_bm_block_start+i*sizeof(llenar),SEEK_SET);
                            fread(&actual, sizeof(llenar),1,arch);

                            if(actual==0)
                            {
                                fseek(arch, superBlock.s_bm_block_start+i*sizeof(llenar),SEEK_SET);
                                fwrite(&llenar, sizeof(llenar),1,arch);
                                break;
                            }

                        }



                    }
                }
                else
                {
                    //cuando hay mas de 1 bloque
                    if(noBloques<97)
                    {
                        //si estan en los bloques directos

                        string nuevaCadena = "";
                        for(int i =0; i<15; i++) //para recorrer los bloques directos del inodo
                        {
                            //cout<<"wl apuntador es: "<<apuntadoresArch.b_pointers[i]<<endl;
                            if(inodoTemp1.i_block[i]==-1)
                            {
                                if(i < 11)
                                {
                                    fseek(arch,superBlock.s_block_start+(inodoTemp1.i_block[i-1]*sizeof(BArchivo)),SEEK_SET);
                                    fread(&texto,sizeof(BArchivo),1,arch);
                                    nuevaCadena=texto.b_content;

                                    nuevaC = split(nuevaCadena,'?');

                                    nuevaC[0]+=nuevoCont;

                                    if(nuevaC[0].size()<64)
                                    {
                                        strcpy(texto.b_content,nuevaC[0].c_str());
                                        fseek(arch, superBlock.s_block_start+inodoTemp1.i_block[i-1]*sizeof(BArchivo),SEEK_SET);
                                        fwrite(&texto,sizeof(BArchivo),1,arch);

                                        time_t rawtime;
                                        struct tm *timeinfo;
                                        time(&rawtime);
                                        timeinfo = localtime(&rawtime);
                                        string fecha = asctime(timeinfo);
                                        _mTime fcreacion;
                                        strcpy(fcreacion.mbr_fecha_creacion,fecha.c_str());
                                        inodoTemp1.i_mtime = fcreacion;

                                        fseek(arch, superBlock.s_inode_start+sizeof(Inodo),SEEK_SET);
                                        fwrite(&inodoTemp1,sizeof(Inodo),1,arch);
                                        cout<<"Se creo el usuario exitosamente: " <<nombreUser<<endl;
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
                                                fseek(arch,(superBlock.s_block_start+inodoTemp1.i_block[i-1]*sizeof(BArchivo)),SEEK_SET);
                                                fwrite(&texto,sizeof(BArchivo),1,arch);
                                                textEnBlock = "";
                                                j--;
                                            }
                                            bitCont ++;
                                        }

                                        strcpy(texto.b_content,textEnBlock.c_str());
                                        fseek(arch, superBlock.s_block_start+(superBlock.s_first_blo*sizeof(BArchivo)),SEEK_SET);
                                        fwrite(&texto,sizeof(BArchivo),1,arch);
                                        inodoTemp1.i_block[i]=superBlock.s_first_blo;

                                        time_t rawtime;
                                        struct tm *timeinfo;
                                        time(&rawtime);
                                        timeinfo = localtime(&rawtime);
                                        string fecha = asctime(timeinfo);
                                        _mTime fcreacion;
                                        strcpy(fcreacion.mbr_fecha_creacion,fecha.c_str());
                                        inodoTemp1.i_mtime = fcreacion;

                                        fseek(arch, superBlock.s_inode_start+sizeof(Inodo),SEEK_SET);
                                        fwrite(&inodoTemp1,sizeof(Inodo),1,arch);
                                        superBlock.s_first_blo = superBlock.s_first_blo + 1;
                                        superBlock.s_free_blocks_count = superBlock.s_free_blocks_count - 1;
                                        fseek(arch,iniPart,SEEK_SET);
                                        fwrite(&superBlock,sizeof(SupB),1,arch);

                                        cout<<"Se creo el usuario exitosamente: " <<nombreUser<<endl;

                                        int llenar = 1;
                                        int actual;
                                        for(int j = 0 ; j < superBlock.s_blocks_count; j++)
                                        {
                                            fseek(arch, superBlock.s_bm_block_start+j*sizeof(llenar),SEEK_SET);
                                            fread(&actual, sizeof(llenar),1,arch);

                                            if(actual==0)
                                            {
                                                fseek(arch, superBlock.s_bm_block_start+j*sizeof(llenar),SEEK_SET);
                                                fwrite(&llenar, sizeof(llenar),1,arch);
                                                break;
                                            }

                                        }

                                    }
                                    break;


                                }
                                else
                                {
                                    if(i==12)
                                    {
                                        fseek(arch,superBlock.s_block_start+(inodoTemp1.i_block[i-1]*sizeof(BArchivo)),SEEK_SET);
                                        fread(&texto,sizeof(BArchivo),1,arch);
                                        nuevaCadena=texto.b_content;

                                        nuevaC = split(nuevaCadena,'?');

                                        nuevaC[0]+=nuevoCont;
                                        if(nuevaC[0].size()<64)
                                        {
                                            strcpy(texto.b_content,nuevaC[0].c_str());
                                            fseek(arch, superBlock.s_block_start+inodoTemp1.i_block[i-1]*sizeof(BArchivo),SEEK_SET);
                                            fwrite(&texto,sizeof(BArchivo),1,arch);

                                            time_t rawtime;
                                            struct tm *timeinfo;
                                            time(&rawtime);
                                            timeinfo = localtime(&rawtime);
                                            string fecha = asctime(timeinfo);
                                            _mTime fcreacion;
                                            strcpy(fcreacion.mbr_fecha_creacion,fecha.c_str());
                                            inodoTemp1.i_mtime = fcreacion;

                                            fseek(arch, superBlock.s_inode_start+sizeof(Inodo),SEEK_SET);
                                            fwrite(&inodoTemp1,sizeof(Inodo),1,arch);
                                            cout<<"Se creo el usuario exitosamente: " <<nombreUser<<endl;
                                        }
                                        else
                                        {
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
                                                    fseek(arch,(superBlock.s_block_start+inodoTemp1.i_block[i-1]*sizeof(BArchivo)),SEEK_SET);
                                                    fwrite(&texto,sizeof(BArchivo),1,arch);
                                                    textEnBlock = "";
                                                    j--;
                                                }
                                                bitCont ++;
                                            }
                                            BApun apuntadorNuevo;
                                            apuntadorNuevo.b_apuntadores[0].b_inodo= superBlock.s_first_blo + 1;
                                            apuntadorNuevo.b_apuntadores[1].b_inodo= -1;
                                            apuntadorNuevo.b_apuntadores[2].b_inodo= -1;
                                            apuntadorNuevo.b_apuntadores[3].b_inodo= -1;

                                            string apunt = "Apuntador1";
                                            strcpy(apuntadorNuevo.b_apuntadores[0].b_name,apunt.c_str());
                                            apunt = "Apuntador2";
                                            strcpy(apuntadorNuevo.b_apuntadores[1].b_name,apunt.c_str());
                                            apunt = "Apuntador3";
                                            strcpy(apuntadorNuevo.b_apuntadores[2].b_name,apunt.c_str());
                                            apunt = "Apuntador4";
                                            strcpy(apuntadorNuevo.b_apuntadores[3].b_name,apunt.c_str());

                                            inodoTemp1.i_block[i]=superBlock.s_first_blo;

                                            fseek(arch, superBlock.s_block_start+(superBlock.s_first_blo)*sizeof(BArchivo),SEEK_SET);
                                            fwrite(&apuntadorNuevo,sizeof(BApun),1,arch);

                                            strcpy(texto.b_content,textEnBlock.c_str());
                                            fseek(arch, superBlock.s_block_start+(superBlock.s_first_blo+ 1)*sizeof(BArchivo),SEEK_SET);
                                            fwrite(&texto,sizeof(BArchivo),1,arch);


                                            time_t rawtime;
                                            struct tm *timeinfo;
                                            time(&rawtime);
                                            timeinfo = localtime(&rawtime);
                                            string fecha = asctime(timeinfo);
                                            _mTime fcreacion;
                                            strcpy(fcreacion.mbr_fecha_creacion,fecha.c_str());
                                            inodoTemp1.i_mtime = fcreacion;

                                            fseek(arch, superBlock.s_inode_start+sizeof(Inodo),SEEK_SET);
                                            fwrite(&inodoTemp1,sizeof(Inodo),1,arch);
                                            superBlock.s_first_blo = superBlock.s_first_blo + 2;
                                            superBlock.s_free_blocks_count = superBlock.s_free_blocks_count - 2;
                                            fseek(arch,iniPart,SEEK_SET);
                                            fwrite(&superBlock,sizeof(SupB),1,arch);

                                            cout<<"Se creo el usuario exitosamente: " <<nombreUser<<endl;

                                            int llenar = 1;
                                            int actual;
                                            for(int j = 0 ; j < superBlock.s_blocks_count; j++)
                                            {
                                                fseek(arch, superBlock.s_bm_block_start+j*sizeof(llenar),SEEK_SET);
                                                fread(&actual, sizeof(llenar),1,arch);

                                                if(actual==0)
                                                {
                                                    fseek(arch, superBlock.s_bm_block_start+j*sizeof(llenar),SEEK_SET);
                                                    fwrite(&llenar, sizeof(llenar),1,arch);
                                                    fwrite(&llenar, sizeof(llenar),1,arch);
                                                    break;
                                                }

                                            }

                                        }
                                        break;


                                    }
                                    else if(i == 13)
                                    {
                                        BApun apuntadorNuevo;
                                        fseek(arch,superBlock.s_block_start+(inodoTemp1.i_block[i-1]*sizeof(BArchivo)),SEEK_SET);
                                        fread(&apuntadorNuevo,sizeof(BApun),1,arch);

                                        for(int j = 0 ; j<4; j++)
                                        {
                                            if(apuntadorNuevo.b_apuntadores[j].b_inodo==-1)
                                            {
                                                fseek(arch,superBlock.s_block_start+(apuntadorNuevo.b_apuntadores[j-1].b_inodo*sizeof(BArchivo)),SEEK_SET);
                                                fread(&texto,sizeof(BArchivo),1,arch);
                                                nuevaCadena=texto.b_content;
                                                nuevaC = split(nuevaCadena,'?');

                                                nuevaC[0]+=nuevoCont;
                                                if(nuevaC[0].size()<64)
                                                {
                                                    strcpy(texto.b_content,nuevaC[0].c_str());
                                                    fseek(arch, superBlock.s_block_start+apuntadorNuevo.b_apuntadores[j-1].b_inodo*sizeof(BArchivo),SEEK_SET);
                                                    fwrite(&texto,sizeof(BArchivo),1,arch);

                                                    time_t rawtime;
                                                    struct tm *timeinfo;
                                                    time(&rawtime);
                                                    timeinfo = localtime(&rawtime);
                                                    string fecha = asctime(timeinfo);
                                                    _mTime fcreacion;
                                                    strcpy(fcreacion.mbr_fecha_creacion,fecha.c_str());
                                                    inodoTemp1.i_mtime = fcreacion;

                                                    fseek(arch, superBlock.s_inode_start+sizeof(Inodo),SEEK_SET);
                                                    fwrite(&inodoTemp1,sizeof(Inodo),1,arch);
                                                    cout<<"Se creo el usuario exitosamente: " <<nombreUser<<endl;
                                                }
                                                else
                                                {
                                                    nuevaCadena = nuevaC[0];
                                                    string textEnBlock = "";
                                                    int bitCont = 0;
                                                    for(int k = 0; k< nuevaC[0].size(); k++)
                                                    {
                                                        if(bitCont<63)
                                                        {
                                                            textEnBlock += nuevaCadena[k];
                                                        }
                                                        else
                                                        {
                                                            bitCont = -1;
                                                            strcpy(texto.b_content,textEnBlock.c_str());
                                                            fseek(arch,(superBlock.s_block_start+apuntadorNuevo.b_apuntadores[j-1].b_inodo*sizeof(BArchivo)),SEEK_SET);
                                                            fwrite(&texto,sizeof(BArchivo),1,arch);
                                                            textEnBlock = "";
                                                            j--;
                                                        }
                                                        bitCont ++;
                                                    }

                                                    apuntadorNuevo.b_apuntadores[j].b_inodo= superBlock.s_first_blo;

                                                    fseek(arch, superBlock.s_block_start+(inodoTemp1.i_block[12])*sizeof(BArchivo),SEEK_SET);
                                                    fwrite(&apuntadorNuevo,sizeof(BApun),1,arch);

                                                    strcpy(texto.b_content,textEnBlock.c_str());
                                                    fseek(arch, (superBlock.s_block_start+apuntadorNuevo.b_apuntadores[j].b_inodo*sizeof(BArchivo)),SEEK_SET);
                                                    fwrite(&texto,sizeof(BArchivo),1,arch);


                                                    time_t rawtime;
                                                    struct tm *timeinfo;
                                                    time(&rawtime);
                                                    timeinfo = localtime(&rawtime);
                                                    string fecha = asctime(timeinfo);
                                                    _mTime fcreacion;
                                                    strcpy(fcreacion.mbr_fecha_creacion,fecha.c_str());
                                                    inodoTemp1.i_mtime = fcreacion;

                                                    fseek(arch, superBlock.s_inode_start+sizeof(Inodo),SEEK_SET);
                                                    fwrite(&inodoTemp1,sizeof(Inodo),1,arch);
                                                    superBlock.s_first_blo = superBlock.s_first_blo + 1;
                                                    superBlock.s_free_blocks_count = superBlock.s_free_blocks_count - 1;
                                                    fseek(arch,iniPart,SEEK_SET);
                                                    fwrite(&superBlock,sizeof(SupB),1,arch);

                                                    cout<<"Se creo el usuario exitosamente: " <<nombreUser<<endl;

                                                    int llenar = 1;
                                                    int actual;
                                                    for(int k = 0 ; k < superBlock.s_blocks_count; k++)
                                                    {
                                                        fseek(arch, superBlock.s_bm_block_start+k*sizeof(llenar),SEEK_SET);
                                                        fread(&actual, sizeof(llenar),1,arch);

                                                        if(actual==0)
                                                        {
                                                            fseek(arch, superBlock.s_bm_block_start+k*sizeof(llenar),SEEK_SET);
                                                            fwrite(&llenar, sizeof(llenar),1,arch);
                                                            break;
                                                        }

                                                    }

                                                }
                                                break;

                                            }
                                            else if(j==3)
                                            {
                                                fseek(arch,superBlock.s_block_start+(apuntadorNuevo.b_apuntadores[j].b_inodo*sizeof(BArchivo)),SEEK_SET);
                                                fread(&texto,sizeof(BArchivo),1,arch);
                                                nuevaCadena=texto.b_content;
                                                nuevaC = split(nuevaCadena,'?');

                                                nuevaC[0]+=nuevoCont;
                                                if(nuevaC[0].size()<64)
                                                {
                                                    strcpy(texto.b_content,nuevaC[0].c_str());
                                                    fseek(arch, superBlock.s_block_start+apuntadorNuevo.b_apuntadores[j].b_inodo*sizeof(BArchivo),SEEK_SET);
                                                    fwrite(&texto,sizeof(BArchivo),1,arch);

                                                    time_t rawtime;
                                                    struct tm *timeinfo;
                                                    time(&rawtime);
                                                    timeinfo = localtime(&rawtime);
                                                    string fecha = asctime(timeinfo);
                                                    _mTime fcreacion;
                                                    strcpy(fcreacion.mbr_fecha_creacion,fecha.c_str());
                                                    inodoTemp1.i_mtime = fcreacion;

                                                    fseek(arch, superBlock.s_inode_start+sizeof(Inodo),SEEK_SET);
                                                    fwrite(&inodoTemp1,sizeof(Inodo),1,arch);
                                                    cout<<"Se creo el usuario exitosamente: " <<nombreUser<<endl;
                                                }
                                                else
                                                {
                                                    nuevaCadena = nuevaC[0];
                                                    string textEnBlock = "";
                                                    int bitCont = 0;
                                                    for(int k = 0; k< nuevaC[0].size(); k++)
                                                    {
                                                        if(bitCont<63)
                                                        {
                                                            textEnBlock += nuevaCadena[k];
                                                        }
                                                        else
                                                        {
                                                            bitCont = -1;
                                                            strcpy(texto.b_content,textEnBlock.c_str());
                                                            fseek(arch,(superBlock.s_block_start+apuntadorNuevo.b_apuntadores[j].b_inodo*sizeof(BArchivo)),SEEK_SET);
                                                            fwrite(&texto,sizeof(BArchivo),1,arch);
                                                            textEnBlock = "";
                                                            j--;
                                                        }
                                                        bitCont ++;
                                                    }

                                                    BApun apuntadorIndirectoD1;
                                                    apuntadorIndirectoD1.b_apuntadores[0].b_inodo= superBlock.s_first_blo + 2;
                                                    apuntadorIndirectoD1.b_apuntadores[1].b_inodo= -1;
                                                    apuntadorIndirectoD1.b_apuntadores[2].b_inodo= -1;
                                                    apuntadorIndirectoD1.b_apuntadores[3].b_inodo= -1;

                                                    string apunt = "Apuntador1";
                                                    strcpy(apuntadorIndirectoD1.b_apuntadores[0].b_name,apunt.c_str());
                                                    apunt = "Apuntador2";
                                                    strcpy(apuntadorIndirectoD1.b_apuntadores[1].b_name,apunt.c_str());
                                                    apunt = "Apuntador3";
                                                    strcpy(apuntadorIndirectoD1.b_apuntadores[2].b_name,apunt.c_str());
                                                    apunt = "Apuntador4";
                                                    strcpy(apuntadorIndirectoD1.b_apuntadores[3].b_name,apunt.c_str());

                                                    BApun apuntadorIndirectoD;
                                                    apuntadorIndirectoD.b_apuntadores[0].b_inodo= superBlock.s_first_blo + 1;
                                                    apuntadorIndirectoD.b_apuntadores[1].b_inodo= -1;
                                                    apuntadorIndirectoD.b_apuntadores[2].b_inodo= -1;
                                                    apuntadorIndirectoD.b_apuntadores[3].b_inodo= -1;

                                                    apunt = "Apuntador1";
                                                    strcpy(apuntadorIndirectoD.b_apuntadores[0].b_name,apunt.c_str());
                                                    apunt = "Apuntador2";
                                                    strcpy(apuntadorIndirectoD.b_apuntadores[1].b_name,apunt.c_str());
                                                    apunt = "Apuntador3";
                                                    strcpy(apuntadorIndirectoD.b_apuntadores[2].b_name,apunt.c_str());
                                                    apunt = "Apuntador4";
                                                    strcpy(apuntadorIndirectoD.b_apuntadores[3].b_name,apunt.c_str());



                                                    inodoTemp1.i_block[13]=superBlock.s_first_blo;

                                                    fseek(arch, superBlock.s_block_start+(superBlock.s_first_blo)*sizeof(BArchivo),SEEK_SET);
                                                    fwrite(&apuntadorIndirectoD,sizeof(BApun),1,arch);

                                                    fseek(arch, superBlock.s_block_start+(superBlock.s_first_blo+1)*sizeof(BArchivo),SEEK_SET);
                                                    fwrite(&apuntadorIndirectoD1,sizeof(BApun),1,arch);

                                                    strcpy(texto.b_content,textEnBlock.c_str());
                                                    fseek(arch, superBlock.s_block_start+(superBlock.s_first_blo+2)*sizeof(BArchivo),SEEK_SET);
                                                    fwrite(&texto,sizeof(BArchivo),1,arch);


                                                    time_t rawtime;
                                                    struct tm *timeinfo;
                                                    time(&rawtime);
                                                    timeinfo = localtime(&rawtime);
                                                    string fecha = asctime(timeinfo);
                                                    _mTime fcreacion;
                                                    strcpy(fcreacion.mbr_fecha_creacion,fecha.c_str());
                                                    inodoTemp1.i_mtime = fcreacion;

                                                    fseek(arch, superBlock.s_inode_start+sizeof(Inodo),SEEK_SET);
                                                    fwrite(&inodoTemp1,sizeof(Inodo),1,arch);
                                                    superBlock.s_first_blo = superBlock.s_first_blo + 3;
                                                    superBlock.s_free_blocks_count = superBlock.s_free_blocks_count - 3;
                                                    fseek(arch,iniPart,SEEK_SET);
                                                    fwrite(&superBlock,sizeof(SupB),1,arch);

                                                    cout<<"Se creo el usuario exitosamente: " <<nombreUser<<endl;

                                                    int llenar = 1;
                                                    int actual;
                                                    for(int k = 0 ; k < superBlock.s_blocks_count; k++)
                                                    {
                                                        fseek(arch, superBlock.s_bm_block_start+k*sizeof(llenar),SEEK_SET);
                                                        fread(&actual, sizeof(llenar),1,arch);

                                                        if(actual==0)
                                                        {
                                                            fseek(arch, superBlock.s_bm_block_start+k*sizeof(llenar),SEEK_SET);
                                                            fwrite(&llenar, sizeof(llenar),1,arch);
                                                            fwrite(&llenar, sizeof(llenar),1,arch);
                                                            fwrite(&llenar, sizeof(llenar),1,arch);
                                                            break;
                                                        }

                                                    }

                                                }
                                                break;
                                            }


                                        }

                                        break;
                                    }
                                    else if(i == 14)
                                    {
                                        BApun apuntadorNuevo;
                                        fseek(arch,superBlock.s_block_start+(inodoTemp1.i_block[i-1]*sizeof(BArchivo)),SEEK_SET);
                                        fread(&apuntadorNuevo,sizeof(BApun),1,arch);
                                        for(int j = 0 ; j<4; j++)
                                        {
                                            if(apuntadorNuevo.b_apuntadores[j].b_inodo==-1)
                                            {
                                                BApun segundoApuntador;
                                                fseek(arch,superBlock.s_block_start+(apuntadorNuevo.b_apuntadores[j-1].b_inodo*sizeof(BArchivo)),SEEK_SET);
                                                fread(&segundoApuntador,sizeof(BApun),1,arch);

                                                for(int k=0; k < 4; k++)
                                                {
                                                    if(segundoApuntador.b_apuntadores[k].b_inodo==-1)
                                                    {
                                                        fseek(arch,superBlock.s_block_start+(segundoApuntador.b_apuntadores[k-1].b_inodo*sizeof(BArchivo)),SEEK_SET);
                                                        fread(&texto,sizeof(BArchivo),1,arch);
                                                        nuevaCadena=texto.b_content;
                                                        nuevaC = split(nuevaCadena,'?');

                                                        nuevaC[0]+=nuevoCont;
                                                        if(nuevaC[0].size()<64)
                                                        {
                                                            strcpy(texto.b_content,nuevaC[0].c_str());
                                                            fseek(arch, superBlock.s_block_start+segundoApuntador.b_apuntadores[k-1].b_inodo*sizeof(BArchivo),SEEK_SET);
                                                            fwrite(&texto,sizeof(BArchivo),1,arch);

                                                            time_t rawtime;
                                                            struct tm *timeinfo;
                                                            time(&rawtime);
                                                            timeinfo = localtime(&rawtime);
                                                            string fecha = asctime(timeinfo);
                                                            _mTime fcreacion;
                                                            strcpy(fcreacion.mbr_fecha_creacion,fecha.c_str());
                                                            inodoTemp1.i_mtime = fcreacion;

                                                            fseek(arch, superBlock.s_inode_start+sizeof(Inodo),SEEK_SET);
                                                            fwrite(&inodoTemp1,sizeof(Inodo),1,arch);
                                                            cout<<"Se creo el usuario exitosamente: " <<nombreUser<<endl;
                                                        }
                                                        else
                                                        {
                                                            nuevaCadena = nuevaC[0];
                                                            string textEnBlock = "";
                                                            int bitCont = 0;
                                                            for(int l = 0; l< nuevaC[0].size(); l++)
                                                            {
                                                                if(bitCont<63)
                                                                {
                                                                    textEnBlock += nuevaCadena[l];
                                                                }
                                                                else
                                                                {
                                                                    bitCont = -1;
                                                                    strcpy(texto.b_content,textEnBlock.c_str());
                                                                    fseek(arch,(superBlock.s_block_start+segundoApuntador.b_apuntadores[k-1].b_inodo*sizeof(BArchivo)),SEEK_SET);
                                                                    fwrite(&texto,sizeof(BArchivo),1,arch);
                                                                    textEnBlock = "";
                                                                    j--;
                                                                }
                                                                bitCont ++;
                                                            }

                                                            segundoApuntador.b_apuntadores[k].b_inodo =superBlock.s_first_blo;


                                                            fseek(arch,superBlock.s_block_start+(apuntadorNuevo.b_apuntadores[j].b_inodo*sizeof(BArchivo)),SEEK_SET);
                                                            fwrite(&segundoApuntador,sizeof(BApun),1,arch);


                                                            strcpy(texto.b_content,textEnBlock.c_str());
                                                            fseek(arch, superBlock.s_block_start+(superBlock.s_first_blo)*sizeof(BArchivo),SEEK_SET);
                                                            fwrite(&texto,sizeof(BArchivo),1,arch);


                                                            time_t rawtime;
                                                            struct tm *timeinfo;
                                                            time(&rawtime);
                                                            timeinfo = localtime(&rawtime);
                                                            string fecha = asctime(timeinfo);
                                                            _mTime fcreacion;
                                                            strcpy(fcreacion.mbr_fecha_creacion,fecha.c_str());
                                                            inodoTemp1.i_mtime = fcreacion;

                                                            fseek(arch, superBlock.s_inode_start+sizeof(Inodo),SEEK_SET);
                                                            fwrite(&inodoTemp1,sizeof(Inodo),1,arch);
                                                            superBlock.s_first_blo = superBlock.s_first_blo + 1;
                                                            superBlock.s_free_blocks_count = superBlock.s_free_blocks_count - 1;
                                                            fseek(arch,iniPart,SEEK_SET);
                                                            fwrite(&superBlock,sizeof(SupB),1,arch);

                                                            cout<<"Se creo el usuario exitosamente: " <<nombreUser<<endl;

                                                            int llenar = 1;
                                                            int actual;
                                                            for(int l = 0 ; l < superBlock.s_blocks_count; l++)
                                                            {
                                                                fseek(arch, superBlock.s_bm_block_start+l*sizeof(llenar),SEEK_SET);
                                                                fread(&actual, sizeof(llenar),1,arch);

                                                                if(actual==0)
                                                                {
                                                                    fseek(arch, superBlock.s_bm_block_start+l*sizeof(llenar),SEEK_SET);
                                                                    fwrite(&llenar, sizeof(llenar),1,arch);

                                                                    break;
                                                                }

                                                            }

                                                        }

                                                        break;

                                                    }
                                                    else if(k==3)
                                                    {
                                                        fseek(arch,superBlock.s_block_start+(segundoApuntador.b_apuntadores[k].b_inodo*sizeof(BArchivo)),SEEK_SET);
                                                        fread(&texto,sizeof(BArchivo),1,arch);
                                                        nuevaCadena=texto.b_content;
                                                        nuevaC = split(nuevaCadena,'?');

                                                        nuevaC[0]+=nuevoCont;
                                                        if(nuevaC[0].size()<64)
                                                        {
                                                            strcpy(texto.b_content,nuevaC[0].c_str());
                                                            fseek(arch, superBlock.s_block_start+segundoApuntador.b_apuntadores[k].b_inodo*sizeof(BArchivo),SEEK_SET);
                                                            fwrite(&texto,sizeof(BArchivo),1,arch);

                                                            time_t rawtime;
                                                            struct tm *timeinfo;
                                                            time(&rawtime);
                                                            timeinfo = localtime(&rawtime);
                                                            string fecha = asctime(timeinfo);
                                                            _mTime fcreacion;
                                                            strcpy(fcreacion.mbr_fecha_creacion,fecha.c_str());
                                                            inodoTemp1.i_mtime = fcreacion;

                                                            fseek(arch, superBlock.s_inode_start+sizeof(Inodo),SEEK_SET);
                                                            fwrite(&inodoTemp1,sizeof(Inodo),1,arch);
                                                            cout<<"Se creo el usuario exitosamente: " <<nombreUser<<endl;
                                                        }
                                                        else
                                                        {
                                                            nuevaCadena = nuevaC[0];
                                                            string textEnBlock = "";
                                                            int bitCont = 0;
                                                            for(int l = 0; l< nuevaC[0].size(); l++)
                                                            {
                                                                if(bitCont<63)
                                                                {
                                                                    textEnBlock += nuevaCadena[l];
                                                                }
                                                                else
                                                                {
                                                                    bitCont = -1;
                                                                    strcpy(texto.b_content,textEnBlock.c_str());
                                                                    fseek(arch,(superBlock.s_block_start+segundoApuntador.b_apuntadores[k].b_inodo*sizeof(BArchivo)),SEEK_SET);
                                                                    fwrite(&texto,sizeof(BArchivo),1,arch);
                                                                    textEnBlock = "";
                                                                    j--;
                                                                }
                                                                bitCont ++;
                                                            }
                                                            BApun apuntadorIndirectoD2;
                                                            apuntadorIndirectoD2.b_apuntadores[0].b_inodo= superBlock.s_first_blo + 1;
                                                            apuntadorIndirectoD2.b_apuntadores[1].b_inodo= -1;
                                                            apuntadorIndirectoD2.b_apuntadores[2].b_inodo= -1;
                                                            apuntadorIndirectoD2.b_apuntadores[3].b_inodo= -1;

                                                            string apunt = "Apuntador1";
                                                            strcpy(apuntadorIndirectoD2.b_apuntadores[0].b_name,apunt.c_str());
                                                            apunt = "Apuntador2";
                                                            strcpy(apuntadorIndirectoD2.b_apuntadores[1].b_name,apunt.c_str());
                                                            apunt = "Apuntador3";
                                                            strcpy(apuntadorIndirectoD2.b_apuntadores[2].b_name,apunt.c_str());
                                                            apunt = "Apuntador4";
                                                            strcpy(apuntadorIndirectoD2.b_apuntadores[3].b_name,apunt.c_str());

                                                            apuntadorNuevo.b_apuntadores[j].b_inodo =superBlock.s_first_blo;


                                                            fseek(arch,superBlock.s_block_start+(inodoTemp1.i_block[i-1]*sizeof(BArchivo)),SEEK_SET);
                                                            fwrite(&apuntadorNuevo,sizeof(BApun),1,arch);

                                                            fseek(arch, superBlock.s_block_start+(superBlock.s_first_blo)*sizeof(BArchivo),SEEK_SET);
                                                            fwrite(&apuntadorIndirectoD2,sizeof(BApun),1,arch);


                                                            strcpy(texto.b_content,textEnBlock.c_str());
                                                            fseek(arch, superBlock.s_block_start+(superBlock.s_first_blo+1)*sizeof(BArchivo),SEEK_SET);
                                                            fwrite(&texto,sizeof(BArchivo),1,arch);


                                                            time_t rawtime;
                                                            struct tm *timeinfo;
                                                            time(&rawtime);
                                                            timeinfo = localtime(&rawtime);
                                                            string fecha = asctime(timeinfo);
                                                            _mTime fcreacion;
                                                            strcpy(fcreacion.mbr_fecha_creacion,fecha.c_str());
                                                            inodoTemp1.i_mtime = fcreacion;

                                                            fseek(arch, superBlock.s_inode_start+sizeof(Inodo),SEEK_SET);
                                                            fwrite(&inodoTemp1,sizeof(Inodo),1,arch);
                                                            superBlock.s_first_blo = superBlock.s_first_blo + 2;
                                                            superBlock.s_free_blocks_count = superBlock.s_free_blocks_count - 2;
                                                            fseek(arch,iniPart,SEEK_SET);
                                                            fwrite(&superBlock,sizeof(SupB),1,arch);

                                                            cout<<"Se creo el usuario exitosamente: " <<nombreUser<<endl;

                                                            int llenar = 1;
                                                            int actual;
                                                            for(int l = 0 ; l < superBlock.s_blocks_count; l++)
                                                            {
                                                                fseek(arch, superBlock.s_bm_block_start+l*sizeof(llenar),SEEK_SET);
                                                                fread(&actual, sizeof(llenar),1,arch);

                                                                if(actual==0)
                                                                {
                                                                    fseek(arch, superBlock.s_bm_block_start+l*sizeof(llenar),SEEK_SET);
                                                                    fwrite(&llenar, sizeof(llenar),1,arch);
                                                                    fwrite(&llenar, sizeof(llenar),1,arch);
                                                                    break;
                                                                }

                                                            }

                                                        }
                                                        break;


                                                    }


                                                }

                                                break;

                                            }
                                            else if(j==3)
                                            {
                                                BApun segundoApuntador;
                                                fseek(arch,superBlock.s_block_start+(apuntadorNuevo.b_apuntadores[j].b_inodo*sizeof(BArchivo)),SEEK_SET);
                                                fread(&segundoApuntador,sizeof(BApun),1,arch);

                                                for(int k=0; k < 4; k++)
                                                {
                                                    if(segundoApuntador.b_apuntadores[k].b_inodo==-1)
                                                    {
                                                        fseek(arch,superBlock.s_block_start+(segundoApuntador.b_apuntadores[k-1].b_inodo*sizeof(BArchivo)),SEEK_SET);
                                                        fread(&texto,sizeof(BArchivo),1,arch);
                                                        nuevaCadena=texto.b_content;
                                                        nuevaC = split(nuevaCadena,'?');

                                                        nuevaC[0]+=nuevoCont;
                                                        if(nuevaC[0].size()<64)
                                                        {
                                                            strcpy(texto.b_content,nuevaC[0].c_str());
                                                            fseek(arch, superBlock.s_block_start+segundoApuntador.b_apuntadores[k-1].b_inodo*sizeof(BArchivo),SEEK_SET);
                                                            fwrite(&texto,sizeof(BArchivo),1,arch);

                                                            time_t rawtime;
                                                            struct tm *timeinfo;
                                                            time(&rawtime);
                                                            timeinfo = localtime(&rawtime);
                                                            string fecha = asctime(timeinfo);
                                                            _mTime fcreacion;
                                                            strcpy(fcreacion.mbr_fecha_creacion,fecha.c_str());
                                                            inodoTemp1.i_mtime = fcreacion;

                                                            fseek(arch, superBlock.s_inode_start+sizeof(Inodo),SEEK_SET);
                                                            fwrite(&inodoTemp1,sizeof(Inodo),1,arch);
                                                            cout<<"Se creo el usuario exitosamente: " <<nombreUser<<endl;
                                                        }
                                                        else
                                                        {
                                                            nuevaCadena = nuevaC[0];
                                                            string textEnBlock = "";
                                                            int bitCont = 0;
                                                            for(int l = 0; l< nuevaC[0].size(); l++)
                                                            {
                                                                if(bitCont<63)
                                                                {
                                                                    textEnBlock += nuevaCadena[l];
                                                                }
                                                                else
                                                                {
                                                                    bitCont = -1;
                                                                    strcpy(texto.b_content,textEnBlock.c_str());
                                                                    fseek(arch,(superBlock.s_block_start+segundoApuntador.b_apuntadores[k-1].b_inodo*sizeof(BArchivo)),SEEK_SET);
                                                                    fwrite(&texto,sizeof(BArchivo),1,arch);
                                                                    textEnBlock = "";
                                                                    j--;
                                                                }
                                                                bitCont ++;
                                                            }

                                                            segundoApuntador.b_apuntadores[k].b_inodo =superBlock.s_first_blo;


                                                            fseek(arch,superBlock.s_block_start+(apuntadorNuevo.b_apuntadores[j].b_inodo*sizeof(BArchivo)),SEEK_SET);
                                                            fwrite(&segundoApuntador,sizeof(BApun),1,arch);


                                                            strcpy(texto.b_content,textEnBlock.c_str());
                                                            fseek(arch, superBlock.s_block_start+(superBlock.s_first_blo+3)*sizeof(BArchivo),SEEK_SET);
                                                            fwrite(&texto,sizeof(BArchivo),1,arch);


                                                            time_t rawtime;
                                                            struct tm *timeinfo;
                                                            time(&rawtime);
                                                            timeinfo = localtime(&rawtime);
                                                            string fecha = asctime(timeinfo);
                                                            _mTime fcreacion;
                                                            strcpy(fcreacion.mbr_fecha_creacion,fecha.c_str());
                                                            inodoTemp1.i_mtime = fcreacion;

                                                            fseek(arch, superBlock.s_inode_start+sizeof(Inodo),SEEK_SET);
                                                            fwrite(&inodoTemp1,sizeof(Inodo),1,arch);
                                                            superBlock.s_first_blo = superBlock.s_first_blo + 1;
                                                            superBlock.s_free_blocks_count = superBlock.s_free_blocks_count - 1;
                                                            fseek(arch,iniPart,SEEK_SET);
                                                            fwrite(&superBlock,sizeof(SupB),1,arch);

                                                            cout<<"Se creo el usuario exitosamente: " <<nombreUser<<endl;

                                                            int llenar = 1;
                                                            int actual;
                                                            for(int l = 0 ; l < superBlock.s_blocks_count; l++)
                                                            {
                                                                fseek(arch, superBlock.s_bm_block_start+l*sizeof(llenar),SEEK_SET);
                                                                fread(&actual, sizeof(llenar),1,arch);

                                                                if(actual==0)
                                                                {
                                                                    fseek(arch, superBlock.s_bm_block_start+l*sizeof(llenar),SEEK_SET);
                                                                    fwrite(&llenar, sizeof(llenar),1,arch);

                                                                    break;
                                                                }

                                                            }

                                                        }

                                                        break;

                                                    }
                                                    else if(k==3)
                                                    {
                                                        fseek(arch,superBlock.s_block_start+(segundoApuntador.b_apuntadores[k].b_inodo*sizeof(BArchivo)),SEEK_SET);
                                                        fread(&texto,sizeof(BArchivo),1,arch);
                                                        nuevaCadena=texto.b_content;
                                                        nuevaC = split(nuevaCadena,'?');

                                                        nuevaC[0]+=nuevoCont;
                                                        if(nuevaC[0].size()<64)
                                                        {
                                                            strcpy(texto.b_content,nuevaC[0].c_str());
                                                            fseek(arch, superBlock.s_block_start+segundoApuntador.b_apuntadores[k].b_inodo*sizeof(BArchivo),SEEK_SET);
                                                            fwrite(&texto,sizeof(BArchivo),1,arch);

                                                            time_t rawtime;
                                                            struct tm *timeinfo;
                                                            time(&rawtime);
                                                            timeinfo = localtime(&rawtime);
                                                            string fecha = asctime(timeinfo);
                                                            _mTime fcreacion;
                                                            strcpy(fcreacion.mbr_fecha_creacion,fecha.c_str());
                                                            inodoTemp1.i_mtime = fcreacion;

                                                            fseek(arch, superBlock.s_inode_start+sizeof(Inodo),SEEK_SET);
                                                            fwrite(&inodoTemp1,sizeof(Inodo),1,arch);
                                                            cout<<"Se creo el usuario exitosamente: " <<nombreUser<<endl;
                                                        }
                                                        else
                                                        {
                                                            nuevaCadena = nuevaC[0];
                                                            string textEnBlock = "";
                                                            int bitCont = 0;
                                                            for(int l = 0; l< nuevaC[0].size(); l++)
                                                            {
                                                                if(bitCont<63)
                                                                {
                                                                    textEnBlock += nuevaCadena[l];
                                                                }
                                                                else
                                                                {
                                                                    bitCont = -1;
                                                                    strcpy(texto.b_content,textEnBlock.c_str());
                                                                    fseek(arch,(superBlock.s_block_start+segundoApuntador.b_apuntadores[k].b_inodo*sizeof(BArchivo)),SEEK_SET);
                                                                    fwrite(&texto,sizeof(BArchivo),1,arch);
                                                                    textEnBlock = "";
                                                                    j--;
                                                                }
                                                                bitCont ++;
                                                            }
                                                            BApun apuntadorIndirectoD2;
                                                            apuntadorIndirectoD2.b_apuntadores[0].b_inodo= superBlock.s_first_blo + 3;
                                                            apuntadorIndirectoD2.b_apuntadores[1].b_inodo= -1;
                                                            apuntadorIndirectoD2.b_apuntadores[2].b_inodo= -1;
                                                            apuntadorIndirectoD2.b_apuntadores[3].b_inodo= -1;

                                                            string apunt = "Apuntador1";
                                                            strcpy(apuntadorIndirectoD2.b_apuntadores[0].b_name,apunt.c_str());
                                                            apunt = "Apuntador2";
                                                            strcpy(apuntadorIndirectoD2.b_apuntadores[1].b_name,apunt.c_str());
                                                            apunt = "Apuntador3";
                                                            strcpy(apuntadorIndirectoD2.b_apuntadores[2].b_name,apunt.c_str());
                                                            apunt = "Apuntador4";
                                                            strcpy(apuntadorIndirectoD2.b_apuntadores[3].b_name,apunt.c_str());

                                                            BApun apuntadorIndirectoD1;
                                                            apuntadorIndirectoD1.b_apuntadores[0].b_inodo= superBlock.s_first_blo + 2;
                                                            apuntadorIndirectoD1.b_apuntadores[1].b_inodo= -1;
                                                            apuntadorIndirectoD1.b_apuntadores[2].b_inodo= -1;
                                                            apuntadorIndirectoD1.b_apuntadores[3].b_inodo= -1;

                                                            apunt = "Apuntador1";
                                                            strcpy(apuntadorIndirectoD1.b_apuntadores[0].b_name,apunt.c_str());
                                                            apunt = "Apuntador2";
                                                            strcpy(apuntadorIndirectoD1.b_apuntadores[1].b_name,apunt.c_str());
                                                            apunt = "Apuntador3";
                                                            strcpy(apuntadorIndirectoD1.b_apuntadores[2].b_name,apunt.c_str());
                                                            apunt = "Apuntador4";
                                                            strcpy(apuntadorIndirectoD1.b_apuntadores[3].b_name,apunt.c_str());

                                                            BApun apuntadorIndirectoD;
                                                            apuntadorIndirectoD.b_apuntadores[0].b_inodo= superBlock.s_first_blo + 1;
                                                            apuntadorIndirectoD.b_apuntadores[1].b_inodo= -1;
                                                            apuntadorIndirectoD.b_apuntadores[2].b_inodo= -1;
                                                            apuntadorIndirectoD.b_apuntadores[3].b_inodo= -1;

                                                            apunt = "Apuntador1";
                                                            strcpy(apuntadorIndirectoD.b_apuntadores[0].b_name,apunt.c_str());
                                                            apunt = "Apuntador2";
                                                            strcpy(apuntadorIndirectoD.b_apuntadores[1].b_name,apunt.c_str());
                                                            apunt = "Apuntador3";
                                                            strcpy(apuntadorIndirectoD.b_apuntadores[2].b_name,apunt.c_str());
                                                            apunt = "Apuntador4";
                                                            strcpy(apuntadorIndirectoD.b_apuntadores[3].b_name,apunt.c_str());



                                                            inodoTemp1.i_block[14]=superBlock.s_first_blo;

                                                            fseek(arch, superBlock.s_block_start+(superBlock.s_first_blo)*sizeof(BArchivo),SEEK_SET);
                                                            fwrite(&apuntadorIndirectoD,sizeof(BApun),1,arch);

                                                            fseek(arch, superBlock.s_block_start+(superBlock.s_first_blo+1)*sizeof(BArchivo),SEEK_SET);
                                                            fwrite(&apuntadorIndirectoD1,sizeof(BApun),1,arch);

                                                            fseek(arch, superBlock.s_block_start+(superBlock.s_first_blo+2)*sizeof(BArchivo),SEEK_SET);
                                                            fwrite(&apuntadorIndirectoD2,sizeof(BApun),1,arch);

                                                            strcpy(texto.b_content,textEnBlock.c_str());
                                                            fseek(arch, superBlock.s_block_start+(superBlock.s_first_blo+3)*sizeof(BArchivo),SEEK_SET);
                                                            fwrite(&texto,sizeof(BArchivo),1,arch);


                                                            time_t rawtime;
                                                            struct tm *timeinfo;
                                                            time(&rawtime);
                                                            timeinfo = localtime(&rawtime);
                                                            string fecha = asctime(timeinfo);
                                                            _mTime fcreacion;
                                                            strcpy(fcreacion.mbr_fecha_creacion,fecha.c_str());
                                                            inodoTemp1.i_mtime = fcreacion;

                                                            fseek(arch, superBlock.s_inode_start+sizeof(Inodo),SEEK_SET);
                                                            fwrite(&inodoTemp1,sizeof(Inodo),1,arch);
                                                            superBlock.s_first_blo = superBlock.s_first_blo + 4;
                                                            superBlock.s_free_blocks_count = superBlock.s_free_blocks_count - 4;
                                                            fseek(arch,iniPart,SEEK_SET);
                                                            fwrite(&superBlock,sizeof(SupB),1,arch);

                                                            cout<<"Se creo el usuario exitosamente: " <<nombreUser<<endl;

                                                            int llenar = 1;
                                                            int actual;
                                                            for(int l = 0 ; l < superBlock.s_blocks_count; l++)
                                                            {
                                                                fseek(arch, superBlock.s_bm_block_start+l*sizeof(llenar),SEEK_SET);
                                                                fread(&actual, sizeof(llenar),1,arch);

                                                                if(actual==0)
                                                                {
                                                                    fseek(arch, superBlock.s_bm_block_start+l*sizeof(llenar),SEEK_SET);
                                                                    fwrite(&llenar, sizeof(llenar),1,arch);
                                                                    fwrite(&llenar, sizeof(llenar),1,arch);
                                                                    fwrite(&llenar, sizeof(llenar),1,arch);
                                                                    fwrite(&llenar, sizeof(llenar),1,arch);
                                                                    break;
                                                                }

                                                            }

                                                        }
                                                        break;

                                                    }


                                                }


                                                break;


                                            }

                                        }


                                        break;
                                    }

                                }


                            }
                            else if(i == 14)
                            {
                                //cuando estamos en el ultimo bloque del inodo, los bloques indirectos triples
                                BApun bapuntadorPrimero;
                                fseek(arch,superBlock.s_block_start+(inodoTemp1.i_block[i]*sizeof(BArchivo)),SEEK_SET);
                                fread(&bapuntadorPrimero,sizeof(BApun),1,arch);
                                for(int j = 0 ; j<4; j++)
                                {
                                    if(bapuntadorPrimero.b_apuntadores[j].b_inodo==-1)
                                    {
                                        BApun bapuntadorSegundo;
                                        fseek(arch,superBlock.s_block_start+(bapuntadorPrimero.b_apuntadores[j-1].b_inodo*sizeof(BArchivo)),SEEK_SET);
                                        fread(&bapuntadorSegundo,sizeof(BApun),1,arch);
                                        for(int k = 0 ; k<4; k++)
                                        {
                                            if(bapuntadorSegundo.b_apuntadores[k].b_inodo==-1)
                                            {
                                                BApun bapuntadorTercero;
                                                fseek(arch,superBlock.s_block_start+(bapuntadorSegundo.b_apuntadores[k-1].b_inodo*sizeof(BArchivo)),SEEK_SET);
                                                fread(&bapuntadorTercero,sizeof(BApun),1,arch);
                                                for(int l = 0 ; l<4; l++)
                                                {

                                                    if(bapuntadorTercero.b_apuntadores[l].b_inodo==-1)
                                                    {
                                                        fseek(arch,superBlock.s_block_start+(bapuntadorTercero.b_apuntadores[l-1].b_inodo*sizeof(BArchivo)),SEEK_SET);
                                                        fread(&texto,sizeof(BArchivo),1,arch);
                                                        nuevaCadena=texto.b_content;
                                                        nuevaC = split(nuevaCadena,'?');

                                                        nuevaC[0]+=nuevoCont;
                                                        if(nuevaC[0].size()<64)
                                                        {
                                                            strcpy(texto.b_content,nuevaC[0].c_str());
                                                            fseek(arch, superBlock.s_block_start+bapuntadorTercero.b_apuntadores[l-1].b_inodo*sizeof(BArchivo),SEEK_SET);
                                                            fwrite(&texto,sizeof(BArchivo),1,arch);

                                                            time_t rawtime;
                                                            struct tm *timeinfo;
                                                            time(&rawtime);
                                                            timeinfo = localtime(&rawtime);
                                                            string fecha = asctime(timeinfo);
                                                            _mTime fcreacion;
                                                            strcpy(fcreacion.mbr_fecha_creacion,fecha.c_str());
                                                            inodoTemp1.i_mtime = fcreacion;

                                                            fseek(arch, superBlock.s_inode_start+sizeof(Inodo),SEEK_SET);
                                                            fwrite(&inodoTemp1,sizeof(Inodo),1,arch);
                                                            cout<<"Se creo el usuario exitosamente: " <<nombreUser<<endl;
                                                        }
                                                        else
                                                        {
                                                            nuevaCadena = nuevaC[0];
                                                            string textEnBlock = "";
                                                            int bitCont = 0;
                                                            for(int n = 0; n< nuevaC[0].size(); n++)
                                                            {
                                                                if(bitCont<63)
                                                                {
                                                                    textEnBlock += nuevaCadena[n];
                                                                }
                                                                else
                                                                {
                                                                    bitCont = -1;
                                                                    strcpy(texto.b_content,textEnBlock.c_str());
                                                                    fseek(arch,(superBlock.s_block_start+bapuntadorTercero.b_apuntadores[l-1].b_inodo*sizeof(BArchivo)),SEEK_SET);
                                                                    fwrite(&texto,sizeof(BArchivo),1,arch);
                                                                    textEnBlock = "";
                                                                    j--;
                                                                }
                                                                bitCont ++;
                                                            }
                                                            bapuntadorTercero.b_apuntadores[l].b_inodo = superBlock.s_first_blo;
                                                            fseek(arch,superBlock.s_block_start+(bapuntadorSegundo.b_apuntadores[k-1].b_inodo*sizeof(BArchivo)),SEEK_SET);
                                                            fwrite(&bapuntadorTercero,sizeof(BApun),1,arch);

                                                            strcpy(texto.b_content,textEnBlock.c_str());
                                                            fseek(arch,(superBlock.s_block_start+bapuntadorTercero.b_apuntadores[l].b_inodo*sizeof(BArchivo)),SEEK_SET);
                                                            fwrite(&texto,sizeof(BArchivo),1,arch);


                                                            time_t rawtime;
                                                            struct tm *timeinfo;
                                                            time(&rawtime);
                                                            timeinfo = localtime(&rawtime);
                                                            string fecha = asctime(timeinfo);
                                                            _mTime fcreacion;
                                                            strcpy(fcreacion.mbr_fecha_creacion,fecha.c_str());
                                                            inodoTemp1.i_mtime = fcreacion;

                                                            fseek(arch, superBlock.s_inode_start+sizeof(Inodo),SEEK_SET);
                                                            fwrite(&inodoTemp1,sizeof(Inodo),1,arch);
                                                            superBlock.s_first_blo = superBlock.s_first_blo + 1;
                                                            superBlock.s_free_blocks_count = superBlock.s_free_blocks_count - 1;
                                                            fseek(arch,iniPart,SEEK_SET);
                                                            fwrite(&superBlock,sizeof(SupB),1,arch);

                                                            cout<<"Se creo el usuario exitosamente: " <<nombreUser<<endl;

                                                            int llenar = 1;
                                                            int actual;
                                                            for(int n = 0 ; n < superBlock.s_blocks_count; n++)
                                                            {
                                                                fseek(arch, superBlock.s_bm_block_start+n*sizeof(llenar),SEEK_SET);
                                                                fread(&actual, sizeof(llenar),1,arch);

                                                                if(actual==0)
                                                                {
                                                                    fseek(arch, superBlock.s_bm_block_start+n*sizeof(llenar),SEEK_SET);
                                                                    fwrite(&llenar, sizeof(llenar),1,arch);

                                                                    break;
                                                                }

                                                            }
                                                        }
                                                        break;

                                                    }
                                                    else if(l==3)
                                                    {
                                                        fseek(arch,superBlock.s_block_start+(bapuntadorTercero.b_apuntadores[l].b_inodo*sizeof(BArchivo)),SEEK_SET);
                                                        fread(&texto,sizeof(BArchivo),1,arch);
                                                        nuevaCadena=texto.b_content;
                                                        nuevaC = split(nuevaCadena,'?');

                                                        nuevaC[0]+=nuevoCont;
                                                        if(nuevaC[0].size()<64)
                                                        {
                                                            strcpy(texto.b_content,nuevaC[0].c_str());
                                                            fseek(arch, superBlock.s_block_start+bapuntadorTercero.b_apuntadores[l].b_inodo*sizeof(BArchivo),SEEK_SET);
                                                            fwrite(&texto,sizeof(BArchivo),1,arch);

                                                            time_t rawtime;
                                                            struct tm *timeinfo;
                                                            time(&rawtime);
                                                            timeinfo = localtime(&rawtime);
                                                            string fecha = asctime(timeinfo);
                                                            _mTime fcreacion;
                                                            strcpy(fcreacion.mbr_fecha_creacion,fecha.c_str());
                                                            inodoTemp1.i_mtime = fcreacion;

                                                            fseek(arch, superBlock.s_inode_start+sizeof(Inodo),SEEK_SET);
                                                            fwrite(&inodoTemp1,sizeof(Inodo),1,arch);
                                                            cout<<"Se creo el usuario exitosamente: " <<nombreUser<<endl;
                                                        }
                                                        else
                                                        {
                                                        cout<<"ERROR: No se pueden crear mas usuarios, espacio insuficiente"<<endl;
                                                        }
                                                        break;

                                                    }
                                                }
                                                break;


                                            }
                                            else if(k==3)
                                            {
                                                BApun bapuntadorTercero;
                                                fseek(arch,superBlock.s_block_start+(bapuntadorSegundo.b_apuntadores[k].b_inodo*sizeof(BArchivo)),SEEK_SET);
                                                fread(&bapuntadorTercero,sizeof(BApun),1,arch);
                                                for(int l = 0 ; l<4; l++)
                                                {

                                                    if(bapuntadorTercero.b_apuntadores[l].b_inodo==-1)
                                                    {
                                                        fseek(arch,superBlock.s_block_start+(bapuntadorTercero.b_apuntadores[l-1].b_inodo*sizeof(BArchivo)),SEEK_SET);
                                                        fread(&texto,sizeof(BArchivo),1,arch);
                                                        nuevaCadena=texto.b_content;
                                                        nuevaC = split(nuevaCadena,'?');

                                                        nuevaC[0]+=nuevoCont;
                                                        if(nuevaC[0].size()<64)
                                                        {
                                                            strcpy(texto.b_content,nuevaC[0].c_str());
                                                            fseek(arch, superBlock.s_block_start+bapuntadorTercero.b_apuntadores[l-1].b_inodo*sizeof(BArchivo),SEEK_SET);
                                                            fwrite(&texto,sizeof(BArchivo),1,arch);

                                                            time_t rawtime;
                                                            struct tm *timeinfo;
                                                            time(&rawtime);
                                                            timeinfo = localtime(&rawtime);
                                                            string fecha = asctime(timeinfo);
                                                            _mTime fcreacion;
                                                            strcpy(fcreacion.mbr_fecha_creacion,fecha.c_str());
                                                            inodoTemp1.i_mtime = fcreacion;

                                                            fseek(arch, superBlock.s_inode_start+sizeof(Inodo),SEEK_SET);
                                                            fwrite(&inodoTemp1,sizeof(Inodo),1,arch);
                                                            cout<<"Se creo el usuario exitosamente: " <<nombreUser<<endl;
                                                        }
                                                        else
                                                        {
                                                            nuevaCadena = nuevaC[0];
                                                            string textEnBlock = "";
                                                            int bitCont = 0;
                                                            for(int n = 0; n< nuevaC[0].size(); n++)
                                                            {
                                                                if(bitCont<63)
                                                                {
                                                                    textEnBlock += nuevaCadena[n];
                                                                }
                                                                else
                                                                {
                                                                    bitCont = -1;
                                                                    strcpy(texto.b_content,textEnBlock.c_str());
                                                                    fseek(arch,(superBlock.s_block_start+bapuntadorTercero.b_apuntadores[l-1].b_inodo*sizeof(BArchivo)),SEEK_SET);
                                                                    fwrite(&texto,sizeof(BArchivo),1,arch);
                                                                    textEnBlock = "";
                                                                    j--;
                                                                }
                                                                bitCont ++;
                                                            }
                                                            bapuntadorTercero.b_apuntadores[l].b_inodo = superBlock.s_first_blo;
                                                            fseek(arch,superBlock.s_block_start+(bapuntadorSegundo.b_apuntadores[k].b_inodo*sizeof(BArchivo)),SEEK_SET);
                                                            fwrite(&bapuntadorTercero,sizeof(BApun),1,arch);

                                                            strcpy(texto.b_content,textEnBlock.c_str());
                                                            fseek(arch,(superBlock.s_block_start+bapuntadorTercero.b_apuntadores[l].b_inodo*sizeof(BArchivo)),SEEK_SET);
                                                            fwrite(&texto,sizeof(BArchivo),1,arch);


                                                            time_t rawtime;
                                                            struct tm *timeinfo;
                                                            time(&rawtime);
                                                            timeinfo = localtime(&rawtime);
                                                            string fecha = asctime(timeinfo);
                                                            _mTime fcreacion;
                                                            strcpy(fcreacion.mbr_fecha_creacion,fecha.c_str());
                                                            inodoTemp1.i_mtime = fcreacion;

                                                            fseek(arch, superBlock.s_inode_start+sizeof(Inodo),SEEK_SET);
                                                            fwrite(&inodoTemp1,sizeof(Inodo),1,arch);
                                                            superBlock.s_first_blo = superBlock.s_first_blo + 1;
                                                            superBlock.s_free_blocks_count = superBlock.s_free_blocks_count - 1;
                                                            fseek(arch,iniPart,SEEK_SET);
                                                            fwrite(&superBlock,sizeof(SupB),1,arch);

                                                            cout<<"Se creo el usuario exitosamente: " <<nombreUser<<endl;

                                                            int llenar = 1;
                                                            int actual;
                                                            for(int n = 0 ; n < superBlock.s_blocks_count; n++)
                                                            {
                                                                fseek(arch, superBlock.s_bm_block_start+n*sizeof(llenar),SEEK_SET);
                                                                fread(&actual, sizeof(llenar),1,arch);

                                                                if(actual==0)
                                                                {
                                                                    fseek(arch, superBlock.s_bm_block_start+n*sizeof(llenar),SEEK_SET);
                                                                    fwrite(&llenar, sizeof(llenar),1,arch);

                                                                    break;
                                                                }

                                                            }
                                                        }
                                                        break;

                                                    }
                                                    else if(l==3)
                                                    {
                                                        fseek(arch,superBlock.s_block_start+(bapuntadorTercero.b_apuntadores[l].b_inodo*sizeof(BArchivo)),SEEK_SET);
                                                        fread(&texto,sizeof(BArchivo),1,arch);
                                                        nuevaCadena=texto.b_content;
                                                        nuevaC = split(nuevaCadena,'?');

                                                        nuevaC[0]+=nuevoCont;
                                                        if(nuevaC[0].size()<64)
                                                        {
                                                            strcpy(texto.b_content,nuevaC[0].c_str());
                                                            fseek(arch, superBlock.s_block_start+bapuntadorTercero.b_apuntadores[l].b_inodo*sizeof(BArchivo),SEEK_SET);
                                                            fwrite(&texto,sizeof(BArchivo),1,arch);

                                                            time_t rawtime;
                                                            struct tm *timeinfo;
                                                            time(&rawtime);
                                                            timeinfo = localtime(&rawtime);
                                                            string fecha = asctime(timeinfo);
                                                            _mTime fcreacion;
                                                            strcpy(fcreacion.mbr_fecha_creacion,fecha.c_str());
                                                            inodoTemp1.i_mtime = fcreacion;

                                                            fseek(arch, superBlock.s_inode_start+sizeof(Inodo),SEEK_SET);
                                                            fwrite(&inodoTemp1,sizeof(Inodo),1,arch);
                                                            cout<<"Se creo el usuario exitosamente: " <<nombreUser<<endl;
                                                        }
                                                        else
                                                        {
                                                            nuevaCadena = nuevaC[0];
                                                            string textEnBlock = "";
                                                            int bitCont = 0;
                                                            for(int n = 0; n< nuevaC[0].size(); n++)
                                                            {
                                                                if(bitCont<63)
                                                                {
                                                                    textEnBlock += nuevaCadena[n];
                                                                }
                                                                else
                                                                {
                                                                    bitCont = -1;
                                                                    strcpy(texto.b_content,textEnBlock.c_str());
                                                                    fseek(arch,(superBlock.s_block_start+bapuntadorTercero.b_apuntadores[l-1].b_inodo*sizeof(BArchivo)),SEEK_SET);
                                                                    fwrite(&texto,sizeof(BArchivo),1,arch);
                                                                    textEnBlock = "";
                                                                    j--;
                                                                }
                                                                bitCont ++;
                                                            }

                                                            BApun apuntadorIndirectoD2;
                                                            apuntadorIndirectoD2.b_apuntadores[0].b_inodo= superBlock.s_first_blo + 2;
                                                            apuntadorIndirectoD2.b_apuntadores[1].b_inodo= -1;
                                                            apuntadorIndirectoD2.b_apuntadores[2].b_inodo= -1;
                                                            apuntadorIndirectoD2.b_apuntadores[3].b_inodo= -1;

                                                            string apunt = "Apuntador1";
                                                            strcpy(apuntadorIndirectoD2.b_apuntadores[0].b_name,apunt.c_str());
                                                            apunt = "Apuntador2";
                                                            strcpy(apuntadorIndirectoD2.b_apuntadores[1].b_name,apunt.c_str());
                                                            apunt = "Apuntador3";
                                                            strcpy(apuntadorIndirectoD2.b_apuntadores[2].b_name,apunt.c_str());
                                                            apunt = "Apuntador4";
                                                            strcpy(apuntadorIndirectoD2.b_apuntadores[3].b_name,apunt.c_str());

                                                            BApun apuntadorIndirectoD1;
                                                            apuntadorIndirectoD1.b_apuntadores[0].b_inodo= superBlock.s_first_blo + 1;
                                                            apuntadorIndirectoD1.b_apuntadores[1].b_inodo= -1;
                                                            apuntadorIndirectoD1.b_apuntadores[2].b_inodo= -1;
                                                            apuntadorIndirectoD1.b_apuntadores[3].b_inodo= -1;

                                                            apunt = "Apuntador1";
                                                            strcpy(apuntadorIndirectoD1.b_apuntadores[0].b_name,apunt.c_str());
                                                            apunt = "Apuntador2";
                                                            strcpy(apuntadorIndirectoD1.b_apuntadores[1].b_name,apunt.c_str());
                                                            apunt = "Apuntador3";
                                                            strcpy(apuntadorIndirectoD1.b_apuntadores[2].b_name,apunt.c_str());
                                                            apunt = "Apuntador4";
                                                            strcpy(apuntadorIndirectoD1.b_apuntadores[3].b_name,apunt.c_str());

                                                            bapuntadorPrimero.b_apuntadores[j].b_inodo = superBlock.s_first_blo;
                                                            fseek(arch,superBlock.s_block_start+(inodoTemp1.i_block[i]*sizeof(BArchivo)),SEEK_SET);
                                                            fwrite(&bapuntadorPrimero,sizeof(BApun),1,arch);

                                                            fseek(arch, superBlock.s_block_start+(superBlock.s_first_blo)*sizeof(BArchivo),SEEK_SET);
                                                            fwrite(&apuntadorIndirectoD1,sizeof(BApun),1,arch);

                                                            fseek(arch, superBlock.s_block_start+(superBlock.s_first_blo+1)*sizeof(BArchivo),SEEK_SET);
                                                            fwrite(&apuntadorIndirectoD2,sizeof(BApun),1,arch);

                                                            strcpy(texto.b_content,textEnBlock.c_str());
                                                            fseek(arch, superBlock.s_block_start+(superBlock.s_first_blo+2)*sizeof(BArchivo),SEEK_SET);
                                                            fwrite(&texto,sizeof(BArchivo),1,arch);



                                                            time_t rawtime;
                                                            struct tm *timeinfo;
                                                            time(&rawtime);
                                                            timeinfo = localtime(&rawtime);
                                                            string fecha = asctime(timeinfo);
                                                            _mTime fcreacion;
                                                            strcpy(fcreacion.mbr_fecha_creacion,fecha.c_str());
                                                            inodoTemp1.i_mtime = fcreacion;

                                                            fseek(arch, superBlock.s_inode_start+sizeof(Inodo),SEEK_SET);
                                                            fwrite(&inodoTemp1,sizeof(Inodo),1,arch);
                                                            superBlock.s_first_blo = superBlock.s_first_blo + 3;
                                                            superBlock.s_free_blocks_count = superBlock.s_free_blocks_count - 3;
                                                            fseek(arch,iniPart,SEEK_SET);
                                                            fwrite(&superBlock,sizeof(SupB),1,arch);

                                                            cout<<"Se creo el usuario exitosamente: " <<nombreUser<<endl;

                                                            int llenar = 1;
                                                            int actual;
                                                            for(int n = 0 ; n < superBlock.s_blocks_count; n++)
                                                            {
                                                                fseek(arch, superBlock.s_bm_block_start+n*sizeof(llenar),SEEK_SET);
                                                                fread(&actual, sizeof(llenar),1,arch);

                                                                if(actual==0)
                                                                {
                                                                    fseek(arch, superBlock.s_bm_block_start+n*sizeof(llenar),SEEK_SET);
                                                                    fwrite(&llenar, sizeof(llenar),1,arch);
                                                                    fwrite(&llenar, sizeof(llenar),1,arch);
                                                                    fwrite(&llenar, sizeof(llenar),1,arch);

                                                                    break;
                                                                }

                                                            }

                                                        }
                                                        break;

                                                    }
                                                }
                                                break;

                                            }

                                        }
                                        break;

                                    }
                                    else if(j==3)
                                    {
                                        BApun bapuntadorSegundo;
                                        fseek(arch,superBlock.s_block_start+(bapuntadorPrimero.b_apuntadores[j].b_inodo*sizeof(BArchivo)),SEEK_SET);
                                        fread(&bapuntadorSegundo,sizeof(BApun),1,arch);
                                        for(int k = 0 ; k<4; k++)
                                        {
                                            if(bapuntadorSegundo.b_apuntadores[k].b_inodo==-1)
                                            {
                                                BApun bapuntadorTercero;
                                                fseek(arch,superBlock.s_block_start+(bapuntadorSegundo.b_apuntadores[k-1].b_inodo*sizeof(BArchivo)),SEEK_SET);
                                                fread(&bapuntadorTercero,sizeof(BApun),1,arch);
                                                for(int l = 0 ; l<4; l++)
                                                {

                                                    if(bapuntadorTercero.b_apuntadores[l].b_inodo==-1)
                                                    {
                                                        fseek(arch,superBlock.s_block_start+(bapuntadorTercero.b_apuntadores[l-1].b_inodo*sizeof(BArchivo)),SEEK_SET);
                                                        fread(&texto,sizeof(BArchivo),1,arch);
                                                        nuevaCadena=texto.b_content;
                                                        nuevaC = split(nuevaCadena,'?');

                                                        nuevaC[0]+=nuevoCont;
                                                        if(nuevaC[0].size()<64)
                                                        {
                                                            strcpy(texto.b_content,nuevaC[0].c_str());
                                                            fseek(arch, superBlock.s_block_start+bapuntadorTercero.b_apuntadores[l-1].b_inodo*sizeof(BArchivo),SEEK_SET);
                                                            fwrite(&texto,sizeof(BArchivo),1,arch);

                                                            time_t rawtime;
                                                            struct tm *timeinfo;
                                                            time(&rawtime);
                                                            timeinfo = localtime(&rawtime);
                                                            string fecha = asctime(timeinfo);
                                                            _mTime fcreacion;
                                                            strcpy(fcreacion.mbr_fecha_creacion,fecha.c_str());
                                                            inodoTemp1.i_mtime = fcreacion;

                                                            fseek(arch, superBlock.s_inode_start+sizeof(Inodo),SEEK_SET);
                                                            fwrite(&inodoTemp1,sizeof(Inodo),1,arch);
                                                            cout<<"Se creo el usuario exitosamente: " <<nombreUser<<endl;
                                                        }
                                                        else
                                                        {
                                                            nuevaCadena = nuevaC[0];
                                                            string textEnBlock = "";
                                                            int bitCont = 0;
                                                            for(int n = 0; n< nuevaC[0].size(); n++)
                                                            {
                                                                if(bitCont<63)
                                                                {
                                                                    textEnBlock += nuevaCadena[n];
                                                                }
                                                                else
                                                                {
                                                                    bitCont = -1;
                                                                    strcpy(texto.b_content,textEnBlock.c_str());
                                                                    fseek(arch,(superBlock.s_block_start+bapuntadorTercero.b_apuntadores[l-1].b_inodo*sizeof(BArchivo)),SEEK_SET);
                                                                    fwrite(&texto,sizeof(BArchivo),1,arch);
                                                                    textEnBlock = "";
                                                                    j--;
                                                                }
                                                                bitCont ++;
                                                            }
                                                            bapuntadorTercero.b_apuntadores[l].b_inodo = superBlock.s_first_blo;
                                                            fseek(arch,superBlock.s_block_start+(bapuntadorSegundo.b_apuntadores[k-1].b_inodo*sizeof(BArchivo)),SEEK_SET);
                                                            fwrite(&bapuntadorTercero,sizeof(BApun),1,arch);

                                                            strcpy(texto.b_content,textEnBlock.c_str());
                                                            fseek(arch,(superBlock.s_block_start+bapuntadorTercero.b_apuntadores[l].b_inodo*sizeof(BArchivo)),SEEK_SET);
                                                            fwrite(&texto,sizeof(BArchivo),1,arch);


                                                            time_t rawtime;
                                                            struct tm *timeinfo;
                                                            time(&rawtime);
                                                            timeinfo = localtime(&rawtime);
                                                            string fecha = asctime(timeinfo);
                                                            _mTime fcreacion;
                                                            strcpy(fcreacion.mbr_fecha_creacion,fecha.c_str());
                                                            inodoTemp1.i_mtime = fcreacion;

                                                            fseek(arch, superBlock.s_inode_start+sizeof(Inodo),SEEK_SET);
                                                            fwrite(&inodoTemp1,sizeof(Inodo),1,arch);
                                                            superBlock.s_first_blo = superBlock.s_first_blo + 1;
                                                            superBlock.s_free_blocks_count = superBlock.s_free_blocks_count - 1;
                                                            fseek(arch,iniPart,SEEK_SET);
                                                            fwrite(&superBlock,sizeof(SupB),1,arch);

                                                            cout<<"Se creo el usuario exitosamente: " <<nombreUser<<endl;

                                                            int llenar = 1;
                                                            int actual;
                                                            for(int n = 0 ; n < superBlock.s_blocks_count; n++)
                                                            {
                                                                fseek(arch, superBlock.s_bm_block_start+n*sizeof(llenar),SEEK_SET);
                                                                fread(&actual, sizeof(llenar),1,arch);

                                                                if(actual==0)
                                                                {
                                                                    fseek(arch, superBlock.s_bm_block_start+n*sizeof(llenar),SEEK_SET);
                                                                    fwrite(&llenar, sizeof(llenar),1,arch);

                                                                    break;
                                                                }

                                                            }
                                                        }
                                                        break;

                                                    }
                                                    else if(l==3)
                                                    {
                                                        fseek(arch,superBlock.s_block_start+(bapuntadorTercero.b_apuntadores[l].b_inodo*sizeof(BArchivo)),SEEK_SET);
                                                        fread(&texto,sizeof(BArchivo),1,arch);
                                                        nuevaCadena=texto.b_content;
                                                        nuevaC = split(nuevaCadena,'?');

                                                        nuevaC[0]+=nuevoCont;
                                                        if(nuevaC[0].size()<64)
                                                        {
                                                            strcpy(texto.b_content,nuevaC[0].c_str());
                                                            fseek(arch, superBlock.s_block_start+bapuntadorTercero.b_apuntadores[l].b_inodo*sizeof(BArchivo),SEEK_SET);
                                                            fwrite(&texto,sizeof(BArchivo),1,arch);

                                                            time_t rawtime;
                                                            struct tm *timeinfo;
                                                            time(&rawtime);
                                                            timeinfo = localtime(&rawtime);
                                                            string fecha = asctime(timeinfo);
                                                            _mTime fcreacion;
                                                            strcpy(fcreacion.mbr_fecha_creacion,fecha.c_str());
                                                            inodoTemp1.i_mtime = fcreacion;

                                                            fseek(arch, superBlock.s_inode_start+sizeof(Inodo),SEEK_SET);
                                                            fwrite(&inodoTemp1,sizeof(Inodo),1,arch);
                                                            cout<<"Se creo el usuario exitosamente: " <<nombreUser<<endl;
                                                        }
                                                        else
                                                        {
                                                        cout<<"ERROR: No se pueden crear mas usuarios, espacio insuficiente"<<endl;
                                                        }
                                                        break;

                                                    }
                                                }
                                                break;


                                            }
                                            else if(k==3)
                                            {
                                                BApun bapuntadorTercero;
                                                fseek(arch,superBlock.s_block_start+(bapuntadorSegundo.b_apuntadores[k].b_inodo*sizeof(BArchivo)),SEEK_SET);
                                                fread(&bapuntadorTercero,sizeof(BApun),1,arch);
                                                for(int l = 0 ; l<4; l++)
                                                {

                                                    if(bapuntadorTercero.b_apuntadores[l].b_inodo==-1)
                                                    {
                                                        fseek(arch,superBlock.s_block_start+(bapuntadorTercero.b_apuntadores[l-1].b_inodo*sizeof(BArchivo)),SEEK_SET);
                                                        fread(&texto,sizeof(BArchivo),1,arch);
                                                        nuevaCadena=texto.b_content;
                                                        nuevaC = split(nuevaCadena,'?');

                                                        nuevaC[0]+=nuevoCont;
                                                        if(nuevaC[0].size()<64)
                                                        {
                                                            strcpy(texto.b_content,nuevaC[0].c_str());
                                                            fseek(arch, superBlock.s_block_start+bapuntadorTercero.b_apuntadores[l-1].b_inodo*sizeof(BArchivo),SEEK_SET);
                                                            fwrite(&texto,sizeof(BArchivo),1,arch);

                                                            time_t rawtime;
                                                            struct tm *timeinfo;
                                                            time(&rawtime);
                                                            timeinfo = localtime(&rawtime);
                                                            string fecha = asctime(timeinfo);
                                                            _mTime fcreacion;
                                                            strcpy(fcreacion.mbr_fecha_creacion,fecha.c_str());
                                                            inodoTemp1.i_mtime = fcreacion;

                                                            fseek(arch, superBlock.s_inode_start+sizeof(Inodo),SEEK_SET);
                                                            fwrite(&inodoTemp1,sizeof(Inodo),1,arch);
                                                            cout<<"Se creo el usuario exitosamente: " <<nombreUser<<endl;
                                                        }
                                                        else
                                                        {
                                                            nuevaCadena = nuevaC[0];
                                                            string textEnBlock = "";
                                                            int bitCont = 0;
                                                            for(int n = 0; n< nuevaC[0].size(); n++)
                                                            {
                                                                if(bitCont<63)
                                                                {
                                                                    textEnBlock += nuevaCadena[n];
                                                                }
                                                                else
                                                                {
                                                                    bitCont = -1;
                                                                    strcpy(texto.b_content,textEnBlock.c_str());
                                                                    fseek(arch,(superBlock.s_block_start+bapuntadorTercero.b_apuntadores[l-1].b_inodo*sizeof(BArchivo)),SEEK_SET);
                                                                    fwrite(&texto,sizeof(BArchivo),1,arch);
                                                                    textEnBlock = "";
                                                                    j--;
                                                                }
                                                                bitCont ++;
                                                            }
                                                            bapuntadorTercero.b_apuntadores[l].b_inodo = superBlock.s_first_blo;
                                                            fseek(arch,superBlock.s_block_start+(bapuntadorSegundo.b_apuntadores[k].b_inodo*sizeof(BArchivo)),SEEK_SET);
                                                            fwrite(&bapuntadorTercero,sizeof(BApun),1,arch);

                                                            strcpy(texto.b_content,textEnBlock.c_str());
                                                            fseek(arch,(superBlock.s_block_start+bapuntadorTercero.b_apuntadores[l].b_inodo*sizeof(BArchivo)),SEEK_SET);
                                                            fwrite(&texto,sizeof(BArchivo),1,arch);


                                                            time_t rawtime;
                                                            struct tm *timeinfo;
                                                            time(&rawtime);
                                                            timeinfo = localtime(&rawtime);
                                                            string fecha = asctime(timeinfo);
                                                            _mTime fcreacion;
                                                            strcpy(fcreacion.mbr_fecha_creacion,fecha.c_str());
                                                            inodoTemp1.i_mtime = fcreacion;

                                                            fseek(arch, superBlock.s_inode_start+sizeof(Inodo),SEEK_SET);
                                                            fwrite(&inodoTemp1,sizeof(Inodo),1,arch);
                                                            superBlock.s_first_blo = superBlock.s_first_blo + 1;
                                                            superBlock.s_free_blocks_count = superBlock.s_free_blocks_count - 1;
                                                            fseek(arch,iniPart,SEEK_SET);
                                                            fwrite(&superBlock,sizeof(SupB),1,arch);

                                                            cout<<"Se creo el usuario exitosamente: " <<nombreUser<<endl;

                                                            int llenar = 1;
                                                            int actual;
                                                            for(int n = 0 ; n < superBlock.s_blocks_count; n++)
                                                            {
                                                                fseek(arch, superBlock.s_bm_block_start+n*sizeof(llenar),SEEK_SET);
                                                                fread(&actual, sizeof(llenar),1,arch);

                                                                if(actual==0)
                                                                {
                                                                    fseek(arch, superBlock.s_bm_block_start+n*sizeof(llenar),SEEK_SET);
                                                                    fwrite(&llenar, sizeof(llenar),1,arch);

                                                                    break;
                                                                }

                                                            }
                                                        }
                                                        break;

                                                    }
                                                    else if(l==3)
                                                    {
                                                        fseek(arch,superBlock.s_block_start+(bapuntadorTercero.b_apuntadores[l].b_inodo*sizeof(BArchivo)),SEEK_SET);
                                                        fread(&texto,sizeof(BArchivo),1,arch);
                                                        nuevaCadena=texto.b_content;
                                                        nuevaC = split(nuevaCadena,'?');

                                                        nuevaC[0]+=nuevoCont;
                                                        if(nuevaC[0].size()<64)
                                                        {
                                                            strcpy(texto.b_content,nuevaC[0].c_str());
                                                            fseek(arch, superBlock.s_block_start+bapuntadorTercero.b_apuntadores[l].b_inodo*sizeof(BArchivo),SEEK_SET);
                                                            fwrite(&texto,sizeof(BArchivo),1,arch);

                                                            time_t rawtime;
                                                            struct tm *timeinfo;
                                                            time(&rawtime);
                                                            timeinfo = localtime(&rawtime);
                                                            string fecha = asctime(timeinfo);
                                                            _mTime fcreacion;
                                                            strcpy(fcreacion.mbr_fecha_creacion,fecha.c_str());
                                                            inodoTemp1.i_mtime = fcreacion;

                                                            fseek(arch, superBlock.s_inode_start+sizeof(Inodo),SEEK_SET);
                                                            fwrite(&inodoTemp1,sizeof(Inodo),1,arch);
                                                            cout<<"Se creo el usuario exitosamente: " <<nombreUser<<endl;
                                                        }
                                                        else
                                                        {
                                                        cout<<"ERROR: No se pueden crear mas usuarios, espacio insuficiente"<<endl;
                                                        }
                                                        break;

                                                    }
                                                }
                                                break;

                                            }

                                        }
                                        break;

                                    }

                                }
                                break;


                            }
                        }

                    }
                    else
                    {
                        //mas de 96 bloques
                        cout<<"ERROR: No se pueden crear mas usuarios"<<endl;
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


