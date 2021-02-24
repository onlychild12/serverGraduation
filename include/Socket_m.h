#ifndef SOCKET_M_H
#define SOCKET_M_H
#include<sys/socket.h>
#include<string.h>
#include<errno.h>
#include<signal.h>
#include<vector>
#include<time.h>
#include<pthread.h>
#include<iostream>
#include"Score.h"
#include <linux/types.h>
#include <asm/byteorder.h>
#include<netinet/in.h>
#include<unistd.h>
#include<netinet/tcp.h>
#include"PacketFormat.h"
#include"afx_mysignal.h"
#include"errno.h"
class Socket_m
{
    protected:
     std::vector<int>typetime;
    private:
    int inxed=0;
      pthread_mutex_t mutex_send_lit;
      pthread_mutex_t mutex_send_thread;
      pthread_mutex_t mutex_rec_thread;
      pthread_mutex_t mutex_bool_socket;
    int socket_m;
    std::vector<char*>RawData;//存储接收数据
    char *data;
    long target=package;
    bool sset=true;//显示socket连接状态
    long package=500;//包大小
    pthread_t Send_thread_flag;
    pthread_t GetData;//接收数据线程
  
  std::vector<littlepakage*>sendlist_d;
    int threadclass=0;
    public:
    friend void *getData(void *nptr);
    bool getState();
    char *getdata();//给外部的接口

     
    Socket_m(int socket_p);
    template<class Data>
    bool litSend(Data *data);//发送
    template<class Data>
    void litEncode(char *,Data *);//解码
    friend void *Sendthread(void *);
    char *rec_m();
    void setPacksize(long size);
    void SetHighAuclass();//设置最高优先级
    void SetHighclass(int);
    int getclass();
    void settarget(int flag);
    template<class Data>
    void LitSend(Data *data);
    ~Socket_m();
};
 template<class Data>
void Socket_m::LitSend(Data *data)//发送接口
{
  pthread_mutex_lock(&mutex_send_thread);
  sendlist_d.push_back(data);
  pthread_mutex_unlock(&mutex_send_thread);
}
  template<class Data>
    bool Socket_m::litSend(Data *data)//发送
    {
      bool bset=false;
      bool bset_tmp=false;
      char *data_teee;
     char *data_t;
        pthread_mutex_lock(&mutex_send_lit);
        int count_send=data->size/package;
        if(data->size>package){
         bset_tmp=true;
        data_teee=new char[(data->size%package==0?data->size:((data->size/package+1)*500))];
        memset(data_teee,'z',(data->size%package==0?data->size:((data->size/package+1)*500)));
        data_t=new char[data->size];
        memset(data_t,'z',data->size);
        memcpy(data_t,data->content,data->size);
        }
        else{
          data_t=new char[package];
          memset(data_t,'z',package);
          if(data->type==SIGNAL)
          {
   
          data_t[0]=(char)((long)data->content);
          }
          else
          memcpy(data_t,data->content,data->size);
        }
         char *tmpdata;
        if(data->size%package>0)
        {
          count_send++;
          bset=true;
        }
        for(int i=0;i<count_send;i++)
        {
        int v;
        // if(data->time_flag!=0)
      
     tcp_info info;
     int len = sizeof(info);
    getsockopt(socket_m,IPPROTO_TCP, TCP_INFO, &info, (socklen_t *) & len);
    if ((info.tcpi_state != TCP_ESTABLISHED)) {
           delete data_t;
           data_t=NULL;
            delete data;
            pthread_mutex_lock(&mutex_bool_socket);
            sset=false;
             pthread_mutex_unlock(&mutex_bool_socket);
            pthread_mutex_unlock(&mutex_send_lit);
           return false;
    }
       std::cout<<"单"<<std::endl;
 
   
     
       if(bset&&(i==(count_send-1)||count_send==1))
       {
       char *data_tmp_tmp=new char[package];
       memset(data_tmp_tmp,'z',package);
      //  if(bset_tmp)
      //  {
      //    Reader_Imformation ri;
      //    memcpy(data_teee+i*package,data_t+i*package,data->size-i*package);
      //    memcpy(&ri,data_teee,sizeof(Reader_Imformation));
      //    std::cout<<ri.iconsize;
      //    memcpy(&ri,data_t,sizeof(Reader_Imformation));
      //     std::cout<<ri.iconsize;
      //  }
       memcpy(data_tmp_tmp,data_t+i*package,data->size-i*package);
       v=send(socket_m,data_tmp_tmp,package,0);
       delete[] data_tmp_tmp;
       }
       else{
         tmpdata=(data_t+i*package);
         if(bset_tmp)
         {
            memcpy(data_teee+i*package,tmpdata,package);
         }
          v=send(socket_m,tmpdata,package,0);
       }
       
  
       inxed+=v;
        std::cout<<inxed;
   
        if(v<500)
        std::cout<<"发送失败"<<strerror(errno);
       if(v==-1)
       for(int i=0;v==-1&&i<3;i++)
       {
        v=send(socket_m,data_t,package,0);
        sleep(1);
        std::cout<<"发送失败"<<strerror(errno);
        
         if(i==2)
            {
               delete data_t;
              data_t=NULL;
              // delete data;
              pthread_mutex_unlock(&mutex_send_lit);
              std::cout<<"强制退出"<<std::endl;
              return false;
            }
       }
       else
       { 
          std::cout<<"双"<<v<<std::endl;  
       
         
       }
        } 
           pthread_mutex_unlock(&mutex_send_lit);
           delete data_t;
          delete data;
          data=NULL;
            data_t=NULL;
          return true;/* code */

    }
    template<class Data>
    void Socket_m::litEncode(char*data,Data*desd)
    {
            memcpy(desd,data,sizeof(Data));
            delete[] data;
    }
#endif