#include"./include/FileEngine.h"
#include"include/afx_mysignal.h"
#include<string.h>
#include<iostream>
#include<errno.h>
string  FileEngine::write(const char *id_office,const int signal,const char *content,long size)
{
    string filename;
    char *filename_real=new char[100];
if(signal==BOOKICON)
{                                       
  filename="/GraduationThesis/serverframe/bookicon/%s";
   
   sprintf(filename_real,filename.c_str(),id_office);

}
else if(signal==READERICON)
{
  filename="/GraduationThesis/serverframe/readericon/%s";
   
   sprintf(filename_real,filename.c_str(),id_office);

}
else if(signal==WRITERICON)
{
  filename="/GraduationThesis/serverframe/writericon/%s";
   
   sprintf(filename_real,filename.c_str(),id_office);
}
else if(signal==SORTICON)
{
  filename="/GraduationThesis/serverframe/sorticon/%s";
   
   sprintf(filename_real,filename.c_str(),id_office);
}
fstream file;
file.open(filename_real,ios::binary|ios::out|ios::trunc);
if(!file)
{
  return NULL;
}
file.write(content,size);

file.close();
return filename_real;

}//写入文件
read_file_data * FileEngine::read(const char *filename)//读取文件
{
  
  read_file_data* ttdata=new read_file_data();
ifstream file(filename,ios::binary|ios::in);


if(!file) {std::cout<<strerror(errno); return NULL;}
memset(ttdata->data,'z',256*1024);

file.seekg(0,ios::end);
 ttdata->size=file.tellg();
 file.clear();
 file.seekg(0,ios::beg);
 file.read(ttdata->data,ttdata->size);
 file.close();
return ttdata;
}