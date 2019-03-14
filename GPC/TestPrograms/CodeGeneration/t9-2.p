program boo( input, output );
 var m,n: integer;
 function gcd(a,b: integer):integer;
 begin
	if (b = 0) then
		gcd := a 
	else
		gcd := gcd(b, a mod b)
 end;
begin
 read(m);
 read(n);
 m := gcd(m,n);
 write(m)
end.

