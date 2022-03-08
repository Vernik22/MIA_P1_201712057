#include "mkfs.h"
#include <iostream>
#include <string.h>
#include <cstdlib>
#include <math.h>
#include <time.h>
#include <stdio.h>

mkfs::mkfs()
{
    //ctor
}

mkfs::~mkfs()
{
    //dtor
}

void mkfs::ejecutarComandoMkfs(mkfs *fs, mount paMoun[])
{
    cout<<"\n************Ejecutar MKFS************\n"<<endl;

    if(existeIdMount(fs->getId(),paMoun))
    {

        MBR mbrTemp;
        FILE *arch;
        string pathD = "";
        string nombreParticion = "";
        for(int i =0; i<100; i++)
        {
            if(fs->getId() == paMoun[i].getId())
            {
                pathD = paMoun[i].getPath();
                nombreParticion = paMoun[i].getName();
                break;

            }

        }
        arch = fopen(pathD.c_str(),"rb+");
        if(arch != NULL)
        {
            fseek(arch,0,SEEK_SET);
            fread(&mbrTemp,sizeof(MBR),1,arch);
            fclose(arch);
            int tamParticion, iniPart;
            returnDatosPart(mbrTemp, pathD,nombreParticion,tamParticion, iniPart);

            SupB superBloque;

            //int numero_bloques_por_inodo = 12 + 16 + (16*16) + (16*16*16);
            //int capacidad_carpeta = numero_bloques_por_inodo * 4 - 2;
            //int capacidad_archivo = numero_bloques_por_inodo * 64;

            //tamaño_particion = sizeof(SupB)+ n +n*sizeof(Bitacora) + 3*n+n*sizeof(Inodo) + 3*n*sizeof(BArchivo) numero_estructuras =floor(n);
            int n ;
            if(fs->getFs()=="3fs")
            {
                //Ext3
                //n = (tamParticion - sizeof(SupB))/(1 + 3 + sizeof(Inodo) + 3*sizeof(BArchivo)+sizeof(Bitacora));
                n = (tamParticion - sizeof(SupB)) / (4 + sizeof(Inodo) + sizeof(Bitacora) + 3 * 64); //voy a usar esta
                //floor(n);
            }
            else
            {
                //Ext2
                //n = (tamParticion - sizeof(SupB))/(1 + 3 + sizeof(Inodo) + 3*sizeof(BArchivo));
                n = (tamParticion - sizeof(SupB)) / (4 + sizeof(Inodo) + 3 * 64); //voy a usar esta
                //floor(n);
            }

            //No estructuras
            int cantBlockCarp = n;
            int cantBlockArch = n; //cantidadDD  cantidad de carpetas
            int cantidadInodos = n;
            int cantBlockApunt = n;
            int Bitacoras = n;


            //Bitmaps
            int inicioBitmapInodo = iniPart + sizeof(SupB);
            int inicioBitmapBlockApun = inicioBitmapInodo + cantidadInodos;
            int inicioBitmapBlockCarp = inicioBitmapBlockApun + cantBlockCarp;
            int inicioBitmapBlockArch = inicioBitmapBlockCarp + cantBlockArch;
            //Inicio bloques e inodos
            int inicioInodo = inicioBitmapInodo + cantidadInodos + 3*n;
            int inicioBlockApun = inicioInodo + cantidadInodos*(sizeof(Inodo));
            int inicioBlockCarp = inicioBlockApun + cantBlockApunt*(sizeof(BApun));
            int inicioBlockArch = inicioBlockCarp + cantBlockArch*(sizeof(BCarpeta));

            int inicioBitacora = 0;
            //int inicioCopiaSB = 0;
            if(fs->getFs()=="3fs")
            {
                inicioBitacora = iniPart + sizeof(SupB);

                //Bitmaps
                inicioBitmapInodo = inicioBitacora + Bitacoras*(sizeof(Bitacora));
                inicioBitmapBlockApun = inicioBitmapInodo + cantidadInodos;
                inicioBitmapBlockCarp = inicioBitmapBlockApun + cantBlockApunt;
                inicioBitmapBlockArch = inicioBitmapBlockCarp + cantBlockCarp;

                //Inicio bloques e inodos
                inicioInodo = inicioBitmapInodo + cantidadInodos + 3*n;
                inicioBlockApun = inicioInodo + cantidadInodos*(sizeof(Inodo));
                inicioBlockCarp = inicioBlockApun + cantBlockApunt*(sizeof(BApun));
                inicioBlockArch = inicioBlockCarp + cantBlockArch*(sizeof(BCarpeta));
            }


            cout<< "-----------------------------"<<endl;
            cout<<"pesoSB "<< sizeof(SupB) <<" pesoBlo "<<sizeof(BArchivo)<< " pesoInodos "<<sizeof(Inodo)<< " pesoBAp "<<sizeof(BApun)<<" pesoBitacora "<<sizeof(Bitacora)<<endl;
            cout<< "-----------------------------"<<endl;
            cout<<"CantidadB "<<cantBlockCarp<<" CantidadInodos "<<cantidadInodos<<" CantidadBitacoras "<<Bitacoras<<endl;

            //inicializando SuperBloque
            if(fs->getFs()=="3fs")
            {
                //ext3
                superBloque.s_filesystem_type = 3;
            }
            else  //ext2
            {
                superBloque.s_filesystem_type = 2;
            }
            superBloque.s_inodes_count = cantidadInodos;
            superBloque.s_blocks_count = cantidadInodos*3;
            superBloque.s_free_blocks_count = cantidadInodos*3;
            superBloque.s_free_inodes_count = cantidadInodos;

            time_t t = time(NULL);
            struct tm tiempoLocal = *localtime(&t);
            // El lugar en donde se pondrá la fecha y hora formateadas
            char fechaHora[70];
            // El formato. Mira más en https://en.cppreference.com/w/c/chrono/strftime
            char *formato = "%Y-%m-%d %H:%M:%S";
            // Intentar formatear
            int bytesEscritos =
                strftime(fechaHora, sizeof fechaHora, formato, &tiempoLocal);


            //time_t rawtime;
            //struct tm *timeinfo;
            //time(&rawtime);
            //timeinfo = localtime(&rawtime);
            //string fecha = asctime(timeinfo);
            string fecha = fechaHora;
            _mTime fcreacion;
            strcpy(fcreacion.mbr_fecha_creacion,fecha.c_str());
            superBloque.s_mtime = fcreacion;
            superBloque.s_umtime = fcreacion;
            superBloque.s_mnt_count = 1;
            superBloque.s_umnt_count = 0;
            superBloque.s_magic = 0xEF53;
            superBloque.s_inode_size = sizeof(Inodo);
            superBloque.s_block_size = sizeof(BArchivo);
            superBloque.s_first_ino = 0;
            superBloque.s_first_blo = 0;
            superBloque.s_bm_inode_start = inicioBitmapInodo;
            superBloque.s_bm_block_start = inicioBitmapInodo + cantidadInodos;
            //superBloque.s_bm_block_start = inicioBitmapInodo + cantidadInodos*sizeof(n);

            superBloque.s_inode_start = superBloque.s_bm_block_start + cantidadInodos*3;
            //superBloque.s_inode_start = superBloque.s_bm_block_start + cantidadInodos*3*sizeof(n);
            superBloque.s_block_start = superBloque.s_inode_start + cantidadInodos*sizeof(Inodo);
            //superBloque.s_blockAr_start = inicioBlockArch;
            //superBloque.s_blockC_start = inicioBlockCarp;

            //ahora escribir el superbloque en la particion
            arch = fopen(pathD.c_str(),"rb+");

            fseek(arch,iniPart,SEEK_SET);
            fwrite(&superBloque,sizeof(SupB),1,arch);
            fclose(arch);

            inicializarBitmaps(pathD,iniPart,superBloque);
            //creo el directorio raiz
            crearRaiz(pathD, iniPart);
            cout<<"NO. Estructuras: " << n<<endl;
            cout<<"Particion a formatear: "<<nombreParticion<<endl;
            cout<<"Tamaño Particion: "<<tamParticion<<endl;
            cout<<"Fecha: "<<mbrTemp.mbr_fecha_creacion.mbr_fecha_creacion<<endl;

            cout<<endl<<"Se creo el sistema de archivos correctamente"<<endl;

        }
        else
        {
            cout<<"Error, no se puede acceder al disco"<<endl;
        }


        // fclose(arch);

    }
    else
    {
        cout<<"ERROR: El ID de la particion no existe, o no esta montada"<<endl;
    }

}

