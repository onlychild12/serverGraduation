#include "PacketFormat.h"
#include "Litsql.h"
#include <iostream>
#include <unistd.h>
#include <string>
#include <errno.h>
#include <vector>
#include <stdlib.h>
#include "FileEngine.h"
#include"afx_mysignal.h"
char *Litsql::db = NULL;
unsigned int port = 0; //端口
char *unix_socket = NULL;
unsigned long flag = 0;
// int Litsql::onebatchrank[2]={507,550};
// int Litsql::secondbatchrank[2]={464,526};
// int Litsql::thirdbatchrank[2]={433,501};
// int Litsql::techlogyrank[2]={200,200};
// char Litsql::type_class[2][45]={"理科","文科"};
std::vector<Parame *> Litsql::parame;
bool Litsql::givenovelmoney(const char *username, const char *novelname, int bytes)
{
    MYSQL *conn = mysql_init(NULL);
    connect(conn);
    char *tmp = new char[400];
    std::string sql = "update reader,novel set novel.give=novel.give+%d,reader.bytes=reader.bytes-%d,novel.money=novel.money+(%d/100) where reader.name='%s' and novel.name='%s';";
    std::string sql_t = "update monthtotal set monthgive=monthgive+%d where rid=(select id from reader where name='%s');";
    sprintf(tmp, sql.c_str(), bytes, bytes, bytes, username, novelname);
    mysql_query(conn, "set names utf8;");
    char *tmp_ptr = new char[400];
    sprintf(tmp_ptr, sql_t.c_str(), bytes, username);
    int result = mysql_query(conn, tmp);
    mysql_query(conn, tmp_ptr);
    delete[] tmp;
    delete[] tmp_ptr;
    if (result == 0)
        return true;
    else
    {
        mysql_rollback(conn);
        return false;
    }
}
void Litsql::connect(MYSQL *conn)
{

    if (mysql_real_connect(conn, "localhost", "root", "server2020725", "GraduationThesis", 0, NULL, CLIENT_TRANSACTIONS) == NULL)
    {
        std::cout << "连接失败";
        // exit(0);
    }
    mysql_query(conn, "set names utf8;");
}
MYSQL_RES *Litsql::getReader(const char *name)
{
    MYSQL *conn = mysql_init(NULL);
    connect(conn);
    char *tmp = new char[400];
    std::string sql = "select * from reader where name='%s';";
    sprintf(tmp, sql.c_str(), name);
    mysql_query(conn, tmp);
    MYSQL_RES *res = mysql_store_result(conn);
    delete[] tmp;
    mysql_close(conn);
    return res;
}
bool Litsql::Login(const char *username, const char *password, int signal) //读者登录确认
{
    MYSQL *conn = mysql_init(NULL);

    connect(conn);

    std::string sql;
    switch (signal)
    {

    case USERLOGIN:
        sql = "select id from reader where name='%s' and password='%s';";
        break;
    case WRITERLOGIN:
        sql = "select id from writer where name='%s' and password='%s';";
        break;
    }

    char *tmp = new char[400];

    sprintf(tmp, sql.c_str(), username, password);

    mysql_query(conn, "set names utf-8");
    mysql_query(conn, tmp);
    MYSQL_RES *res = mysql_store_result(conn);
    int count = mysql_num_rows(res);
    mysql_free_result(res);
    delete[] tmp;

    std::cout << count;
    return count > 0 ? true : false;
}
bool Litsql::RegisterWriter(const char *username, const char *password, const char *id_office)
{
    MYSQL *conn = mysql_init(NULL);
    connect(conn);
    int result_real = -1;
    int off = 0;
    std::string sql = "insert into writer(name,password,id_office,icon) values('%s','%s','%s','%s');";

    char *sql_r = new char[700];
    sprintf(sql_r, sql.c_str(), username, password, id_office, " ");

    mysql_query(conn, "set names utf-8;");
    int result = mysql_query(conn, sql_r);

    if (result == 0)
    {
        mysql_close(conn);
        return true;
    }
    else
    {

        mysql_rollback(conn);
        mysql_close(conn);
        return false;
    }
}
bool Litsql::Register(const char *username, const char *password, const char *icon_path, const char *id_office)
{
    MYSQL *conn = mysql_init(NULL);
    connect(conn);
    int result_real = -1;
    int off = 0;
    std::string sql = "insert into reader(name,password,icon,id_office) values('%s','%s','%s','%s');";
    std::string sql_tp = "insert into monthtotal(rid,monthgive) values((select id from reader where name='%s'),0);";
    char *sql_r = new char[700];
    sprintf(sql_r, sql.c_str(), username, password, icon_path, id_office);

    mysql_query(conn, "set names utf-8;");
   
    int result = mysql_query(conn, sql_r);
    int result_create = -1;
    if (result != 0)
    {
        std::cout << "失败" << mysql_error(conn);
        mysql_rollback(conn);
        return false;
    }
    else
    {
    
        char *data_p = new char[600];
        sprintf(data_p, sql_tp.c_str(), username);
        result_real = mysql_query(conn, data_p);
    }
    mysql_close(conn);
    return result_real == 0 ? true : false;
}
MYSQL_RES *Litsql::getshelf(const char *name)
{
    MYSQL *conn = mysql_init(NULL);
    connect(conn);
    char *tmp = new char[400];
    std::string sql = "select novel.name,novel.word,novel.state,writer.name,UNIX_TIMESTAMP(novel.updatetime),chapter.title from novel,writer,chapter  where novel.id in(select nid from testshelf where rid=(select id from reader where name='%s')) and novel.wid=writer.id and chapter.id=novel.cid;";
    sprintf(tmp, sql.c_str(), name);
    mysql_query(conn, "set names utf8;");
    mysql_query(conn, tmp);
    MYSQL_RES *res = mysql_store_result(conn);
    delete[] tmp;
    mysql_close(conn);
    return res;
}
bool Litsql::updateReader(const char *icon, const char *username, long size)
{
    MYSQL *conn = mysql_init(NULL);
    connect(conn);
    std::string sql_tmp = "select id_office from reader where name='%s';";
    char *tmp_tt = new char[400];
    sprintf(tmp_tt, sql_tmp.c_str(), username);
    mysql_query(conn, "set names utf-8;");
    mysql_query(conn, tmp_tt);
    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row;
    row = mysql_fetch_row(res);
    char *id_office = new char[19];
    strcpy(id_office, row[0]);
    mysql_free_result(res);
    std::string icon_m = FileEngine::write(id_office, READERICON, icon, size);
    char *tmp = new char[400];
    std::string sql = "update reader set icon='%s' where name='%s';";

    sprintf(tmp, sql.c_str(), icon_m.c_str(), username);
    mysql_query(conn, "set names utf-8");
    int result = mysql_query(conn, tmp);

    delete[] tmp;
    mysql_close(conn);
    return result == 0 ? true : false;
}
bool Litsql::updatepassword(const char *username, const char *password, const char *username_old, const char *password_old)
{
    MYSQL *conn = mysql_init(NULL);
    connect(conn);
    char *tmp = new char[400];
    std::string sql = "update reader set name=%s,password=%s where name=%s and ((select count(*) from reader where name=%s and password=%s)>0)";
    sprintf(tmp, sql.c_str(), username, password, username_old, username_old, password_old);
    mysql_query(conn, "set names utf-8");
    int result = mysql_query(conn, tmp);

    delete[] tmp;
    mysql_close(conn);
    return result == 0 ? true : false;
}
MYSQL_RES *Litsql::getRecommend(const char *name)
{
    MYSQL *conn = mysql_init(NULL);
    connect(conn);
    char *tmp = new char[400];
    std::string sql = "select novel.icon,novel.name,novel.word,,writer.name,novel.novelless,,novel.state from recommendtitle join recommendtitle on recommend title.id=recommendlist.rid join  novel on novel.id=recommendlist.nid join writer on writer.id=novel.wid where recommendtitle.titlename=%s";
    sprintf(tmp, sql.c_str(), name);
    mysql_query(conn, "set names utf-8");
    int result = mysql_query(conn, tmp);
    MYSQL_RES *res = mysql_store_result(conn);
    delete[] tmp;
    mysql_close(conn);
    return res;
}
Writer_Imformation Litsql::getAuthor(const char *name)
{
    Writer_Imformation wi;
    MYSQL *conn = mysql_init(NULL);
    connect(conn);
    char *tmp = new char[400];
    std::string sql = "select count(id),sum(word) from novel where wid=(select id from writer where name='%s');";
    sprintf(tmp, sql.c_str(), name);
    mysql_query(conn, "set names utf-8");
    int result = mysql_query(conn, tmp);
    MYSQL_RES *res = mysql_store_result(conn);
    if (res == NULL)
    {
        wi.wordcount = -1;
        delete[] tmp;
        mysql_close(conn);
        return wi;
    }
    if (mysql_num_rows(res) > 0)
    {
        MYSQL_ROW row = mysql_fetch_row(res);
        wi.bookcount = atoi(row[0]);
        wi.wordcount = atoi(row[1]);
    }
    else
    {
        wi.bookcount = -1; /* code */
    }

    delete[] tmp;
    mysql_free_result(res);
    mysql_close(conn);
    return wi;
}
Content *Litsql::getChapter(const char *bookname, int index, int bset)
{
    MYSQL *conn = mysql_init(NULL);
    Content *tmpcontent = new Content;
    connect(conn);
    char *tmp = new char[400];
    std::string sql;
    if (bset == 0)
    {
        sql = "select content,title,id,needbytes from chapter where id=%d;";
        sprintf(tmp, sql.c_str(), index);
    }
    else if (bset == 1)
    {
        sql = "select content,title,id,needbytes from chapter where id>%d and nid=(select id from novel where name='%s') order by id limit 1;";
        sprintf(tmp, sql.c_str(), index, bookname);
    }
    else if (bset == 2)
    {
        sql = "select content,title,id,needbytes from chapter where id<%d and nid=(select id from novel where name='%s') order by id desc limit 1;";
        sprintf(tmp, sql.c_str(), index, bookname);
    }

    int result = mysql_query(conn, tmp);
    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row;
    int rowcount = mysql_num_rows(res);
    if (rowcount > 0)
    {

        MYSQL_ROW row;
        row = mysql_fetch_row(res);
        strcpy(tmpcontent->content, row[0]);
        strcpy(tmpcontent->chaptername, row[1]);
        tmpcontent->index = atoi(row[2]);
        tmpcontent->bytesmoney = atoi(row[3]);
        if (scanchapterstate(tmpcontent->index) == -1)
        {
            tmpcontent->bytesmoney = -1;
        }
        delete[] tmp;
        mysql_free_result(res);
        mysql_close(conn);
        return tmpcontent;
    }
    delete[] tmp;
    mysql_close(conn);
    mysql_free_result(res);
    return NULL;
}
MYSQL_RES *Litsql::getBookList(char *name)
{
    MYSQL *conn = mysql_init(NULL);
    connect(conn);
    char *tmp = new char[400];
    std::string sql = "select novel.icon,novel.name,writer.name,novel.word,novel.state,catogry.name,novel.novelless, from recommendlist join recommendtitle on recommendlist.rid=recommendtitle.id join novel on novel.id=recommendlist.nid join (select novel.id,writer.name from novel join writer on novel.wid=writer.id) join novel on novel.cid=catogry.id  where recommendtitle.name=%s";
    sprintf(tmp, sql.c_str(), name);
    mysql_query(conn, "set names utf-8");
    int result = mysql_query(conn, tmp);
    MYSQL_RES *res = mysql_store_result(conn);
    delete[] tmp;
    mysql_close(conn);
    return res;
}
bool Litsql::addbytes(const char *username, int money)
{
    MYSQL *conn = mysql_init(NULL);
    connect(conn);
    char *tmp = new char[400];

    std::string update_sql = "update reader,manager set reader.bytes=reader.bytes+%d*(select bytesmoney from rule where id=1),manager.total_in=manager.total_in+%d where reader.name='%s' and manager.id=1;";
    std::string updatehistory = "insert into givemoney(rid,givemoney,time) values((select id from reader where name='%s'),%d,now());";
    sprintf(tmp, update_sql.c_str(), money, money, username);
    int result = mysql_query(conn, tmp);
    sprintf(tmp, updatehistory.c_str(), username, money);
    int result_r = mysql_query(conn, tmp);
    delete[] tmp;
    if (result == 0 && result_r == 0)
    {

        mysql_close(conn);

        return true;
    }
    else
    {
        std::cout << mysql_error(conn);
        mysql_rollback(conn);
        mysql_close(conn);
        return false;
    }
}
bool Litsql::toticket(const char *username, const char *novelname, int ticket)
{
    MYSQL *conn = mysql_init(NULL);
    connect(conn);
    char *tmp = new char[400];
    std::string sql = "update reader,novel set reader.monthticket=reader.monthticket-%d,novel.month_ticket=novel.month_ticket+%d where reader.name='%s' and novel.name='%s';";
    sprintf(tmp, sql.c_str(), ticket, ticket, username, novelname);

    mysql_query(conn, "set names utf8;");
    int result = mysql_query(conn, tmp);
    delete[] tmp;
    if (result == 0)
    {
        return true;
    }
    else
    {
        mysql_rollback(conn);
        mysql_close(conn);
        return false;
    }
}
MYSQL_RES *Litsql::getbookrank(int flag)
{
    MYSQL *conn = mysql_init(NULL);
    connect(conn);
    std::string sql;
    char *tmp = new char[400];
    if (flag == MONTHTICKET)
    {
        sql = "select novel.icon,novel.name,writer.name,novel.word,catogry.name,novel.month_ticket,novel.recommend_ticket from novel join writer on novel.wid=writer.id join  catogry on novel.catogryid=catogry.id order by month_ticket desc limit 80;";
    }
    else if (flag == RECOMMENDTICKET)
    {
        sql = "select novel.icon,novel.name,writer.name,novel.word,catogry.name,novel.month_ticket,novel.recommend_ticket from novel join writer on novel.wid=writer.id join  catogry on novel.catogryid=catogry.id order by recommend_ticket desc limit 80;";
    }
    sprintf(tmp, sql.c_str());
    int result = mysql_query(conn, tmp);
    MYSQL_RES *res = mysql_store_result(conn);
    delete[] tmp;
    mysql_close(conn);
    return res;
}
MYSQL_RES *Litsql::getCatogry()
{
    MYSQL *conn = mysql_init(NULL);
    connect(conn);
    char *tmp = new char[400];
    std::string sql = "select name,icon,sex from catogry;";
    int result = mysql_query(conn, sql.c_str());
    MYSQL_RES *res = mysql_store_result(conn);
    delete[] tmp;
    mysql_close(conn);
    return res;
}
MYSQL_RES *Litsql::getBOOKimformation(const char *novelname)
{
    MYSQL *conn = mysql_init(NULL);
    connect(conn);
    char *tmp = new char[400];
    std::string sql = "select novel.icon,novel.name,word,writer.name,writer.icon,UNIX_TIMESTAMP(updatetime),state,catogry.name,recommend_ticket,month_ticket,noveless  from novel join writer on writer.id=novel.wid join catogry on catogry.id=novel.catogryid where novel.name='%s';";

    sprintf(tmp, sql.c_str(), novelname);
    int result = mysql_query(conn, tmp);

    MYSQL_RES *res = mysql_store_result(conn);
    delete[] tmp;
    mysql_close(conn);
    return res;
}
bool Litsql::insertintobook(const char *name, const char *writer, const char *type, const char *sex, long size,const char *novelless)
{
    MYSQL *conn = mysql_init(NULL);
    connect(conn);
    int result_real = -1;
    int off = 0;
    std::string sql = "insert into novel(name,wid,icon,catogryid,noveless) values('%s',(select id from writer where name='%s'),'%s',(select id from catogry where name='%s' and sex='%s'),'%s');";
   // std::string filename = FileEngine::write(name, BOOKICON, icon, size);
    char *sql_r = new char[700];
    sprintf(sql_r, sql.c_str(), name, writer, " ", type, sex,novelless);

    mysql_query(conn, "set names utf8;");
    int result = mysql_query(conn, sql_r);
    delete sql_r;
    if (result == 0)
    {
        mysql_close(conn);
        return true;
    }
    else
    {
        std::cout << mysql_error(conn);
        mysql_rollback(conn);
        mysql_close(conn);
        return false;
    }
}
bool Litsql::CreateCatogry(const char *name, const char *sex, long size)
{
    MYSQL *conn = mysql_init(NULL);
    connect(conn);
    int result_real = -1;
    int off = 0;
    std::string sql = "insert into catogry(name,sex,icon) values('%s','%s','  ');";
    // std::string filename = FileEngine::write(name, SORTICON, icon, size);
    char *sql_r = new char[700];
    sprintf(sql_r, sql.c_str(), name, sex);

    int result = mysql_query(conn, sql_r);

    if (result == 0)
    {
        mysql_close(conn);
        return true;
    }
    else
    {

        mysql_rollback(conn);
        mysql_close(conn);
        return false;
    }
}
MYSQL_RES *Litsql::getCatogrybooklist(const char *name, const char *sex,const int set)
{
    MYSQL *conn = mysql_init(NULL);
    connect(conn);
    char *tmp = new char[400];
    std::string sql ;
    if(set==0)
      sql = "select novel.name,word,novel.icon,state,noveless,writer.name from novel join writer on novel.wid=writer.id where catogryid=(select id from catogry where name='%s' and sex='%s') order by novel.month_ticket desc;";
    else if(set==1)
    {
         sql = "select novel.name,word,novel.icon,state,noveless,writer.name from novel join writer on novel.wid=writer.id where catogryid=(select id from catogry where name='%s' and sex='%s') order by novel.recommend_ticket desc;";
    }
    sprintf(tmp, sql.c_str(), name, sex);
    int result = mysql_query(conn, tmp);
    MYSQL_RES *res = mysql_store_result(conn);
    delete[] tmp;
    mysql_close(conn);
    return res;
}
bool Litsql::addshelf(const char *username, const char *novelname)
{
    MYSQL *conn = mysql_init(NULL);
    connect(conn);
    char *tmp = new char[400];

    std::string update_sql = "insert into testshelf(rid,nid) values((select id from reader where name='%s'),(select id from novel where name='%s'));";
    sprintf(tmp, update_sql.c_str(),username, novelname);
    int result = mysql_query(conn, tmp);
    delete[] tmp;
    if (result == 0)
    {

        mysql_close(conn);

        return true;
    }
    else
    {
        std::cout << mysql_error(conn);
        mysql_rollback(conn);
        mysql_close(conn);
        return false;
    }
}
bool Litsql::getbookshelfstate(const char *username, const char *novelname)
{
    MYSQL *conn = mysql_init(NULL);
    connect(conn);

    std::string sql;
    sql = "select id from testshelf where rid=(select id from reader where name='%s') and nid=(select id from novel where name='%s');";
    char *tmp = new char[400];
    sprintf(tmp, sql.c_str(),username, novelname);

    mysql_query(conn, "set names utf8;");
    mysql_query(conn, tmp);
    MYSQL_RES *res = mysql_store_result(conn);
    if (res == NULL)
        return false;
    int count = mysql_num_rows(res);
    mysql_free_result(res);
    delete[] tmp;

    std::cout << count;
    return count > 0 ? true : false;
}
bool Litsql::torecommendticket(const char *username, const char *novelname, int ticket)
{
    MYSQL *conn = mysql_init(NULL);
    connect(conn);
    char *tmp = new char[400];
    std::string sql = "update reader,novel set novel.recommend_ticket=novel.recommend_ticket+%d,reader.recommendticket=reader.recommendticket-%d where reader.name='%s' and novel.name='%s';";
    sprintf(tmp, sql.c_str(), ticket, ticket, username, novelname);
    mysql_query(conn, "set names utf8;");
    int result = mysql_query(conn, tmp);
    if (result == 0)
        return true;
    else
    {
        mysql_rollback(conn);
        return false;
    }
}
bool Litsql::insertintochapter(const char *novelname, const char *chaptername, const char *content)
{

    int contactstate = 0;
    if (getcontact(novelname))
    {

        contactstate = (strlen(content));
    }
        MYSQL *conn = mysql_init(NULL);
    connect(conn);
    char *tmp = new char[sizeof(Content) + 400];
    std::string sql = "insert into chapter(nid,content,title,time,wordcout,needbytes) values((select id from novel where name='%s'),'%s','%s',now(),%d,%d/1000*(select thoudswordmoney from rule));";
    sprintf(tmp, sql.c_str(), novelname, content, chaptername, strlen(content), contactstate);
    mysql_query(conn, "set names utf8;");
    int result = mysql_query(conn, tmp);
    std::string sql_p="call updatecid('%s',%d);";
    char ttp[400];
    sprintf(ttp,sql_p.c_str(),novelname,strlen(content));
    result= mysql_query(conn, ttp);
   if (result == 0)
        return true;
    else
    {
        std::cout << mysql_error(conn);
        mysql_rollback(conn);
        return false;
    }
}
MYSQL_RES *Litsql::getchapterlist(const char *novelname)
{
    MYSQL *conn = mysql_init(NULL);
    connect(conn);
    char *tmp = new char[400];
    std::string sql = "select title,id,UNIX_TIMESTAMP(time),needbytes from chapter where nid=(select id from novel where name='%s');";
    sprintf(tmp, sql.c_str(), novelname);
    // mysql_query(conn, "set names utf8;");
    int result = mysql_query(conn, tmp);
    MYSQL_RES *res = mysql_store_result(conn);
    delete[] tmp;
    mysql_close(conn);
    return res;
}
MYSQL_RES *Litsql::getmoneyhistory(const char *username)
{

    MYSQL *conn = mysql_init(NULL);
    connect(conn);
    char *tmp = new char[400];
    std::string sql = "select UNIX_TIMESTAMP(time),givemoney from givemoney where rid=(select id from reader where name='%s') order by time desc;";
    sprintf(tmp, sql.c_str(), username);
    mysql_query(conn, "set names utf8;");
    int result = mysql_query(conn, tmp);
    MYSQL_RES *res = mysql_store_result(conn);
    delete[] tmp;
    mysql_close(conn);
    return res;
}
bool Litsql::getcontact(const char *bookname)
{
    MYSQL *conn = mysql_init(NULL);
    connect(conn);
    char *tmp = new char[400];
    std::string sql = "select id from contact where nid=(select id from novel where name='%s');";
    sprintf(tmp, sql.c_str(), bookname);
    mysql_query(conn, "set names utf8;");
    int result = mysql_query(conn, tmp);
    MYSQL_RES *res = mysql_store_result(conn);
    int count = mysql_num_rows(res);

    delete[] tmp;
    mysql_close(conn);
    if (count > 0)
    {
        return true;
    }
    else
        return false;
}
int Litsql::scanchapterstate(int id)
{
    MYSQL *conn = mysql_init(NULL);
    connect(conn);
    char *tmp = new char[400];
    std::string sql = "select id from buychapter where cid=%d;";
    sprintf(tmp, sql.c_str(), id);
    mysql_query(conn, "set names utf8;");
    int result = mysql_query(conn, tmp);
    MYSQL_RES *res = mysql_store_result(conn);
    int count = mysql_num_rows(res);

    delete[] tmp;
    mysql_close(conn);
    if (count > 0)
    {
        return -1;
    }
    else
        return 0;
}
bool Litsql::buychapter(const char *username, const int cid)
{
    MYSQL *conn = mysql_init(NULL);
    connect(conn);
    char *tmp = new char[400];
    std::string sql = "update novel,reader set novel.give=novel.give+(select needbytes from chapter where id=%d),reader.bytes=reader.bytes-(select needbytes from chapter where id=%d) where novel.id=(select nid from chapter where id=%d) and reader.name='%s';";
    std::string sql_r = "insert into buychapter(rid,cid) values((select id from reader where name='%s'),%d);";
    std::string sql_t = "update monthtotal set monthgive=monthgive+(select needbytes from chapter where id=%d) where rid=(select id from reader where name='%s');";
    sprintf(tmp, sql.c_str(), cid, cid, cid, username);
    mysql_query(conn, "set names utf8;");
    int result = mysql_query(conn, tmp);
    std::cout << tmp;
    sprintf(tmp, sql_r.c_str(), username, cid);
    int result_r = mysql_query(conn, tmp);
    sprintf(tmp, sql_t.c_str(), cid, username);
    int result_t = mysql_query(conn, tmp);
    sql = "select needbytes,novel.name from chapter join novel on novel.id=chapter.nid where id=%d;";
    sprintf(tmp, sql.c_str(), cid);
    int moneyr = mysql_query(conn, tmp);

    if (moneyr == 0)
    {
        MYSQL_RES *res = mysql_store_result(conn);
        MYSQL_ROW row = mysql_fetch_row(res);
        addfanvalue(username, row[1], atoi(row[0]));
        mysql_free_result(res);
    }
    delete[] tmp;
    mysql_close(conn);
    if (result == 0 && result_r == 0 && result_t == 0)
    {
        return true;
    }
    else
        return false;
}
MYSQL_RES *Litsql::getauthorbooklist(const char *name)
{
    MYSQL *conn = mysql_init(NULL);
    connect(conn);
    char *tmp = new char[400];
    std::string sql = "select name,word,UNIX_TIMESTAMP(updatetime),state,(select name from catogry where id=catogryid),noveless,(select sex from catogry where id=catogryid) from novel where wid=(select id from writer where name='%s');";
    sprintf(tmp, sql.c_str(), name);
    mysql_query(conn, "set names utf8;");
    int result = mysql_query(conn, tmp);
    MYSQL_RES *res = mysql_store_result(conn);
    delete[] tmp;
    mysql_close(conn);
    return res;
}

