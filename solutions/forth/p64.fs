\ Project Euler 64: Odd Period Square Roots.
require math.fs

: p64 ( -- n ) 0 0 0 0 0 0 { total root m d a period }
  10001 2 do i integer-sqrt to root
    root root * i <> if
      0 to m 1 to d root to a 0 to period
      begin d a * m - to m i m m * - d / to d
        root m + d / to a period 1+ to period
        a root 2* = until
      period 2 mod if total 1+ to total then
    then
  loop total ;

[undefined] euler-testing [if] p64 . cr bye [then]
