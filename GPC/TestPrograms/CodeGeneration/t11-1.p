program boo( input, output );
 var x,y,z,a,b,c: integer;
begin
 a := 1;
 b := 2;
 c := 3;
 x := (((a * (b + c)) + (a * (b + c))) * ((a * (b + c)) + (a * (b + c)))) +
 		(((a * (b + c)) + (a * (b + c))) * ((a * (b + c)) + (a * (b + c))));
 write(x)
end.

