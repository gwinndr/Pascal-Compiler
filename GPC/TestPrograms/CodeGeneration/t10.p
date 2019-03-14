program boo( input, output );
 var n,i: integer;
 var a: array[1 .. 10] of integer;
begin
 read(n);
 i := 0;
 while ( i <= n ) do
 begin
	a[i] := i*i;
	i := i + 1
 end;
 i := 0;
 while ( i <= n ) do
 begin
	write(a[i]);
	i := i + 1
 end
end.

