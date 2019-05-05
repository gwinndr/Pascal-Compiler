program boo( input, output );
 var x,y,z: integer;
   procedure bar(a,b,c: integer);
    procedure baz(a: integer);
      begin
        write(a)
      end;
   begin
  	write(a);
  	write(b);
  	write(c);
    baz(a*b*c)
   end;

   procedure foo(a,b: integer);
    begin
      write(a*b)
    end;

begin
 read(x);
 read(y);
 read(z);
 foo(x,y);
 bar(x,y,z)
end.
