#include "reporte.h"
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <cstdlib>
#include <string.h>

reporte::reporte()
{
    //ctor
}

reporte::~reporte()
{
    //dtor
}

void reporte::ejecutarRepBmInodo(reporte *rep,mount paMoun[])
{
    SupB superBloque;
    char buffer='0';
    cout<<"\n************Ejecutar REPORTE BmInodo************\n"<<endl;
    string pathDisco="";
    string partName="";
    if(existeIdMount(rep->getId(),paMoun, pathDisco, partName))
    {
        dirExist(rep);
        MBR mbrTemp;
        FILE *arch;
        arch = fopen(pathDisco.c_str(),"rb+");
        if(arch != NULL)
        {
            fseek(arch,0,SEEK_SET);
            fread(&mbrTemp,sizeof(MBR),1,arch);

            int tamParticion, iniPart;
            returnDatosPart(mbrTemp, pathDisco,partName,tamParticion, iniPart);

            fseek(arch,iniPart,SEEK_SET);
            fread(&superBloque,sizeof(SupB),1,arch);
            //fclose(arch);

            string filename(rep->getPath());
            fstream file_out;
            file_out.open(filename,std::ios_base::out);
            if(!file_out.is_open())
            {
                cout<<"no se pudo abrir el archivo"<<endl;
            }
            else
            {
                file_out<<" -------REPORTE BM INODOS------ \n"<<endl;
                string textoFinal ="";
                int contador = 1;
                for(int i =0; i<superBloque.s_inodes_count; i++)
                {

                    if(contador<21)
                    {
                        fseek(arch,superBloque.s_bm_inode_start +(i*sizeof(buffer)),SEEK_SET);
                        fread(&buffer,sizeof(buffer),1,arch);
                        //textoFinal = textoFinal + std::to_string(buffer) + " " ;
                        textoFinal = textoFinal + buffer + " " ;
                        contador++;

                    }
                    else
                    {
                        file_out<<textoFinal<<endl;
                        contador = 1;
                        textoFinal="";
                    }

                }



                cout<<"Reporte creado correctamente"<<endl;

                //string comando = "dot -Tpng ima.txt -o grafo.png";
                //system(comando.c_str());
            }

            fclose(arch);


        }


    }
    else
    {
        cout<<"El id no existe, la particion no esta montada"<<endl;
    }



}

void reporte::ejecutarRepBmBloque(reporte *rep,mount paMoun[])
{
    SupB superBloque;
    char buffer='0';
    cout<<"\n************Ejecutar REPORTE BmBloques************\n"<<endl;
    string pathDisco="";
    string partName="";
    if(existeIdMount(rep->getId(),paMoun, pathDisco, partName))
    {
        dirExist(rep);
        MBR mbrTemp;
        FILE *arch;
        arch = fopen(pathDisco.c_str(),"rb+");
        if(arch != NULL)
        {
            fseek(arch,0,SEEK_SET);
            fread(&mbrTemp,sizeof(MBR),1,arch);

            int tamParticion, iniPart;
            returnDatosPart(mbrTemp, pathDisco,partName,tamParticion, iniPart);

            fseek(arch,iniPart,SEEK_SET);
            fread(&superBloque,sizeof(SupB),1,arch);
            //fclose(arch);

            string filename(rep->getPath());
            fstream file_out;
            file_out.open(filename,std::ios_base::out);
            if(!file_out.is_open())
            {
                cout<<"no se pudo abrir el archivo"<<endl;
            }
            else
            {
                file_out<<" -------REPORTE BM BLOQUES------ \n"<<endl;
                string textoFinal ="";
                int contador = 1;
                for(int i =0; i<superBloque.s_blocks_count; i++)
                {

                    if(contador<21)
                    {
                        fseek(arch,superBloque.s_bm_block_start +(i*sizeof(buffer)),SEEK_SET);
                        fread(&buffer,sizeof(buffer),1,arch);
                        //textoFinal = textoFinal + std::to_string(buffer) + " " ;
                        textoFinal = textoFinal + buffer + " " ;
                        contador++;

                    }
                    else
                    {
                        file_out<<textoFinal<<endl;
                        contador = 1;
                        textoFinal="";
                    }

                }



                cout<<"Reporte creado correctamente"<<endl;

                //string comando = "dot -Tpng ima.txt -o grafo.png";
                //system(comando.c_str());
            }

            fclose(arch);


        }


    }
    else
    {
        cout<<"El id no existe, la particion no esta montada"<<endl;
    }



}

void reporte::ejecutarRepSuperB(reporte *rep,mount paMoun[])
{
    string grafo = "digraph G{\nsubgraph cluster{\nlabel=\"Super Bloque\"\ntbl[shape=box,label=<\n<table border='0' cellborder='1' cellspacing='0' width='300'  height='200' >\n<tr>  <td width='150'> <b>Nombre</b> </td> <td width='150'> <b>Valor</b> </td>  </tr>\n";
    string cuerpo = "";
    string finG = "</table>\n>];\n}\n}";
    SupB superBloque;
    cout<<"\n************Ejecutar REPORTE SuperBloque************\n"<<endl;
    string pathDisco="";
    string partName="";
    if(existeIdMount(rep->getId(),paMoun, pathDisco, partName))
    {
        dirExist(rep);
        MBR mbrTemp;
        FILE *arch;
        arch = fopen(pathDisco.c_str(),"rb+");
        if(arch != NULL)
        {
            fseek(arch,0,SEEK_SET);
            fread(&mbrTemp,sizeof(MBR),1,arch);

            int tamParticion, iniPart;
            returnDatosPart(mbrTemp, pathDisco,partName,tamParticion, iniPart);

            fseek(arch,iniPart,SEEK_SET);
            fread(&superBloque,sizeof(SupB),1,arch);
            fclose(arch);
            vector<string> resultados;
            resultados = split(pathDisco, '/');
            int tam = resultados.size();

            cuerpo += "<tr><td><b>Sb_nombre_hd</b></td><td>"+resultados[tam-1]+"</td> </tr>\n";
            cuerpo += "<tr><td><b>Sb_filesystem_type</b></td><td>" + std::to_string(superBloque.s_filesystem_type) + "</td> </tr>\n";
            cuerpo += "<tr><td><b>Sb_inodes_count</b></td><td>" + std::to_string(superBloque.s_inodes_count) + "</td> </tr>\n";
            cuerpo += "<tr><td><b>Sb_blocks_count</b></td><td>" + std::to_string(superBloque.s_blocks_count) + "</td> </tr>\n";
            cuerpo += "<tr><td><b>Sb_free_blocks_count</b></td><td>" + std::to_string(superBloque.s_free_blocks_count) + "</td> </tr>\n";
            cuerpo += "<tr><td><b>Sb_free_inodes_count</b></td><td>" + std::to_string(superBloque.s_free_inodes_count) + "</td> </tr>\n";
            cuerpo += "<tr><td><b>Sb_mount_time</b></td><td>";
            cuerpo += superBloque.s_mtime.mbr_fecha_creacion ;
            cuerpo += "</td> </tr>\n";
            cuerpo += "<tr><td><b>Sb_unmount_time</b></td><td>";
            cuerpo += superBloque.s_umtime.mbr_fecha_creacion;
            cuerpo += "</td> </tr>\n";
            cuerpo += "<tr><td><b>Sb_mount_count</b></td><td>" + std::to_string(superBloque.s_mnt_count) + "</td> </tr>\n";
            cuerpo += "<tr><td><b>Sb_inode_size</b></td><td>" + std::to_string(superBloque.s_inode_size) + "</td> </tr>\n";
            cuerpo += "<tr><td><b>Sb_block_size</b></td><td>" + std::to_string(superBloque.s_block_size) + "</td> </tr>\n";
            cuerpo += "<tr><td><b>Sb_first_inode</b></td><td>" + std::to_string(superBloque.s_first_ino) + "</td> </tr>\n";
            cuerpo += "<tr><td><b>Sb_first_block</b></td><td>" + std::to_string(superBloque.s_first_blo) + "</td> </tr>\n";
            cuerpo += "<tr><td><b>Sb_bitmap_inode_start</b></td><td>" + std::to_string(superBloque.s_bm_inode_start) + "</td> </tr>\n";
            cuerpo += "<tr><td><b>Sb_bitmap_block_start</b></td><td>" + std::to_string(superBloque.s_bm_block_start) + "</td> </tr>\n";
            cuerpo += "<tr><td><b>Sb_inode_start</b></td><td>" + std::to_string(superBloque.s_inode_start) + "</td> </tr>\n";
            cuerpo += "<tr><td><b>Sb_block_start</b></td><td>" + std::to_string(superBloque.s_block_start) + "</td> </tr>\n";
            cuerpo += "<tr><td><b>Sb_magic</b></td><td>" + std::to_string(superBloque.s_magic) + "</td> </tr>\n";


            string textoFinal =grafo + cuerpo +finG;

            string newPath = pathConComillas(rep->getPath());


            string filename("superB.txt");
            fstream file_out;
            file_out.open(filename,std::ios_base::out);
            if(!file_out.is_open())
            {
                cout<<"no se pudo abrir el archivo"<<endl;
            }
            else
            {

                file_out<<textoFinal<<endl;
                try
                {
                    resultados = split(rep->getPath(), '.');
                    string comando = "dot -T"+resultados[1] +" superB.txt -o " + newPath;
                    system(comando.c_str());
                    cout<<"Reporte SuperBloque creado correctamente"<<endl;
                }
                catch(...)
                {
                    cout<<"ERROR: no se pudo crear el reporte"<<endl;

                }
            }

        }


    }
    else
    {
        cout<<"El id no existe, la particion no esta montada"<<endl;
    }



}

