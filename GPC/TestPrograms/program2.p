(*
Damon Gwinn
Testing out type semantics
*)
program main(input, output);
    var a,b: integer;
    var c,d: real;
begin
    (* Should pass *)
    a := 5;
    b := 3;

    (* Should fail *)
    a := 5.0;
    a := c + a;
    b := 3.0
end.