void mkfs::crearRaiz(string path,int inicioPart)
{
    FILE *arch;
    arch = fopen(path.c_str(),"rb+");
    if(arch != NULL)
    {
        fseek(arch, inicioPart,SEEK_SET);
        SupB superB;
        fread(&superB,sizeof(SupB),1,arch);

        char llenar = '0';
        //lleno de ceros el espacio del bitmap de inodos
        fseek(arch,superB.s_bm_inode_start,SEEK_SET);
        for(int i = 0; i < superB.s_inodes_count; i++)
        {
            fwrite(&llenar,sizeof(llenar),1,arch);
        }

        //lleno de ceros el espacio del bitmap de bloques
        fseek(arch, superB.s_bm_block_start,SEEK_SET);
        for(int i=0; i< superB.s_blocks_count; i++)
        {
            fwrite(&llenar, sizeof(llenar),1,arch);
        }
        char bit = '1'; //para cambiar el estado de una posicion en los bitmaps
        //escribir 1 en el bitmap de inodo y escribir el inodo carpeta raiz
        fseek(arch, superB.s_bm_inode_start,SEEK_SET);
        fwrite(&bit,sizeof(bit),1,arch); //escribe inodo de la carpeta raiz es el primero primerisimo de este se deriba el bloque carpeta que es la carpeta /
        Inodo inodoTemp;
        for(int i=0; i<15; i++)
        {
            inodoTemp.i_block[i] = -1;
        }
        inodoTemp.i_atime = superB.s_mtime;
        inodoTemp.i_ctime = superB.s_mtime;
        inodoTemp.i_mtime = superB.s_mtime;
        inodoTemp.i_block[0]= 0;
        inodoTemp.i_gid = 1;
        inodoTemp.i_uid = 1;
        inodoTemp.i_size = 0;
        inodoTemp.i_type = '0';
        inodoTemp.i_perm = 777;
        fseek(arch, superB.s_inode_start,SEEK_SET);
        fwrite(&inodoTemp,sizeof(Inodo),1,arch);
        cout<<"Se creo la carpeta raiz (/)"<<endl;

        fseek(arch, superB.s_bm_inode_start+sizeof(bit),SEEK_SET);
        fwrite(&bit,sizeof(bit),1,arch);
        inodoTemp.i_block[0]= 1;
        inodoTemp.i_gid = 1;
        inodoTemp.i_uid = 1;
        inodoTemp.i_size = 27;
        inodoTemp.i_type = '1';
        inodoTemp.i_perm = 777;
        fseek(arch, superB.s_inode_start+sizeof(Inodo),SEEK_SET);
        fwrite(&inodoTemp,sizeof(Inodo),1,arch);

        //escribir 1 en el bitmap de carpeta y escribir carpeta
        fseek(arch, superB.s_bm_block_start,SEEK_SET);
        fwrite(&bit,sizeof(bit),1,arch);
        BCarpeta carpetRaiz;
        string raiz = "users.txt";
        string punto = ".";
        string Dpunto = "..";
        strcpy(carpetRaiz.b_content[0].b_name,punto.c_str());
        strcpy(carpetRaiz.b_content[1].b_name,Dpunto.c_str());
        strcpy(carpetRaiz.b_content[2].b_name,raiz.c_str());
        carpetRaiz.b_content[0].b_inodo = 0;
        carpetRaiz.b_content[1].b_inodo = 0;
        carpetRaiz.b_content[2].b_inodo = 1;
        carpetRaiz.b_content[3].b_inodo = -1;
        fseek(arch, superB.s_block_start,SEEK_SET);
        fwrite(&carpetRaiz,sizeof(BCarpeta),1,arch);

        //escribir 1 en el bitmap de archivo y escribir el root en el users.txt
        fseek(arch, superB.s_bm_block_start+ sizeof(bit),SEEK_SET);
        fwrite(&bit,sizeof(bit),1,arch);
        BArchivo contenidoUsers;
        string contenido = "1,G,root\n1,U,root,root,123\n?";
        strcpy(contenidoUsers.b_content,contenido.c_str());
        fseek(arch, superB.s_block_start+sizeof(BArchivo),SEEK_SET);
        fwrite(&contenidoUsers,sizeof(BArchivo),1,arch);
        cout<<"Se creo el archivo /users.txt"<<endl<<endl;

        superB.s_free_blocks_count = superB.s_free_blocks_count - 2;
        superB.s_free_inodes_count = superB.s_free_inodes_count - 2;
        superB.s_first_blo =  2;
        superB.s_first_ino =  2;
        //actualizar el superbloque
        fseek(arch, inicioPart,SEEK_SET);
        fwrite(&superB,sizeof(SupB),1,arch);

        fclose(arch);
    }
    else
    {
        cout<<"Error, no se puede acceder al disco"<<endl;
    }

}

