(*
Damon Gwinn
Program that computes c = a+b then d = c * a
Good test for functions and procedures
*)
program main(input, output);
    var a,b,c,d: integer;

    (* Adds numbers *)
    function Add(a: integer; b: integer) : integer;
    begin
        Add := a + b
    end;

    (* Multiplies numbers *)
    function Mult(c: integer; a: integer) : integer;
    begin
        Mult := c * a
    end;

    (* Using this to test procedures *)
    procedure multadd;
    begin
        c := Add(a, b);
        d := Mult(c, a)
    end;

begin
    a := 5;
    b := 3;
    multadd;
    while (a > b) do
      b := b + 1
    (*writeln(c);
    writeln(d)*)
end.
