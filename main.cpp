#include<map>
#include"include/Score.h"
#include<iostream>
#include<string.h>
#include"include/Server.h"
#include"include/Litsql.h"
#include<stdlib.h>
int main()
{
  // Litsql::Register("廖耀","teststst","D:\\Graduation","456");
  // bool result= Litsql::Login("廖耀","teststst");
  // if(result)
  // {
  //   std::cout<<"测试登录成功";
  // }
  // else{

  //   std::cout<<"失败";
  // }
    // requesttrety t;67
    // std::cout<<sizeof(t);
  /*数据库测试
   Score score;
   score.rank=101327;
   score.score=476;
   strcpy(score.type_class,"理科");
   MYSQL_RES*res= Litsql::selectscore(score);
   MYSQL_ROW row;
    while(row=mysql_fetch_row(res))
    {
      littlepakage *lp=new littlepakage;
      if(row==NULL)
           break;
      strcpy(lp->Schoolname,row[1]);
      lp->year=atoi(row[2]);
      lp->max=atoi(row[3]);
      lp->average=atoi(row[4]);
      lp->min=atoi(row[5]);
      lp->rank=atoi(row[6]);
      lp->passscore=atoi(row[7]);
      strcpy(lp->type,row[8]);
      strcpy(lp->batch,row[9]);
      strcpy(lp->subject,row[10]);
      lp->dvalue=atoi(row[11]);
      strcpy(lp->region,row[12]);
      strcpy(lp->site,row[13]);
      std::cout<<lp->Schoolname;
    delete lp;
    }*/
    // // std::string a;
    // p="%d";
    // std::string buff;
    // char *t=new char[100];
    // sprintf(t,p,1);
    // buff=t;
    // buff+="%d";
    // strcpy(t,buff.c_str());
    //  p=new char[100];
    //  sprintf(p,t,2);
    //  std::cout<<p;
  
    // Litsql::Litquery("dssd",1,2,"m",1);
    // Litsql::connect();
    //套接字测试


  
  
    int a;
    
    Server server;

    server.init();
    std::cout<<"开始运行";
   
    std::cout<<sizeof(Sort);
     while(true)
    std::cin>>a;

    // std::map<int,char>test;
    // test.insert(std::pair<int,char>(1,'z'));
    // test.insert(std::pair<int,char1>(2,'t'));
    // test.erase(3);
    // std::cout<<test.size();
    // return 0;
}