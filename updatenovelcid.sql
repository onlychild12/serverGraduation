 create procedure updatecid(novelname varchar(45),words int)
     begin
     declare result int;
     select max(id) into result from chapter where nid=(select id from novel where name=novelname);
     update novel set cid=result,word=word+words,updatetime=now() where name=novelname; 
    
end;$