void reporte::ejecutarRepDisk(reporte *rep,mount paMoun[])
{
    char PartName[16];
    PartName[0]='\0';

    string grafo = "digraph G{\ntbl [\nshape=box\nlabel=<\n<table border='0' cellborder='2' width='100' height=\"30\" color='lightblue4'>\n<tr>";
    string cuerpo = "";
    string finG = "     </tr>\n</table>\n>];\n}";
    cout<<"\n************Ejecutar REPORTE Disk************\n"<<endl;
    string pathDisco="";
    string partName="";
    if(existeIdMount(rep->getId(),paMoun, pathDisco, partName))
    {
        dirExist(rep);
        MBR mbrTemp;
        FILE *arch;
        arch = fopen(pathDisco.c_str(),"rb+");
        if(arch != NULL)
        {
            float porcentajeUtilizado = 0.0;
            int espacioUtilizado = 0;
            fseek(arch,0,SEEK_SET);
            fread(&mbrTemp,sizeof(MBR),1,arch);

            int tamanioDisco = mbrTemp.mbr_tamano;
            Particion parts[4];
            parts[0] = mbrTemp.mbr_partition_1;
            parts[1] = mbrTemp.mbr_partition_2;
            parts[2] = mbrTemp.mbr_partition_3;
            parts[3] = mbrTemp.mbr_partition_4;

            cuerpo += "<td height='30' width='75'> MBR </td>";

            for(int i= 0; i < 4; i++)
            {
                if(parts[i].part_name!= PartName && parts[i].part_type == 'P')
                {
                    float partfff = parts[i].part_size;
                    porcentajeUtilizado = (partfff/tamanioDisco)*100;
                    cuerpo += "<td height='30' width='75.0'>PRIMARIA <br/>";
                    cuerpo += parts[i].part_name;
                    cuerpo += " <br/> Ocupado: "+ std::to_string(porcentajeUtilizado)+"%</td>";
                    espacioUtilizado += parts[i].part_size;


                }
                else if(parts[i].part_status == '0')
                {
                    cuerpo += "<td height='30' width='75.0'>Libre</td>";

                }
                if(parts[i].part_type == 'E')
                {
                    float partfff = parts[i].part_size;
                    espacioUtilizado += parts[i].part_size;
                    porcentajeUtilizado = (partfff/tamanioDisco)*100;
                    cuerpo += "<td  height='30' width='15.0'>\n";
                    cuerpo += "<table border='5'  height='30' WIDTH='15.0' cellborder='1'>\n";
                    cuerpo += " <tr>  <td height='60' colspan='100%'>EXTENDIDA <br/>";
                    cuerpo += parts[i].part_name;
                    cuerpo += " <br/> Ocupado:" + std::to_string(porcentajeUtilizado) +"%</td>  </tr>\n<tr>";
                    int inicioExtendida = parts[i].part_start;

                    EBR ebrTemp;
                    fseek(arch,inicioExtendida,SEEK_SET);
                    fread(&ebrTemp,sizeof(EBR),1,arch);
                    if(ebrTemp.part_next == -1)
                    {
                        cout<<"No hay particiones logicas"<<endl;

                    }
                    else
                    {
                        espacioUtilizado = 0;
                        int conta = 0;
                        fseek(arch,inicioExtendida,SEEK_SET);
                        fread(&ebrTemp,sizeof(EBR),1,arch);

                        while(true)
                        {
                            if(ebrTemp.part_next == -1)
                            {
                                break;

                            }
                            else
                            {
                                fseek(arch,ebrTemp.part_next,SEEK_SET);
                                fread(&ebrTemp,sizeof(EBR),1,arch);
                                float partfff = parts[i].part_size;
                                float ebrParff = ebrTemp.part_size;
                                espacioUtilizado += ebrTemp.part_size;
                                porcentajeUtilizado = (ebrParff/partfff)*100;
                                cuerpo += "<td height='30'>EBR</td><td height='30'> Logica:  ";
                                cuerpo += ebrTemp.part_name;
                                cuerpo += " "+std::to_string(porcentajeUtilizado) +"%</td>";
                                conta ++;

                            }


                        }
                        if(parts[i].part_size-espacioUtilizado > 0)
                        {
                            float h1 = tamanioDisco;
                            float h2 = espacioUtilizado;
                            porcentajeUtilizado = ((h1-h2)/h1)*100;
                            cuerpo +="<td height='30' width='100%'>Libre: "+ std::to_string(porcentajeUtilizado)+"%</td>";

                        }
                    }
                    cuerpo += "</tr>\n";
                    cuerpo += "</table>\n</td>";


                }

            }
            if((tamanioDisco - espacioUtilizado) > 0)
            {
                cout<<espacioUtilizado<<endl;
                float h1 = tamanioDisco;
                float h2 = espacioUtilizado;
                porcentajeUtilizado = ((h1-h2)/h1)*100;
                cuerpo += "<td height='30' width='75.0'>Libre: "+ std::to_string(porcentajeUtilizado)+"%</td>";
            }


            fclose(arch);



            string textoFinal =grafo + cuerpo +finG;

            string newPath = pathConComillas(rep->getPath());


            string filename("repDisk.txt");
            fstream file_out;
            file_out.open(filename,std::ios_base::out);
            if(!file_out.is_open())
            {
                cout<<"no se pudo abrir el archivo"<<endl;
            }
            else
            {

                file_out<<textoFinal<<endl;
                try
                {
                    vector<string> resultados;
                    resultados = split(rep->getPath(), '.');
                    string comando = "dot -T"+resultados[1] +" repDisk.txt -o " + newPath;
                    system(comando.c_str());
                    cout<<"Reporte Disk creado correctamente"<<endl;
                }
                catch(...)
                {
                    cout<<"ERROR: no se pudo crear el reporte"<<endl;

                }
            }

        }

    }
    else
    {
        cout<<"El id no existe, la particion no esta montada"<<endl;
    }
}

void reporte::ejecutarRepMBR(reporte *rep,mount paMoun[])
{
    char PartName[16];
    PartName[0]='\0';

    string grafo = "digraph G{\nsubgraph cluster{\nlabel=\"MBR\"\ntbl[shape=box,label=<\n<table border='0' cellborder='1' cellspacing='0' width='300'  height='200' >\n<tr>  <td width='150'> <b>Nombre</b> </td> <td width='150'> <b>Valor</b> </td>  </tr>\n";
    string cuerpo = "";
    string finG = "}\n";
    cout<<"\n************Ejecutar REPORTE MBR-EBR************\n"<<endl;
    string pathDisco="";
    string partName="";
    if(existeIdMount(rep->getId(),paMoun, pathDisco, partName))
    {

        dirExist(rep);
        MBR mbrTemp;
        FILE *arch;
        arch = fopen(pathDisco.c_str(),"rb+");
        if(arch != NULL)
        {
            fseek(arch,0,SEEK_SET);
            fread(&mbrTemp,sizeof(MBR),1,arch);

            cuerpo += "<tr>  <td><b>mbr_tamaño</b></td><td>"+ std::to_string(mbrTemp.mbr_tamano) +"</td>  </tr>\n";
            cuerpo += "<tr>  <td><b>mbr_Fecha_Creacion</b></td><td>";
            cuerpo += mbrTemp.mbr_fecha_creacion.mbr_fecha_creacion;
            cuerpo += "</td>  </tr>\n";
            cuerpo += "<tr>  <td><b>mbr_Disk_Signature</b></td><td>" + std::to_string(mbrTemp.mbr_dsk_signature) + "</td>  </tr>\n";
            cuerpo += "<tr>  <td><b>mbr_Disk_Fit</b></td><td>";
            cuerpo += mbrTemp.dsk_fit;
            cuerpo += "</td>  </tr>";

            Particion parts[4];
            parts[0] = mbrTemp.mbr_partition_1;
            parts[1] = mbrTemp.mbr_partition_2;
            parts[2] = mbrTemp.mbr_partition_3;
            parts[3] = mbrTemp.mbr_partition_4;


            for(int i =0; i < 4; i++)
            {
                if(parts[i].part_name!= PartName)
                {
                    string agre = "bgcolor=\"lightblue\"";
                    cuerpo += "<tr>  <td "+ agre +"><b>part_Status_"+ std::to_string(i+1)+ "</b></td><td "+agre+">" ;
                    cuerpo += parts[i].part_status;
                    cuerpo += "</td>  </tr>\n";
                    cuerpo += "<tr>  <td><b>Tipo_Particion_"+ std::to_string(i+1)+ "</b></td><td>";
                    cuerpo += parts[i].part_type;
                    cuerpo += "</td>  </tr>\n";
                    cuerpo += "<tr>  <td><b>Tipo_Ajuste_"+ std::to_string(i+1)+ "</b></td><td>";
                    cuerpo += parts[i].part_fit;
                    cuerpo += "</td>  </tr>\n";
                    cuerpo += "<tr>  <td><b>Inicio_Particion_"+ std::to_string(i+1)+ "</b></td><td>" + std::to_string(parts[i].part_start) + "</td>  </tr>\n";
                    cuerpo += "<tr>  <td><b>TamanioTotal_"+std::to_string(i+1) + "</b></td><td>" + std::to_string(parts[i].part_size) + "</td>  </tr>\n";
                    cuerpo += "<tr>  <td><b>NombreParticion_"+ std::to_string(i+1)+ "</b></td><td>";
                    cuerpo += parts[i].part_name;
                    cuerpo += "</td>  </tr>\n";

                }

            }

            cuerpo += "</table>\n>];\n}";

            //verificar si hay EBR
            for(int i=0; i<4; i++)
            {
                if(parts[i].part_type == 'E')
                {
                    cuerpo += "subgraph cluster_1{\n label=\"EBR_Inicial\"\ntbl_1[shape=box, label=<\n<table border='0' cellborder='1' cellspacing='0'  width='300' height='160' >\n<tr>  <td width='150'><b>Nombre</b></td> <td width='150'><b>Valor</b></td>  </tr>\n";
                    int inicioExtendida = parts[i].part_start;
                    EBR ebrTemp;
                    fseek(arch,inicioExtendida,SEEK_SET);
                    fread(&ebrTemp,sizeof(EBR),1,arch);

                    string agre = "bgcolor=\"pink\"";
                    cuerpo += "<tr>  <td "+agre+"><b>Tipo_Ajuste_"+ std::to_string(i+1)+ "</b></td><td "+agre+">";
                    cuerpo += ebrTemp.part_fit;
                    cuerpo += "</td>  </tr>\n";
                    cuerpo += "<tr>  <td><b>Part_Status"+ std::to_string(i+1)+ "</b></td><td>";
                    cuerpo += ebrTemp.part_status;
                    cuerpo += "</td>  </tr>\n";
                    cuerpo += "<tr>  <td><b>Inicio_Particion_"+ std::to_string(i+1)+ "</b></td><td>" + std::to_string(ebrTemp.part_start) + "</td>  </tr>\n";
                    cuerpo += "<tr>  <td><b>Particion_Siguiente_"+ std::to_string(i+1)+ "</b></td><td>" + std::to_string(ebrTemp.part_next) + "</td>  </tr>\n";
                    cuerpo += "<tr>  <td><b>TamanioTotal_"+std::to_string(i+1) + "</b></td><td>" + std::to_string(ebrTemp.part_size) + "</td>  </tr>\n";
                    cuerpo += "<tr>  <td><b>NombreParticion_"+ std::to_string(i+1)+ "</b></td><td>";
                    cuerpo += ebrTemp.part_name;
                    cuerpo += "</td>  </tr>\n";
                    cuerpo += "</table>\n>];\n}";

                    if(ebrTemp.part_next == -1)
                    {
                        cout<<"No hay particiones Logicas"<<endl;

                    }
                    else
                    {
                        int cont = 0;
                        fseek(arch,ebrTemp.part_next,SEEK_SET);
                        fread(&ebrTemp,sizeof(EBR),1,arch);
                        while(true)
                        {
                            if(ebrTemp.part_next == -1)
                            {

                                break;
                            }
                            else
                            {
                                fseek(arch,ebrTemp.part_next,SEEK_SET);
                                fread(&ebrTemp,sizeof(EBR),1,arch);
                                cont++;

                            }
                            cuerpo += "subgraph cluster_"+ std::to_string(cont+1) + "{\n label=\"EBR_"+std::to_string(cont+1)+"\"\ntbl_"+ std::to_string(cont+1) + "[shape=box, label=<\n<table border='0' cellborder='1' cellspacing='0'  width='300' height='160' >\n<tr>  <td width='150'><b>Nombre</b></td> <td width='150'><b>Valor</b></td>  </tr>\n";
                            string agre = "bgcolor=\"pink\"";
                            cuerpo += "<tr>  <td "+agre+"><b>Tipo_Ajuste_"+ std::to_string(i+1)+ "</b></td><td "+agre+">";
                            cuerpo += ebrTemp.part_fit;
                            cuerpo += "</td>  </tr>\n";
                            cuerpo += "<tr>  <td><b>Part_Status"+ std::to_string(i+1)+ "</b></td><td>";
                            cuerpo += ebrTemp.part_status;
                            cuerpo += "</td>  </tr>\n";
                            cuerpo += "<tr>  <td><b>Inicio_Particion_"+ std::to_string(i+1)+ "</b></td><td>" + std::to_string(ebrTemp.part_start) + "</td>  </tr>\n";
                            cuerpo += "<tr>  <td><b>Particion_Siguiente_"+ std::to_string(i+1)+ "</b></td><td>" + std::to_string(ebrTemp.part_next) + "</td>  </tr>\n";
                            cuerpo += "<tr>  <td><b>TamanioTotal_"+std::to_string(i+1) + "</b></td><td>" + std::to_string(ebrTemp.part_size) + "</td>  </tr>\n";
                            cuerpo += "<tr>  <td><b>NombreParticion_"+ std::to_string(i+1)+ "</b></td><td>";
                            cuerpo += ebrTemp.part_name;
                            cuerpo += "</td>  </tr>\n";
                            cuerpo += "</table>\n>];\n}";



                        }


                    }

                }


            }


            string textoFinal =grafo + cuerpo +finG;

            string newPath = pathConComillas(rep->getPath());


            string filename("repMBR.txt");
            fstream file_out;
            file_out.open(filename,std::ios_base::out);
            if(!file_out.is_open())
            {
                cout<<"no se pudo abrir el archivo"<<endl;
            }
            else
            {

                file_out<<textoFinal<<endl;
                try
                {
                    vector<string> resultados;
                    resultados = split(rep->getPath(), '.');
                    string comando = "dot -T"+resultados[1] +" repMBR.txt -o " + newPath;
                    system(comando.c_str());
                    cout<<"Reporte MBR-EBR creado correctamente"<<endl;
                }
                catch(...)
                {
                    cout<<"ERROR: no se pudo crear el reporte"<<endl;

                }
            }

        }

    }
    else
    {
        cout<<"El id no existe, la particion no esta montada"<<endl;
    }


}

