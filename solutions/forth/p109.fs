\ Project Euler 109: Darts.
require math.fs

: p109 ( -- n )
  62 cells allocate throw 0 0 0 0 0 { scores count total finish last value }
  4 1 do
    21 1 do i j * count scores prefix! count 1+ to count loop
  loop
  25 count scores prefix! 50 count 1+ scores prefix! 62 to count
  22 1 do
    i 21 = if 50 else i 2* then to last
    total 1+ to total
    count 0 do
      i scores prefix@ last + 100 < if total 1+ to total then
      count i do
        i scores prefix@ j scores prefix@ + last + 100 < if total 1+ to total then
      loop
    loop
  loop
  scores free throw total ;

[undefined] euler-testing [if] p109 . cr bye [then]
