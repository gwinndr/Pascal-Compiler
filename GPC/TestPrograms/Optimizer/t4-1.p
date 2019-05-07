program boo( input, output );
 var x, y: integer;
  procedure baz;
    var x, y: integer;
    begin
      x := 100 * 2 + (5 - 3);
      y := 100 * 100 + 25 * 4;

      write(x);
      write(y)
    end;
begin
  x := 50 * 2 + (5 - 3);
  y := 10 * 10 + 25 * 4;

 write(x);
 write(y);
 baz
end.
