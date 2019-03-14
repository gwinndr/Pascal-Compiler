program boo( input, output );
 var n,i,sum: integer;
 var a: array[1 .. 10] of integer;
begin
 read(n);
 i := 1;
 while (i < n) do
 begin
   a[i] := i;
   write(a[i]);
   i := i + 1
 end;
 write(n)
end.

