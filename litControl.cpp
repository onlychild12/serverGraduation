#include"include/litControl.h"
#include<iostream>
#include<unistd.h>
#include<stdlib.h>
#include"include/Score.h"
#include"include/Litsql.h"
#include<stdlib.h>
#include"include/FileEngine.h"
#include"include/afx_mysignal.h"
// #include<fstream>
// std::ofstream ofile("a.txt");
pthread_mutex_t mutex_school;
pthread_mutex_t mutex_score;
pthread_mutex_t mutex_sql_score;
pthread_mutex_t mutex_sql_school;
void litControl::init()//初始化犹如通信包大小，响应等级等设置
{
  pthread_mutex_init(&mutex_school,NULL);
  pthread_mutex_init(&mutex_score,NULL);
    pthread_mutex_init(&mutex_sql_school,NULL);
  pthread_mutex_init(&mutex_sql_score,NULL);
  for(int i=0;i<2;i++)
  {
    int tmp_time=0;
 
  typetime.push_back(tmp_time);
  }

  SetHighclass(0);
  setPacksize(500);
  settarget(1);
}
 void litControl::Resencodechapterlist(MYSQL_RES* res,const char *chaptername)
 {
      MYSQL_ROW row;
    while(row=mysql_fetch_row(res))
    {
    novelchapter *si=new novelchapter;
    //  strcpy(si->icon,FileEngine::read(row[0])->data);
     strcpy(si->chaptername,row[0]);
     strcpy(si->bookname,chaptername);
     si->chapterindex=atoi(row[1]);
     si->time=atol(row[2]);
     si->bytemoneys=atoi(row[3]);
     if(Litsql::scanchapterstate(si->chapterindex)==-1)
     {
      si->bytemoneys=-1;
     }

    //  si->update=atoi(row[5]);
  
    chapterlist_v.push_back(si);
    }
mysql_free_result(res);
 }
  void litControl::Resencodecatogrylittle(MYSQL_RES*res)
  {
     MYSQL_ROW row;
    while(row=mysql_fetch_row(res))
    {
    requestcatogrybook *rcb=new requestcatogrybook;
    //  strcpy(si->icon,FileEngine::read(row[0])->data);
    strcpy(rcb->sex,row[1]);
    strcpy(rcb->name,row[0]);
    catogrylittlelist.push_back(rcb);
    //  si->update=atoi(row[5]);
  
   
    }
  }
// void litControl::Resencode(MYSQL_ROW row,littlepakage *lp)
// {

//   strcpy(lp->Schoolname,row[1]);
//         // std::cout<<lp->Schoolname;
//       lp->year=atoi(row[2]);
//       lp->max=atoi(row[3]);
//       lp->average=atoi(row[4]);
//       lp->min=atoi(row[5]);
//       lp->rank=atoi(row[6]);
//       lp->passscore=atoi(row[7]);
//       strcpy(lp->type,row[8]);
//       strcpy(lp->batch,row[9]);
//       strcpy(lp->subject,row[10]);

//       if(row[11]==NULL)
//        lp->dvalue=-1;
//       else
//       lp->dvalue=atoi(row[11]);
//       strcpy(lp->region,row[12]);
//       strcpy(lp->site,row[13]);
      
// }
Reader_Imformation* litControl::ResencodeReaderImfromation(MYSQL_RES*res){
 MYSQL_ROW row;
 Reader_Imformation *ri=new Reader_Imformation;
while(row=mysql_fetch_row(res))
{
  read_file_data*rfd=FileEngine::read(row[1]);
  //  memcpy(ri->icon,rfd->data,rfd->size);
  if(rfd!=NULL)
  ri->iconsize=rfd->size;
  else 
  ri->iconsize=0;
  strcpy(ri->name,row[2]);
  ri->bytes=atoi(row[4]);
  ri->monthticket=atoi(row[5]);
  ri->recommendticket=atoi(row[6]);
  // // ri->recommendticket=atoi(row[6]);   
  // reader_imformation.push_back(ri);
  if(rfd!=NULL)   
delete rfd;
}
mysql_free_result(res);
return ri;
}

