
#ifndef PACKET_FORMAT_H
#define PACKET_FORMAT_H
#include<time.h>
//服务器信息包
struct Reader_Imformation{//个人信息包
// char icon[256*1024];
long iconsize;
char name[45];
int monthticket;
int bytes;
int recommendticket;
};
struct Action_Tobytes{
char novelname[25];
int bytes;
};
struct Shelf_Imformation{

   //  char icon[256*1024];
    char bookname[45];
    int wordcount;
    char writername[45];
    time_t update;
    char state[10];
    int noreadchapter=0;
    char chaptername[20];
    int flag=0;
    char type[10];
};
struct Writer_Imformation{
  long iconsize;
   int bookcount;
   int wordcount;
};
struct History_res{
   long time;
   int money;
};
struct Book_Imformation{

   //  char icon[256*1024];
   //  long iconsize;
   //char writericon[256*1024];
   //char writericonsize;
    char bookname[45];
    int wordcount;
    char writername[45];
    time_t update;
    char state[10];
    char sort[10];
    char sex[5];
    bool addshelf=0;
    int flag=0;
    int month_ticket;
    int recommend_ticket;
    char noveless[400];
};
struct Recommend_Ticket{
   char icon[256*1024];
   char title[20];
   char bookname[45];
   int wordcount;
   char writername[45];
   char novelless[300];
   char state[10];
};
struct Sort{
// char icon[256*1024];
char title[20];
char title_parent[20];
int flag=0;
long iconsize;

};
struct Rank{
// char icon[256*1024];
int wordcount;
int recommendcount;
int monthcount;
long iconsize;
char writername[45];
char name[40];
char sort[40];
char flag[20];
};
struct BookList{
   //  char icon[256*1024];

   char bookname[45];
   int wordcount;
   char writername[45];
   char novelless[500];
   char state[10];
   long iconsize;
   int flag=0;
   char type[5];
};
// struct Update_Fan{
// int recommendticket_count;   
// int monthticket_count;
// int bytes;
// int givebytes;
// int flag=0;

// };
struct Content{
char bookname[100];
char chaptername[100];
char content[100000];
int index;
int bytesmoney;


};
struct requestbuychapter{
    int chapterindex;
};
struct littlepakage{
   void *content;
   int size;
   int type;  //time_t time_flag;
    
};
//客户端信息包
struct requestLogin{
char account[20];
char password[9];
};
struct requestRegister{
int index;
char username[45];
char password[9];
char id_office[19];

};

struct requestbookimformation{
    char name[25];
};
class requestbooklist{
char title[10];

};
struct requestcreatebook{
   public:
   char bookname[45];
   char catogory[25];
   char sex[5];
   // char icon[256*1024];
   long iconsize;
   char novelless[400];
   char state[25];

};
struct requestcreatesort{
   char name[45];
   // char icon[256*1024];
   char sex[5];
   long iconsize;
};
struct requestcatogrybook{
   char name[45];
   char sex[5];
   int set;
};
struct requestaddbytes{
   int money;
};
struct requestchapter{
   char bookname[45];
   int novelindex;
   int setnext;
};
struct requestnovelchapter{
   char bookname[40];
};
struct novelchapter
{
   char bookname[40];
   char chaptername[50];
   int chapterindex;
   int bytemoneys;
   long time;
   
};
struct requestwriterimformation
{
    char writername[45];

};
struct fanlist{
char name[45];
int fanvalue;
int state;

};
struct SHELFinventory{
 char name[45];
 int tyflag;
 int sfflag;
 int chapterindex;
};
#endif
