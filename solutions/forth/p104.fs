\ Project Euler 104: Pandigital Fibonacci Ends.
require math.fs

: bceil-div10! { big } ( bigint -- )
  10 big bdiv-small! 0<> if 1 big badd-small! then ;

: bsmall-u { big } ( bigint <=9 digits -- n )
  0 big blen@ 0 do
    i big blimb@ i 4 * pow10 * +
  loop ;

: bleading-nine 0 { big temp } ( bignum -- 9-digit integer )
  12 bnew to temp big temp bcopy
  begin temp bdecimal-length 9 > while 10 temp bdiv-small! drop repeat
  temp bsmall-u temp bfree ;

: p104 ( -- n )
  12 bnew 12 bnew 12 bnew 12 bnew 12 bnew 12 bnew 12 bnew
  1 1 2 0 0 0 0
  { low-a low-b low-next high-a high-b high-next temp tail-a tail-b index result old low-head high-head }
  1 low-a bset-u 1 low-b bset-u
  1 high-a bset-u 1 high-b bset-u
  begin
    tail-a tail-b + 1000000000 mod to result
    tail-b to tail-a result to tail-b
    index 1+ to index
    low-a low-b low-next badd
    high-a high-b high-next badd
    low-a to old low-b to low-a low-next to low-b old to low-next
    high-a to old high-b to high-a high-next to high-b old to high-next
    low-b bdecimal-length 40 > if
      10 low-a bdiv-small! drop 10 low-b bdiv-small! drop
      high-a bceil-div10! high-b bceil-div10!
    then
    tail-b pandigital-nine? if
      low-b bleading-nine to low-head
      high-b bleading-nine to high-head
      low-head high-head = low-head pandigital-nine? and if
        low-a bfree low-b bfree low-next bfree
        high-a bfree high-b bfree high-next bfree temp bfree
        index exit
      then
    then
    index 1000000 > abort" Pandigital Fibonacci solution not found"
  again ;

[undefined] euler-testing [if] p104 . cr bye [then]
