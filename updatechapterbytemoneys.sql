 create procedure updatechapterbytemoneys(cid int)
     begin
     declare result int;
     select count(id) into result from contact where nid=(select nid from chapter where id=cid);
     if result>0 then update chapter set needbytes=(wordcout/1000)*(select thoudswordmoney from rule) where id=cid;
     end if;
    
end;$