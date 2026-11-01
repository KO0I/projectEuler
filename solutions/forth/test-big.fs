\ Self-contained tests for the Forth dynamic-limb big integer implementation.
include solutions/forth/big.fs
: equals? ( actual expected -- ) <> abort" Forth bignum assertion failed" ;

1 bnew constant big-a
1 bnew constant big-b
1 bnew constant big-result
9999 big-a bset-u
1 big-b bset-u
big-a big-b big-result badd
big-result bdecimal-length 5 equals?
big-result bdigitsum 1 equals?
10000 big-a bset-u
1 big-b bset-u
big-a big-b bsubabs!
big-a bdecimal-length 4 equals?
big-a bdigitsum 36 equals?

9999 big-a bset-u
9999 big-b bset-u
big-a big-b big-result bmul
big-result bdecimal-length 8 equals?
big-result bdigitsum 36 equals?

5 big-a bset-u
-1 big-a bsign!
12 big-b bset-u
big-a big-b big-result badd
big-result bdigitsum 7 equals?
big-result bsign@ 1 equals?

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

big-a bfree big-b bfree big-result bfree
bye
