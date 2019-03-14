program boo( input, output );
 var m,n: integer;
 function factorial(a: integer):integer;
 begin
	if (a <= 1) then
		factorial := 1
	else
		factorial := a * factorial(a-1)
 end;
begin
 read(n);
 m := factorial(n);
 write(m)
end.

