\ Self-contained regression tests; control structures run inside a definition.
include big.fs
: equals? ( actual expected -- ) <> abort" Forth bignum assertion failed" ;

1 bnew constant big-a
1 bnew constant big-b
1 bnew constant big-result

: run-big-tests ( -- )
  depth >r
  9999 big-a bset-u 1 big-b bset-u
  big-a big-b big-result badd
  big-result bdecimal-length 5 equals?
  big-result bdigitsum 1 equals?
  10000 big-a bset-u
  big-a big-b bsubabs!
  big-a bdecimal-length 4 equals?
  big-a bdigitsum 36 equals?

  9999 big-a bset-u 9999 big-b bset-u
  big-a big-b big-result bmul
  big-result bdecimal-length 8 equals?
  big-result bdigitsum 36 equals?
  9999 big-result bdiv-small! 0 equals?
  big-result big-a babscompare 0 equals?

  5 big-a bset-u -1 big-a bsign!
  12 big-b bset-u
  big-a big-b big-result badd
  big-result bdigitsum 7 equals?
  big-result bsign@ 1 equals?
  12 big-a bset-u -1 big-a bsign!
  5 big-b bset-u
  big-a big-b big-result badd
  big-result bdigitsum 7 equals?
  big-result bsign@ -1 equals?
  12 big-b bset-u
  big-a big-b big-result badd
  big-result blen@ 0 equals?
  big-result bsign@ 0 equals?

  1 big-a bset-u
  1000 0 do 2 big-a bmul-small! loop
  big-a bdecimal-length 302 equals?
  big-a bdigitsum 1366 equals?
  1 big-b bset-u
  101 2 do i big-b bmul-small! loop
  big-b bdigitsum 648 equals?

  s" 123456789012345678901234567890" big-result bfrom-dec
  big-result bdecimal-length 30 equals?
  big-result bdigitsum 135 equals?
  big-result big-a bcopy
  big-result big-a bsubabs!
  big-result blen@ 0 equals?
  big-result bsign@ 0 equals?

  \ Grow dynamically to 1201 digits, then divide back to one.
  1 big-a bset-u
  1200 0 do 10 big-a bmul-small! loop
  big-a bdecimal-length 1201 equals?
  big-a bdigitsum 1 equals?
  big-a blen@ 301 equals?
  1200 0 do 10 big-a bdiv-small! 0 equals? loop
  big-a bdecimal-length 1 equals?
  big-a bdigitsum 1 equals?
  0 big-a bmul-small!
  big-a blen@ 0 equals?
  big-a bsign@ 0 equals?
  depth r> equals? ;

run-big-tests
big-a bfree big-b bfree big-result bfree
.( Forth big-integer regression tests passed.) cr
bye
