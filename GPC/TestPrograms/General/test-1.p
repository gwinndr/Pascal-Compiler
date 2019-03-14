(* this is a sample test quiz Pascal program *)
(* date: March 23, 2006 *)

program main( input, output );

 var x,y,z: integer;
 var p,q: real;
 var a: array [ 1..10 ] of integer;
 var b: array [ 10..15 ] of real;

 function foo( x: integer; c: real; y: integer; b: real ) : integer;

  var p: integer;
  var q: real;

  procedure boo( z: real; k: integer );
  begin (* boo *)
   a[k] := y
  end; (* boo *)

 begin (* foo *)
	boo( b[a[x+y]+1], foo( x,a,y,b ));
	foo := foo( a[k-k], b[20], a[x-y], q )
 end; (* foo *)

begin
 y := 5;
 for x := 1 to (2*y) do
 begin
 	a[x] := foo( 1, b[10], x, b[a[10]] );
	if ( x <= 5 and x > 0 and x < y ) then
	  if ( a[x] < foo( a[1], 3.14, y+z*a[3], 2.13 )) then
		y := a[x]
	  else
		y := foo( a[x]+1, b[a[x]], a[a[x+1]], b[13+foo(1,2.3,4,5.6)] )
  (* ADDED to remove dangling else *)
  (*else
    begin end*)
 end
end.
