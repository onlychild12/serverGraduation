create procedure updatemonthticket()
begin
update reader,monthtotal set monthticket=monthgive/10000,monthgive=0 where monthtotal.rid=reader.id;
end;
create event if not exists updatemonth
on schedule every 30 second 
on completion PRESERVE
do call updatemonthticket();


create event if not exists updatemonth
on schedule every 30 second 
on completion PRESERVE
do call updatemonthticket();