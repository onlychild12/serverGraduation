#include<sys/socket.h>
#include"Server.h"
#include<pthread.h>
#include<netinet/in.h>
#include<unistd.h>
#include<signal.h>
#include<time.h>
#include"litControl.h"
#include"afx_mysignal.h"
pthread_mutex_t mutex_clean;//线程锁
pthread_mutex_t mutex_threadlist;
pthread_mutex_t mutex_free;
struct tdd//用以处理线程传参
{
    int soc;
    int index;
    Server *server;
};
struct tdget
{
   int soc;

    Server *server;  /* data */
};

struct dad//用以多线程处理传参
{
 litControl *control;
 char*data;
 int flag;
};
void *threaddeal_m(void *nptr)
{
    dad *tmpdo=static_cast<dad*>(nptr);
    litControl *control=tmpdo->control;
    char *data=tmpdo->data;
    control->Deal(data,tmpdo->flag);
    delete nptr;
return 0;
}
void *Threaddeal(void *nptr)
{
    tdd* td=static_cast<tdd*>(nptr);
    Server*server=td->server;
    int index=td->index;
    litControl *mycontrol=new litControl(td->soc);
    std::vector<pthread_t*>tmpdealthread;
    mycontrol->init();
    while(!server->exit&&mycontrol->getState())
    {
           int flag_tmp;
           
           char*data=mycontrol->rec_m();
           if(data==NULL)
             break;
           flag_tmp=(int)data[0];
           delete[] data;
           mycontrol->settarget(flag_tmp);
           data=mycontrol->rec_m();
           if(data!=NULL)
           {         
            if(mycontrol->getclass()==0)
            {
                mycontrol->Deal(data,flag_tmp);
                mycontrol->settarget(SIGNAL);
            }
            else if(mycontrol->getclass()==-1)
            {
                dad *littmp=new dad;
                pthread_t *tmp=new pthread_t;
                littmp->control=mycontrol;
                littmp->data=data;
                littmp->flag=flag_tmp;
                pthread_create(tmp,NULL,threaddeal_m,static_cast<void*>(littmp));
                tmpdealthread.push_back(tmp);
            }
            else if(mycontrol->getclass()<-1)
            {
                break;
            }
            else{
            
                    while(tmpdealthread.size()>=mycontrol->getclass())
                    {
                        for(int i=0;i<tmpdealthread.size();i++)
                        {
                            int t=*tmpdealthread[i];
                            if(pthread_kill(*tmpdealthread[i],0)==ESRCH);
                            {
                                pthread_join(*tmpdealthread[i],NULL);
                                delete tmpdealthread[i];
                                tmpdealthread.erase(tmpdealthread.begin()+i);
                                i--;
                            }
                        }
                        sleep(0.2);
                    }
                dad *littmp=new dad;
              
                pthread_t *tmp=new pthread_t;
                littmp->control=mycontrol;
                littmp->data=data;
                pthread_create(tmp,NULL,threaddeal_m,(void*)littmp);
                tmpdealthread.push_back(tmp);
            }
    }
      }
               for(;tmpdealthread.size()>0;)
               {
               for(int i=0;i<tmpdealthread.size();i++)
               {
                 if(pthread_kill(*tmpdealthread[i],0)==ESRCH)
                 {
                     delete tmpdealthread[i];
                    tmpdealthread.erase(tmpdealthread.begin()+i);
                    i--;
                 }
               }
               sleep(0.2);
               }
               delete mycontrol;
                 for(int i=0;i<tmpdealthread.size();i++)
                 {
                     delete tmpdealthread[i];
                 }
               tmpdealthread.clear();
               pthread_mutex_lock(&mutex_clean);
                server->cleanlist.push_back(index);//进栈
                pthread_mutex_unlock(&mutex_clean); 
                delete nptr;
    return 0;
}
void *clean_m(void *nptr)
{
Server*server=(Server*)nptr;
while(!server->exit)
{
    pthread_mutex_lock(&mutex_clean);
    if(server->cleanlist.size()>0)
    {
        pthread_mutex_lock(&mutex_threadlist);
    for(int i=0;i<server->cleanlist.size();)
    {
       pthread_join(*server->threadlist[server->cleanlist[i]],0);
       delete server->threadlist[server->cleanlist[i]];
       pthread_mutex_lock(&mutex_free);
       server->freelist.push_back(server->cleanlist[i]);
       pthread_mutex_unlock(&mutex_free);
       server->threadlist.erase(server->cleanlist[i]);
       server->cleanlist.erase(server->cleanlist.begin());
    }
    pthread_mutex_unlock(&mutex_threadlist);
    }

pthread_mutex_unlock(&mutex_clean);
sleep(0.2);
    
}
for(int i=0;i<server->threadlist.size();i++)
{
pthread_join(*server->threadlist[i],NULL);
delete server->threadlist[i];

}

server->threadlist.clear();
return 0;
}
void* listen_m(void* ptr)
{
    pthread_detach(pthread_self());
    sockaddr_in client;
    Server *parent=(Server*)ptr;
    int len=sizeof(client);
    while (!parent->exit/* condition */)
    {
       int saccept=accept(parent->server,(sockaddr*)&client,(socklen_t*)&len); /* code */
       pthread_t *tmpthread=new pthread_t;
          pthread_t *GetData;//接收数据线程
       tdd *tmptdd=new tdd;
       tmptdd->index=parent->index;
       tmptdd->soc=saccept;
       tmptdd->server=parent;
       pthread_create(tmpthread,NULL,Threaddeal,(void*)tmptdd);
      
       pthread_mutex_lock(&mutex_threadlist);
       parent->threadlist.insert(std::pair<int,pthread_t*>(parent->index,tmpthread));//插入队列
       pthread_mutex_unlock(&mutex_threadlist);
       pthread_mutex_lock(&mutex_free);
       if(parent->freelist.size()>0)
       {
           parent->index=parent->freelist[0];
           parent->freelist.erase(parent->freelist.begin());
           pthread_mutex_unlock(&mutex_free);
       }
       else{
           parent->index=parent->threadlist.size();
       }
        pthread_mutex_unlock(&mutex_free);

    }
    return 0;
}
void Server::init()
{
    pthread_mutex_init(&mutex_clean,NULL);
    pthread_mutex_init(&mutex_free,NULL);
    pthread_mutex_init(&mutex_threadlist,NULL); 
    server=socket(AF_INET,SOCK_STREAM,0);
    sockaddr_in sin;
    sin.sin_family=AF_INET;
    sin.sin_port=htons(7070);
    sin.sin_addr.s_addr=INADDR_ANY;
    bind(server,(sockaddr*)&sin,sizeof(sin));
    listen(server,1);
    pthread_create(&listen_thread,NULL,listen_m,(void*)this);//开启监听线程
    pthread_create(&clean_thread,NULL,clean_m,static_cast<void*>(this));

}

Server::~Server()
{
    pthread_join(clean_thread,NULL);
   
    cleanlist.clear();
    freelist.clear();
}

Server::Server(/* args */)
{
}
