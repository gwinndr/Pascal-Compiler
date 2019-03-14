program boo( input, output );
 var m,n: integer;
 function sum(a: integer):integer;
 begin
	if (a = 1) then
		sum := 1
	else
		sum := a + sum(a-1)
 end;
begin
 read(n);
 m := sum(n);
 write(m)
end.