void mkfs::inicializarBitmaps(string path,int partStart,SupB superBlock)
{
    FILE *arch;
    arch = fopen(path.c_str(),"rb+");

    Inodo inodoTemp;
    //BCarpeta blockCarpeta;
    //BArchivo blockArch; //contenido del archivo users.txt
    //BApun blockApuntador;
    Bitacora journaling;

    if(superBlock.s_filesystem_type==3)
    {
        //ext3
        fseek(arch,(partStart+sizeof(SupB)),SEEK_SET);
        journaling.Size = -1;
        //lleno de bitacoras vacias el espacio de la bitacora
        for(int i=0; i<superBlock.s_inodes_count; i++)
        {
            fwrite(&journaling,sizeof(Bitacora),1,arch);
        }

    }

    int llenar = 0;
    //lleno de ceros el espacio del bitmap de inodos
    fseek(arch,superBlock.s_bm_inode_start,SEEK_SET);
    for(int i = 0; i < superBlock.s_inodes_count; i++)
    {
        fwrite(&llenar,sizeof(llenar),1,arch);
    }

    //lleno de ceros el espacio del bitmap de bloques
    fseek(arch, superBlock.s_bm_block_start,SEEK_SET);
    for(int i=0; i< superBlock.s_blocks_count; i++)
    {
        fwrite(&llenar, sizeof(llenar),1,arch);
    }
    /*
    fseek(arch, superBlock.s_bm_blockC_start,SEEK_SET);
    for(int i=0; i< superBlock.s_inodes_count; i++)
    {
        fwrite(&llenar, sizeof(llenar),1,arch);
    }
    fseek(arch, superBlock.s_bm_blockAr_start,SEEK_SET);
    for(int i=0; i< superBlock.s_inodes_count; i++)
    {
        fwrite(&llenar, sizeof(llenar),1,arch);
    }
    */
    //escribir tabla de inodos
    fseek(arch,superBlock.s_inode_start,SEEK_SET);
    for(int i=0; i<15; i++)
    {
        inodoTemp.i_block[i] = -1;
    }
    inodoTemp.i_type = '-';
    inodoTemp.i_atime = superBlock.s_mtime;
    inodoTemp.i_ctime = superBlock.s_mtime;
    inodoTemp.i_mtime = superBlock.s_mtime;


    for(int i=0; i<superBlock.s_inodes_count; i++)
    {
        fwrite(&inodoTemp,sizeof(Inodo),1,arch);
    }
    /*

    //escribir los bloques

    blockApuntador.b_apuntadores[0].b_inodo= -1;
    blockApuntador.b_apuntadores[1].b_inodo= -1;
    blockApuntador.b_apuntadores[2].b_inodo= -1;
    blockApuntador.b_apuntadores[3].b_inodo= -1;

    string apunt = "Apuntador1";
    strcpy(blockApuntador.b_apuntadores[0].b_name,apunt.c_str());
    apunt = "Apuntador2";
    strcpy(blockApuntador.b_apuntadores[1].b_name,apunt.c_str());
    apunt = "Apuntador3";
    strcpy(blockApuntador.b_apuntadores[2].b_name,apunt.c_str());
    apunt = "Apuntador4";
    strcpy(blockApuntador.b_apuntadores[3].b_name,apunt.c_str());

    fseek(arch,superBlock.s_blockAp_start, SEEK_SET);
    for(int i=0; i<superBlock.s_inodes_count; i++)
    {
        fwrite(&blockApuntador,sizeof(BApun),1,arch);
    }



    fseek(arch,superBlock.s_blockC_start, SEEK_SET);
    blockCarpeta.b_content[0].b_inodo=-1;
    blockCarpeta.b_content[1].b_inodo=-1;
    blockCarpeta.b_content[2].b_inodo=-1;
    blockCarpeta.b_content[3].b_inodo=-1;
    for(int j = 0; j<superBlock.s_inodes_count; j++)
    {
        fwrite(&blockCarpeta,sizeof(BCarpeta),1,arch);
    }

    fseek(arch,superBlock.s_blockAr_start, SEEK_SET);
    blockArch.b_content[0]='-';
    for(int i=0; i<superBlock.s_inodes_count; i++)
    {
        fwrite(&blockArch,sizeof(BArchivo),1,arch);
        // fwrite(&blockArch,64,1,arch);
    }



        int prueba = 1;
        fseek(arch, superBlock.s_bm_blockC_start, SEEK_SET);
        fread(&prueba,sizeof(prueba),1,arch);

        cout<<prueba<<endl;
    */

    fclose(arch);
}


bool mkfs::existeIdMount(string idB,mount paMoun[])
{
    for(int i =0; i<100; i++)
    {
        if(idB == paMoun[i].getId())
        {
            return true;

        }

    }
    return false;
}

void mkfs::returnDatosPart(MBR mbrTemp, string pathD,string nombrePart,int &tamPart, int &iniPart)
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