void reporte::ejecutarRepInodo(reporte *rep,mount paMoun[])
{
    string grafo = "digraph G{ \n rankdir=LR; \n";
    string cuerpo = "";
    string finG = "\n}";


    SupB superBloque;
    char buffer='0';
    cout<<"\n************Ejecutar REPORTE Inodo************\n"<<endl;
    string pathDisco="";
    string partName="";
    if(existeIdMount(rep->getId(),paMoun, pathDisco, partName))
    {
        dirExist(rep);
        MBR mbrTemp;
        FILE *arch;
        arch = fopen(pathDisco.c_str(),"rb+");
        if(arch != NULL)
        {
            fseek(arch,0,SEEK_SET);
            fread(&mbrTemp,sizeof(MBR),1,arch);

            int tamParticion, iniPart;
            returnDatosPart(mbrTemp, pathDisco,partName,tamParticion, iniPart);

            fseek(arch,iniPart,SEEK_SET);
            fread(&superBloque,sizeof(SupB),1,arch);
            int contador = 0;

            for(int i =0; i<superBloque.s_inodes_count; i++)
            {

                fseek(arch,superBloque.s_bm_inode_start +(i*sizeof(buffer)),SEEK_SET);
                fread(&buffer,sizeof(buffer),1,arch);
                if(buffer == '1')
                {
                    Inodo inodoTemp;
                    fseek(arch,superBloque.s_inode_start +(i*sizeof(Inodo)),SEEK_SET);
                    fread(&inodoTemp,sizeof(Inodo),1,arch);

                    cuerpo += "struct"+std::to_string(i)+" [shape=record,label=\"<f0> Inodo "+ std::to_string(i)+"|i_uid : "+std::to_string(inodoTemp.i_uid)+"|i_gid : "+std::to_string(inodoTemp.i_gid)+"|i_size : "+std::to_string(inodoTemp.i_size)+"|i_atime : ";
                    cuerpo += inodoTemp.i_atime.mbr_fecha_creacion;
                    cuerpo += "|i_ctime : ";
                    cuerpo += inodoTemp.i_ctime.mbr_fecha_creacion;
                    cuerpo += "|i_mtime : ";
                    cuerpo += inodoTemp.i_mtime.mbr_fecha_creacion;
                    cuerpo += "|i_type : ";
                    cuerpo += inodoTemp.i_type;

                    contador ++;

                    for(int j=0; j<15; j++)
                    {

                        cuerpo+= "|i_block"+std::to_string(j+1)+" : "+std::to_string(inodoTemp.i_block[j]);

                    }
                    cuerpo += "|i_perm : "+std::to_string(inodoTemp.i_perm);

                    cuerpo += "\"];";


                }

            }
            cuerpo += "\n \n";

            for(int i=1; i<contador; i++)
            {

                cuerpo += "struct"+std::to_string(i-1)+":f0 -> struct"+std::to_string(i)+":f0  ";
            }


            fclose(arch);

            string textoFinal =grafo + cuerpo +finG;

            string newPath = pathConComillas(rep->getPath());

            string filename("repInodo.txt");
            fstream file_out;
            file_out.open(filename,std::ios_base::out);
            if(!file_out.is_open())
            {
                cout<<"no se pudo abrir el archivo"<<endl;
            }
            else
            {

                file_out<<textoFinal<<endl;
                try
                {
                    vector<string> resultados;
                    resultados = split(rep->getPath(), '.');
                    string comando = "dot -T"+resultados[1] +" repInodo.txt -o " + newPath;
                    system(comando.c_str());
                    cout<<"Reporte Inodo creado correctamente"<<endl;
                }
                catch(...)
                {
                    cout<<"ERROR: no se pudo crear el reporte"<<endl;

                }
            }


        }

    }
    else
    {
        cout<<"El id no existe, la particion no esta montada"<<endl;
    }

}

