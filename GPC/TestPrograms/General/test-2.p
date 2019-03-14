(* this is a sample test quiz Pascal program *)
(* date: March 23, 2006 *)

program main( input, output );

 var x,y: integer;
 var p,q: real;
 var a: array [ 1..10 ] of integer;
 var b: array [ 10..15 ] of real;

 function foo( i: integer; u: real; j: integer; v: real ) : integer;

  var k: integer;
  var w: real;

  procedure boo( r: real; s: integer );
  begin (* boo *)
   a[s] := j
  end; (* boo *)

 begin (* foo *)
	boo( b[a[x+y]+i], foo( i,u,j,v ));
	foo := foo( a[y-y], b[20+x*x], a[x-y], q-p )
 end; (* foo *)

begin
 y := 5;
 for x := 1 to ( 2 * y + a[x] ) do
 begin
 	a[x] := foo( 1, b[x]+b[y], x, b[a[10]] );
	if ( (x <= 5) and (x > 0) and (x < y) ) then
	  if ( a[x] < foo( a[1], 3.14, y+x*a[3], 2.13 )) then
		y := a[x]
	  else
		y := foo( a[x]+1, b[a[x]], a[a[x+1]], b[13+foo(1,2.3,4,5.6)] )
 end
end.