void litControl::ResencodeShelfImformation(MYSQL_RES*res)
{
    
    MYSQL_ROW row;
    if(res!=NULL)
    while(row=mysql_fetch_row(res))
    {
     Shelf_Imformation *si=new Shelf_Imformation();
    //  strcpy(si->icon,FileEngine::read(row[0])->data);
     strcpy(si->state,row[2]);
     strcpy(si->bookname,row[0]);
     strcpy(si->writername,row[3]);
     si->wordcount=atoi(row[1]);
     si->update=atol(row[4]);
     strcpy(si->chaptername,row[5]);
    //  si->update=atoi(row[5]);
     shelf_imformation.push_back(si);
    }
mysql_free_result(res);
}
    void litControl::ResencodeTicket(MYSQL_RES* res)
    {

         
    }
    void litControl::ResencodeSort(MYSQL_RES*res)
    {
      MYSQL_ROW row;
      while(row=mysql_fetch_row(res))
      {
       
        Sort *s=new Sort();
        strcpy(s->title,row[0]);
        strcpy(s->title_parent,row[2]);
  
        sort.push_back(s);
      }
           
       mysql_free_result(res);
    }
    void litControl::ResencodeRank(MYSQL_RES* res,int flag){
         MYSQL_ROW row;
      while(row=mysql_fetch_row(res))
      {
       Rank *r=new Rank();
       read_file_data *rdf=FileEngine::read(row[0]);
       if(flag==RECOMMENDTICKET) 
        strcpy(r->flag,"推荐榜");
        else if(flag==MONTHTICKET)
        strcpy(r->flag,"月票榜");
        // memcpy(r->icon,rdf->data,rdf->size);
  
        strcpy(r->writername,row[2]);
        strcpy(r->name,row[1]);
        r->wordcount=atoi(row[3]);
        strcpy(r->sort,row[4]);
        r->monthcount=atoi(row[5]);
        r->recommendcount=atoi(row[6]);
        
        rank.push_back(r);
      }
           
       mysql_free_result(res);
    }

    void litControl::Resencodebooklist(MYSQL_RES*res)
    {
          MYSQL_ROW row;
      while(row=mysql_fetch_row(res))
      {
        // read_file_data *rfd=FileEngine::read(row[2]);
        BookList *b=new BookList;
        // memcpy(b->icon,rfd->data,rfd->size);
        strcpy(b->bookname,row[0]);
        b->wordcount=atoi(row[1]);
        strcpy(b->writername,row[5]);
        strcpy(b->novelless,row[4]);
        strcpy(b->state,row[3]);
        // delete rfd;
        booklist.push_back(b);
      }
           
       mysql_free_result(res);
    }
     void litControl::ResencodeWriterbooklist(MYSQL_RES*res)
     {
         MYSQL_ROW row;
      while(row=mysql_fetch_row(res))
      {
        // read_file_data *rfd=FileEngine::read(row[2]);
       Book_Imformation *b=new Book_Imformation;
        // memcpy(b->icon,rfd->data,rfd->size);
        strcpy(b->bookname,row[0]);
        b->wordcount=atoi(row[1]);
       b->update=atol(row[2]);
       strcpy(b->state,row[3]);
       strcpy(b->sort,row[4]);
        std::cout<<b->sort;
        strcpy(b->noveless,row[5]);
        strcpy(b->sex,row[6]);
        
        // delete rfd;
        writerbklist.push_back(b);
      }
           
       mysql_free_result(res);
     }
    Book_Imformation* litControl::Resencodebookimformation(MYSQL_RES*res)
    {
          MYSQL_ROW row;
          Book_Imformation *bi=new Book_Imformation;
      while(row=mysql_fetch_row(res))
      {
        strcpy(bi->bookname,row[1]);
        bi->wordcount=atoi(row[2]);
        strcpy(bi->writername,row[3]);
        bi->update=atol(row[5]);
      strcpy(bi->state,row[6]);
      strcpy(bi->sort,row[7]);
      bi->recommend_ticket=atoi(row[8]);
      bi->month_ticket=atoi(row[9]);
    strcpy(bi->noveless,row[10]);
      }
      return bi;
    }
    void litControl::ResencodeFanlist(MYSQL_RES*res)
    {
                MYSQL_ROW row;
      while(row=mysql_fetch_row(res))
      {
        fanlist *fl=new fanlist;
       strcpy(fl->name,row[0]);
       fl->fanvalue=atoi(row[1]);
       fl->state=atoi(row[2]);
       fanlist_v.push_back(fl);
      
     
      }
           
       mysql_free_result(res);
    }
    void litControl::Resencodeupdate_fan(MYSQL_RES* res){
          MYSQL_ROW row;
      while(row=mysql_fetch_row(res))
      {
        Sort *s=new Sort();
    
        strcpy(s->title,row[1]);
        strcpy(s->title_parent,row[2]);
        sort.push_back(s);
      }
           
       mysql_free_result(res);
    }
    void litControl::ResencodeHistoryres(MYSQL_RES*res)
    {
      MYSQL_ROW row;
      while(row=mysql_fetch_row(res))
      {
         History_res *hrs=new History_res;
         hrs->time=atol(row[0]);
         hrs->money=atoi(row[1]);
         history.push_back(hrs);
      }
      mysql_free_result(res);
    }
     void litControl::Resencoderecommendlist(MYSQL_RES* res)
     {
           MYSQL_ROW row;
           writerbklist.clear();
      while(row=mysql_fetch_row(res))
      {
       int cid=atoi(row[0]);
      MYSQL_RES *resc= Litsql::getcatogrybookthree(cid);
      MYSQL_ROW rowc;

      while(rowc=mysql_fetch_row(resc))
      {
        Book_Imformation *bi=new Book_Imformation;
        strcpy(bi->bookname,rowc[0]);
        bi->wordcount=atoi(rowc[1]);
        bi->update=atol(rowc[2]);
      strcpy(bi->state,rowc[3]);
       strcpy(bi->noveless,rowc[4]);
      strcpy(bi->sort,rowc[5]);
        bi->month_ticket=atoi(rowc[6]);
      bi->recommend_ticket=atoi(rowc[7]);

      strcpy(bi->writername,rowc[8]);
      strcpy(bi->sex,rowc[9]);
       writerbklist.push_back(bi);
   
      }
      mysql_free_result(resc);
      }
      mysql_free_result(res);
  

     }
     void litControl::Resencodebooklist_n(MYSQL_RES*res)
    {
          MYSQL_ROW row;
      while(row=mysql_fetch_row(res))
      {
        // read_file_data *rfd=FileEngine::read(row[2]);
        BookList *b=new BookList;
        // memcpy(b->icon,rfd->data,rfd->size);
        strcpy(b->bookname,row[0]);
        b->wordcount=atoi(row[1]);
        strcpy(b->writername,row[5]);
        strcpy(b->novelless,row[4]);
        strcpy(b->state,row[3]);
        strcpy(b->type,row[6]);
        // delete rfd;
        booklist.push_back(b);
      }
           
       mysql_free_result(res);
    }
    // void litControl::Resencodecontent(MYSQL_RES *res){
    //       MYSQL_ROW row;
    //   while(row=mysql_fetch_row(res))
    //   {
  
    //     strcpy(s->icon,FileEngine::read(row[0]));
    //     strcpy(s->title,row[1]);
    //     strcpy(s->title_parent,row[2]);
    //     sort.push_back(s);
    //   }
           
    //    mysql_free_result(res);
    // }
