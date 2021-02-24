#ifndef FILE_ENGINE_H
#include<fstream>
#include<string>
#include<unistd.h>
using namespace std;
struct read_file_data
{
  long size;
  char data[256*1024];
};
class FileEngine{
    public:
static string write(const char *id_office,const int signal,const char*content,long size);//写入文件
static read_file_data * read(const char *filename);//读取文件

};


#endif