bool Litsql::addfanvalue(const char *username, const char *novelname, const int bytes)
{
    MYSQL *conn = mysql_init(NULL);
    connect(conn);
    char *tmp = new char[400];
    std::string sql = "call addfan('%s','%s',%d);";
    sprintf(tmp, sql.c_str(), username, novelname, bytes);
    int result = mysql_query(conn, tmp);

    delete[] tmp;
    mysql_close(conn);
    if (result == 0)
    {
        return true;
    }
    else
        return false;
}
MYSQL_RES *Litsql::getfanlist(const char *novelname)
{
    MYSQL *conn = mysql_init(NULL);
    connect(conn);
    char *tmp = new char[400];
    std::string sql = "select reader.name,value,class from fanvalue join reader on reader.id=fanvalue.rid where nid=(select id from novel where name='%s') order by value desc limit 100;";
    sprintf(tmp, sql.c_str(), novelname);
    mysql_query(conn, "set names utf8;");
    int result = mysql_query(conn, tmp);
    MYSQL_RES *res = mysql_store_result(conn);
    delete[] tmp;
    mysql_close(conn);
    if (result == 0)
        return res;
    else
    {
        delete res;
        return NULL;
    }
}
fanlist *Litsql::getpersonvalue(const char *username, const char *novelname)
{
    MYSQL *conn = mysql_init(NULL);
    connect(conn);
    char *tmp = new char[400];
    std::string sql = "select reader.name,value,class from fanvalue join reader on reader.id=fanvalue.rid where nid=(select id from novel where name='%s') and rid=(select id from reader where name='%s');";
    sprintf(tmp, sql.c_str(), novelname, username);
    mysql_query(conn, "set names utf8;");
    int result = mysql_query(conn, tmp);
    MYSQL_RES *res = mysql_store_result(conn);
    delete[] tmp;

    MYSQL_ROW row = mysql_fetch_row(res);

    fanlist *fl = new fanlist;
    if (mysql_num_rows(res) == 0)
    {
        fl->fanvalue = 0;
        fl->state = 0;
    }
    else
    {
        strcpy(fl->name, row[0]);
        fl->fanvalue = atoi(row[1]);
        fl->state = atoi(row[2]);
    }
    mysql_close(conn);
    return fl;
}
bool Litsql::savereadstate(const char *username, const int chapterid)
{
    MYSQL *conn = mysql_init(NULL);
    connect(conn);
    char *tmp = new char[400];
    std::string sql = "call savereadstate('%s',%d);";
    sprintf(tmp, sql.c_str(), username, chapterid);
    int result = mysql_query(conn, tmp);

    delete[] tmp;
    mysql_close(conn);
    if (result == 0)
    {
        return true;
    }
    else
        return false;
}
Content *Litsql::getreadstate(const char *username, const char *novelname)
{
    MYSQL *conn = mysql_init(NULL);
    connect(conn);
    char *tmp = new char[400];
    std::string sql = "select cid from readstate where rid=(select id from reader where name='%s') and nid=(select id from novel where name='%s');";
    sprintf(tmp, sql.c_str(), username, novelname);
    int result = mysql_query(conn, tmp);
    if (result == 0)
    {
        MYSQL_RES *res = mysql_store_result(conn);
        int z = mysql_num_rows(res);
        if (z > 0)
        {
            MYSQL_ROW row = mysql_fetch_row(res);
            delete[] tmp;
            mysql_close(conn);
           
            int chapterindex= atoi(row[0]);
             mysql_free_result(res);
            return getChapter(novelname,chapterindex, 0);
        }
        else
        {
            mysql_free_result(res);
            std::string sql_i = "select id from chapter where nid=(select id from novel where name='%s') order by id asc limit 1;";
            char tmpp[400];
            sprintf(tmpp, sql_i.c_str(), novelname);
            int result_gi = mysql_query(conn, tmpp);

            if (result_gi == 0)
            {
                MYSQL_RES *ress = mysql_store_result(conn);
            
                MYSQL_ROW row = mysql_fetch_row(ress);
                int idoo = atoi(row[0]);
                mysql_free_result(ress);
                delete[] tmp;
                mysql_close(conn);
                return getChapter(novelname, idoo, 0);
            }
            else
            {
                delete[] tmp;
                mysql_close(conn);
                return NULL;
            }
        }
    }
    else
    {
        delete[] tmp;
        mysql_close(conn);
        return NULL;
    }
}
bool Litsql::addlikecatogry(const char *sex, const char *catogry, const char *username)
{
       MYSQL *conn = mysql_init(NULL);
    connect(conn);
    char *tmp = new char[400];
    std::string sql = "insert into liketable(rid,catogryid) values((select id from reader where name='%s'),(select id from catogry where sex='%s' and name='%s'));";
    sprintf(tmp, sql.c_str(), username, sex,catogry);
    int result = mysql_query(conn, tmp);

    delete[] tmp;
    mysql_close(conn);
    if (result == 0)
    {
        return true;
    }
    else
        return false;
}

