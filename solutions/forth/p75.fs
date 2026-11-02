\ Project Euler 75: Singular Integer Right Triangles.
require math.fs

: p75 ( -- n )
  1500001 allocate throw 0 0 0 0 { counts answer perimeter m n }
  counts 1500001 erase
  867 2 do i to m
    m 1 do i to n
      m n - 2 mod 0<> m n integer-gcd 1 = and if
        m m n + * 2* to perimeter
        perimeter 1500000 <= if
          1500001 perimeter do counts i + dup c@ 1+ 2 min swap c! perimeter +loop
        then
      then
    loop
  loop
  1500001 1 do counts i + c@ 1 = if answer 1+ to answer then loop
  counts free throw answer ;

[undefined] euler-testing [if] p75 . cr bye [then]
