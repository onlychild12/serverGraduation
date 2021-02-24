 create procedure updatelikecatogry(username varchar(45),sexp varchar(45),title varchar(45))
     begin
     declare result int;
     delete from liketable where rid=(select id from reader where name=username) and catogryid=(select id from catogry where sex=sexp and name=title)
    
end;$