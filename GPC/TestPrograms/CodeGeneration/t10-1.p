program boo( input, output );
 var n,i: integer;
 var a: array[1 .. 10] of integer;
 function foo( n: integer ) : integer;
 begin
	if ( n <= 1 ) then
		foo := 1
	else
		foo := foo( n-1 ) * n
 end;
begin
 read(n);
 i := 1;
 while ( i <= n ) do
 begin
	a[i] := i;
	i := i + 1
 end;
 i := 1;
 while ( i <= n ) do
 begin
	a[i] := foo(a[i]+1);
	i := i + 1
 end;
 i := 1;
 while ( i <= n ) do
 begin
	write(a[i]);
	i := i + 1
 end
end.