void reporte::ejecutarRepBloque(reporte *rep,mount paMoun[])
{
    string grafo = "digraph G{ \n rankdir=LR; \n";
    string cuerpo = "";
    string finG = "\n}";


    SupB superBloque;
    char buffer='0';
    cout<<"\n************Ejecutar REPORTE Bloque************\n"<<endl;
    string pathDisco="";
    string partName="";
    if(existeIdMount(rep->getId(),paMoun, pathDisco, partName))
    {
        dirExist(rep);
        MBR mbrTemp;
        FILE *arch;
        arch = fopen(pathDisco.c_str(),"rb+");
        if(arch != NULL)
        {
            fseek(arch,0,SEEK_SET);
            fread(&mbrTemp,sizeof(MBR),1,arch);

            int tamParticion, iniPart;
            returnDatosPart(mbrTemp, pathDisco,partName,tamParticion, iniPart);

            fseek(arch,iniPart,SEEK_SET);
            fread(&superBloque,sizeof(SupB),1,arch);
            int contador = 0;

            for(int i =0; i<superBloque.s_inodes_count; i++)
            {

                fseek(arch,superBloque.s_bm_inode_start +(i*sizeof(buffer)),SEEK_SET);
                fread(&buffer,sizeof(buffer),1,arch);
                if(buffer == '1')
                {
                    Inodo inodoTemp;
                    fseek(arch,superBloque.s_inode_start +(i*sizeof(Inodo)),SEEK_SET);
                    fread(&inodoTemp,sizeof(Inodo),1,arch);

                    for(int j=0; j<15; j++)
                    {

                        if(j<12)
                        {
                            if(inodoTemp.i_block[j] != -1 )
                            {
                                if(inodoTemp.i_type== '0' )
                                {
                                    BCarpeta bloTemp;
                                    fseek(arch,superBloque.s_block_start +(inodoTemp.i_block[j]*sizeof(BCarpeta)),SEEK_SET);
                                    fread(&bloTemp,sizeof(BCarpeta),1,arch);

                                    cuerpo += "struct"+std::to_string(contador)+" [shape=record, width = 3,label=\"<f0> Bloque Carpeta"+ std::to_string(contador)+"|{B_name | B_inodo}";

                                    for(int k=0; k<4; k++)
                                    {
                                        cuerpo += "|{";
                                        cuerpo += bloTemp.b_content[k].b_name;
                                        cuerpo += "|"+ std::to_string(bloTemp.b_content[k].b_inodo)+"}";

                                    }


                                    contador ++;
                                    cuerpo += "\"];";

                                }
                                else   //inodo tipo archivo
                                {
                                    BArchivo bloTemp;
                                    fseek(arch,superBloque.s_block_start +(inodoTemp.i_block[j]*sizeof(BCarpeta)),SEEK_SET);
                                    fread(&bloTemp,sizeof(BCarpeta),1,arch);

                                    cuerpo += "struct"+std::to_string(contador)+" [shape=record,label=\"<f0> Bloque Archivo"+ std::to_string(contador)+"|";


                                    string contenido = bloTemp.b_content;

                                    vector<string> saltos;
                                    saltos = split(contenido, '\n');

                                    for(int k=0; k< saltos.size(); k++)
                                    {
                                        cuerpo += saltos[k] + "\\n";

                                    }


                                    contador ++;
                                    cuerpo += "\"];";

                                }


                            }

                        }
                        else if(j==12)
                        {
                            if(inodoTemp.i_block[j] != -1 )
                            {
                                cuerpo += "struct"+std::to_string(contador)+" [shape=record,label=\"<f0> Bloque Apuntador"+ std::to_string(contador)+"| ";
                                BApun apuntadores;
                                fseek(arch,superBloque.s_block_start +(inodoTemp.i_block[j]*sizeof(BCarpeta)),SEEK_SET);
                                fread(&apuntadores,sizeof(BApun),1,arch);

                                for(int k=0; k<4; k++)
                                {
                                    cuerpo += std::to_string(apuntadores.b_apuntadores [k].b_inodo) + ", ";

                                }
                                contador ++;
                                cuerpo += "\"];";

                                if(inodoTemp.i_type== '0' )
                                {
                                    for(int k=0; k<4; k++)
                                    {
                                        if(apuntadores.b_apuntadores [k].b_inodo != -1)
                                        {

                                            BCarpeta bloTemp;
                                            fseek(arch,superBloque.s_block_start +(apuntadores.b_apuntadores [k].b_inodo*sizeof(BCarpeta)),SEEK_SET);
                                            fread(&bloTemp,sizeof(BCarpeta),1,arch);

                                            cuerpo += "struct"+std::to_string(contador)+" [shape=record, width = 3,label=\"<f0> Bloque Carpeta"+ std::to_string(contador)+"|{B_name | B_inodo}";

                                            for(int k=0; k<4; k++)
                                            {
                                                cuerpo += "|{";
                                                cuerpo += bloTemp.b_content[k].b_name;
                                                cuerpo += "|"+ std::to_string(bloTemp.b_content[k].b_inodo)+"}";

                                            }


                                            contador ++;
                                            cuerpo += "\"];";
                                        }

                                    }

                                }
                                else   //inodo tipo archivo
                                {
                                    for(int k=0; k<4; k++)
                                    {
                                        if(apuntadores.b_apuntadores [k].b_inodo != -1)
                                        {
                                            BArchivo bloTemp;
                                            fseek(arch,superBloque.s_block_start +(apuntadores.b_apuntadores [k].b_inodo*sizeof(BCarpeta)),SEEK_SET);
                                            fread(&bloTemp,sizeof(BCarpeta),1,arch);

                                            cuerpo += "struct"+std::to_string(contador)+" [shape=record,label=\"<f0> Bloque Archivo"+ std::to_string(contador)+"|";


                                            string contenido = bloTemp.b_content;

                                            vector<string> saltos;
                                            saltos = split(contenido, '\n');

                                            for(int l=0; l< saltos.size(); l++)
                                            {
                                                cuerpo += saltos[l] + "\\n";

                                            }


                                            contador ++;
                                            cuerpo += "\"];";

                                        }
                                    }



                                }
                            }

                        }
                        else if(j==13)
                        {
                            if(inodoTemp.i_block[j] != -1 )
                            {
                                cuerpo += "struct"+std::to_string(contador)+" [shape=record,label=\"<f0> Bloque Apuntador"+ std::to_string(contador)+"| ";
                                BApun apuntadores;
                                fseek(arch,superBloque.s_block_start +(inodoTemp.i_block[j]*sizeof(BCarpeta)),SEEK_SET);
                                fread(&apuntadores,sizeof(BApun),1,arch);

                                for(int k=0; k<4; k++)
                                {
                                    cuerpo += std::to_string(apuntadores.b_apuntadores [k].b_inodo) + ", ";

                                }
                                contador ++;
                                cuerpo += "\"];";

                                for(int k=0; k<4; k++)
                                {
                                    if(apuntadores.b_apuntadores [k].b_inodo != -1)
                                    {

                                        cuerpo += "struct"+std::to_string(contador)+" [shape=record,label=\"<f0> Bloque Apuntador"+ std::to_string(contador)+"| ";
                                        BApun apun1;
                                        fseek(arch,superBloque.s_block_start +(apuntadores.b_apuntadores [k].b_inodo*sizeof(BCarpeta)),SEEK_SET);
                                        fread(&apun1,sizeof(BApun),1,arch);

                                        for(int l=0; l<4; l++)
                                        {
                                            cuerpo += std::to_string(apun1.b_apuntadores [l].b_inodo) + ", ";

                                        }
                                        contador ++;
                                        cuerpo += "\"];";
                                    }


                                }


                                if(inodoTemp.i_type== '0' )
                                {
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
                                                    BCarpeta bloTemp;
                                                    fseek(arch,superBloque.s_block_start +(apun1.b_apuntadores [l].b_inodo*sizeof(BCarpeta)),SEEK_SET);
                                                    fread(&bloTemp,sizeof(BCarpeta),1,arch);

                                                    cuerpo += "struct"+std::to_string(contador)+" [shape=record, width = 3,label=\"<f0> Bloque Carpeta"+ std::to_string(contador)+"|{B_name | B_inodo}";

                                                    for(int m=0; m<4; m++)
                                                    {
                                                        cuerpo += "|{";
                                                        cuerpo += bloTemp.b_content[m].b_name;
                                                        cuerpo += "|"+ std::to_string(bloTemp.b_content[m].b_inodo)+"}";

                                                    }


                                                    contador ++;
                                                    cuerpo += "\"];";
                                                }
                                            }


                                        }

                                    }

                                }
                                else   //inodo tipo archivo
                                {
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
                                                    BArchivo bloTemp;
                                                    fseek(arch,superBloque.s_block_start +(apun1.b_apuntadores [k].b_inodo*sizeof(BCarpeta)),SEEK_SET);
                                                    fread(&bloTemp,sizeof(BCarpeta),1,arch);

                                                    cuerpo += "struct"+std::to_string(contador)+" [shape=record,label=\"<f0> Bloque Archivo"+ std::to_string(contador)+"|";


                                                    string contenido = bloTemp.b_content;

                                                    vector<string> saltos;
                                                    saltos = split(contenido, '\n');

                                                    for(int n=0; n< saltos.size(); n++)
                                                    {
                                                        cuerpo += saltos[n] + "\\n";

                                                    }


                                                    contador ++;
                                                    cuerpo += "\"];";
                                                }
                                            }


                                        }

                                    }



                                }
                            }

                        }
                        else if(j==14)
                        {
                            if(inodoTemp.i_block[j] != -1 )
                            {
                                cuerpo += "struct"+std::to_string(contador)+" [shape=record,label=\"<f0> Bloque Apuntador"+ std::to_string(contador)+"| ";
                                BApun apuntadores;
                                fseek(arch,superBloque.s_block_start +(inodoTemp.i_block[j]*sizeof(BCarpeta)),SEEK_SET);
                                fread(&apuntadores,sizeof(BApun),1,arch);

                                for(int k=0; k<4; k++)
                                {
                                    cuerpo += std::to_string(apuntadores.b_apuntadores [k].b_inodo) + ", ";

                                }
                                contador ++;
                                cuerpo += "\"];";

                                for(int k=0; k<4; k++)
                                {
                                    if(apuntadores.b_apuntadores [k].b_inodo != -1)
                                    {

                                        cuerpo += "struct"+std::to_string(contador)+" [shape=record,label=\"<f0> Bloque Apuntador"+ std::to_string(contador)+"| ";
                                        BApun apun1;
                                        fseek(arch,superBloque.s_block_start +(apuntadores.b_apuntadores [k].b_inodo*sizeof(BCarpeta)),SEEK_SET);
                                        fread(&apun1,sizeof(BApun),1,arch);

                                        for(int l=0; l<4; l++)
                                        {
                                            cuerpo += std::to_string(apun1.b_apuntadores [l].b_inodo) + ", ";

                                        }
                                        contador ++;
                                        cuerpo += "\"];";
                                    }


                                }
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

                                                cuerpo += "struct"+std::to_string(contador)+" [shape=record,label=\"<f0> Bloque Apuntador"+ std::to_string(contador)+"| ";
                                                BApun apun2;
                                                fseek(arch,superBloque.s_block_start +(apun1.b_apuntadores [l].b_inodo*sizeof(BCarpeta)),SEEK_SET);
                                                fread(&apun2,sizeof(BApun),1,arch);

                                                for(int n=0; n<4; n++)
                                                {
                                                    cuerpo += std::to_string(apun2.b_apuntadores [n].b_inodo) + ", ";

                                                }
                                                contador ++;
                                                cuerpo += "\"];";
                                            }


                                        }
                                    }


                                }

                                if(inodoTemp.i_type== '0' )
                                {
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
                                                    BApun apun2;
                                                    fseek(arch,superBloque.s_block_start +(apun1.b_apuntadores [l].b_inodo*sizeof(BCarpeta)),SEEK_SET);
                                                    fread(&apun2,sizeof(BApun),1,arch);

                                                    for(int n=0; n<4; n++)
                                                    {
                                                        if(apun2.b_apuntadores [n].b_inodo != -1)
                                                        {
                                                            BCarpeta bloTemp;
                                                            fseek(arch,superBloque.s_block_start +(apun2.b_apuntadores [n].b_inodo*sizeof(BCarpeta)),SEEK_SET);
                                                            fread(&bloTemp,sizeof(BCarpeta),1,arch);


                                                            cuerpo += "struct"+std::to_string(contador)+" [shape=record, width = 3,label=\"<f0> Bloque Carpeta"+ std::to_string(contador)+"|{B_name | B_inodo}";

                                                            for(int m=0; m<4; m++)
                                                            {
                                                                cuerpo += "|{";
                                                                cuerpo += bloTemp.b_content[m].b_name;
                                                                cuerpo += "|"+ std::to_string(bloTemp.b_content[m].b_inodo)+"}";

                                                            }


                                                            contador ++;
                                                            cuerpo += "\"];";
                                                        }
                                                    }

                                                }
                                            }


                                        }

                                    }

                                }
                                else   //inodo tipo archivo
                                {
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
                                                            BArchivo bloTemp;
                                                            fseek(arch,superBloque.s_block_start +(apun2.b_apuntadores[n].b_inodo*sizeof(BCarpeta)),SEEK_SET);
                                                            fread(&bloTemp,sizeof(BCarpeta),1,arch);

                                                            cuerpo += "struct"+std::to_string(contador)+" [shape=record,label=\"<f0> Bloque Archivo"+ std::to_string(contador)+"|";


                                                            string contenido = bloTemp.b_content;

                                                            vector<string> saltos;
                                                            saltos = split(contenido, '\n');

                                                            for(int n=0; n< saltos.size(); n++)
                                                            {
                                                                cuerpo += saltos[n] + "\\n";

                                                            }


                                                            contador ++;
                                                            cuerpo += "\"];";
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
            cuerpo += "\n \n";

            for(int i=1; i<contador; i++)
            {

                cuerpo += "struct"+std::to_string(i-1)+":f0 -> struct"+std::to_string(i)+":f0  ";
            }




            fclose(arch);

            string textoFinal =grafo + cuerpo +finG;

            string newPath = pathConComillas(rep->getPath());

            string filename("repBlock.txt");
            fstream file_out;
            file_out.open(filename,std::ios_base::out);
            if(!file_out.is_open())
            {
                cout<<"no se pudo abrir el archivo"<<endl;
            }
            else
            {

                file_out<<textoFinal<<endl;
                try
                {
                    vector<string> resultados;
                    resultados = split(rep->getPath(), '.');
                    string comando = "dot -T"+resultados[1] +" repBlock.txt -o " + newPath;
                    system(comando.c_str());
                    cout<<"Reporte Bloque creado correctamente"<<endl;
                }
                catch(...)
                {
                    cout<<"ERROR: no se pudo crear el reporte"<<endl;

                }
            }

        }

    }
    else
    {
        cout<<"El id no existe, la particion no esta montada"<<endl;
    }


}