void litControl::Deal(char*data,int signal)//请解码
{
  littlepakage *child_l=new littlepakage;
  switch(signal)
  {
    case USERLOGIN:
    {
     requestLogin *re_t=new requestLogin();
   
     memcpy(re_t,data,sizeof(requestLogin));
     std::cout<<re_t->password<<endl;
     std::cout<<re_t->account;
      child_l->type=SIGNAL;
      child_l->size=sizeof(int);
     if(Litsql::Login(re_t->account,re_t->password,USERLOGIN))
     {
       username=new char[9];
       strcpy(username,re_t->account);
       child_l->content=(void *)LOGINSUCESSFULRES;
     }
    else{
      
       child_l->content=(void *)LOGINFAILURERES;
 
    }
     child_l->size=sizeof(int);
      LitSend(child_l);
    delete re_t;
      break;
    }
  case REGISTER:
    {
    requestRegister *rg=new requestRegister;
    memcpy(rg,data,sizeof(requestRegister));
    child_l->type=SIGNAL;
    if(Litsql::Register(rg->username,rg->password," ",rg->id_office))
    {
      if(username==NULL)
      {
         username=new char[9];
      }
      else 
      {
        delete[] username;
         username=new char[9];
      }
      child_l->content=(void *)REGISTERSUCESSFUL;
    strcpy(username,rg->username);
    }
    else
    {
      child_l->content=(void *)REGISTERFAILURE;
    }
     child_l->size=sizeof(int);
      LitSend(child_l);
      delete rg;
      break;
    }
  case PERSONIMFORMATION:
  {
      if(username!=NULL)
      { Reader_Imformation *ri=Litsql::getReader(username)==NULL?NULL:ResencodeReaderImfromation(Litsql::getReader(username));
   if(ri!=NULL)
   {
     
     child_l->content=(void*)PERSONIMFROMATIONRES;
      child_l->size=sizeof(int);
      child_l->type=SIGNAL;
      LitSend(child_l);
      littlepakage *child_content=new littlepakage;  
      child_content->content=(void *)ri;
      child_content->size=sizeof(Reader_Imformation);
      child_content->type=PERSONIMFROMATIONRES;
      LitSend(child_content);
   }
      }
       littlepakage *lps=new littlepakage;
      lps->type=SIGNAL;
      lps->size=4;
      lps->content=(void*)END;
      LitSend(lps);

      break;
    }
  case MODEFYIMFOEMATION:
   {     
        Reader_Imformation *imformation=new Reader_Imformation;
  
    memcpy(imformation,data,sizeof(Reader_Imformation));
      char *data_icon=new char[imformation->iconsize];
      // memcpy(data_icon,imformation->icon,imformation->iconsize);
      if(Litsql::updateReader(data_icon,username,imformation->iconsize));
      {
        child_l->type=SIGNAL;
        child_l->size=sizeof(int);
        child_l->content=(void *)MODEFYSUCCESSFUL;
        LitSend(child_l);
      }
      delete imformation;
      delete[] data_icon;
          break;
   }
  case REGISTERWRITER:
  {
     requestRegister *register_my=new requestRegister;
     memcpy(register_my,data,sizeof(requestRegister));
       child_l->type=SIGNAL;
       child_l->size=sizeof(int);
     if(Litsql::RegisterWriter(register_my->username,register_my->password,register_my->id_office))
     {
        username=new char[9];
       logintype=1;
       strcpy(username,register_my->username);
       child_l->content=(void*)REGISTERSUCESSFUL;
     }
     else{
       child_l->content=(void*)REGISTERFAILURE;
     }
     LitSend(child_l);
     break;
   }
  case WRITERLOGIN:
  {
     requestLogin *re_t=new requestLogin();
     memcpy(re_t,data,sizeof(requestLogin));
     std::cout<<re_t->password<<endl;
     std::cout<<re_t->account;
      child_l->type=SIGNAL;
      child_l->size=sizeof(int);
     if(Litsql::Login(re_t->account,re_t->password,WRITERLOGIN))
     {
       username=new char[9];
       logintype=1;
       strcpy(username,re_t->account);
       child_l->content=(void *)LOGINSUCESSFULRES;
     }
    else{
      
       child_l->content=(void *)LOGINFAILURERES;
 
    }
     child_l->size=sizeof(int);
      LitSend(child_l);
    delete re_t;
  break;
  }
  case CREATESORT:{
   requestcreatesort *rcs=new requestcreatesort;
   memcpy(rcs,data,sizeof(requestcreatesort));
    child_l->type=SIGNAL;
      child_l->size=sizeof(int);
   if(Litsql::CreateCatogry(rcs->name,rcs->sex,rcs->iconsize))
   {
    child_l->content=(void*)CREATESORTSUCCESSFUL;
   }
   else
   {child_l->content=(void *)CREATESORTSUCCESSFUL; }
    LitSend(child_l);
    delete rcs;
    break;
  }
  case CREATEBOOK:{
   requestcreatebook *rcb=new requestcreatebook;
   memcpy(rcb,data,sizeof(requestcreatebook));
   child_l->type=SIGNAL;
  child_l->size=sizeof(int);
   if(Litsql::insertintobook(rcb->bookname,username,rcb->catogory,rcb->sex,rcb->iconsize,rcb->novelless))
{
  child_l->content=(void*)CREATEBOOKSUCCESSFUL;
}
else 
child_l->content=(void*)CREATEBOOKFAILURE;
LitSend(child_l);
break;
  }
  case SORT:{
    MYSQL_RES *res= Litsql::getCatogry();
    child_l->type=SIGNAL;
    child_l->size=4;
    if(res!=NULL)
    {
       ResencodeSort(res);
       child_l->content=(void*)SORTRES;
      while(sort.size()!=0)
      {
         
         littlepakage *lp=new littlepakage;
         lp->size=sizeof(Sort);
         lp->type=SORTRES;
         Sort *sort_tttmp_l=new Sort;
         *sort_tttmp_l=*sort[0];
         lp->content=(void*)sort_tttmp_l;
        littlepakage *child_l_t=new littlepakage;
        *child_l_t=*child_l; 
          LitSend(child_l_t);
          LitSend(lp);
        sort.erase(sort.begin());
         
      }
    
    }
    else{
   child_l->content=(void*)SORTRESFAILURE;
   
    }
    LitSendSignal(END);
  
    
    break;
  }
  case CATOGRYLIST:{
    child_l->size=4;
    child_l->type=SIGNAL;
    child_l->content=(void*)CATOGRYBOOKLISTRES;
    requestcatogrybook rcb;
    memcpy(&rcb,data,sizeof(requestcatogrybook));

    MYSQL_RES *res=Litsql::getCatogrybooklist(rcb.name,rcb.sex,rcb.set);
    if(res!=NULL)
    {
       Resencodebooklist(res);
       for(;booklist.size()>0;)
       {
         littlepakage *tmp=new littlepakage;
         *tmp=*child_l;
         LitSend(tmp);
         littlepakage *content=new littlepakage;
         content->size=sizeof(BookList);
         strcpy(booklist[0]->type,rcb.name);
         content->content=(void*)booklist[0];
         LitSend(content);
         booklist.erase(booklist.begin());
       }
    }
    LitSendSignal(END);
  }
  case RANK:{
     MYSQL_RES *res= Litsql::getbookrank(MONTHTICKET);
     ResencodeRank(res,MONTHTICKET);
     res=Litsql::getbookrank(RECOMMENDTICKET);
     ResencodeRank(res,RECOMMENDTICKET);
     while(rank.size()>0)
     {
       littlepakage *lp=new littlepakage;
       littlepakage *lpd=new littlepakage;
       lp->size=4;
       lp->type=SIGNAL;
       lp->content=(void*)RANKLISTRES;
       lpd->content=(void*)rank[0];
       rank.erase(rank.begin());
       lpd->size=sizeof(Rank);
       lpd->type=RANKLISTRES;
       LitSend(lp);
       LitSend(lpd);
     }

    break;
  }
  case GETBYTES:{
    requestaddbytes rdb;
    memcpy(&rdb,data,sizeof(requestaddbytes));
    Litsql::addbytes(username,rdb.money);
    littlepakage *lp=new littlepakage;
     littlepakage *lpt=new littlepakage;
  lpt->type=PERSONIMFROMATIONRES;
  lpt->size=sizeof(Reader_Imformation);

  lpt->content=(void*)ResencodeReaderImfromation(Litsql::getReader(username));
   lp->type=SIGNAL;
   lp->content=(void*)PERSONIMFROMATIONRES;
   lp->size=4;
   LitSend(lp);
   LitSend(lpt);
   break;

  }
  case BOOKIMFROMATION:{
    requestbookimformation *rbif=new requestbookimformation;
    memcpy(rbif,data,sizeof(requestbookimformation));
     Book_Imformation *bi=Resencodebookimformation(Litsql::getBOOKimformation(rbif->name));
     bi->addshelf=Litsql::getbookshelfstate(username,rbif->name);
     littlepakage *lp=new littlepakage;
     littlepakage *lpt=new littlepakage;
     lp->type=SIGNAL;
     lp->size=sizeof(int);
     lp->content=(void*)BOOKIMFORMATIONRES;
     lpt->size=sizeof(Book_Imformation);
     lpt->type=BOOKIMFORMATIONRES;
     lpt->content=(void*)bi;
     LitSend(lp);
     LitSend(lpt);
     LitSendSignal(END);
     break;
  }
  case ADDSHELF:{
    requestbookimformation *rbif=new requestbookimformation;
    memcpy(rbif,data,sizeof(requestbookimformation));
    littlepakage *lp=new littlepakage;
    lp->size=4;
    lp->type=SIGNAL;
    if(Litsql::addshelf(username,rbif->name))
    {
      lp->content=(void*)ADDSHELFSUCCESSFUL;
    }
    else {
      lp->content=(void*)ADDSHELFFAILURE;
    }
    LitSend(lp);
    break;
  }
  case SHELFIMFOEMATION:{
    
    ResencodeShelfImformation(Litsql::getshelf(username));
    while(shelf_imformation.size()>0)
    {
      littlepakage *lps=new littlepakage;
      littlepakage*lpc=new littlepakage;
      lps->type=SIGNAL;
      lps->size=4;
      lps->content=(void*)SHELFIMFORMATIONRES;
      lpc->type=SHELFIMFORMATIONRES;
      lpc->size=sizeof(Shelf_Imformation);
      lpc->content=(void*)shelf_imformation[0];
      LitSend(lps);
      LitSend(lpc);
      shelf_imformation.erase(shelf_imformation.begin());
    }
     littlepakage *lps=new littlepakage;
      lps->type=SIGNAL;
      lps->size=4;
      lps->content=(void*)END;
      LitSend(lps);
    break;
  }
  case GIVEMONEY:{
    Action_Tobytes *at=new Action_Tobytes;
    memcpy(at,data,sizeof(Action_Tobytes));
     littlepakage *lp=new littlepakage;
      lp->size=4;
      lp->type=SIGNAL;
    if(Litsql::givenovelmoney(username,at->novelname,at->bytes))
    {
     
     lp->content=(void*)GIVEMONEYSUCCESSFUL;
     Litsql::addfanvalue(username,at->novelname,at->bytes);
    }
    else 
       {
         lp->content=(void*)GIVEMONEYFAILURE;
       }
    
      LitSend(lp);
      Reader_Imformation *ri=Litsql::getReader(username)==NULL?NULL:ResencodeReaderImfromation(Litsql::getReader(username));
   if(ri!=NULL)
   {
     littlepakage *child_lo=new littlepakage;
     child_lo->content=(void*)PERSONIMFROMATIONRES;
      child_lo->size=sizeof(int);
      child_lo->type=SIGNAL;
      LitSend(child_lo);
      littlepakage *child_content=new littlepakage;  
      child_content->content=(void *)ri;
      child_content->size=sizeof(Reader_Imformation);
      child_content->type=PERSONIMFROMATIONRES;
      LitSend(child_content);
   }
   break;
  }
  case RECOMMENDTICKET:{
        Action_Tobytes *at=new Action_Tobytes;
    memcpy(at,data,sizeof(Action_Tobytes));
     littlepakage *lp=new littlepakage;
      lp->size=4;
      lp->type=SIGNAL;
    if(Litsql::torecommendticket(username,at->novelname,at->bytes))
    {
     
     lp->content=(void*)RECOMMENDTICKETSUCCESSFUL;

    }
    else 
       {
         lp->content=(void*)RECOMMENDTICKETFAILURE;
       }
    
      LitSend(lp);
      Reader_Imformation *ri=Litsql::getReader(username)==NULL?NULL:ResencodeReaderImfromation(Litsql::getReader(username));
   if(ri!=NULL)
   {
     littlepakage *child_lo=new littlepakage;
     child_lo->content=(void*)PERSONIMFROMATIONRES;
      child_lo->size=sizeof(int);
      child_lo->type=SIGNAL;
      LitSend(child_lo);
      littlepakage *child_content=new littlepakage;  
      child_content->content=(void *)ri;
      child_content->size=sizeof(Reader_Imformation);
      child_content->type=PERSONIMFROMATIONRES;
      LitSend(child_content);
   }
   break;
  }
  case MONTHTICKET:{
    Action_Tobytes *acb=new Action_Tobytes;
    littlepakage *lp=new littlepakage;
    memcpy(acb,data,sizeof(Action_Tobytes));
   if(Litsql::toticket(username,acb->novelname,acb->bytes))
   {
    lp->size=4;
    lp->content=(void*)MONTHTICKETSUCCESSFUL;
    lp->type=SIGNAL;
    LitSend(lp);  
    
   }
   else 
   {
     lp->size=4;
      lp->content=(void*)MONTHTICKETFAILURE;
    lp->type=SIGNAL;
    LitSend(lp);  
   }
         Reader_Imformation *ri=Litsql::getReader(username)==NULL?NULL:ResencodeReaderImfromation(Litsql::getReader(username));
   if(ri!=NULL)
   {
     littlepakage *child_lo=new littlepakage;
     child_lo->content=(void*)PERSONIMFROMATIONRES;
      child_lo->size=sizeof(int);
      child_lo->type=SIGNAL;
      LitSend(child_lo);
      littlepakage *child_content=new littlepakage;  
      child_content->content=(void *)ri;
      child_content->size=sizeof(Reader_Imformation);
      child_content->type=PERSONIMFROMATIONRES;
      LitSend(child_content);
   }
 
   
    break;
    
  }
  case CHAPTERINSERT:{
   Content content;
   memcpy(&content,data,sizeof(Content));
    littlepakage *lp=new littlepakage;
    lp->size=4;
    lp->type=SIGNAL;
  
   if(Litsql::insertintochapter(content.bookname,content.chaptername,content.content))
   {
    lp->content=(void*)CHAPTERINSERTSUCCESSFUL;
   }
   else {
   lp->content=(void*)CHAPTERFAILURE;
   }
   LitSend(lp);
    break;
  }
  case CHAPTERINDEX:{
    requestnovelchapter rnc;
    memcpy(&rnc,data,sizeof(requestnovelchapter));
    MYSQL_RES*res= Litsql::getchapterlist(rnc.bookname);

    littlepakage *lp=new littlepakage;
    lp->size=4;
    lp->type=SIGNAL;
    if(res!=NULL)
    {
      Resencodechapterlist(res,rnc.bookname);
     for(;chapterlist_v.size()>0;)
     { 
       littlepakage *lpp=new littlepakage;
        littlepakage *lpt=new littlepakage;
        lpp->type=SIGNAL;
        lpp->size=4;
        lpp->content=(void*)CHAPTERLISTRES;
        LitSend(lpp);

        lpt->size=sizeof(novelchapter);
        lpt->type=CHAPTERLISTRES;
        lpt->content=(void*)chapterlist_v[0];
        LitSend(lpt);
        chapterlist_v.erase(chapterlist_v.begin());

     }
     LitSendSignal(CHAPTERINDEXEND);
    }
    LitSendSignal(FAILURE);
    
    break;
  }
  case CHAPTER:{
    requestchapter rc;
    memcpy(&rc,data,sizeof(requestchapter));
    Content *c=Litsql::getChapter(rc.bookname,rc.novelindex,rc.setnext);
   if(c==NULL)
    {
       c=new Content;
       c->index=-1;//标志最后一页
    }
     littlepakage *lp=new littlepakage;
    littlepakage *lpt=new littlepakage;
    lp->type=SIGNAL;
    lp->size=4;
    lp->content=(void*)CHAPTERRES;
    lpt->content=(void*)c;
    lpt->size=sizeof(Content);
    lpt->type=CHAPTERRES;
    LitSend(lp);
    LitSend(lpt);

    break;
  }
  case HISTORYMONEY:{
    MYSQL_RES *res=Litsql::getmoneyhistory(username);
    ResencodeHistoryres(res);
    for(;history.size()>0;)
    {
       littlepakage *lp=new littlepakage;
        littlepakage *lpt=new littlepakage;
        lp->content=(void*)HISTORYMONEYRES;
        lp->size=4;
        lp->type=SIGNAL;
        lpt->content=(void*)history[0];
        lpt->size=sizeof(History_res);
        lpt->type=HISTORYMONEYRES;
        history.erase(history.begin());
        LitSend(lp);
        LitSend(lpt);
    }
     littlepakage *lp=new littlepakage;
     lp->type=SIGNAL;
     lp->size=1;
     lp->content=(void*)HISTORYMONEYEND;
     LitSend(lp);
    break;
  }
  case BUYCHAPTER:{
   requestbuychapter rbc;
   memcpy(&rbc,data,sizeof(requestbuychapter));
    littlepakage *lp=new littlepakage;
      lp->type=SIGNAL;
     lp->size=4;
   if(Litsql::buychapter(username,rbc.chapterindex))
   {
    
     lp->content=(void*)BUYCHATERSUCCESSFUL;
   
   }
   else 
   {
     lp->content=(void*)BUYCHAPTERFAILURE;
   }
   LitSend(lp);
   break;
  }
  case WRITERIMF:{
    if(logintype==1)
    {
        ResencodeWriterbooklist(Litsql::getauthorbooklist(username));
            for(;writerbklist.size()>0;)
    {
    LitSendrrl(writerbklist[0],WRITERIMFLISTRES);
    writerbklist.erase(writerbklist.begin());
    }
 
    }
    else if(logintype==0)
    {
  requestwriterimformation rwi;
  memcpy(&rwi,data,sizeof(requestwriterimformation));
  Writer_Imformation wi=Litsql::getAuthor(rwi.writername);
  if(wi.bookcount==-1)
  {
     littlepakage *lp=new littlepakage;
     lp->content=(void*)WRITERIMFRESFAILURE;
     lp->size=4;
     lp->type=SIGNAL;
     LitSend(lp);
  }
  else
  {
    littlepakage *lp=new littlepakage;
    littlepakage *lpt=new littlepakage;
    Writer_Imformation *wio=new Writer_Imformation;
    *wio=wi;
    lp->content=(void*)WRITERIMFRES;
    lp->type=SIGNAL;
    lp->size=4;
    lpt->content=(void*)wio;
    lpt->size=sizeof(Writer_Imformation);
    lpt->type=WRITERIMFRES;
    LitSend(lp);
    LitSend(lpt);
    ResencodeWriterbooklist(Litsql::getauthorbooklist(rwi.writername));
    for(;writerbklist.size()>0;)
    {
      littlepakage *lp=new littlepakage;
    littlepakage *lpt=new littlepakage;
    *wio=wi;
    lp->content=(void*)WRITERIMFLISTRES;
    lp->type=SIGNAL;
    lp->size=4;
    lpt->content=(void*)writerbklist[0];
    lpt->size=sizeof(Book_Imformation);
    lpt->type=WRITERIMFLISTRES;
    LitSend(lp);
    LitSend(lpt);
    writerbklist.erase(writerbklist.begin());
    }
  }

    }
       littlepakage *lp=new littlepakage;
     lp->content=(void*)WRITERIMFRESEND;
     lp->size=1;
     lp->type=SIGNAL;
     LitSend(lp);
  break;
  }
  case FANLIST:{
   requestnovelchapter rnc;
   memcpy(&rnc,data,sizeof(requestnovelchapter));
   MYSQL_RES *res=Litsql::getfanlist(rnc.bookname);
   if(res!=NULL)
   {
    ResencodeFanlist(res);
    for(;fanlist_v.size()>0;)
    {
    littlepakage *lp=new littlepakage;
    littlepakage *lpt=new littlepakage;
    lp->content=(void*)FANLISTRES;
    lp->type=SIGNAL;
    lp->size=4;
    lpt->content=(void*)fanlist_v[0];
    lpt->size=sizeof(fanlist);
    lpt->type=FANLISTRES;
    LitSend(lp);
    LitSend(lpt);
     fanlist_v.erase(fanlist_v.begin());
    }
      littlepakage *lp=new littlepakage;
    lp->content=(void*)FANLISTRESEND;
    lp->type=SIGNAL;
    lp->size=4;
    LitSend(lp);
   }
   else 
   {
       littlepakage *lp=new littlepakage;
    lp->content=(void*)FAILURE;
    lp->type=SIGNAL;
    lp->size=4;
    LitSend(lp);
   }
break;
  }
 case GETPERSONFAN:{
   requestnovelchapter rnc;
   memcpy(&rnc,data,sizeof(requestnovelchapter));
   fanlist *f=Litsql::getpersonvalue(username,rnc.bookname);
   littlepakage *lp=new littlepakage;
    littlepakage *lpt=new littlepakage;
    lp->content=(void*)GETPERSONFANRES;
    lp->type=SIGNAL;
    lp->size=1;
    lpt->content=(void*)f;
    lpt->size=sizeof(fanlist);
    lpt->type=GETPERSONFANRES;
    LitSend(lp);
    LitSend(lpt);
   break;
 }
  case DELETESHELF:{
    requestbookimformation rbi;
    memcpy(&rbi,data,sizeof(requestbookimformation));
     littlepakage *lp=new littlepakage;
     lp->type=SIGNAL;
     lp->size=4;
    if(Litsql::deleteshelf(username,rbi.name))
    {
     lp->content=(void*)SHELFDELETERES;
    }
    else 
    {
      lp->content=(void*)FAILURE;
    }
  LitSend(lp);
  break;
  }
 case READSTATE:{
   requestbookimformation rbi;
   memcpy(&rbi,data,sizeof(requestbookimformation));
   Content *c= Litsql::getreadstate(username,rbi.name);
   if(c!=NULL)
   {
     LitSendrrl(c,CHAPTERRES);
   }
   else
   {
    LitSendSignal(FAILURE);
   }
   break;
 }
 case READSTATESAVE:{
   requestchapter rc;
   memcpy(&rc,data,sizeof(requestchapter));
   if(Litsql::savereadstate(username,rc.novelindex))
   {
     LitSendSignal(READSTATESAVESUCCESSFUL);
   }
   else{
    LitSendSignal(FAILURE);
   }
    break;
 }
 case CATOGRYLITTLE:{
   MYSQL_RES*res= Litsql::getcatogrylittle();
   if(res==NULL)
   {
     LitSendSignal(FAILURE);
   }
   else{
     Resencodecatogrylittle(res);
    for(;catogrylittlelist.size()>0;)
    {
       LitSendrrl(catogrylittlelist[0],CATOGRYLITTLERES);
       catogrylittlelist.erase(catogrylittlelist.begin());
    }
    LitSendSignal(CATOGRYLITTLEEND);
   }
   break;
 }
 case CATOGRYLIKE:{
   requestcatogrybook rcb;
   memcpy(&rcb,data,sizeof(requestcatogrybook));

  if(Litsql::addlikecatogry(rcb.sex,rcb.name,username))
  {
    std::cout<<"成功添加按钮";
    LitSendSignal(CATOGRYLIKESUCCESSFF);
  }

  else
    LitSendSignal(FAILURE);
  break;
 }
 case DELETECATOGRYLIKE:{
   if(!Litsql::deletelikecatogry(username))
   {
   LitSendSignal(FAILURE);
   }
   else 
   {
     LitSendSignal(DELETECATOGRYSUCCESSFULL);
   }
   break;
 }
 case GETRECOMMENDBOOKLIST:{
  MYSQL_RES*res= Litsql::getcatogryid(username);
  if(res==NULL)
  {
    LitSendSignal(FAILURE);
  }
  else
  {
    Resencoderecommendlist(res);
    for(;writerbklist.size()>0;)
    {
      LitSendrrl(writerbklist[0],BOOKRECOMMENDLISTRES);
      writerbklist.erase(writerbklist.begin());
    }
    LitSendSignal(BOOKRECOMMENDLISTEND);
  }
  break;
  }
  case GETENDBOOKLIST:{
    MYSQL_RES*res=Litsql::getendbooklist();
    if(res!=NULL)
    {
         Resencodebooklist_n(res);
       for(;booklist.size()>0;)
       {
      
         LitSendrrl(booklist[0],CATOGRYBOOKLISTRES);
         booklist.erase(booklist.begin());
       }
    }
    LitSendSignal(END);
    break;
  }
  case SHELFINVENTORY:{
    SHELFinventory si;
    memcpy(&si,data,sizeof(SHELFinventory));
    if(si.tyflag!=CHAPTER_ENABAL)
{
     if(Litsql::shelfinventory(si.name,si.tyflag,si.sfflag))
    {
      LitSendSignal(SHELFINVENTORYSUCCESSFUL);
    }
    else 
    LitSendSignal(FAILURE);
}
else {

     if(Litsql::enablechapter(si.chapterindex,si.sfflag))
    {
      LitSendSignal(ENABLECHAPTERSUCCESSFUL);
    }
    else 
    LitSendSignal(FAILURE);

}
   break;
  }

  case GETFREEBOOKLIST:{
    MYSQL_RES*res=Litsql::getfreebooklist();
    if(res!=NULL)
    {
         Resencodebooklist_n(res);
       for(;booklist.size()>0;)
       {
      
         LitSendrrl(booklist[0],CATOGRYBOOKLISTRES);
         booklist.erase(booklist.begin());
       }
    }
    LitSendSignal(END);
    break;

  }
  case SEARCH:{
   requestnovelchapter rnc;
   memcpy(&rnc,data,sizeof(requestnovelchapter));
     MYSQL_RES*res=Litsql::search(rnc.bookname);
    if(res!=NULL)
    {
         Resencodebooklist_n(res);
       for(;booklist.size()>0;)
       {
      
         LitSendrrl(booklist[0],CATOGRYBOOKLISTRES);
         booklist.erase(booklist.begin());
       }
    }
    LitSendSignal(END);
    break;

    break;
  }
  case UPDATENOVEL:{
    requestcreatebook rcb;
    memcpy(&rcb,data,sizeof(requestcreatebook));
    if(Litsql::updatenovelimformation(rcb.bookname,rcb.novelless,rcb.catogory,rcb.sex,rcb.state))
    LitSendSignal(UPDATENOVELSUCCESSFUL);
    else 
    LitSendSignal(FAILURE);
    break;
  }
  case DELETENOVEL:{
    requestnovelchapter rnc;
    memcpy(&rnc,data,sizeof(requestnovelchapter));
    if(Litsql::deletenovelbook(rnc.bookname))
    {
    LitSendSignal(DELETENOVELSUCCESSFUL);

    }
    else 
    {
      LitSendSignal(FAILURE);
    }
    break;
  }
  case DELETECHAPTER:{
    requestbuychapter rc;
    memcpy(&rc,data,sizeof(requestbuychapter));
  if(Litsql::deletechapter(rc.chapterindex))
  {
    LitSendSignal(DELETECHAPTERSUCCESSFUL);
  }
  else 
  LitSendSignal(FAILURE);

    break;
  }
  case UPDATECHAPTER:{
    Content c;
    int z=sizeof(Content);
    memcpy(&c,data,sizeof(Content));
   if(Litsql::updatechapterimfotmation(c.chaptername,c.content,c.index))
   {
     
    LitSendSignal(UPDATECHAPTERSUCCESSFUL);
   }
   else 
   {
     LitSendSignal(FAILURE);
   }
   break;
  }
  case GETREADHISTORY:{
    MYSQL_RES* res= Litsql::getreadhistory(username);
    ResencodeShelfImformation(res);
    if(res!=NULL)
    {
     for(;shelf_imformation.size()>0;)
     {
       LitSendrrl(shelf_imformation[0],READHISTORYRES);
       shelf_imformation.erase(shelf_imformation.begin());
     }
     LitSendSignal(READHISTORYRESEND);
    }
    else{
      LitSendSignal(FAILURE);
    }
    break;
  }
  default:break;
  }
   delete[] data;
  
  // bool result= Litsql::Login("test","12345678");
 // if(result)
  // {
  //   std::cout<<"测试登录成功";
  // }
  // std::cout<<data;
  
//  std::cout<<data;
//  sleep(1);
}
void litControl::LitSendSignal(int signal)
{
  littlepakage *lp=new littlepakage;
    lp->content=(void*)signal;
    lp->type=SIGNAL;
    lp->size=1;
    LitSend(lp);
}