 create procedure savereadstate(username varchar(45),chapterid int)
     begin
     declare result int;
   select count(id) into result from readstate where rid=(select id from reader where name=username) and nid=(select nid from chapter where id=chapterid);
     if result >0 then update readstate set cid=chapterid,readtime=now() where rid=(select id from reader where name=username) and nid=(select nid from chapter where id=chapterid); 
   else insert into readstate(rid,nid,cid,readtime) values((select id from reader where name=username),(select nid from chapter where id=chapterid),chapterid,now());
   end if;
end;$