void reporte::ejecutarRepTree(reporte *rep,mount paMoun[])
{
    string grafo = "digraph G{ \n rankdir=LR;\n node [shape = record, style=filled, fillcolor=lightBlue]; \n";
    string cuerpo = "";
    string finG = "\n}";


    SupB superBloque;
    char buffer='0';
    cout<<"\n************Ejecutar REPORTE Tree************\n"<<endl;
    string pathDisco="";
    string partName="";
    if(existeIdMount(rep->getId(),paMoun, pathDisco, partName))
    {
        dirExist(rep);
        MBR mbrTemp;
        FILE *arch;
        arch = fopen(pathDisco.c_str(),"rb+");
        if(arch != NULL)
        {
            fseek(arch,0,SEEK_SET);
            fread(&mbrTemp,sizeof(MBR),1,arch);

            int tamParticion, iniPart;
            returnDatosPart(mbrTemp, pathDisco,partName,tamParticion, iniPart);

            fseek(arch,iniPart,SEEK_SET);
            fread(&superBloque,sizeof(SupB),1,arch);
            int contadorIno = 0;
            int actualIno = 0;
            int contador = 0;
            int actualBlo = 0;

            //Graficando los inodos

            for(int i =0; i<superBloque.s_inodes_count; i++)
            {

                fseek(arch,superBloque.s_bm_inode_start +(i*sizeof(buffer)),SEEK_SET);
                fread(&buffer,sizeof(buffer),1,arch);
                if(buffer == '1')
                {
                    Inodo inodoTemp;
                    fseek(arch,superBloque.s_inode_start +(i*sizeof(Inodo)),SEEK_SET);
                    fread(&inodoTemp,sizeof(Inodo),1,arch);

                    cuerpo += "inodo"+std::to_string(contadorIno)+" [shape=record,label=\"<f0> Inodo "+ std::to_string(contadorIno)+"|i_uid : "+std::to_string(inodoTemp.i_uid)+"|i_gid : "+std::to_string(inodoTemp.i_gid)+"|i_size : "+std::to_string(inodoTemp.i_size)+"|i_atime : ";
                    cuerpo += inodoTemp.i_atime.mbr_fecha_creacion;
                    cuerpo += "|i_ctime : ";
                    cuerpo += inodoTemp.i_ctime.mbr_fecha_creacion;
                    cuerpo += "|i_mtime : ";
                    cuerpo += inodoTemp.i_mtime.mbr_fecha_creacion;
                    cuerpo += "|i_type : ";
                    cuerpo += inodoTemp.i_type;

                    contadorIno ++;

                    for(int j=0; j<15; j++)
                    {

                        cuerpo+= "|<f"+std::to_string(j+1)+"> i_block"+std::to_string(j+1)+" : "+std::to_string(inodoTemp.i_block[j]);

                    }
                    cuerpo += "|i_perm : "+std::to_string(inodoTemp.i_perm);

                    cuerpo += "\"];";


                }

            }

            //Graficando Bloques
            cuerpo += "\n node [shape = record, style=filled, fillcolor=pink]; \n";

            for(int i =0; i<superBloque.s_inodes_count; i++)
            {

                fseek(arch,superBloque.s_bm_inode_start +(i*sizeof(buffer)),SEEK_SET);
                fread(&buffer,sizeof(buffer),1,arch);
                if(buffer == '1')
                {
                    Inodo inodoTemp;
                    fseek(arch,superBloque.s_inode_start +(i*sizeof(Inodo)),SEEK_SET);
                    fread(&inodoTemp,sizeof(Inodo),1,arch);

                    for(int j=0; j<15; j++)
                    {

                        if(j<12)
                        {
                            if(inodoTemp.i_block[j] != -1 )
                            {
                                if(inodoTemp.i_type== '0' )
                                {
                                    BCarpeta bloTemp;
                                    fseek(arch,superBloque.s_block_start +(inodoTemp.i_block[j]*sizeof(BCarpeta)),SEEK_SET);
                                    fread(&bloTemp,sizeof(BCarpeta),1,arch);

                                    cuerpo += "\n node [shape = record, style=filled, fillcolor=orange]; \n";
                                    cuerpo += "struct"+std::to_string(contador)+" [shape=record, width = 3,label=\"<f0> Bloque Carpeta"+ std::to_string(contador)+"|{B_name | B_inodo}";

                                    for(int k=0; k<4; k++)
                                    {
                                        cuerpo += "|{";
                                        cuerpo += bloTemp.b_content[k].b_name;
                                        cuerpo += "|<f"+std::to_string(k+1)+"> "+ std::to_string(bloTemp.b_content[k].b_inodo)+"}";

                                    }


                                    contador ++;
                                    cuerpo += "\"];";
                                    cuerpo += "\n node [shape = record, style=filled, fillcolor=pink]; \n";

                                }
                                else   //inodo tipo archivo
                                {
                                    BArchivo bloTemp;
                                    fseek(arch,superBloque.s_block_start +(inodoTemp.i_block[j]*sizeof(BCarpeta)),SEEK_SET);
                                    fread(&bloTemp,sizeof(BCarpeta),1,arch);

                                    cuerpo += "\n node [shape = record, style=filled, fillcolor=gray]; \n";
                                    cuerpo += "struct"+std::to_string(contador)+" [shape=record,label=\"<f0> Bloque Archivo"+ std::to_string(contador)+"|";


                                    string contenido = bloTemp.b_content;

                                    vector<string> saltos;
                                    saltos = split(contenido, '\n');

                                    for(int k=0; k< saltos.size(); k++)
                                    {
                                        cuerpo += saltos[k] + "\\n";

                                    }


                                    contador ++;
                                    cuerpo += "\"];";

                                    cuerpo += "\n node [shape = record, style=filled, fillcolor=pink]; \n";

                                }


                            }

                        }
                        else if(j==12)
                        {
                            if(inodoTemp.i_block[j] != -1 )
                            {
                                cuerpo += "struct"+std::to_string(contador)+" [shape=record,label=\"<f0> Bloque Apuntador"+ std::to_string(contador)+"|{B_name | B_inodo}";
                                BApun apuntadores;
                                fseek(arch,superBloque.s_block_start +(inodoTemp.i_block[j]*sizeof(BCarpeta)),SEEK_SET);
                                fread(&apuntadores,sizeof(BApun),1,arch);

                                for(int k=0; k<4; k++)
                                {
                                    cuerpo += "|{";
                                    cuerpo += apuntadores.b_apuntadores [k].b_name;
                                    cuerpo += "|<f"+std::to_string(k+1)+"> "+ std::to_string(apuntadores.b_apuntadores [k].b_inodo)+"}";

                                }
                                contador ++;
                                cuerpo += "\"];";

                                if(inodoTemp.i_type== '0' )
                                {
                                    for(int k=0; k<4; k++)
                                    {
                                        if(apuntadores.b_apuntadores [k].b_inodo != -1)
                                        {

                                            BCarpeta bloTemp;
                                            fseek(arch,superBloque.s_block_start +(apuntadores.b_apuntadores [k].b_inodo*sizeof(BCarpeta)),SEEK_SET);
                                            fread(&bloTemp,sizeof(BCarpeta),1,arch);

                                            cuerpo += "\n node [shape = record, style=filled, fillcolor=orange]; \n";
                                            cuerpo += "struct"+std::to_string(contador)+" [shape=record, width = 3,label=\"<f0> Bloque Carpeta"+ std::to_string(contador)+"|{B_name | B_inodo}";

                                            for(int k=0; k<4; k++)
                                            {
                                                cuerpo += "|{";
                                                cuerpo += bloTemp.b_content[k].b_name;
                                                cuerpo += "|<f"+std::to_string(k+1)+"> "+ std::to_string(bloTemp.b_content[k].b_inodo)+"}";

                                            }


                                            contador ++;
                                            cuerpo += "\"];";
                                            cuerpo += "\n node [shape = record, style=filled, fillcolor=pink]; \n";
                                        }

                                    }

                                }
                                else   //inodo tipo archivo
                                {
                                    for(int k=0; k<4; k++)
                                    {
                                        if(apuntadores.b_apuntadores [k].b_inodo != -1)
                                        {
                                            BArchivo bloTemp;
                                            fseek(arch,superBloque.s_block_start +(apuntadores.b_apuntadores [k].b_inodo*sizeof(BCarpeta)),SEEK_SET);
                                            fread(&bloTemp,sizeof(BCarpeta),1,arch);

                                            cuerpo += "\n node [shape = record, style=filled, fillcolor=gray]; \n";
                                            cuerpo += "struct"+std::to_string(contador)+" [shape=record,label=\"<f0> Bloque Archivo"+ std::to_string(contador)+"|";


                                            string contenido = bloTemp.b_content;

                                            vector<string> saltos;
                                            saltos = split(contenido, '\n');

                                            for(int l=0; l< saltos.size(); l++)
                                            {
                                                cuerpo += saltos[l] + "\\n";

                                            }


                                            contador ++;
                                            cuerpo += "\"];";
                                            cuerpo += "\n node [shape = record, style=filled, fillcolor=pink]; \n";

                                        }
                                    }



                                }
                            }

                        }
                        else if(j==13)
                        {
                            if(inodoTemp.i_block[j] != -1 )
                            {
                                cuerpo += "struct"+std::to_string(contador)+" [shape=record,label=\"<f0> Bloque Apuntador"+ std::to_string(contador)+"|{B_name | B_inodo}";
                                BApun apuntadores;
                                fseek(arch,superBloque.s_block_start +(inodoTemp.i_block[j]*sizeof(BCarpeta)),SEEK_SET);
                                fread(&apuntadores,sizeof(BApun),1,arch);

                                for(int k=0; k<4; k++)
                                {
                                    cuerpo += "|{";
                                    cuerpo += apuntadores.b_apuntadores [k].b_name;
                                    cuerpo += "|<f"+std::to_string(k+1)+"> "+ std::to_string(apuntadores.b_apuntadores [k].b_inodo)+"}";


                                }
                                contador ++;
                                cuerpo += "\"];";

                                for(int k=0; k<4; k++)
                                {
                                    if(apuntadores.b_apuntadores [k].b_inodo != -1)
                                    {

                                        cuerpo += "struct"+std::to_string(contador)+" [shape=record,label=\"<f0> Bloque Apuntador"+ std::to_string(contador)+"|{B_name | B_inodo}";
                                        BApun apun1;
                                        fseek(arch,superBloque.s_block_start +(apuntadores.b_apuntadores [k].b_inodo*sizeof(BCarpeta)),SEEK_SET);
                                        fread(&apun1,sizeof(BApun),1,arch);

                                        for(int l=0; l<4; l++)
                                        {
                                            cuerpo += "|{";
                                            cuerpo += apun1.b_apuntadores [l].b_name;
                                            cuerpo += "|<f"+std::to_string(l+1)+"> "+ std::to_string(apun1.b_apuntadores [l].b_inodo)+"}";


                                        }
                                        contador ++;
                                        cuerpo += "\"];";
                                    }


                                }


                                if(inodoTemp.i_type== '0' )
                                {
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
                                                    BCarpeta bloTemp;
                                                    fseek(arch,superBloque.s_block_start +(apun1.b_apuntadores [l].b_inodo*sizeof(BCarpeta)),SEEK_SET);
                                                    fread(&bloTemp,sizeof(BCarpeta),1,arch);

                                                    cuerpo += "\n node [shape = record, style=filled, fillcolor=orange]; \n";
                                                    cuerpo += "struct"+std::to_string(contador)+" [shape=record, width = 3,label=\"<f0> Bloque Carpeta"+ std::to_string(contador)+"|{B_name | B_inodo}";

                                                    for(int m=0; m<4; m++)
                                                    {
                                                        cuerpo += "|{";
                                                        cuerpo += bloTemp.b_content[m].b_name;
                                                        cuerpo += "|<f"+std::to_string(m+1)+"> "+ std::to_string(bloTemp.b_content[m].b_inodo)+"}";

                                                    }


                                                    contador ++;
                                                    cuerpo += "\"];";
                                                    cuerpo += "\n node [shape = record, style=filled, fillcolor=pink]; \n";
                                                }
                                            }


                                        }

                                    }

                                }
                                else   //inodo tipo archivo
                                {
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
                                                    BArchivo bloTemp;
                                                    fseek(arch,superBloque.s_block_start +(apun1.b_apuntadores [k].b_inodo*sizeof(BCarpeta)),SEEK_SET);
                                                    fread(&bloTemp,sizeof(BCarpeta),1,arch);

                                                    cuerpo += "\n node [shape = record, style=filled, fillcolor=gray]; \n";
                                                    cuerpo += "struct"+std::to_string(contador)+" [shape=record,label=\"<f0> Bloque Archivo"+ std::to_string(contador)+"|";


                                                    string contenido = bloTemp.b_content;

                                                    vector<string> saltos;
                                                    saltos = split(contenido, '\n');

                                                    for(int n=0; n< saltos.size(); n++)
                                                    {
                                                        cuerpo += saltos[n] + "\\n";

                                                    }


                                                    contador ++;
                                                    cuerpo += "\"];";
                                                    cuerpo += "\n node [shape = record, style=filled, fillcolor=pink]; \n";
                                                }
                                            }


                                        }

                                    }



                                }
                            }

                        }
                        else if(j==14)
                        {
                            if(inodoTemp.i_block[j] != -1 )
                            {
                                cuerpo += "struct"+std::to_string(contador)+" [shape=record,label=\"<f0> Bloque Apuntador"+ std::to_string(contador)+"|{B_name | B_inodo}";
                                BApun apuntadores;
                                fseek(arch,superBloque.s_block_start +(inodoTemp.i_block[j]*sizeof(BCarpeta)),SEEK_SET);
                                fread(&apuntadores,sizeof(BApun),1,arch);

                                for(int k=0; k<4; k++)
                                {
                                    cuerpo += "|{";
                                    cuerpo += apuntadores.b_apuntadores [k].b_name;
                                    cuerpo += "|<f"+std::to_string(k+1)+"> "+ std::to_string(apuntadores.b_apuntadores [k].b_inodo)+"}";


                                }
                                contador ++;
                                cuerpo += "\"];";

                                for(int k=0; k<4; k++)
                                {
                                    if(apuntadores.b_apuntadores [k].b_inodo != -1)
                                    {

                                        cuerpo += "struct"+std::to_string(contador)+" [shape=record,label=\"<f0> Bloque Apuntador"+ std::to_string(contador)+"|{B_name | B_inodo}";
                                        BApun apun1;
                                        fseek(arch,superBloque.s_block_start +(apuntadores.b_apuntadores [k].b_inodo*sizeof(BCarpeta)),SEEK_SET);
                                        fread(&apun1,sizeof(BApun),1,arch);

                                        for(int l=0; l<4; l++)
                                        {
                                            cuerpo += "|{";
                                            cuerpo += apun1.b_apuntadores [l].b_name;
                                            cuerpo += "|<f"+std::to_string(l+1)+"> "+ std::to_string(apun1.b_apuntadores [l].b_inodo)+"}";


                                        }
                                        contador ++;
                                        cuerpo += "\"];";
                                    }


                                }
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

                                                cuerpo += "struct"+std::to_string(contador)+" [shape=record,label=\"<f0> Bloque Apuntador"+ std::to_string(contador)+"|{B_name | B_inodo}";
                                                BApun apun2;
                                                fseek(arch,superBloque.s_block_start +(apun1.b_apuntadores [l].b_inodo*sizeof(BCarpeta)),SEEK_SET);
                                                fread(&apun2,sizeof(BApun),1,arch);

                                                for(int n=0; n<4; n++)
                                                {

                                                    cuerpo += "|{";
                                                    cuerpo += apun2.b_apuntadores [n].b_name;
                                                    cuerpo += "|<f"+std::to_string(n+1)+"> "+ std::to_string(apun2.b_apuntadores [n].b_inodo)+"}";

                                                }
                                                contador ++;
                                                cuerpo += "\"];";
                                            }


                                        }
                                    }


                                }

                                if(inodoTemp.i_type== '0' )
                                {
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
                                                    BApun apun2;
                                                    fseek(arch,superBloque.s_block_start +(apun1.b_apuntadores [l].b_inodo*sizeof(BCarpeta)),SEEK_SET);
                                                    fread(&apun2,sizeof(BApun),1,arch);

                                                    for(int n=0; n<4; n++)
                                                    {
                                                        if(apun2.b_apuntadores [n].b_inodo != -1)
                                                        {
                                                            BCarpeta bloTemp;
                                                            fseek(arch,superBloque.s_block_start +(apun2.b_apuntadores [n].b_inodo*sizeof(BCarpeta)),SEEK_SET);
                                                            fread(&bloTemp,sizeof(BCarpeta),1,arch);


                                                            cuerpo += "\n node [shape = record, style=filled, fillcolor=orange]; \n";
                                                            cuerpo += "struct"+std::to_string(contador)+" [shape=record, width = 3,label=\"<f0> Bloque Carpeta"+ std::to_string(contador)+"|{B_name | B_inodo}";

                                                            for(int m=0; m<4; m++)
                                                            {
                                                                cuerpo += "|{";
                                                                cuerpo += bloTemp.b_content[m].b_name;
                                                                cuerpo += "|<f"+std::to_string(l+1)+"> "+ std::to_string(bloTemp.b_content[m].b_inodo)+"}";

                                                            }


                                                            contador ++;
                                                            cuerpo += "\"];";
                                                            cuerpo += "\n node [shape = record, style=filled, fillcolor=pink]; \n";
                                                        }
                                                    }

                                                }
                                            }


                                        }

                                    }

                                }
                                else   //inodo tipo archivo
                                {
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
                                                            BArchivo bloTemp;
                                                            fseek(arch,superBloque.s_block_start +(apun2.b_apuntadores[n].b_inodo*sizeof(BCarpeta)),SEEK_SET);
                                                            fread(&bloTemp,sizeof(BCarpeta),1,arch);

                                                            cuerpo += "\n node [shape = record, style=filled, fillcolor=gray]; \n";
                                                            cuerpo += "struct"+std::to_string(contador)+" [shape=record,label=\"<f0> Bloque Archivo"+ std::to_string(contador)+"|";


                                                            string contenido = bloTemp.b_content;

                                                            vector<string> saltos;
                                                            saltos = split(contenido, '\n');

                                                            for(int n=0; n< saltos.size(); n++)
                                                            {
                                                                cuerpo += saltos[n] + "\\n";

                                                            }


                                                            contador ++;
                                                            cuerpo += "\"];";
                                                            cuerpo += "\n node [shape = record, style=filled, fillcolor=pink]; \n";
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


            cuerpo += "\n \n";
            //creando las lineas de los inodos a los bloques

            for(int i =0; i<superBloque.s_inodes_count; i++)
            {

                fseek(arch,superBloque.s_bm_inode_start +(i*sizeof(buffer)),SEEK_SET);
                fread(&buffer,sizeof(buffer),1,arch);
                if(buffer == '1')
                {
                    Inodo inodoTemp;
                    fseek(arch,superBloque.s_inode_start +(i*sizeof(Inodo)),SEEK_SET);
                    fread(&inodoTemp,sizeof(Inodo),1,arch);

                    for(int j=0; j<15; j++)
                    {
                        if(inodoTemp.i_block[j] != -1)
                        {
                            cuerpo += "inodo"+std::to_string(i)+":f"+std::to_string(j+1) +"-> struct"+std::to_string(inodoTemp.i_block[j])+":f0  \n";
                        }

                    }

                }

            }

            //creando las lineas para los bloques a los inodos


            for(int i =0; i<superBloque.s_inodes_count; i++)
            {

                fseek(arch,superBloque.s_bm_inode_start +(i*sizeof(buffer)),SEEK_SET);
                fread(&buffer,sizeof(buffer),1,arch);
                if(buffer == '1')
                {
                    Inodo inodoTemp;
                    fseek(arch,superBloque.s_inode_start +(i*sizeof(Inodo)),SEEK_SET);
                    fread(&inodoTemp,sizeof(Inodo),1,arch);

                    for(int j=0; j<15; j++)
                    {

                        if(j<12)
                        {
                            if(inodoTemp.i_block[j] != -1 )
                            {
                                if(inodoTemp.i_type== '0' )
                                {
                                    BCarpeta bloTemp;
                                    fseek(arch,superBloque.s_block_start +(inodoTemp.i_block[j]*sizeof(BCarpeta)),SEEK_SET);
                                    fread(&bloTemp,sizeof(BCarpeta),1,arch);


                                    for(int k=0; k<4; k++)
                                    {
                                        string p = ".";
                                        string pp = "..";
                                        if(bloTemp.b_content[k].b_inodo != -1 && bloTemp.b_content[k].b_name != p && bloTemp.b_content[k].b_name != pp)
                                        {
                                            cuerpo += "struct"+std::to_string(actualBlo)+":f"+std::to_string(k+1) +"-> inodo"+std::to_string(bloTemp.b_content[k].b_inodo)+":f0  \n";
                                            actualBlo ++;
                                        }

                                    }
                                }
                                else
                                {
                                    actualBlo ++;
                                }
                            }

                        }
                        else if(j==12)
                        {
                            if(inodoTemp.i_block[j] != -1 )
                            {
                                BApun apuntadores;
                                fseek(arch,superBloque.s_block_start +(inodoTemp.i_block[j]*sizeof(BCarpeta)),SEEK_SET);
                                fread(&apuntadores,sizeof(BApun),1,arch);

                                for(int k=0; k<4; k++)
                                {
                                    if(apuntadores.b_apuntadores[k].b_inodo != -1)
                                    {
                                        cuerpo += "struct"+std::to_string(actualBlo)+":f"+std::to_string(k+1) +"-> inodo"+std::to_string(apuntadores.b_apuntadores[k].b_inodo)+":f0  \n";
                                        actualBlo++;
                                    }
                                }

                                if(inodoTemp.i_type== '0' )
                                {
                                    for(int k=0; k<4; k++)
                                    {
                                        if(apuntadores.b_apuntadores [k].b_inodo != -1)
                                        {

                                            BCarpeta bloTemp;
                                            fseek(arch,superBloque.s_block_start +(apuntadores.b_apuntadores [k].b_inodo*sizeof(BCarpeta)),SEEK_SET);
                                            fread(&bloTemp,sizeof(BCarpeta),1,arch);

                                            for(int k=0; k<4; k++)
                                            {
                                                string p = ".";
                                                string pp = "..";
                                                if(bloTemp.b_content[k].b_inodo != -1 && bloTemp.b_content[k].b_name != p && bloTemp.b_content[k].b_name != pp)
                                                {

                                                    cuerpo += "struct"+std::to_string(actualBlo)+":f"+std::to_string(k+1) +"-> inodo"+std::to_string(bloTemp.b_content[k].b_inodo)+":f0  \n";
                                                    actualBlo++;
                                                }
                                            }

                                        }

                                    }

                                }
                                else
                                {
                                    actualBlo ++;
                                }
                            }

                        }
                        else if(j==13)
                        {
                            if(inodoTemp.i_block[j] != -1 )
                            {

                                BApun apuntadores;
                                fseek(arch,superBloque.s_block_start +(inodoTemp.i_block[j]*sizeof(BCarpeta)),SEEK_SET);
                                fread(&apuntadores,sizeof(BApun),1,arch);

                                for(int k=0; k<4; k++)
                                {
                                    if(apuntadores.b_apuntadores[k].b_inodo != -1)
                                    {
                                        cuerpo += "struct"+std::to_string(actualBlo)+":f"+std::to_string(k+1) +"-> inodo"+std::to_string(apuntadores.b_apuntadores[k].b_inodo)+":f0  \n";
                                        actualBlo++;
                                    }
                                }


                                for(int k=0; k<4; k++)
                                {
                                    if(apuntadores.b_apuntadores [k].b_inodo != -1)
                                    {


                                        BApun apun1;
                                        fseek(arch,superBloque.s_block_start +(apuntadores.b_apuntadores [k].b_inodo*sizeof(BCarpeta)),SEEK_SET);
                                        fread(&apun1,sizeof(BApun),1,arch);


                                        for(int l=0; l<4; l++)
                                        {
                                            if(apun1.b_apuntadores[l].b_inodo != -1)
                                            {
                                                cuerpo += "struct"+std::to_string(actualBlo)+":f"+std::to_string(l+1) +"-> inodo"+std::to_string(apun1.b_apuntadores[l].b_inodo)+":f0  \n";
                                                actualBlo++;
                                            }
                                        }

                                    }



                                }


                                if(inodoTemp.i_type== '0' )
                                {
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
                                                    BCarpeta bloTemp;
                                                    fseek(arch,superBloque.s_block_start +(apun1.b_apuntadores [l].b_inodo*sizeof(BCarpeta)),SEEK_SET);
                                                    fread(&bloTemp,sizeof(BCarpeta),1,arch);


                                                    for(int m=0; m<4; m++)
                                                    {
                                                        string p = ".";
                                                        string pp = "..";
                                                        if(bloTemp.b_content[m].b_inodo != -1 && bloTemp.b_content[m].b_name != p && bloTemp.b_content[m].b_name != pp)
                                                        {

                                                            cuerpo += "struct"+std::to_string(actualBlo)+":f"+std::to_string(m+1) +"-> inodo"+std::to_string(bloTemp.b_content[m].b_inodo)+":f0  \n";
                                                            actualBlo++;
                                                        }
                                                    }
                                                }
                                            }


                                        }

                                    }

                                }
                                else
                                {
                                    actualBlo ++;
                                }

                            }

                        }
                        else if(j==14)
                        {
                            if(inodoTemp.i_block[j] != -1 )
                            {

                                BApun apuntadores;
                                fseek(arch,superBloque.s_block_start +(inodoTemp.i_block[j]*sizeof(BCarpeta)),SEEK_SET);
                                fread(&apuntadores,sizeof(BApun),1,arch);

                                for(int k=0; k<4; k++)
                                {
                                    if(apuntadores.b_apuntadores[k].b_inodo != -1)
                                    {
                                        cuerpo += "struct"+std::to_string(actualBlo)+":f"+std::to_string(k+1) +"-> inodo"+std::to_string(apuntadores.b_apuntadores[k].b_inodo)+":f0  \n";
                                        actualBlo++;
                                    }
                                }


                                for(int k=0; k<4; k++)
                                {
                                    if(apuntadores.b_apuntadores [k].b_inodo != -1)
                                    {


                                        BApun apun1;
                                        fseek(arch,superBloque.s_block_start +(apuntadores.b_apuntadores [k].b_inodo*sizeof(BCarpeta)),SEEK_SET);
                                        fread(&apun1,sizeof(BApun),1,arch);

                                        for(int l=0; l<4; l++)
                                        {
                                            if(apun1.b_apuntadores[l].b_inodo != -1)
                                            {
                                                cuerpo += "struct"+std::to_string(actualBlo)+":f"+std::to_string(l+1) +"-> inodo"+std::to_string(apun1.b_apuntadores[l].b_inodo)+":f0  \n";
                                                actualBlo++;
                                            }


                                        }

                                    }


                                }
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

                                                cuerpo += "struct"+std::to_string(contador)+" [shape=record,label=\"<f0> Bloque Apuntador"+ std::to_string(contador)+"|{B_name | B_inodo}";
                                                BApun apun2;
                                                fseek(arch,superBloque.s_block_start +(apun1.b_apuntadores [l].b_inodo*sizeof(BCarpeta)),SEEK_SET);
                                                fread(&apun2,sizeof(BApun),1,arch);

                                                for(int n=0; n<4; n++)
                                                {

                                                    if(apun2.b_apuntadores[n].b_inodo != -1)
                                                    {
                                                        cuerpo += "struct"+std::to_string(actualBlo)+":f"+std::to_string(n+1) +"-> inodo"+std::to_string(apun2.b_apuntadores[n].b_inodo)+":f0  \n";
                                                        actualBlo++;

                                                    }

                                                }


                                            }
                                        }


                                    }

                                    if(inodoTemp.i_type== '0' )
                                    {
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
                                                        BApun apun2;
                                                        fseek(arch,superBloque.s_block_start +(apun1.b_apuntadores [l].b_inodo*sizeof(BCarpeta)),SEEK_SET);
                                                        fread(&apun2,sizeof(BApun),1,arch);

                                                        for(int n=0; n<4; n++)
                                                        {
                                                            if(apun2.b_apuntadores [n].b_inodo != -1)
                                                            {
                                                                BCarpeta bloTemp;
                                                                fseek(arch,superBloque.s_block_start +(apun2.b_apuntadores [n].b_inodo*sizeof(BCarpeta)),SEEK_SET);
                                                                fread(&bloTemp,sizeof(BCarpeta),1,arch);


                                                                for(int m=0; m<4; m++)
                                                                {
                                                                    string p = ".";
                                                                    string pp = "..";
                                                                    if(bloTemp.b_content[m].b_inodo != -1 && bloTemp.b_content[m].b_name != p && bloTemp.b_content[m].b_name != pp)
                                                                    {

                                                                        cuerpo += "struct"+std::to_string(actualBlo)+":f"+std::to_string(m+1) +"-> inodo"+std::to_string(bloTemp.b_content[m].b_inodo)+":f0  \n";
                                                                        actualBlo++;

                                                                    }

                                                                }

                                                            }
                                                        }

                                                    }
                                                }


                                            }

                                        }

                                    }
                                    else
                                    {
                                        actualBlo ++;
                                    }
                                }

                            }

                        }


                    }

                }

            }

            /*

            for(int i=1; i<contador; i++)
            {

                cuerpo += "struct"+std::to_string(i-1)+":f0 -> struct"+std::to_string(i)+":f0  ";
            }
            */

            fclose(arch);

            string textoFinal =grafo + cuerpo +finG;

            string newPath = pathConComillas(rep->getPath());

            string filename("repTree.txt");
            fstream file_out;
            file_out.open(filename,std::ios_base::out);
            if(!file_out.is_open())
            {
                cout<<"no se pudo abrir el archivo"<<endl;
            }
            else
            {

                file_out<<textoFinal<<endl;
                try
                {
                    vector<string> resultados;
                    resultados = split(rep->getPath(), '.');
                    string comando = "dot -T"+resultados[1] +" repTree.txt -o " + newPath;
                    system(comando.c_str());
                    cout<<"Reporte Tree creado correctamente"<<endl;
                }
                catch(...)
                {
                    cout<<"ERROR: no se pudo crear el reporte"<<endl;

                }
            }


        }
    }

    else
    {
        cout<<"El id no existe, la particion no esta montada"<<endl;
    }

}