bool Litsql::deletelikecatogry(const char *username)
{
      MYSQL *conn = mysql_init(NULL);
    connect(conn);
    char *tmp = new char[400];
    std::string sql = " delete from liketable where rid=(select id from reader where name='%s');";
    sprintf(tmp, sql.c_str(), username);
    int result = mysql_query(conn, tmp);

    delete[] tmp;
    mysql_close(conn);
    if (result == 0)
    {
        return true;
    }
    else
        return false;
}
MYSQL_RES *Litsql::getlikecatogry(const char *username)
{
      MYSQL *conn = mysql_init(NULL);
    connect(conn);
    char *tmp = new char[400];
    std::string sql = "select name,sex from catogry where id in(select catogryid from liketable where rid=(select id from reader where name='%s'))";
    sprintf(tmp, sql.c_str(),username);
    mysql_query(conn, "set names utf8;");
    int result = mysql_query(conn, tmp);
    MYSQL_RES *res = mysql_store_result(conn);
    delete[] tmp;
    mysql_close(conn);
    if (result == 0)
        return res;
    else
    {
        mysql_free_result(res);
        return NULL;
    }
}
bool Litsql::deleteshelf(const char *username,const char *novelname)
{
      MYSQL *conn = mysql_init(NULL);
    connect(conn);
    char *tmp = new char[600];
    std::string sql = "delete from testshelf where rid=(select id from reader where name='%s') and nid=(select id from novel where name='%s');";
    sprintf(tmp, sql.c_str(), username,novelname);
    int result = mysql_query(conn, tmp);

    delete[] tmp;
    mysql_close(conn);
    if (result == 0)
    {
        return true;
    }
    else
        return false;
}
 MYSQL_RES* Litsql::getcatogrylittle()
 {
 MYSQL *conn = mysql_init(NULL);
    connect(conn);
   
    std::string sql = "select name,sex from catogry;";

    mysql_query(conn, "set names utf8;");
    int result = mysql_query(conn, sql.c_str());
    MYSQL_RES *res = mysql_store_result(conn);
    mysql_close(conn);
    if (result == 0)
        return res;
    else
    {
        mysql_free_result(res);
        return NULL;
    }
 }
 bool Litsql::updatechaptercontent(const char *content,const char *chaptername,const int cid)
 {
     MYSQL *conn = mysql_init(NULL);
    connect(conn);
    char tmp[sizeof(Content)+400];
    std::string sql = "update chapter set content='%s',title='%s',wordcout=%d where id=%d";
    sprintf(tmp, sql.c_str(), content,chaptername,strlen(content),cid);
    int result = mysql_query(conn, tmp);
   std::cout<<tmp<<"r";
    delete[] tmp;
    mysql_close(conn);
    if (result == 0)
    {
        return true;
    }
    else
        return false;   
 }
 MYSQL_RES* Litsql::getcatogryid(const char *username)
 {
 MYSQL *conn = mysql_init(NULL);
    connect(conn);
   
    std::string sql = "select catogryid from liketable where rid=(select id from reader where name='%s');";
    char tmp[400];
    sprintf(tmp,sql.c_str(),username);
    mysql_query(conn, "set names utf8;");
    int result = mysql_query(conn, tmp);
    MYSQL_RES *res = mysql_store_result(conn);
    mysql_close(conn);
    if (result == 0)
        return res;
    else
    {
        mysql_free_result(res);
        return NULL;
    }

 }
 MYSQL_RES* Litsql::getcatogrybookthree(const int cid)
 {
      MYSQL *conn = mysql_init(NULL);
    connect(conn);
   
    std::string sql = "select novel.name,word,UNIX_TIMESTAMP(updatetime),state,noveless,catogry.name,month_ticket,recommend_ticket,writer.name,catogry.sex from novel,catogry,writer where novel.catogryid=%d and novel.catogryid= catogry.id and novel.wid=writer.id order by recommend_ticket desc limit 3;";
    char tmp[400];
    sprintf(tmp,sql.c_str(),cid);
    mysql_query(conn, "set names utf8;");
    int result = mysql_query(conn, tmp);
    MYSQL_RES *res = mysql_store_result(conn);
    mysql_close(conn);
    if (result == 0)
        return res;
    else
    {
        mysql_free_result(res);
        return NULL;
    }
 }
 bool Litsql::deletechapter(const int cid)
 {
        MYSQL *conn = mysql_init(NULL);
    connect(conn);
    char *tmp = new char[600];
    std::string sql = "delete from chapter where id =%d;";
    sprintf(tmp, sql.c_str(), cid);
    int result = mysql_query(conn, tmp);

    delete[] tmp;
    mysql_close(conn);
    if (result == 0)
    {
        return true;
    }
    else
        return false;
 }
 bool Litsql::deletenovelbook(const char *novelname)
 {
        MYSQL *conn = mysql_init(NULL);
    connect(conn);
    char *tmp = new char[600];
    std::string sql = "delete from novel where name='%s';";
    sprintf(tmp, sql.c_str(),novelname);
    int result = mysql_query(conn, tmp);

    delete[] tmp;
    mysql_close(conn);
    if (result == 0)
    {
        return true;
    }
    else
        return false;
 }
  bool Litsql::updatenovelimformation(const char *novelname,const char *novelless,const char *type,const char *sex,const char *state)
  {
    MYSQL *conn = mysql_init(NULL);
    connect(conn);
    char *tmp = new char[600];
    std::string sql = "update novel set noveless='%s',catogryid=(select id from catogry where name='%s' and sex='%s'),state='%s' where name='%s';";
    sprintf(tmp, sql.c_str(),novelless,type,sex,state,novelname);
    int result = mysql_query(conn, tmp);

    delete[] tmp;
    mysql_close(conn);
    if (result == 0)
    {
        return true;
    }
    else
        return false;
  }
 bool Litsql::updatechapterimfotmation(const char *title,const char *content,const int cid)
 {
    

      MYSQL *conn = mysql_init(NULL);
    connect(conn);
    std::string sql = "update chapter set title='";
     std::string sql1="',content='";
    std::string sql2=  "',wordcout=";
    std::string sql3=" where id=";
    std::string sql4=";";
    sql=sql+title;
    sql=sql+sql1;
    sql=sql+content;
    sql=sql+sql2;
    sql=sql+std::to_string(strlen(content));
    sql=sql+sql3;
    sql=sql+std::to_string(cid);
    sql=sql+sql4;
    int result = mysql_query(conn, sql.c_str());

    if (result == 0)
    { 
         char ttp[400];
    sql="call updatechapterbytemoneys(%d);";
    sprintf(ttp,sql.c_str(),cid);
        result=mysql_query(conn,ttp);
            mysql_close(conn);
        if(result==0)
        return true;
        else 
        return false;
    }
    else
    {
        std::cout<<mysql_error(conn);
            mysql_close(conn);
        return false;
    }
 }
 bool Litsql:: enablechapter(int index,int sfflag)
 {
     const char raw[]="update chapter set showstate=%d where index=%d";
    char sql[1024];
    sprintf(sql,raw,sfflag,index);
    MYSQL *conn = mysql_init(NULL);
    connect(conn);
    int result= mysql_query(conn,sql);
    
    mysql_close(conn);
    if(result!=0)
    return false;

    return true;

 }
