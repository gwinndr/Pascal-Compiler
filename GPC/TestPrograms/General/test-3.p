program main( input, output );
 var a,b,c: integer;
 function foo( x,y: integer ): integer;
 begin
	foo := x + foo( x, y )
 end;
begin
 a := foo( b, c )
end.

