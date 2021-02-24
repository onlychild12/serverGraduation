#ifndef LIT_SQL_H
#define LIT_SQL_H
#include<mysql/mysql.h>
#include<fstream>
#include<iostream>
#include<string>
#include<string.h>
#include<vector>
#include<typeinfo>
#include"Score.h"
#include"afx_mysignal.h"
#include"PacketFormat.h"
struct Parame{
int a;
char b[100];
float c;
double d;
long e;
std::string flag;
};
class Litsql{
private:
static std::vector<Parame*> parame;
static char *host;
static char *user;
static char *password;
static char *db;//数据库
static unsigned int port;//端口
static char *unix_socket;
static unsigned long flag;

public:
static int onebatchrank[2];
static int secondbatchrank[2];
static int thirdbatchrank[2];
static int techlogyrank[2];
static char type_class[2][45];
~Litsql();
static void connect(MYSQL*conn);
static void init();
// template<typename...T>
// static MYSQL_RES*Litslectquery(std::string sql,T &...t);
// template<typename...T>
// static bool Litquery(std::string sql,const T & ...t);
// template<typename t,typename...T>
// static void getparame(t &t_1,T &...tp);
// template<typename t>
// static void getparame(t & t_1);
// static MYSQL_RES* selectscore(Score);
static bool RegisterWriter(const char*username,const char*password,const char*id_office);
static MYSQL_RES* selectSchool(char schoolname[]);
static bool Login(const char*username,const char *password,int signal);
static bool Register(const char*username,const char*password,const char *icon_path,const char *id_office);
static MYSQL_RES* getshelf(const char*usernname);
static MYSQL_RES* getRecommend(const char*name);
static Writer_Imformation getAuthor(const char*name);
static Content* getChapter(const char *bookname,int chapterindex,int bset);
static MYSQL_RES* getBookList(char* name);
static bool getbookshelfstate(const char *username,const char *novelname);
static MYSQL_RES* getReader(const char*name);
static bool updateReader(const char*icon,const char *username,long size);
static bool  addbytes(const char*username,int money);
static bool toticket(const char*username,const char *novelname,int ticket);
static bool updatepassword(const char*username,const char*password,const char *username_old,const char*password_old);
static MYSQL_RES* getbookrank(int);
static MYSQL_RES* getCatogry();
static MYSQL_RES* getBOOKimformation(const char *novel_name);
static bool insertintobook(const char *name,const char *writer,const char *type,const char *sex,long size,const char *novelless);
static bool CreateCatogry(const char*name,const char *sex,long size);
static MYSQL_RES* getCatogrybooklist(const char *name,const char *sex,const int set=0);
static bool addshelf(const char*username,const char *novel);
static bool givenovelmoney(const char*username,const char *novelname,int bytes);
static bool torecommendticket(const char *username,const char *novelname,int ticket);
static bool insertintochapter(const char *novelname,const char *chaptername,const char *content);
static MYSQL_RES* getchapterlist(const char*novename);
static MYSQL_RES* getmoneyhistory(const char *username);
static bool getcontact(const char *bookname);
static int scanchapterstate(int id);
static bool buychapter(const char*username,const int cid);
static MYSQL_RES* getauthorbooklist(const char *username);
static bool addfanvalue(const char*username,const char *novelname,const int bytes);
static MYSQL_RES*getfanlist(const char *novelname);
static fanlist*getpersonvalue(const char*username,const char*novelname);
static bool savereadstate(const char *username,const int chapterid);
static Content* getreadstate(const char *username,const char *novelname);
static bool addlikecatogry(const char *sex,const char *catogry,const char *username);
static bool deletelikecatogry(const char *username);
static MYSQL_RES* getlikecatogry(const char*username);
static bool deleteshelf(const char *username,const char *novelname);
static MYSQL_RES*getcatogrylittle();
static bool updatechaptercontent(const char *content,const char *chaptername,const int cid);
static MYSQL_RES*getcatogryid(const char *username);
static MYSQL_RES*getcatogrybookthree(const int cid);
static bool deletechapter(const int cid);
static bool deletenovelbook(const char *novelname);
static bool updatenovelimformation(const char *novelname,const char *novelless,const char *type,const char *sex,const char*state);
static bool updatechapterimfotmation(const char *title,const char *content,const int cid);
static MYSQL_RES*getendbooklist();
static MYSQL_RES*getfreebooklist();
static MYSQL_RES *search(const char *bookname);
static MYSQL_RES *getreadhistory(const char *username);
static bool shelfinventory(const char *name,int tyflag,int sfflag);
static bool enablechapter(int index,int sfflag);
};
// template<typename t>
// void Litsql::getparame(t &t_1)
// {
//       Parame *tmp_pu=new Parame();
//     if(t_1!=NULL)
//     {
//         std::string type_t=typeid(t_1).name(); 
//         tmp_pu->flag=type_t;
//     //     if(type_t=="i")   
//     //    {
//     //     tmp_pu->a=t_1;
    
//     //    }
//     //     else if(type_t=="f")
//     //     {
//     //     tmp_pu->c=t_1;
//     //     }
//     //     else if(type_t=="d")
//     //     {
//     //     tmp_pu->d=t_1;
//     //     }
//     //     else if(type_t=="l")
//     //     {
//     //     tmp_pu->e=t_1;
//     //     }
//         // else
//         // strcpy(tmp_pu->b,t_1);
//         parame.push_back(tmp_pu);
//     }
// }
// template<typename...T>
// bool Litsql::Litquery(std::string sql,const T & ...t)
// {
// getparame(t...);
// setsql(sql);

// // mysql_query();
// return true;
// }

// template<typename t,typename...T>
// void Litsql::getparame(t &t_1,T &...tp)
// {
//     Parame *tmp_pu=new Parame();
   
//     // if(t_1!=NULL)
//     // {
//         std::string type_t=typeid(t_1).name(); 
//          tmp_pu->flag=type_t;
//         //  std::cout<<type_t<<std::endl;

//         // if(type_t=="i")   
//         // tmp_pu->a=t_1;
//         // else if(type_t=="f")
//         // tmp_pu->c=t_1;
//         // else if(type_t=="d")
//         // tmp_pu->d=t_1;
//         // else if(type_t=="l")
//         // tmp_pu->e=t_1;
      
//     /*else*/ if(type_t.length()>=3)
//         {
            
//             if(type_t.substr(type_t.length()-2,2)=="_c")
//                { 
//                }
//         }
        
//         parame.push_back(tmp_pu);
//         getparame(tp...);
//     // }
//     // else
//     //    {

//     //    }
       
// }
#endif
