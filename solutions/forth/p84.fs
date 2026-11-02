\ Project Euler 84: Monopoly Odds.
require math.fs

create p84-current 120 floats allot

create p84-next 120 floats allot

create p84-used 40 allot
fvariable p84-maximum

: p84-store { square doubles } ( F: probability -- )
  p84-next square 3 * doubles + floats +
  dup f@ f+ f! ;

: p84-cc { square doubles } ( F: probability -- )
  fdup 16e f/ 0 doubles p84-store
  fdup 16e f/ 10 0 p84-store
  14e f* 16e f/ square doubles p84-store ;

: p84-ch { square doubles } ( F: probability -- )
  fdup 16e f/ 0 doubles p84-store
  fdup 16e f/ 10 0 p84-store
  fdup 16e f/ 11 doubles p84-store
  fdup 16e f/ 24 doubles p84-store
  fdup 16e f/ 39 doubles p84-store
  fdup 16e f/ 5 doubles p84-store
  fdup 8e f/ square 7 = if 15 else square 22 = if 25 else 5 then then doubles p84-store
  fdup 16e f/ square 22 = if 28 else 12 then doubles p84-store
  fdup 16e f/
    square 36 = if 33 doubles p84-cc else square 3 - doubles p84-store then
  6e f* 16e f/ square doubles p84-store ;

: p84-step 0 0 { square doubles roll pair next-square next-doubles } ( square doubles dice-sum same? -- ) ( F: prob -- )
  pair doubles 2 = and if 10 0 p84-store exit then
  pair if doubles 1+ else 0 then to next-doubles
  square roll + 40 mod to next-square
  next-square 30 = if 10 0 p84-store exit then
  next-square 7 = next-square 22 = or next-square 36 = or if
    next-square next-doubles p84-ch exit
  then
  next-square 2 = next-square 17 = or next-square 33 = or if
    next-square next-doubles p84-cc exit
  then
  next-square next-doubles p84-store ;

: p84-square-prob { square } ( F: -- p )
  0e
  3 0 do p84-current square 3 * i + floats + f@ f+ loop ;

: p84 0 0 0 0 0 0 { state square doubles chosen result probability } ( -- 6digit code )
  p84-used 40 erase
  120 0 do 1e 120e f/ p84-current i floats + f! loop
  300 0 do
    p84-next 120 floats erase
    120 0 do
      i to state
      state 3 / to square state 3 mod to doubles
      5 1 do 5 1 do
        p84-current state floats + f@ 16e f/
        square doubles i j + i j = p84-step
      loop loop
    loop
    p84-next p84-current 120 floats move
  loop
  0 to result
  3 0 do
    -1e p84-maximum f!
    -1 to chosen
    40 0 do
      p84-used i + c@ 0= if
        i p84-square-prob fdup p84-maximum f@ f> if
          p84-maximum f! i to chosen
        else fdrop then
      then
    loop
    chosen 0< abort" Failed to rank Monopoly squares"
    1 p84-used chosen + c!
    result 100 * chosen + to result
  loop result ;

\ Problem 98: square mappings are checked as bijections in both directions.

[undefined] euler-testing [if] p84 . cr bye [then]
