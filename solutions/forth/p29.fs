\ Project Euler 29: Distinct Powers.
require math.fs

: primitive-root 0 0 0 0 { n base power exp current-exp } ( n -- root exp )
  n to base 1 to exp
  n 1+ 2 ?do i to power 1 to current-exp
    begin power n < while power i * to power current-exp 1+ to current-exp repeat
    power n = if i to base current-exp to exp leave then
  loop base exp ;

create power-seen 61000 allot

: p29 ( -- answer )
  power-seen 61000 erase 0 0 0 0 { count root exp key }
  101 2 do i primitive-root to exp to root
    101 2 do root 601 * exp i * + to key
      power-seen key + c@ 0= if 1 power-seen key + c! count 1+ to count then
    loop
  loop count ;

[undefined] euler-testing [if] p29 . cr bye [then]