MYSQL_RES *Litsql::getendbooklist()
{
    MYSQL *conn = mysql_init(NULL);
    connect(conn);
    char *tmp = new char[400];
    std::string sql ;
  
    sql = "select novel.name,word,novel.icon,state,noveless,writer.name,catogry.name from novel,writer,catogry where state='完本' and novel.wid=writer.id and novel.catogryid=catogry.id order by novel.month_ticket desc;";
   
    sprintf(tmp, sql.c_str());
    int result = mysql_query(conn, tmp);
    MYSQL_RES *res = mysql_store_result(conn);
    delete[] tmp;
    mysql_close(conn);
    if(result!=0)
    {
        mysql_free_result(res);
        return NULL;
    }
    return res;
}
 MYSQL_RES*Litsql::getfreebooklist()
{
  MYSQL *conn = mysql_init(NULL);
    connect(conn);
    char *tmp = new char[400];
    std::string sql ;
  
    sql = "select novel.name,word,novel.icon,state,noveless,writer.name,catogry.name from novel,writer,catogry where novel.id not in(select nid from contact) and novel.wid=writer.id and novel.catogryid=catogry.id order by novel.month_ticket desc;";
   
    sprintf(tmp, sql.c_str());
    int result = mysql_query(conn, tmp);
    MYSQL_RES *res = mysql_store_result(conn);
    delete[] tmp;
    mysql_close(conn);
       if(result!=0)
    {
        mysql_free_result(res);
        return NULL;
    }
    return res;
}
MYSQL_RES *Litsql::search(const char *bookname)
{
      MYSQL *conn = mysql_init(NULL);
    connect(conn);
    char *tmp = new char[600];

    std::string sql;
  
    sql = "select novel.name,word,novel.icon,state,noveless,writer.name,catogry.name from novel,writer,catogry where novel.name like '%";
   std::string sql_w= "%' and novel.wid=writer.id and novel.catogryid=catogry.id;";
   std::string sql_tmp=bookname;
   sql=sql+sql_tmp+sql_w;
    strcpy(tmp, sql.c_str());
    int result = mysql_query(conn, tmp);
    MYSQL_RES *res = mysql_store_result(conn);
    delete[] tmp;
    mysql_close(conn);
       if(result!=0)
    {
        mysql_free_result(res);
        return NULL;
    }
    return res;
}
MYSQL_RES* Litsql::getreadhistory(const char *username)
{
   MYSQL *conn = mysql_init(NULL);
    connect(conn);
    char *tmp = new char[400];
    std::string sql ;
  
    sql = "select novel.name,novel.word,novel.state,writer.name,UNIX_TIMESTAMP(readstate.readtime),chapter.title from readstate,writer,chapter,novel  where novel.id =readstate.nid and novel.wid=writer.id and chapter.id=readstate.cid and readstate.rid=(select id from reader where name='%s') order by readstate.readtime desc;";
   
    sprintf(tmp,sql.c_str(), username);
    int result = mysql_query(conn, tmp);
    MYSQL_RES *res = mysql_store_result(conn);
    delete[] tmp;
    mysql_close(conn);
    if(result!=0)
    {
        mysql_free_result(res);
        return NULL;
    }
    return res;
}
bool Litsql:: shelfinventory(const char *name,int tyflag,int sfflag)
{
const char book[]="update novel set showstate=%d where name='%s'";
const char author[]="update novel set showstate=%d where wid=(select id from writer where name='%s');";
char sql_b[1024];
if(tyflag==AUTHOR_SHELF)
{
sprintf(sql_b,author,sfflag,name);
}
else if(tyflag==BOOK_SHELF)
{
sprintf(sql_b,book,sfflag,name);
}
else {
return false;

}

   MYSQL *conn = mysql_init(NULL);
    connect(conn);
    int result=mysql_query(conn,sql_b);
    mysql_close(conn);
    if(result!=0)
    {
      return false;

    }
    return true;
}
// MYSQL_RES *Litsql::selectscore(Score score)
// {
//     MYSQL *conn=mysql_init(NULL);
//     connect(conn);
//     int value;
//     int dvalue;
//     std::string batch;
//     for(int i=0;i<2;i++)
//     {
//         if(strcmp(score.type_class,type_class[i])==0)
//         {
//             if(score.score>=onebatchrank[i])
//     {
//         batch="本科一批";
//         value=score.score-onebatchrank[i];
//     }
//     else if(score.score>=secondbatchrank[i])
//     {
//         batch="本科二批";
//         value=score.score-secondbatchrank[i];
//     }
//     else if(score.score>=thirdbatchrank[i])
//     {
//         batch="本科三批";
//         value=score.score-thirdbatchrank[i];
//     }
//     else if(score.score>=techlogyrank[i])
//     {
//         batch="专科批";
//         value=score.score-techlogyrank[i];
//     }
//         }
//     }
//     char *tmp=new char[400];
//     std::string sql="select * from score.score where ((dvalue between %d-10 and %d+2) or(rank between %d-15000 and %d+15000))and subject='%s' and batch='%s'and 2020-year<=2;";
//     sprintf(tmp,sql.c_str(),value,value,score.rank,score.rank,score.type_class,batch.c_str());
//     mysql_query(conn,"set names utf8");
//     mysql_query(conn,tmp);
//     MYSQL_RES *res=mysql_store_result(conn);
//     // MYSQL_ROW row;
//     // while(row=mysql_fetch_row(res))
//     // {
//     //     if(row==NULL)
//     //        break;
//     //     for(int i=0;i<mysql_num_fields(res);i++)
//     //     std::cout<<row[i];
//     //     std::cout<<std::endl;
//     // }
//     // std::cout<<tmp;
//     delete[] tmp;
//     tmp=NULL;
//     mysql_close(conn);
//    return res;
// }
// MYSQL_RES* Litsql::selectSchool(char schoolname[])
// {
//     char *data=new char[200];
//     std::string sql="select * from score.score where Schoolname='%s'";
//     sprintf(data,sql.c_str(),schoolname);
//     MYSQL *conn=mysql_init(NULL);
//     Litsql::connect(conn);
//     mysql_query(conn,"set names utf8");
//     mysql_query(conn,data);
//     MYSQL_RES *res=mysql_store_result(conn);
//     mysql_close(conn);
//     return res;
// }
// std::string Litsql::setsql(std::string sql)
// {
//     int position=0;
//     int preposi=position;
//     std::string tmp;
//     int i=0;
//     while((position=sql.find("%",position))!=std::string::npos)
//     {
//         char *data=new char[500];
//         int tmp_ttt;
//         if((tmp_ttt=sql.find('%',position+1))!=std::string::npos)
//                position=tmp_ttt-1;
//         else if((tmp_ttt=sql.find('\(',position))!=std::string::npos)
//         {
//             int ytmp=sql.find('\)',position);
//             position=ytmp;
//         }
//         else{
//             position=sql.length();
//         }
//         tmp+=sql.substr(preposi,position-preposi+1);
//         preposi=position+1;
//         if(parame[i]->flag=="i")
//         sprintf(data,tmp.c_str(),parame[i]->a);
//         else if(parame[i]->flag=="f")
//         {
//             sprintf(data,tmp.c_str(),parame[i]->c);
//         }
//         else if(parame[i]->flag=="d")
//         sprintf(data,tmp.c_str(),parame[i]->d);
//         else if(parame[i]->flag=="l")
//         sprintf(data,tmp.c_str(),parame[i]->e);
//         tmp=data;
//         delete[] data;
//         i++;
//     }
//     std::cout<<tmp;
//     return tmp;
// }