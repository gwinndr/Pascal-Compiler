program boo( input, output );
 var x,y,z: integer;
 procedure bar(a,b,c: integer);
 begin
	write(a);
	write(b);
	write(c)
 end;
begin
 read(x);
 read(y);
 read(z);
 write(x);
 write(y);
 write(z);
 bar(x,y,z)
end.

