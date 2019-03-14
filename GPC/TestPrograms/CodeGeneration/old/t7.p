program boo( input, output );
 var x,y: integer;
 function foo(a: integer): integer;
 begin
	foo := a + 10
 end;
begin
 read(x);
 y := foo(x);
 write(y)
end.

