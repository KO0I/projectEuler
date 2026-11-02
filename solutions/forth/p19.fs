\ Project Euler 19: Counting Sundays.
require math.fs

: leap? { year } ( year -- flag )
  year 4 mod 0= year 100 mod 0<> year 400 mod 0= or and ;

create month-days 31 , 28 , 31 , 30 , 31 , 30 , 31 , 31 , 30 , 31 , 30 , 31 ,

: p19 ( -- answer )
  1 0 0 { weekday answer days }
  2001 1900 do
    12 0 do
      j 1901 >= weekday 0= and if answer 1+ to answer then
      month-days i cells + @ to days
      i 1 = j leap? and if days 1+ to days then
      weekday days + 7 mod to weekday
    loop
  loop answer ;

[undefined] euler-testing [if] p19 . cr bye [then]
