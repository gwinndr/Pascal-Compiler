program main(input, output);
    var a,b: integer;
    var x,y,z: real;

    (* Functions return stuff *)
    (* Illegal to change non-local variables *)
    (* Can call procedure from function, but procedure must abide by the no-change rule *)
    function foo(b: integer) : integer;
    begin
        foo := 2*b;
    end;

    (* Procedures do not return stuff *)
    (* Procedures can edit non-local variables *)
    procedure boo(p: integer; q: real);
        var a: real;
        var z: integer;
    begin
        writeln(p + 13); (* Semicolons are seperaters, don't need one below this *)
        writeln(q)
    end;

begin
    boo(13, 3.14)
end.