void reporte::ejecutarRepFile(reporte *rep,mount paMoun[])
{
    string grafo = "digraph G{ \n rankdir=LR; \n";
    string cuerpo = "";
    string finG = "\n}";

    vector<string> rutaArchivo;
    rutaArchivo = split(rep->getRuta(), '/');

    SupB superBloque;
    char buffer='0';
    cout<<"\n************Ejecutar REPORTE File************\n"<<endl;
    string pathDisco="";
    string partName="";
    if(existeIdMount(rep->getId(),paMoun, pathDisco, partName))
    {
        dirExist(rep);
        MBR mbrTemp;
        FILE *arch;
        arch = fopen(pathDisco.c_str(),"rb+");
        if(arch != NULL)
        {
            fseek(arch,0,SEEK_SET);
            fread(&mbrTemp,sizeof(MBR),1,arch);

            int tamParticion, iniPart;
            returnDatosPart(mbrTemp, pathDisco,partName,tamParticion, iniPart);

            fseek(arch,iniPart,SEEK_SET);
            fread(&superBloque,sizeof(SupB),1,arch);
            int contadorCarpetas = 0;

            Inodo inodoTemp;
            fseek(arch,superBloque.s_inode_start,SEEK_SET);
            fread(&inodoTemp,sizeof(Inodo),1,arch);


            bool existeArchivo = false;
            bool existeCarpeta = false;
            int carp = rutaArchivo.size();

            cuerpo += "struct0 [shape=record,label=\"<f0> Archivo | nombre : "+rutaArchivo[carp-1] + " | Contenido | ";


            for(int i= 1; i< carp +1; i++)
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
                else
                {
                    cout<<"Se encontro el archivo :)"<<endl;
                    existeArchivo= true;
                    break;

                }

                if(!existeCarpeta)
                {
                    cout<<"ERROR: no se encontro la ruta del archivo"<<endl;
                    break;
                }


            }

            if(existeArchivo)
            {

                for(int i = 0; i<15; i++)
                {
                    if(i<12)
                    {
                        if(inodoTemp.i_block[i] != -1 )
                        {

                            BArchivo bloTemp;
                            fseek(arch,superBloque.s_block_start +(inodoTemp.i_block[i]*sizeof(BCarpeta)),SEEK_SET);
                            fread(&bloTemp,sizeof(BCarpeta),1,arch);

                            //cuerpo += "struct"+std::to_string(contador)+" [shape=record,label=\"<f0> Bloque Archivo"+ std::to_string(contador)+"|";


                            string contenido = bloTemp.b_content;

                            vector<string> saltos;
                            saltos = split(contenido, '\n');

                            for(int k=0; k< saltos.size(); k++)
                            {
                                cuerpo += saltos[k] + "\\n";

                            }



                        }

                    }
                    else if(i==12)
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
                                    BArchivo bloTemp;
                                    fseek(arch,superBloque.s_block_start +(apuntadores.b_apuntadores [k].b_inodo*sizeof(BCarpeta)),SEEK_SET);
                                    fread(&bloTemp,sizeof(BCarpeta),1,arch);

                                    //cuerpo += "struct"+std::to_string(contador)+" [shape=record,label=\"<f0> Bloque Archivo"+ std::to_string(contador)+"|";


                                    string contenido = bloTemp.b_content;

                                    vector<string> saltos;
                                    saltos = split(contenido, '\n');

                                    for(int k=0; k< saltos.size(); k++)
                                    {
                                        cuerpo += saltos[k] + "\\n";

                                    }

                                }
                            }




                        }

                    }
                    else if(i==13)
                    {
                        if(inodoTemp.i_block[i] != -1 )
                        {
                            // cuerpo += "struct"+std::to_string(contador)+" [shape=record,label=\"<f0> Bloque Apuntador"+ std::to_string(contador)+"| ";
                            BApun apuntadores;
                            fseek(arch,superBloque.s_block_start +(inodoTemp.i_block[i]*sizeof(BCarpeta)),SEEK_SET);
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
                                            BArchivo bloTemp;
                                            fseek(arch,superBloque.s_block_start +(apun1.b_apuntadores [l].b_inodo*sizeof(BCarpeta)),SEEK_SET);
                                            fread(&bloTemp,sizeof(BCarpeta),1,arch);

                                            //cuerpo += "struct"+std::to_string(contador)+" [shape=record,label=\"<f0> Bloque Archivo"+ std::to_string(contador)+"|";


                                            string contenido = bloTemp.b_content;

                                            vector<string> saltos;
                                            saltos = split(contenido, '\n');

                                            for(int k=0; k< saltos.size(); k++)
                                            {
                                                cuerpo += saltos[k] + "\\n";

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
                                                    BArchivo bloTemp;
                                                    fseek(arch,superBloque.s_block_start +(apun2.b_apuntadores[n].b_inodo*sizeof(BCarpeta)),SEEK_SET);
                                                    fread(&bloTemp,sizeof(BCarpeta),1,arch);

                                                    //cuerpo += "struct"+std::to_string(contador)+" [shape=record,label=\"<f0> Bloque Archivo"+ std::to_string(contador)+"|";


                                                    string contenido = bloTemp.b_content;

                                                    vector<string> saltos;
                                                    saltos = split(contenido, '\n');

                                                    for(int n=0; n< saltos.size(); n++)
                                                    {
                                                        cuerpo += saltos[n] + "\\n";

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

                cuerpo += "\"];";


                fclose(arch);

                string textoFinal =grafo + cuerpo +finG;

                string newPath = pathConComillas(rep->getPath());

                string filename("repFile.txt");
                fstream file_out;
                file_out.open(filename,std::ios_base::out);
                if(!file_out.is_open())
                {
                    cout<<"no se pudo abrir el archivo"<<endl;
                }
                else
                {

                    file_out<<textoFinal<<endl;
                    try
                    {
                        vector<string> resultados;
                        resultados = split(rep->getPath(), '.');
                        string comando = "dot -T"+resultados[1] +" repFile.txt -o " + newPath;
                        system(comando.c_str());
                        cout<<"Reporte File creado correctamente"<<endl;
                    }
                    catch(...)
                    {
                        cout<<"ERROR: no se pudo crear el reporte"<<endl;

                    }
                }
            }
            else
            {
                cout<<"ERROR: El Archivo no existe"<<endl;
            }


        }

    }
    else
    {
        cout<<"El id no existe, la particion no esta montada"<<endl;
    }


}

