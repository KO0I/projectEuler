\ Project Euler 43: Sub-string Divisibility.
require math.fs

: p43-search 0 0 { position used value total digit } ( position used number -- sum )
  position 10 = if value exit then
  10 0 do
    i to digit
    position 0<> digit 0<> or
    1 digit lshift used and 0= and if
      value 10 * digit +
      position 3 >= if
        dup 1000 mod
        position 3 - case
          0 of 2 endof 1 of 3 endof 2 of 5 endof
          3 of 7 endof 4 of 11 endof 5 of 13 endof 6 of 17 endof
        endcase mod 0<>
      else false then
      if drop else
        position 1+ used 1 digit lshift or rot recurse total + to total
      then
    then
  loop total ;

: p43 ( -- n ) 0 0 0 p43-search ;

[undefined] euler-testing [if] p43 . cr bye [then]
