 create procedure addfan(username varchar(45),novelname varchar(45),byte int)
     begin
     declare result int;
   select count(id) into result from fanvalue where rid=(select id from reader where name=username) and nid=(select id from novel where name=novelname);
     if result >0 then update fanvalue set value=value+byte where rid=(select id from reader where name=username) and nid=(select id from novel where name=novelname); 
   else insert into fanvalue(rid,nid,value) values((select id from reader where name=username),(select id from novel where name=novelname),byte);
   end if;
end;$