void reporte::ejecutarRepLs(reporte *rep,mount paMoun[])
{
    string grafo = "digraph G{ subgraph cluster{ label=\"Carpeta: "+rep->getRuta()+" \" tbl[shape=box,label=< <table border='0' cellborder='1' cellspacing='0' width='300'  height='200' > <tr>  <td width='150'> <b>Permisos</b> </td> <td width='150'> <b>Owner</b> </td> <td width='150'> <b>Grupo</b> </td> <td width='150'> <b>Size</b> </td> <td width='150'> <b>Fecha</b> </td> <td width='150'> <b>Tipo</b> </td> <td width='150'> <b>Nombre</b> </td> </tr>";
    string cuerpo = "";
    string finG = "\n}";

    vector<string> rutaArchivo;
    rutaArchivo = split(rep->getRuta(), '/');

    SupB superBloque;
    char buffer='0';
    cout<<"\n************Ejecutar REPORTE LS************\n"<<endl;
    string pathDisco="";
    string partName="";
    if(existeIdMount(rep->getId(),paMoun, pathDisco, partName))
    {
        dirExist(rep);
        MBR mbrTemp;
        FILE *arch;
        arch = fopen(pathDisco.c_str(),"rb+");
        if(arch != NULL)
        {
            fseek(arch,0,SEEK_SET);
            fread(&mbrTemp,sizeof(MBR),1,arch);

            int tamParticion, iniPart;
            returnDatosPart(mbrTemp, pathDisco,partName,tamParticion, iniPart);

            fseek(arch,iniPart,SEEK_SET);
            fread(&superBloque,sizeof(SupB),1,arch);
            int contadorCarpetas = 0;

            Inodo inodoTemp;
            fseek(arch,superBloque.s_inode_start,SEEK_SET);
            fread(&inodoTemp,sizeof(Inodo),1,arch);


            bool existeArchivo = false;
            bool existeCarpeta = false;
            int carp = rutaArchivo.size();

            //cuerpo += "struct0 [shape=record,label=\"<f0> Carpeta | {Permisos | Owner | Grupo | Size | Fecha | Tipo | Nombre} ";

        if(rep->getRuta() != "/"){
            for(int i= 1; i< carp; i++)
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
                    cout<<"ERROR: no se encontro la ruta del archivo"<<endl;
                    break;
                }


            }

            }else
            {
                existeCarpeta = true;

            }
            if(existeCarpeta)
            {

                for(int i = 0; i<15; i++)
                {
                    if(i<12)
                    {
                        if(inodoTemp.i_block[i] != -1 )
                        {

                            BCarpeta carpetaComprobar;
                            fseek(arch,superBloque.s_block_start +(inodoTemp.i_block[i]*sizeof(BCarpeta)),SEEK_SET);
                            fread(&carpetaComprobar,sizeof(BCarpeta),1,arch);

                            for(int k = 0; k<4; k++)
                            {
                                if(carpetaComprobar.b_content[k].b_inodo != -1 )
                                {
                                    cuerpo += "<tr> <td>"+std::to_string(inodoTemp.i_perm) +" </td><td> "+std::to_string(inodoTemp.i_uid)+" </td><td> "+std::to_string(inodoTemp.i_gid)+" </td><td> "+std::to_string(inodoTemp.i_size)+" </td><td> ";
                                    cuerpo += inodoTemp.i_ctime.mbr_fecha_creacion;
                                    Inodo ver;
                                    fseek(arch,superBloque.s_inode_start +(carpetaComprobar.b_content[k].b_inodo*sizeof(Inodo)),SEEK_SET);
                                    fread(&ver,sizeof(Inodo),1,arch);
                                    if(ver.i_type== '0' )
                                    {
                                        cuerpo += " </td><td> Carpeta </td><td> ";
                                    }
                                    else
                                    {
                                        cuerpo += " </td><td> Archivo </td><td> ";
                                    }
                                    cuerpo += carpetaComprobar.b_content[k].b_name;
                                    cuerpo += "</td></tr>";



                                }
                            }



                        }

                    }
                    else if(i==12)
                    {
                        if(inodoTemp.i_block[i] != -1 )
                        {
                            // cuerpo += "struct"+std::to_string(contador)+" [shape=record,label=\"<f0> Bloque Apuntador"+ std::to_string(contador)+"| ";
                            BApun apuntadores;
                            fseek(arch,superBloque.s_block_start +(inodoTemp.i_block[i]*sizeof(BCarpeta)),SEEK_SET);
                            fread(&apuntadores,sizeof(BApun),1,arch);


                            if(inodoTemp.i_type== '0' )
                            {
                                for(int k=0; k<4; k++)
                                {
                                    if(apuntadores.b_apuntadores [k].b_inodo != -1)
                                    {

                                        BCarpeta bloTemp;
                                        fseek(arch,superBloque.s_block_start +(apuntadores.b_apuntadores [k].b_inodo*sizeof(BCarpeta)),SEEK_SET);
                                        fread(&bloTemp,sizeof(BCarpeta),1,arch);

                                        //cuerpo += "struct"+std::to_string(contador)+" [shape=record, width = 3,label=\"<f0> Bloque Carpeta"+ std::to_string(contador)+"|{B_name | B_inodo}";

                                        for(int k=0; k<4; k++)
                                        {
                                            if(bloTemp.b_content[k].b_inodo != -1 )
                                            {
                                                cuerpo += "<tr> <td>"+std::to_string(inodoTemp.i_perm) +" </td><td>  "+std::to_string(inodoTemp.i_uid)+" </td><td> "+std::to_string(inodoTemp.i_gid)+" </td><td> "+std::to_string(inodoTemp.i_size)+" </td><td> ";
                                                cuerpo += inodoTemp.i_ctime.mbr_fecha_creacion;
                                                Inodo ver;
                                                fseek(arch,superBloque.s_inode_start +(bloTemp.b_content[k].b_inodo*sizeof(Inodo)),SEEK_SET);
                                                fread(&ver,sizeof(Inodo),1,arch);
                                                if(ver.i_type== '0' )
                                                {
                                                    cuerpo += " </td><td> Carpeta </td><td> ";
                                                }
                                                else
                                                {
                                                    cuerpo += " </td><td> Archivo </td><td> ";
                                                }
                                                cuerpo += bloTemp.b_content[k].b_name;
                                                cuerpo += "</td></tr>";



                                            }

                                        }


                                    }

                                }

                            }

                        }

                    }
                    else if(i==13)
                    {
                        if(inodoTemp.i_block[i] != -1 )
                        {
                            // cuerpo += "struct"+std::to_string(contador)+" [shape=record,label=\"<f0> Bloque Apuntador"+ std::to_string(contador)+"| ";
                            BApun apuntadores;
                            fseek(arch,superBloque.s_block_start +(inodoTemp.i_block[i]*sizeof(BCarpeta)),SEEK_SET);
                            fread(&apuntadores,sizeof(BApun),1,arch);


                            if(inodoTemp.i_type== '0' )
                            {
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
                                                BCarpeta bloTemp;
                                                fseek(arch,superBloque.s_block_start +(apun1.b_apuntadores [l].b_inodo*sizeof(BCarpeta)),SEEK_SET);
                                                fread(&bloTemp,sizeof(BCarpeta),1,arch);

                                                //cuerpo += "struct"+std::to_string(contador)+" [shape=record, width = 3,label=\"<f0> Bloque Carpeta"+ std::to_string(contador)+"|{B_name | B_inodo}";

                                                for(int m=0; m<4; m++)
                                                {
                                                    if(bloTemp.b_content[m].b_inodo != -1 )
                                                    {
                                                        cuerpo += "<tr> <td>"+std::to_string(inodoTemp.i_perm) +" </td><td> "+std::to_string(inodoTemp.i_uid)+" </td><td>  "+std::to_string(inodoTemp.i_gid)+" </td><td>  "+std::to_string(inodoTemp.i_size)+" </td><td>  ";
                                                        cuerpo += inodoTemp.i_ctime.mbr_fecha_creacion;
                                                        Inodo ver;
                                                        fseek(arch,superBloque.s_inode_start +(bloTemp.b_content[m].b_inodo*sizeof(Inodo)),SEEK_SET);
                                                        fread(&ver,sizeof(Inodo),1,arch);
                                                        if(ver.i_type== '0' )
                                                        {
                                                            cuerpo += " </td><td>  Carpeta </td><td>  ";
                                                        }
                                                        else
                                                        {
                                                            cuerpo += " </td><td>  Archivo </td><td>  ";
                                                        }
                                                        cuerpo += bloTemp.b_content[m].b_name;
                                                        cuerpo += "</td></tr>";



                                                    }

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



                            if(inodoTemp.i_type== '0' )
                            {
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
                                                BApun apun2;
                                                fseek(arch,superBloque.s_block_start +(apun1.b_apuntadores [l].b_inodo*sizeof(BCarpeta)),SEEK_SET);
                                                fread(&apun2,sizeof(BApun),1,arch);

                                                for(int n=0; n<4; n++)
                                                {
                                                    if(apun2.b_apuntadores [n].b_inodo != -1)
                                                    {
                                                        BCarpeta bloTemp;
                                                        fseek(arch,superBloque.s_block_start +(apun2.b_apuntadores [n].b_inodo*sizeof(BCarpeta)),SEEK_SET);
                                                        fread(&bloTemp,sizeof(BCarpeta),1,arch);


                                                        //cuerpo += "struct"+std::to_string(contador)+" [shape=record, width = 3,label=\"<f0> Bloque Carpeta"+ std::to_string(contador)+"|{B_name | B_inodo}";

                                                        for(int m=0; m<4; m++)
                                                        {
                                                            if(bloTemp.b_content[m].b_inodo != -1 )
                                                            {
                                                                cuerpo += "<tr> <td>"+std::to_string(inodoTemp.i_perm) +" </td><td> "+std::to_string(inodoTemp.i_uid)+" </td><td> "+std::to_string(inodoTemp.i_gid)+" </td><td> "+std::to_string(inodoTemp.i_size)+" </td><td> ";
                                                                cuerpo += inodoTemp.i_ctime.mbr_fecha_creacion;
                                                                Inodo ver;
                                                                fseek(arch,superBloque.s_inode_start +(bloTemp.b_content[m].b_inodo*sizeof(Inodo)),SEEK_SET);
                                                                fread(&ver,sizeof(Inodo),1,arch);
                                                                if(ver.i_type== '0' )
                                                                {
                                                                    cuerpo += " </td><td> Carpeta </td><td> ";
                                                                }
                                                                else
                                                                {
                                                                    cuerpo += " </td><td> Archivo </td><td> ";
                                                                }
                                                                cuerpo += bloTemp.b_content[m].b_name;
                                                                cuerpo += "</td></tr>";



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

                cuerpo += "</table> >]; }";


                fclose(arch);

                string textoFinal =grafo + cuerpo +finG;

                string newPath = pathConComillas(rep->getPath());

                string filename("repLS.txt");
                fstream file_out;
                file_out.open(filename,std::ios_base::out);
                if(!file_out.is_open())
                {
                    cout<<"no se pudo abrir el archivo"<<endl;
                }
                else
                {

                    file_out<<textoFinal<<endl;
                    try
                    {
                        vector<string> resultados;
                        resultados = split(rep->getPath(), '.');
                        string comando = "dot -T"+resultados[1] +" repLS.txt -o " + newPath;
                        system(comando.c_str());
                        cout<<"Reporte LS creado correctamente"<<endl;
                    }
                    catch(...)
                    {
                        cout<<"ERROR: no se pudo crear el reporte"<<endl;

                    }
                }
            }
            else
            {
                cout<<"ERROR: La carpeta no existe"<<endl;
            }


        }

    }
    else
    {
        cout<<"El id no existe, la particion no esta montada"<<endl;
    }

}

bool reporte::existeIdMount(string idP,mount paMoun[], string &pathD, string &partN)
{
    for(int i =0; i<100; i++)
    {
        if(idP == paMoun[i].getId())
        {
            pathD = paMoun[i].getPath();
            partN = paMoun[i].getName();
            return true;

        }

    }
    return false;
}

void reporte::returnDatosPart(MBR mbrTemp, string pathD,string nombrePart,int &tamPart, int &iniPart)
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

void reporte::dirExist(reporte *disco)
{
    //verificar que el directorio existe
    vector<string> resultados;
    resultados = split(disco->getPath(), '/');
    string newpath = "";
    string pathconc = "";
    for (int i = 1; i < resultados.size() - 1; i++)
    {
        //llenar el string con el path sin el disk.dk
        //pathconc += "/\"" + resultados[i] + "\""; //le agrego comillas a los nombres por si vienen con espacios por ejemplo "mis discos"
        if(strstr(resultados[i].c_str(), " ")!=NULL)
        {
            vector<string> conc = split(resultados[i], ' ');
            resultados[i]="\""+conc[0] + " "+conc[1]+"\"";
        }
        newpath += "/" + resultados[i];           //este es sin comillas para buscar el directorio no importa si vienen con espacios
    }
    //verificar 3 veces si el archivo existe, si el path existe, y enviar el path con comillas
    cout<<newpath<<endl;
    struct stat st;
    if (stat(disco->getPath().c_str(), &st) == 0)
    {
        printf("Existe el archivo en: %s\n", disco->getPath().c_str());
    }
    else if (stat(newpath.c_str(), &st) == 0)
    {
        printf("Existe el directorio: %s\n",newpath.c_str());
    }
    else
    {
        try
        {
            printf("No Existe el directorio: %s\n", disco->getPath().c_str());
            string comando = "mkdir -p " + newpath;
            system(comando.c_str());
            printf("Se creo el dir en: %s \n", disco->getPath().c_str());

        }
        catch(...)
        {
            cout<<"ERROR: no se pudo crear la carpeta"<<endl;
        }

    }
}

string reporte::pathConComillas(string pathDestino)
{
    vector<string> resultados;
    resultados = split(pathDestino, '/');
    string newpath = "";
    string pathconc = "";
    for (int i = 1; i < resultados.size(); i++)
    {
        //llenar el string con el path sin el disk.dk
        //pathconc += "/\"" + resultados[i] + "\""; //le agrego comillas a los nombres por si vienen con espacios por ejemplo "mis discos"
        if(strstr(resultados[i].c_str(), " ")!=NULL)
        {
            vector<string> conc = split(resultados[i], ' ');
            resultados[i]="\""+conc[0] + " "+conc[1]+"\"";
        }
        newpath += "/" + resultados[i];           //este es sin comillas para buscar el directorio no importa si vienen con espacios
    }
    //verificar 3 veces si el archivo existe, si el path existe, y enviar el path con comillas
    return newpath;

}

vector<string> reporte::split(string str, char pattern)
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


