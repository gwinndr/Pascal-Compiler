// All declarations should be removed
program boo( input, output );
 var x, y: integer;
    function foo(a, b: integer) : integer;
      var c, d: integer;
        begin
          foo := a+b;
          c := b * a;
          d := b + a;
          c := c * 2;
          d := (d * 2) * (d * 2);
          d := c * 4
        end;
begin
 x := 20;
 x := x * x;
 y := 3;
 y := y + x - 2 * 8;
 write(1)
end.
