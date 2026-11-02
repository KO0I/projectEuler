\ Project Euler 96: Su Doku.
require math.fs

: p96-search 0 0 0 0 0 0 0 0 0 0 0
  { board rows cols boxes choice candidates few row col box digit bit mask count old } ( board rows cols boxes -- solved? )
  -1 to choice 10 to few
  81 0 do
    i board prefix@ 0= if
      i 9 / to row i 9 mod to col
      row 3 / 3 * col 3 / + to box
      row rows prefix@ col cols prefix@ or box boxes prefix@ or invert 1022 and to mask
      mask bit-count to count
      count 0= if false unloop exit then
      count few < if count to few i to choice mask to candidates then
    then
  loop
  choice -1 = if true exit then
  choice 9 / to row choice 9 mod to col
  row 3 / 3 * col 3 / + to box
  10 1 do
    1 i lshift to bit
    candidates bit and if
      i choice board prefix!
      row rows prefix@ bit or row rows prefix!
      col cols prefix@ bit or col cols prefix!
      box boxes prefix@ bit or box boxes prefix!
      board rows cols boxes recurse if true unloop exit then
      0 choice board prefix!
      row rows prefix@ bit xor row rows prefix!
      col cols prefix@ bit xor col cols prefix!
      box boxes prefix@ bit xor box boxes prefix!
    then
  loop false ;

: p96 ( -- n )
  s" tests/data/p096_sudoku.txt" 5000 read-fixture
  81 cells allocate throw 9 cells allocate throw 9 cells allocate throw 9 cells allocate throw
  0 0 0 0 0 0 0
  { raw length board rows cols boxes answer offset ch row col square bit }
  length 4899 <> length 4900 <> and abort" Sudoku fixture has unexpected length"
  50 0 do
    rows 9 cells erase cols 9 cells erase boxes 9 cells erase
    i 98 * 8 + to offset
    9 0 do i to row
      9 0 do i to col
        raw offset + row 10 * + col + c@ [char] 0 - to ch
        ch row 9 * col + board prefix!
        ch 0> if
          1 ch lshift to bit
          row rows prefix@ bit or row rows prefix!
          col cols prefix@ bit or col cols prefix!
          row 3 / 3 * col 3 / + to square
          square boxes prefix@ bit or square boxes prefix!
        then
      loop
    loop
    board rows cols boxes p96-search 0= abort" Sudoku board is unsolvable"
    0 board prefix@ 100 * 1 board prefix@ 10 * + 2 board prefix@ + answer + to answer
  loop
  raw free throw board free throw rows free throw cols free throw boxes free throw answer ;

[undefined] euler-testing [if] p96 . cr bye [then]
