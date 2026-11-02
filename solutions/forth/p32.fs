\ Project Euler 32: Pandigital Products.
require math.fs

: p32 ( -- n )
  10000 allocate throw 0 0 0 { seen total product joined }
  seen 10000 erase
  100 1 do
    10000 i / 1+ i 1+ do
      i j * to product
      j i append-number product append-number to joined
      joined 100000000 >= joined 999999999 <= and if
        joined pandigital-nine? seen product + c@ 0= and if
          1 seen product + c! total product + to total
        then
      then
    loop
  loop seen free throw total ;

[undefined] euler-testing [if] p32 . cr bye [then]
