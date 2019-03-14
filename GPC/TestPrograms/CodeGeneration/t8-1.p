program outer( input, output );

  var c,b,a: integer;

  procedure middle(x: integer);
    procedure alpha(x: integer);
    begin
		a := 42
    end;

    procedure beta(x: integer);
    begin
		alpha(x)
    end;

  begin
    beta(0)
  end;

begin
  a := 0;
  middle(5);
  write(a)
end.

