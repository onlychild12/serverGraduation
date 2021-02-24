
 create procedure search(bookname varchar(25))
     begin
    select novel.name,word,novel.icon,state,noveless,writer.name,catogry.name from novel,writer,catogry where novel.name like '%'+bookname+'%' and novel.wid=writer.id and novel.catogryid=catogry.id;
end;$