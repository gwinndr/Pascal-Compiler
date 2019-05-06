// Testing for subprograms
program boo( input, output );
 var x, y: integer;
   procedure baz(x, y: integer);
    var a, b, c: integer;
    begin
      a := x * y + 1;
      b := a * 2;
      c := a + b;
      write(b)
    end;
begin
 read(x);
 read(y);
 baz(x, y)
end.
