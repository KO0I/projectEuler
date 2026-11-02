\ Project Euler 11: Largest Product in a Grid.
require math.fs

: grid-product 0 0 0 { grid x y dx dy xx yy product } ( grid x y dx dy -- product )
  x dx 3 * + to xx y dy 3 * + to yy
  xx 0< xx 20 >= or yy 0< or yy 20 >= or if 0 exit then
  1 to product
  4 0 do
    grid y i dy * + 20 * x i dx * + + cells + @ product * to product
  loop product ;

: p11 ( -- answer )
  s" tests/data/p011.txt" 4096 read-fixture
  0 0 0 0 0 { buf length grid count value active best }
  400 cells allocate throw to grid
  0 to count 0 to value false to active
  length 0 do
    buf i + c@ dup [char] 0 >= over [char] 9 <= and if
      [char] 0 - value 10 * + to value true to active
    else
      drop
      active if value grid count cells + ! count 1+ to count
        0 to value false to active
      then
    then
  loop
  active if value grid count cells + ! count 1+ to count then
  count 400 <> abort" p011 grid must contain 400 numbers"
  0 to best
  20 0 do 20 0 do
    grid i j 1 0 grid-product best max to best
    grid i j 0 1 grid-product best max to best
    grid i j 1 1 grid-product best max to best
    grid i j -1 1 grid-product best max to best
  loop loop grid free throw buf free throw best ;

[undefined] euler-testing [if] p11 . cr bye [then]
