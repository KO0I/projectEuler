\ Project Euler 119: Digit Power Sum.
require math.fs

: p119-digit-sum { number } ( n -- sum )
  0 begin number 0> while number 10 mod + number 10 / to number repeat ;

: p119-sort 0 0 { values count value position } ( array n -- )
  count 1 do
    i values prefix@ to value
    i to position
    begin position 0> if position 1- values prefix@ value > else false then while
      position 1- values prefix@ position values prefix!
      position 1- to position
    repeat
    value position values prefix!
  loop ;

: p119 ( -- n )
  12000 cells allocate throw 0 100 0 0 0 0
  { values count limit base value result digit-count }
  16 2 do
    i to digit-count
    0 to count
    digit-count 9 * 1+ 2 do
      i to base base base * to value
      begin value limit < while
        value 10 >= if value p119-digit-sum base = if
          count 12000 >= abort" Digit-power candidates capacity exceeded"
          value count values prefix! count 1+ to count
        then then
        value limit 1- base / > if limit to value
        else value base * to value then
      repeat
    loop
    count 30 >= if
      values count p119-sort
      0 to result
      count 0 do
        i 0= if true else
          i values prefix@ i 1- values prefix@ <> then
        if result 1+ to result then
        result 30 = if i values prefix@ values free throw unloop unloop exit then
      loop
    then
    limit 10 * to limit
  loop values free throw abort" Missing digit-power term" ;

[undefined] euler-testing [if] p119 . cr bye [then]
