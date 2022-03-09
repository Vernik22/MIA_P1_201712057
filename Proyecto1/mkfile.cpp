#include "mkfile.h"
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <fstream>
#include <stdio.h>
#include <iostream>

using namespace std;

mkfile::mkfile()
{
    //ctor
}

mkfile::~mkfile()
{
    //dtor
}

void mkfile::ejecutarComandoMkfile(mkfile *mfile,mount paMoun[])
{
    cout<<"\n************Ejecutar MKFILE************\n"<<endl;


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
    modificarArchivo(pathDisco, nombrePart,mfile);


}
vector<string> mkfile::split(string str, char pattern)
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

string mkfile::retFecha()
{

    time_t t = time(NULL);
    struct tm tiempoLocal = *localtime(&t);
    // El lugar en donde se pondrá la fecha y hora formateadas
    char fechaHora[70];
    // El formato. Mira más en https://en.cppreference.com/w/c/chrono/strftime
    char *formato = "%Y-%m-%d %H:%M:%S";
    // Intentar formatear
    int bytesEscritos =
        strftime(fechaHora, sizeof fechaHora, formato, &tiempoLocal);
    string fecha = fechaHora;
    return fecha;
}

void mkfile::modificarArchivo(string pathDisco, string nombrePart, mkfile *mfile)
{
    vector<string> rutaArchivo;
    rutaArchivo = split(mfile->getPath(), '/');
    SupB superBloque;

    FILE *arch;
    arch = fopen(pathDisco.c_str(),"rb+");
    if(arch != NULL)
    {

        MBR mbrTemp;
        fseek(arch,0,SEEK_SET);
        fread(&mbrTemp,sizeof(MBR),1,arch);

        int tamParticion, iniPart;
        returnDatosPart(mbrTemp, pathDisco,nombrePart,tamParticion, iniPart);
        fseek(arch,iniPart,SEEK_SET);
        fread(&superBloque,sizeof(SupB),1,arch);

        Inodo inodoAnterior;
        Inodo inodoTemp;
        fseek(arch,superBloque.s_inode_start,SEEK_SET);
        fread(&inodoTemp,sizeof(Inodo),1,arch);

        bool existeCarpeta = false;
        int carp = rutaArchivo.size();

        //cout<<mfile->getR()<<endl;

        for(int i= 1; i< carp ; i++)
        {
            existeCarpeta = false;

            if(inodoTemp.i_type == '0')
            {
                for(int j = 0; j<15; j++)
                {
                    if(j<12)
                    {
                        if(inodoTemp.i_block[j] != -1 )
                        {
                            BCarpeta carpetaComprobar;
                            fseek(arch,superBloque.s_block_start +(inodoTemp.i_block[j]*sizeof(BCarpeta)),SEEK_SET);
                            fread(&carpetaComprobar,sizeof(BCarpeta),1,arch);

                            for(int k = 0; k<4; k++)
                            {
                                if(rutaArchivo[i] == carpetaComprobar.b_content[k].b_name)
                                {
                                    inodoAnterior = inodoTemp;
                                    fseek(arch,superBloque.s_inode_start+(carpetaComprobar.b_content[k].b_inodo * sizeof(Inodo)),SEEK_SET);
                                    fread(&inodoTemp,sizeof(Inodo),1,arch);
                                    existeCarpeta= true;
                                    j = 20;
                                    break;


                                }
                            }


                        }

                    }
                    else if(j==12)
                    {
                        if(inodoTemp.i_block[j] != -1 )
                        {
                            //cuerpo += "struct"+std::to_string(contador)+" [shape=record,label=\"<f0> Bloque Apuntador"+ std::to_string(contador)+"| ";
                            BApun apuntadores;
                            fseek(arch,superBloque.s_block_start +(inodoTemp.i_block[j]*sizeof(BCarpeta)),SEEK_SET);
                            fread(&apuntadores,sizeof(BApun),1,arch);


                            for(int k=0; k<4; k++)
                            {
                                if(apuntadores.b_apuntadores [k].b_inodo != -1)
                                {
                                    BCarpeta carpetaComprobar;
                                    fseek(arch,superBloque.s_block_start +(apuntadores.b_apuntadores [k].b_inodo*sizeof(BCarpeta)),SEEK_SET);
                                    fread(&carpetaComprobar,sizeof(BCarpeta),1,arch);

                                    for(int l = 0; l<4; l++)
                                    {
                                        if(rutaArchivo[i] == carpetaComprobar.b_content[l].b_name)
                                        {
                                            inodoAnterior = inodoTemp;
                                            fseek(arch,superBloque.s_inode_start+(carpetaComprobar.b_content[l].b_inodo * sizeof(Inodo)),SEEK_SET);
                                            fread(&inodoTemp,sizeof(Inodo),1,arch);
                                            existeCarpeta= true;
                                            j = 20;
                                            break;


                                        }
                                    }

                                }
                            }




                        }

                    }
                    else if(j==13)
                    {
                        if(inodoTemp.i_block[j] != -1 )
                        {
                            // cuerpo += "struct"+std::to_string(contador)+" [shape=record,label=\"<f0> Bloque Apuntador"+ std::to_string(contador)+"| ";
                            BApun apuntadores;
                            fseek(arch,superBloque.s_block_start +(inodoTemp.i_block[j]*sizeof(BCarpeta)),SEEK_SET);
                            fread(&apuntadores,sizeof(BApun),1,arch);


                            for(int k=0; k<4; k++)
                            {
                                if(apuntadores.b_apuntadores [k].b_inodo != -1)
                                {
                                    BApun apun1;
                                    fseek(arch,superBloque.s_block_start +(apuntadores.b_apuntadores [k].b_inodo*sizeof(BCarpeta)),SEEK_SET);
                                    fread(&apun1,sizeof(BApun),1,arch);
                                    for(int l=0; l<4; l++)
                                    {
                                        if(apun1.b_apuntadores [l].b_inodo != -1)
                                        {
                                            BCarpeta carpetaComprobar;
                                            fseek(arch,superBloque.s_block_start +(apuntadores.b_apuntadores [k].b_inodo*sizeof(BCarpeta)),SEEK_SET);
                                            fread(&carpetaComprobar,sizeof(BCarpeta),1,arch);

                                            for(int n = 0; n<4; n++)
                                            {
                                                if(rutaArchivo[i] == carpetaComprobar.b_content[n].b_name)
                                                {
                                                    inodoAnterior = inodoTemp;
                                                    fseek(arch,superBloque.s_inode_start+(carpetaComprobar.b_content[n].b_inodo * sizeof(Inodo)),SEEK_SET);
                                                    fread(&inodoTemp,sizeof(Inodo),1,arch);
                                                    existeCarpeta= true;
                                                    j = 20;
                                                    break;


                                                }
                                            }
                                        }
                                    }


                                }

                            }




                        }

                    }
                    else if(i==14)
                    {
                        if(inodoTemp.i_block[i] != -1 )
                        {
                            //cuerpo += "struct"+std::to_string(contador)+" [shape=record,label=\"<f0> Bloque Apuntador"+ std::to_string(contador)+"| ";
                            BApun apuntadores;
                            fseek(arch,superBloque.s_block_start +(inodoTemp.i_block[i]*sizeof(BCarpeta)),SEEK_SET);
                            fread(&apuntadores,sizeof(BApun),1,arch);


                            for(int k=0; k<4; k++)
                            {
                                if(apuntadores.b_apuntadores [k].b_inodo != -1)
                                {
                                    BApun apun1;
                                    fseek(arch,superBloque.s_block_start +(apuntadores.b_apuntadores[k].b_inodo*sizeof(BCarpeta)),SEEK_SET);
                                    fread(&apun1,sizeof(BApun),1,arch);
                                    for(int l=0; l<4; l++)
                                    {
                                        if(apun1.b_apuntadores [l].b_inodo != -1)
                                        {
                                            BApun apun2;
                                            fseek(arch,superBloque.s_block_start +(apun1.b_apuntadores[l].b_inodo*sizeof(BCarpeta)),SEEK_SET);
                                            fread(&apun2,sizeof(BApun),1,arch);

                                            for(int n=0; n<4; n++)
                                            {
                                                if(apun2.b_apuntadores [n].b_inodo != -1)
                                                {
                                                    BCarpeta carpetaComprobar;
                                                    fseek(arch,superBloque.s_block_start +(apuntadores.b_apuntadores [k].b_inodo*sizeof(BCarpeta)),SEEK_SET);
                                                    fread(&carpetaComprobar,sizeof(BCarpeta),1,arch);

                                                    for(int m = 0; m<4; m++)
                                                    {
                                                        if(rutaArchivo[i] == carpetaComprobar.b_content[m].b_name)
                                                        {
                                                            inodoAnterior = inodoTemp;
                                                            fseek(arch,superBloque.s_inode_start+(carpetaComprobar.b_content[m].b_inodo * sizeof(Inodo)),SEEK_SET);
                                                            fread(&inodoTemp,sizeof(Inodo),1,arch);
                                                            existeCarpeta= true;
                                                            j = 20;
                                                            break;


                                                        }
                                                    }

                                                }
                                            }

                                        }
                                    }


                                }

                            }


                        }

                    }
                }

            }


            if(!existeCarpeta)
            {
                if( i == carp -1 )
                {
                    if(mfile->getBoolCont())
                    {
                        Inodo inodoArchivoNuevo;
                        for(int f=0; f<15; f++)
                        {
                            inodoArchivoNuevo.i_block[f] = -1;
                        }

                        //inodoArchivoNuevo.i_block[0]= 1;
                        inodoArchivoNuevo.i_gid = 1;
                        inodoArchivoNuevo.i_uid = 1;
                        inodoArchivoNuevo.i_type = '1';
                        inodoArchivoNuevo.i_perm = 664;
                        string fecha = retFecha();
                        _mTime fcreacion;
                        strcpy(fcreacion.mbr_fecha_creacion,fecha.c_str());
                        inodoArchivoNuevo.i_mtime = fcreacion;
                        inodoArchivoNuevo.i_atime = fcreacion;
                        inodoArchivoNuevo.i_ctime = fcreacion;

                        ifstream f;
                        string data;

                        f.open(mfile->getCont(), ios::in); //abrimos el archivo en modo lectura

                        if (f.fail())
                        {
                            cout << "no se pudo abrir el archivo" << endl;
                        }
                        else
                        {
                            while (!f.eof()) //mientras no sea el final del archivo
                            {
                                getline(f, data);

                            }
                            f.close(); //cerramos el archivo

                            inodoArchivoNuevo.i_size = data.size();
                            int blocksAUsar = cantidadBloquesUsarString(data);
                            for(int f= 0; f<blocksAUsar; f++)
                            {
                                if(f<12)
                                {
                                    inodoArchivoNuevo.i_block[f] = superBloque.s_first_blo;
                                    superBloque.s_first_blo = superBloque.s_first_blo +1;
                                    superBloque.s_free_blocks_count = superBloque.s_free_blocks_count - 1;

                                    char llenar = '1';
                                    char actual;
                                    for(int j = 0 ; j < superBloque.s_blocks_count; j++)
                                    {
                                        fseek(arch, superBloque.s_bm_block_start+j*sizeof(llenar),SEEK_SET);
                                        fread(&actual, sizeof(llenar),1,arch);

                                        if(actual=='0')
                                        {
                                            fseek(arch, superBloque.s_bm_block_start+j*sizeof(llenar),SEEK_SET);
                                            fwrite(&llenar, sizeof(llenar),1,arch);
                                            break;
                                        }

                                    }

                                }

                            }
                            fseek(arch, superBloque.s_inode_start+(superBloque.s_first_ino*sizeof(Inodo)),SEEK_SET);
                            fwrite(&inodoArchivoNuevo,sizeof(Inodo),1,arch);
                            int condi = 0;
                            int posicion = 0;
                            while(inodoArchivoNuevo.i_block[condi] != -1)
                            {
                                int contNumero = 0;
                                string contenidoNuevo = "";


                                while(true)
                                {
                                    if(contNumero<63 && posicion < data.size())
                                    {
                                        contenidoNuevo += data[posicion];
                                        contNumero++ ;

                                        posicion ++;
                                    }
                                    else
                                    {
                                        break;
                                    }
                                }

                                BArchivo archivoNuevo;
                                strcpy(archivoNuevo.b_content,contenidoNuevo.c_str());
                                fseek(arch, superBloque.s_block_start+(inodoArchivoNuevo.i_block[condi]*sizeof(BArchivo)),SEEK_SET);
                                fwrite(&archivoNuevo,sizeof(BArchivo),1,arch);

                                condi ++;

                            }
                            for(int f = 0; f<15; f++)
                            {
                                if(f<12)
                                {
                                    if(inodoTemp.i_block[f] != -1 )
                                    {
                                        BCarpeta carpetaComprobar;
                                        fseek(arch,superBloque.s_block_start +(inodoTemp.i_block[f]*sizeof(BCarpeta)),SEEK_SET);
                                        fread(&carpetaComprobar,sizeof(BCarpeta),1,arch);

                                        for(int k = 0; k<4; k++)
                                        {
                                            if(carpetaComprobar.b_content[k].b_inodo == -1)
                                            {
                                                strcpy(carpetaComprobar.b_content[k].b_name,rutaArchivo[carp -1].c_str());
                                                carpetaComprobar.b_content[k].b_inodo = superBloque.s_first_ino;
                                                fseek(arch,superBloque.s_block_start +(inodoTemp.i_block[f]*sizeof(BCarpeta)),SEEK_SET);
                                                fwrite(&carpetaComprobar,sizeof(BCarpeta),1,arch);

                                                f = 20;
                                                break;


                                            }
                                        }


                                    }
                                    else
                                    {

                                        //crear nuevo bloque carpeta
                                        BCarpeta carpetaComprobar;
                                        for(int k = 1; k<4; k++)
                                        {
                                            carpetaComprobar.b_content[k].b_inodo = -1;
                                            string vacio = " ";
                                            strcpy(carpetaComprobar.b_content[k].b_name,vacio.c_str());
                                        }
                                        strcpy(carpetaComprobar.b_content[0].b_name,rutaArchivo[carp -1].c_str());
                                        carpetaComprobar.b_content[0].b_inodo = superBloque.s_first_ino;

                                        fseek(arch,superBloque.s_block_start +(superBloque.s_first_blo*sizeof(BCarpeta)),SEEK_SET);
                                        fwrite(&carpetaComprobar,sizeof(BCarpeta),1,arch);

                                        inodoTemp.i_block[f] = superBloque.s_first_blo;

                                        char llenar = '1';
                                        char actual;
                                        for(int j = 0 ; j < superBloque.s_blocks_count; j++)
                                        {
                                            fseek(arch, superBloque.s_bm_block_start+j*sizeof(llenar),SEEK_SET);
                                            fread(&actual, sizeof(llenar),1,arch);

                                            if(actual=='0')
                                            {
                                                fseek(arch, superBloque.s_bm_block_start+j*sizeof(llenar),SEEK_SET);
                                                fwrite(&llenar, sizeof(llenar),1,arch);
                                                break;
                                            }

                                        }
                                        superBloque.s_first_blo = superBloque.s_first_blo +1;
                                        superBloque.s_free_blocks_count = superBloque.s_free_blocks_count - 1;

                                        fseek(arch,superBloque.s_block_start +(inodoTemp.i_block[0]*sizeof(BCarpeta)),SEEK_SET);
                                        fread(&carpetaComprobar,sizeof(BCarpeta),1,arch);

                                        inodoTemp.i_mtime = fcreacion;
                                        fseek(arch,superBloque.s_inode_start + (carpetaComprobar.b_content[0].b_inodo*sizeof(Inodo)),SEEK_SET);
                                        fwrite(&inodoTemp,sizeof(Inodo),1,arch);
                                        break;
                                    }

                                }
                            }

                            superBloque.s_first_ino = superBloque.s_first_ino + 1;
                            superBloque.s_free_inodes_count = superBloque.s_free_inodes_count - 1;
                            fseek(arch,iniPart,SEEK_SET);
                            fwrite(&superBloque,sizeof(SupB),1,arch);
                            char llenar = '1';
                            char actual;
                            for(int j = 0 ; j < superBloque.s_inodes_count; j++)
                            {
                                fseek(arch, superBloque.s_bm_inode_start+j*sizeof(llenar),SEEK_SET);
                                fread(&actual, sizeof(llenar),1,arch);

                                if(actual=='0')
                                {
                                    fseek(arch, superBloque.s_bm_inode_start+j*sizeof(llenar),SEEK_SET);
                                    fwrite(&llenar, sizeof(llenar),1,arch);
                                    fwrite(&llenar, sizeof(llenar),1,arch);
                                    break;
                                }

                            }


                        }
                        cout << "Se creo el Archivo con exito" << endl;
                        break;

                    }
                    else
                    {
                        Inodo inodoArchivoNuevo;
                        for(int f=0; f<15; f++)
                        {
                            inodoArchivoNuevo.i_block[f] = -1;
                        }

                        //inodoArchivoNuevo.i_block[0]= 1;
                        inodoArchivoNuevo.i_gid = 1;
                        inodoArchivoNuevo.i_uid = 1;
                        inodoArchivoNuevo.i_type = '1';
                        inodoArchivoNuevo.i_perm = 664;
                        string fecha = retFecha();
                        _mTime fcreacion;
                        strcpy(fcreacion.mbr_fecha_creacion,fecha.c_str());
                        inodoArchivoNuevo.i_mtime = fcreacion;
                        inodoArchivoNuevo.i_atime = fcreacion;
                        inodoArchivoNuevo.i_ctime = fcreacion;

                        if(mfile->getSize() == 0)
                        {
                            inodoArchivoNuevo.i_size = 0;


                            for(int f = 0; f<15; f++)
                            {
                                if(f<12)
                                {
                                    if(inodoTemp.i_block[f] != -1 )
                                    {
                                        BCarpeta carpetaComprobar;
                                        fseek(arch,superBloque.s_block_start +(inodoTemp.i_block[f]*sizeof(BCarpeta)),SEEK_SET);
                                        fread(&carpetaComprobar,sizeof(BCarpeta),1,arch);

                                        for(int k = 0; k<4; k++)
                                        {
                                            if(carpetaComprobar.b_content[k].b_inodo == -1)
                                            {
                                                strcpy(carpetaComprobar.b_content[k].b_name,rutaArchivo[carp -1].c_str());
                                                carpetaComprobar.b_content[k].b_inodo = superBloque.s_first_ino;
                                                fseek(arch,superBloque.s_block_start +(inodoTemp.i_block[f]*sizeof(BCarpeta)),SEEK_SET);
                                                fwrite(&carpetaComprobar,sizeof(BCarpeta),1,arch);
                                                f = 20;
                                                break;


                                            }
                                        }


                                    }
                                    else
                                    {

                                        //crear nuevo bloque carpeta
                                        BCarpeta carpetaComprobar;
                                        for(int k = 1; k<4; k++)
                                        {
                                            carpetaComprobar.b_content[k].b_inodo = -1;
                                            string vacio = " ";
                                            strcpy(carpetaComprobar.b_content[k].b_name,vacio.c_str());
                                        }
                                        strcpy(carpetaComprobar.b_content[0].b_name,rutaArchivo[carp -1].c_str());
                                        carpetaComprobar.b_content[0].b_inodo = superBloque.s_first_ino;

                                        fseek(arch,superBloque.s_block_start +(superBloque.s_first_blo*sizeof(BCarpeta)),SEEK_SET);
                                        fwrite(&carpetaComprobar,sizeof(BCarpeta),1,arch);

                                        inodoTemp.i_block[f] = superBloque.s_first_blo;

                                        char llenar = '1';
                                        char actual;
                                        for(int j = 0 ; j < superBloque.s_blocks_count; j++)
                                        {
                                            fseek(arch, superBloque.s_bm_block_start+j*sizeof(llenar),SEEK_SET);
                                            fread(&actual, sizeof(llenar),1,arch);

                                            if(actual=='0')
                                            {
                                                fseek(arch, superBloque.s_bm_block_start+j*sizeof(llenar),SEEK_SET);
                                                fwrite(&llenar, sizeof(llenar),1,arch);
                                                break;
                                            }

                                        }
                                        superBloque.s_first_blo = superBloque.s_first_blo +1;
                                        superBloque.s_free_blocks_count = superBloque.s_free_blocks_count - 1;

                                        fseek(arch,superBloque.s_block_start +(inodoTemp.i_block[0]*sizeof(BCarpeta)),SEEK_SET);
                                        fread(&carpetaComprobar,sizeof(BCarpeta),1,arch);

                                        inodoTemp.i_mtime = fcreacion;
                                        fseek(arch,superBloque.s_inode_start + (carpetaComprobar.b_content[0].b_inodo*sizeof(Inodo)),SEEK_SET);
                                        fwrite(&inodoTemp,sizeof(Inodo),1,arch);
                                        break;
                                    }

                                }
                            }
                            fseek(arch, superBloque.s_inode_start+(superBloque.s_first_ino*sizeof(Inodo)),SEEK_SET);
                            fwrite(&inodoArchivoNuevo,sizeof(Inodo),1,arch);

                            superBloque.s_first_ino = superBloque.s_first_ino + 1;
                            superBloque.s_free_inodes_count = superBloque.s_free_inodes_count - 1;
                            fseek(arch,iniPart,SEEK_SET);
                            fwrite(&superBloque,sizeof(SupB),1,arch);
                            char llenar = '1';
                            char actual;
                            for(int j = 0 ; j < superBloque.s_inodes_count; j++)
                            {
                                fseek(arch, superBloque.s_bm_inode_start+j*sizeof(llenar),SEEK_SET);
                                fread(&actual, sizeof(llenar),1,arch);

                                if(actual=='0')
                                {
                                    fseek(arch, superBloque.s_bm_inode_start+j*sizeof(llenar),SEEK_SET);
                                    fwrite(&llenar, sizeof(llenar),1,arch);
                                    fwrite(&llenar, sizeof(llenar),1,arch);
                                    break;
                                }

                            }
                            cout << "Se creo el Archivo con exito" << endl;

                        }
                        else
                        {
                            inodoArchivoNuevo.i_size = mfile->getSize();
                            int blocksAUsar = cantidadBloquesUsar(mfile->getSize());
                            for(int f= 0; f<blocksAUsar; f++)
                            {
                                if(f<12)
                                {
                                    inodoArchivoNuevo.i_block[f] = superBloque.s_first_blo;
                                    superBloque.s_first_blo = superBloque.s_first_blo +1;
                                    superBloque.s_free_blocks_count = superBloque.s_free_blocks_count - 1;

                                    char llenar = '1';
                                    char actual;
                                    for(int j = 0 ; j < superBloque.s_blocks_count; j++)
                                    {
                                        fseek(arch, superBloque.s_bm_block_start+j*sizeof(llenar),SEEK_SET);
                                        fread(&actual, sizeof(llenar),1,arch);

                                        if(actual=='0')
                                        {
                                            fseek(arch, superBloque.s_bm_block_start+j*sizeof(llenar),SEEK_SET);
                                            fwrite(&llenar, sizeof(llenar),1,arch);
                                            break;
                                        }

                                    }

                                }

                            }

                            int condi = 0;
                            int posicion = 0;
                            while(inodoArchivoNuevo.i_block[condi] != -1)
                            {
                                int contNumero = 0;
                                string contenidoNuevo = "";
                                int numero= 0;

                                while(true)
                                {
                                    if(contNumero<63 && posicion< mfile->getSize())
                                    {
                                        contenidoNuevo += std::to_string(numero);
                                        contNumero++ ;
                                        numero ++;
                                        posicion ++;
                                    }
                                    else
                                    {
                                        break;
                                    }

                                    if(numero == 9)
                                    {
                                        numero = 0;

                                    }

                                }



                                BArchivo archivoNuevo;
                                strcpy(archivoNuevo.b_content,contenidoNuevo.c_str());
                                fseek(arch, superBloque.s_block_start+(inodoArchivoNuevo.i_block[condi]*sizeof(BArchivo)),SEEK_SET);
                                fwrite(&archivoNuevo,sizeof(BArchivo),1,arch);

                                condi ++;

                            }

                            for(int f = 0; f<15; f++)
                            {
                                if(f<12)
                                {
                                    if(inodoTemp.i_block[f] != -1 )
                                    {
                                        BCarpeta carpetaComprobar;
                                        fseek(arch,superBloque.s_block_start +(inodoTemp.i_block[f]*sizeof(BCarpeta)),SEEK_SET);
                                        fread(&carpetaComprobar,sizeof(BCarpeta),1,arch);

                                        for(int k = 0; k<4; k++)
                                        {
                                            if(carpetaComprobar.b_content[k].b_inodo == -1)
                                            {
                                                strcpy(carpetaComprobar.b_content[k].b_name,rutaArchivo[carp -1].c_str());
                                                carpetaComprobar.b_content[k].b_inodo = superBloque.s_first_ino;
                                                fseek(arch,superBloque.s_block_start +(inodoTemp.i_block[f]*sizeof(BCarpeta)),SEEK_SET);
                                                fwrite(&carpetaComprobar,sizeof(BCarpeta),1,arch);

                                                f = 20;
                                                break;


                                            }
                                        }


                                    }
                                    else
                                    {

                                        //crear nuevo bloque carpeta
                                        BCarpeta carpetaComprobar;
                                        for(int k = 1; k<4; k++)
                                        {
                                            carpetaComprobar.b_content[k].b_inodo = -1;
                                            string vacio = " ";
                                            strcpy(carpetaComprobar.b_content[k].b_name,vacio.c_str());
                                        }
                                        strcpy(carpetaComprobar.b_content[0].b_name,rutaArchivo[carp -1].c_str());
                                        carpetaComprobar.b_content[0].b_inodo = superBloque.s_first_ino;

                                        fseek(arch,superBloque.s_block_start +(superBloque.s_first_blo*sizeof(BCarpeta)),SEEK_SET);
                                        fwrite(&carpetaComprobar,sizeof(BCarpeta),1,arch);

                                        inodoTemp.i_block[f] = superBloque.s_first_blo;

                                        char llenar = '1';
                                        char actual;
                                        for(int j = 0 ; j < superBloque.s_blocks_count; j++)
                                        {
                                            fseek(arch, superBloque.s_bm_block_start+j*sizeof(llenar),SEEK_SET);
                                            fread(&actual, sizeof(llenar),1,arch);

                                            if(actual=='0')
                                            {
                                                fseek(arch, superBloque.s_bm_block_start+j*sizeof(llenar),SEEK_SET);
                                                fwrite(&llenar, sizeof(llenar),1,arch);
                                                break;
                                            }

                                        }
                                        superBloque.s_first_blo = superBloque.s_first_blo +1;
                                        superBloque.s_free_blocks_count = superBloque.s_free_blocks_count - 1;

                                        fseek(arch,superBloque.s_block_start +(inodoTemp.i_block[0]*sizeof(BCarpeta)),SEEK_SET);
                                        fread(&carpetaComprobar,sizeof(BCarpeta),1,arch);

                                        inodoTemp.i_mtime = fcreacion;
                                        fseek(arch,superBloque.s_inode_start + (carpetaComprobar.b_content[0].b_inodo*sizeof(Inodo)),SEEK_SET);
                                        fwrite(&inodoTemp,sizeof(Inodo),1,arch);
                                        break;
                                    }

                                }
                            }

                            fseek(arch, superBloque.s_inode_start+(superBloque.s_first_ino*sizeof(Inodo)),SEEK_SET);
                            fwrite(&inodoArchivoNuevo,sizeof(Inodo),1,arch);

                            superBloque.s_first_ino = superBloque.s_first_ino + 1;
                            superBloque.s_free_inodes_count = superBloque.s_free_inodes_count - 1;
                            fseek(arch,iniPart,SEEK_SET);
                            fwrite(&superBloque,sizeof(SupB),1,arch);
                            char llenar = '1';
                            char actual;
                            for(int j = 0 ; j < superBloque.s_inodes_count; j++)
                            {
                                fseek(arch, superBloque.s_bm_inode_start+j*sizeof(llenar),SEEK_SET);
                                fread(&actual, sizeof(llenar),1,arch);

                                if(actual=='0')
                                {
                                    fseek(arch, superBloque.s_bm_inode_start+j*sizeof(llenar),SEEK_SET);
                                    fwrite(&llenar, sizeof(llenar),1,arch);
                                    fwrite(&llenar, sizeof(llenar),1,arch);
                                    break;
                                }

                            }
                            cout << "Se creo el Archivo con exito" << endl;

                        }
                        break;
                    }
                }
                else
                {
                    if(mfile->getR())
                    {
                        Inodo inodoArchivoNuevo;
                        for(int f=0; f<15; f++)
                        {
                            inodoArchivoNuevo.i_block[f] = -1;
                        }

                        //inodoArchivoNuevo.i_block[0]= 1;
                        inodoArchivoNuevo.i_gid = 1;
                        inodoArchivoNuevo.i_uid = 1;
                        inodoArchivoNuevo.i_type = '0';
                        inodoArchivoNuevo.i_perm = 664;
                        string fecha = retFecha();
                        _mTime fcreacion;
                        strcpy(fcreacion.mbr_fecha_creacion,fecha.c_str());
                        inodoArchivoNuevo.i_mtime = fcreacion;
                        inodoArchivoNuevo.i_atime = fcreacion;
                        inodoArchivoNuevo.i_ctime = fcreacion;
                        inodoArchivoNuevo.i_block[0] = superBloque.s_first_blo;

                        BCarpeta carpetRaiz;

                        string punto = ".";
                        string Dpunto = "..";
                        string vacio = " ";
                        strcpy(carpetRaiz.b_content[0].b_name,punto.c_str());
                        strcpy(carpetRaiz.b_content[1].b_name,Dpunto.c_str());
                        strcpy(carpetRaiz.b_content[2].b_name,vacio.c_str());
                        strcpy(carpetRaiz.b_content[3].b_name,vacio.c_str());
                        carpetRaiz.b_content[0].b_inodo = superBloque.s_first_ino;
                        BCarpeta carpetaComprobar;
                        fseek(arch, superBloque.s_block_start+(inodoTemp.i_block[0]*sizeof(BCarpeta)),SEEK_SET);
                        fread(&carpetaComprobar,sizeof(BCarpeta),1,arch);

                        carpetRaiz.b_content[1].b_inodo = carpetaComprobar.b_content[0].b_inodo;
                        carpetRaiz.b_content[2].b_inodo = -1;
                        carpetRaiz.b_content[3].b_inodo = -1;
                        fseek(arch, superBloque.s_block_start+(superBloque.s_first_blo*sizeof(BCarpeta)),SEEK_SET);
                        fwrite(&carpetRaiz,sizeof(BCarpeta),1,arch);

                        superBloque.s_first_blo = superBloque.s_first_blo + 1;
                        superBloque.s_free_blocks_count = superBloque.s_free_blocks_count -1;

                        for(int f = 0; f<15; f++)
                        {
                            if(f<12)
                            {
                                if(inodoTemp.i_block[f] != -1 )
                                {
                                    BCarpeta carpetaComprobar1;
                                    fseek(arch,superBloque.s_block_start +(inodoTemp.i_block[f]*sizeof(BCarpeta)),SEEK_SET);
                                    fread(&carpetaComprobar1,sizeof(BCarpeta),1,arch);

                                    for(int k = 0; k<4; k++)
                                    {
                                        if(carpetaComprobar1.b_content[k].b_inodo == -1)
                                        {
                                            strcpy(carpetaComprobar1.b_content[k].b_name,rutaArchivo[i].c_str());
                                            carpetaComprobar1.b_content[k].b_inodo = superBloque.s_first_ino;
                                            fseek(arch,superBloque.s_block_start +(inodoTemp.i_block[f]*sizeof(BCarpeta)),SEEK_SET);
                                            fwrite(&carpetaComprobar1,sizeof(BCarpeta),1,arch);

                                            f = 20;
                                            break;


                                        }
                                    }


                                }
                                else
                                {

                                    //crear nuevo bloque carpeta
                                    BCarpeta carpetaComprobar1;
                                    for(int k = 1; k<4; k++)
                                    {
                                        carpetaComprobar1.b_content[k].b_inodo = -1;
                                        string vacio = " ";
                                        strcpy(carpetaComprobar1.b_content[k].b_name,vacio.c_str());
                                    }
                                    strcpy(carpetaComprobar1.b_content[0].b_name,rutaArchivo[i].c_str());
                                    carpetaComprobar1.b_content[0].b_inodo = superBloque.s_first_ino;

                                    fseek(arch,superBloque.s_block_start +(superBloque.s_first_blo*sizeof(BCarpeta)),SEEK_SET);
                                    fwrite(&carpetaComprobar1,sizeof(BCarpeta),1,arch);

                                    inodoTemp.i_block[f] = superBloque.s_first_blo;

                                    char llenar = '1';
                                    char actual;
                                    for(int j = 0 ; j < superBloque.s_blocks_count; j++)
                                    {
                                        fseek(arch, superBloque.s_bm_block_start+j*sizeof(llenar),SEEK_SET);
                                        fread(&actual, sizeof(llenar),1,arch);

                                        if(actual=='0')
                                        {
                                            fseek(arch, superBloque.s_bm_block_start+j*sizeof(llenar),SEEK_SET);
                                            fwrite(&llenar, sizeof(llenar),1,arch);
                                            break;
                                        }

                                    }
                                    superBloque.s_first_blo = superBloque.s_first_blo +1;
                                    superBloque.s_free_blocks_count = superBloque.s_free_blocks_count - 1;

                                    fseek(arch,superBloque.s_block_start +(inodoTemp.i_block[0]*sizeof(BCarpeta)),SEEK_SET);
                                    fread(&carpetaComprobar,sizeof(BCarpeta),1,arch);

                                    inodoTemp.i_mtime = fcreacion;
                                    fseek(arch,superBloque.s_inode_start + (carpetaComprobar.b_content[0].b_inodo*sizeof(Inodo)),SEEK_SET);
                                    fwrite(&inodoTemp,sizeof(Inodo),1,arch);
                                    break;
                                }

                            }
                        }
                        fseek(arch, superBloque.s_inode_start+(superBloque.s_first_ino*sizeof(Inodo)),SEEK_SET);
                        fwrite(&inodoArchivoNuevo,sizeof(Inodo),1,arch);


                        superBloque.s_first_ino = superBloque.s_first_ino + 1;
                        superBloque.s_free_inodes_count = superBloque.s_free_inodes_count -1;
                        fseek(arch,iniPart,SEEK_SET);
                        fwrite(&superBloque,sizeof(SupB),1,arch);


                        char llenar = '1';
                        char actual;
                        for(int j = 0 ; j < superBloque.s_blocks_count; j++)
                        {
                            fseek(arch, superBloque.s_bm_block_start+j*sizeof(llenar),SEEK_SET);
                            fread(&actual, sizeof(llenar),1,arch);

                            if(actual=='0')
                            {
                                fseek(arch, superBloque.s_bm_block_start+j*sizeof(llenar),SEEK_SET);
                                fwrite(&llenar, sizeof(llenar),1,arch);
                                break;
                            }

                        }


                        for(int j = 0 ; j < superBloque.s_inodes_count; j++)
                        {
                            fseek(arch, superBloque.s_bm_inode_start+j*sizeof(llenar),SEEK_SET);
                            fread(&actual, sizeof(llenar),1,arch);

                            if(actual=='0')
                            {
                                fseek(arch, superBloque.s_bm_inode_start+j*sizeof(llenar),SEEK_SET);
                                fwrite(&llenar, sizeof(llenar),1,arch);
                                fwrite(&llenar, sizeof(llenar),1,arch);
                                break;
                            }

                        }
                        inodoTemp = inodoArchivoNuevo;
                        cout << "Se creo la carpeta: "<<rutaArchivo[i] << endl;

                    }
                    else
                    {
                        cout<<"ERROR: no se encontro la ruta del archivo"<<endl;
                        break;
                    }
                }

            }


        }
    }

}
int mkfile::cantidadBloquesUsar(int data)
{
    int noBloque = 0;
    int cont = 1;
    for(int i=0; i< data; i++ )
    {
        if(cont == 64)
        {
            noBloque = noBloque+1;
            cont = 0;
        }
        cont ++;

    }
    if(data%64 != 0)
    {
        noBloque = noBloque +1;
    }
    return noBloque;
}

int mkfile::cantidadBloquesUsarString(string data)
{

    int noBloque = 0;
    int cont = 1;
    for(int i=0; i< data.size(); i++ )
    {
        if(cont == 64)
        {
            noBloque = noBloque+1;
            cont = 0;
        }
        cont ++;

    }
    if(data.size()%64 != 0)
    {
        noBloque = noBloque +1;
    }
    return noBloque;
}

void mkfile::returnDatosPart(MBR mbrTemp, string pathD,string nombrePart,int &tamPart, int &iniPart)
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
