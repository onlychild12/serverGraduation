#ifndef LITCONTROL_H
#define LITCONTROL_H
#include"Socket_m.h"
#include"Score.h"
#include<mysql.h>
#include"PacketFormat.h"
#include<vector>
using namespace std;
class litControl:public Socket_m
{

private:
 char *username=NULL;//保存用户名
 int logintype=0;
 vector<Reader_Imformation*>reader_imformation;
 vector<Shelf_Imformation*>shelf_imformation;
 vector<Recommend_Ticket*>recommend_ticket;
 vector<novelchapter*>chapterlist_v;
 vector<Sort*>sort;
 vector<Rank*>rank;
 vector<requestcatogrybook*>catogrylittlelist;
 vector<BookList*>booklist;
 vector<History_res*>history;
 vector<fanlist*>fanlist_v;
//  vector<Update_Fan*>update_fan;
 vector<Content*>content;
 vector<Book_Imformation*>writerbklist;
public:
    litControl(int socket_m):Socket_m(socket_m){};
    void LitSendSignal(int signal);
    template<class Data>
void LitSendrrl(Data *d,int signal);

    friend void *ThreadDeal(void*nptr);
    friend void *threaddeal_m(void *nptr);

    void init();//初始化用于设置优先级或者包大小
    void Deal(char*data,int signal);//处理数据和响应
    void Resencode(MYSQL_ROW,littlepakage*);
    Reader_Imformation* ResencodeReaderImfromation(MYSQL_RES*);
    Book_Imformation *Resencodebookimformation(MYSQL_RES*);
    void ResencodeShelfImformation(MYSQL_RES*);
    void ResencodeTicket(MYSQL_RES*);
    void ResencodeSort(MYSQL_RES*);
    void ResencodeRank(MYSQL_RES*,int flag);
    void Resencodebooklist(MYSQL_RES*);
    void Resencodeupdate_fan(MYSQL_RES*);
    void Resencodecontent(MYSQL_RES*);
    void Resencodechapterlist(MYSQL_RES*,const char *chaptername);
    void ResencodeHistoryres(MYSQL_RES*);
    void ResencodeWriterbooklist(MYSQL_RES*);
    void ResencodeFanlist(MYSQL_RES*);
    void Resencodecatogrylittle(MYSQL_RES*);
    void Resencoderecommendlist(MYSQL_RES*);
     void Resencodebooklist_n(MYSQL_RES*res);
  };
   template<class Data>
void litControl::LitSendrrl(Data *d,int signal)
  {
     littlepakage *lp=new littlepakage;
    littlepakage *lpt=new littlepakage;
    lp->content=(void*)signal;
    lp->type=SIGNAL;
    lp->size=1;
    lpt->content=(void*)d;
    lpt->size=sizeof(Data);
    lpt->type=signal;
    LitSend(lp);
    LitSend(lpt);
}
#endif