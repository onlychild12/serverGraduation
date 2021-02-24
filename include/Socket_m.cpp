#include"Socket_m.h"
#include<unistd.h>
#include"PacketFormat.h"
void *getData(void *nptr)
{
    Socket_m *socket_m=(Socket_m*)nptr;
    while(socket_m->sset)
    {
        
        char *data=new char[socket_m->package];
        data=socket_m->rec_m();
        if(data==NULL)
        {
           pthread_mutex_lock(&socket_m->mutex_bool_socket);
            socket_m->sset=false;
            pthread_mutex_unlock(&socket_m->mutex_bool_socket);
            break;
        }
        pthread_mutex_lock(&socket_m->mutex_rec_thread);
        socket_m->RawData.push_back(data);
        pthread_mutex_unlock(&socket_m->mutex_rec_thread);
        sleep(0.2);
    }
        pthread_mutex_lock(&socket_m->mutex_rec_thread);
        for(int i_n=0;i_n<socket_m->RawData.size();i_n++) 
        {
           delete[] socket_m->RawData[i_n];
        } 
        socket_m->RawData.clear();
        pthread_mutex_unlock(&socket_m->mutex_rec_thread);
}
void *Sendthread(void *nptr)
{
    Socket_m *socket_m=(Socket_m*)nptr;
    while(socket_m->sset)
    {
        pthread_mutex_lock(&socket_m->mutex_send_thread);
      
        //   int i=0;
            for(;socket_m->sendlist_d.size()>0;)
            {
                socket_m->litSend(socket_m->sendlist_d[0]);
                socket_m->sendlist_d.erase(socket_m->sendlist_d.begin());
                // i++;
                // if(i*socket_m->package>=10000)
                // {
                //     sleep(0.2);
                //     i=0;
                // }
            }
         
        pthread_mutex_unlock(&socket_m->mutex_send_thread);
          sleep(0.2);
    }
    pthread_mutex_lock(&socket_m->mutex_send_thread);
    for(;socket_m->sendlist_d.size()>0;)
    {
        delete socket_m->sendlist_d[0];
        socket_m->sendlist_d.erase(socket_m->sendlist_d.begin());
    }
    socket_m->sendlist_d.clear();
    pthread_mutex_unlock(&socket_m->mutex_send_thread);
}
Socket_m::Socket_m(int socket_p)
{
    pthread_mutex_init(&mutex_send_thread,NULL);
    pthread_mutex_init(&mutex_send_lit,NULL);
   pthread_mutex_init(&mutex_rec_thread,NULL);
    pthread_mutex_init(&mutex_bool_socket,NULL);
  socket_m=socket_p;
 pthread_create(&Send_thread_flag,NULL,Sendthread,static_cast<void*>(this));
//  pthread_create(&GetData,NULL,getData,(void*)this);
}
int Socket_m::getclass()
{
    return threadclass;
}
Socket_m::~Socket_m()
{
    pthread_mutex_lock(&mutex_bool_socket);
    sset=false;
     pthread_mutex_unlock(&mutex_bool_socket);
    pthread_join(Send_thread_flag,NULL);
    //  pthread_join(GetData,NULL);
    pthread_mutex_destroy(&mutex_send_thread);
    pthread_mutex_destroy(&mutex_send_lit);
     pthread_mutex_destroy(&mutex_rec_thread);
    pthread_mutex_destroy(&mutex_bool_socket);
    close(socket_m);
}
char* Socket_m::rec_m()
{
 
    char *data_tmp_pre=new char[package];
    memset(data_tmp_pre,'z',package);
    long target_str_size=(target%500==0)?target:((target/500)+1)*500;
   
    char* target_str=new char[target_str_size];

    int tmp_size=0;
    do{
     memset(data_tmp_pre,'z',package);
    int state=recv(socket_m,data_tmp_pre,package,0);
    if(state>0){
    memcpy(target_str+tmp_size,data_tmp_pre,state);
    tmp_size+=state;

    }
    if(state<=0)
    {
           delete[] data_tmp_pre;
           return NULL;
       
    }

    }while(target_str_size>tmp_size);  
    
     std::cout<<"已接受"<<tmp_size<<"目标"<<target;
    delete[]  (target>package?data_tmp_pre:target_str);
    target>package?data_tmp_pre=NULL:target_str=NULL;
    return (target>package?target_str:data_tmp_pre);
}
void Socket_m::settarget(int flag)
{
    if(flag==USERLOGIN)
    target=sizeof(requestLogin);
    else if(flag==REGISTER)
    {
        target=sizeof(REGISTER);
    }
    else if(flag==MODEFYIMFOEMATION)
    {
        target=sizeof(Reader_Imformation);
    }
    else if(flag==PERSONIMFORMATION)
    {
        target=1;
    }
    else if(flag==SIGNAL)
    {
        target=1;
    }
    else if(flag==CREATESORT)
    {
         target=sizeof(requestcreatesort);

    }
    else if(flag==CREATEBOOK)
    {
        target=sizeof(requestcreatebook);
    }
    else if(flag==CATOGRYLIST)
    {
        target=sizeof(requestcatogrybook);
    }
    else if(flag==RANK)
    {
        target=1;
    }
    else if(flag==GETBYTES)
    {
        target=sizeof(requestaddbytes);
    }
    else if(flag==BOOKIMFORMATIONRES)
    {
        target=sizeof(requestbookimformation);
    }
    else if(flag==ADDSHELF)
    {
        target=sizeof(requestbookimformation);
    }
    else if(flag==RECOMMENDTICKET)
    {
        target=sizeof(Action_Tobytes);
    }
       else if(flag==GIVEMONEY)
    {
        target=sizeof(Action_Tobytes);
    }
    else if(flag==CHAPTERINSERT)
    {
        target=sizeof(Content);
    }
    else if(flag==CHAPTERINDEX)
    {
        target=sizeof(requestnovelchapter);
    }
    else if(flag==CHAPTER)
    {
        target=sizeof(requestchapter);
    }
    else if(flag==HISTORYMONEY)
    {
        target=500;
    }
    else if(flag==BUYCHAPTER)
    {
        target=sizeof(requestbuychapter);
    }
    else if(flag==WRITERIMF)
    {
        target=sizeof(requestwriterimformation);
    }
    else if(flag==FANLIST)
    {
        target=sizeof(requestnovelchapter);
    }
    else if(flag==GETPERSONFAN)
    {
        target=sizeof(requestnovelchapter);
    }
    else if(flag==DELETESHELF)
    {
        target=sizeof(requestbookimformation);
    }
    else if(flag==READSTATE)
    {
        target=sizeof(requestbookimformation);
    }
    else if(flag==READSTATESAVE)
    {
        target=sizeof(requestchapter);
    }
    else if(flag==GETREADHISTORY)
    {
        target=500;
    }
    else if(flag==CATOGRYLITTLE)
    {
        target=500;
    }
    else if(flag==CATOGRYLIKE)
    {
        target=sizeof(requestcatogrybook);
    }
    else if(flag==DELETECATOGRYLIKE)
    {
        target=500;
    }
    else if(flag==GETRECOMMENDBOOKLIST)
    {
        target=500;
    }
    else if(flag==GETENDBOOKLIST||flag==GETFREEBOOKLIST)
    {
        target=500;
    }
    else if(flag==SEARCH)
    {
        target=sizeof(requestnovelchapter);
    }
    else if(flag==UPDATENOVEL)
    {
        target=sizeof(requestcreatebook);
    }
    else if(flag==DELETENOVEL)
    {
     target=sizeof(requestnovelchapter);
    }
     else if(flag==DELETECHAPTER)
     {
         target=sizeof(requestbuychapter);
     }
     else if(flag==UPDATECHAPTER)
     {
         target=sizeof(Content);
     }
     else if(flag==SHELFINVENTORY)
     {

         target=sizeof(SHELFinventory);
     }
    
}
void Socket_m::setPacksize(long size)
{
    package=size;
    int nSendBuf=500*1024;//设置为
    setsockopt(socket_m,SOL_SOCKET,SO_SNDBUF,(const char*)package,sizeof(int));
    setsockopt(socket_m,SOL_SOCKET,SO_RCVBUF,(const char*)package,sizeof(int));
     timeval timeout;
    //    timeout={5,0};
    //    setsockopt(socket_m,SOL_SOCKET,SO_SNDTIMEO,(char *)&timeout,sizeof(struct timeval));
}
void Socket_m::SetHighAuclass()
{
    threadclass=-1;
}
void Socket_m::SetHighclass(int class_p)
{
    threadclass=class_p;
}
bool Socket_m::getState()
{
    bool tmp;
     pthread_mutex_lock(&mutex_bool_socket);
    tmp=sset;
     pthread_mutex_unlock(&mutex_bool_socket);
     return tmp;
}
char* Socket_m::getdata()
{
     char*data;
     pthread_mutex_lock(&mutex_rec_thread);
     if(RawData.size()>0)
     {
       data=RawData[0];
         RawData.erase(RawData.begin());
     }
     else 
     data=NULL;
   
     pthread_mutex_unlock(&mutex_rec_thread);
     return data;
}