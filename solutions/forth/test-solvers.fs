\ Minimal cell-stack tester using the standard T{ ... -> ... }T convention.
include solvers.fs
variable test-start
variable actual-count
create actual-stack 128 cells allot
: T{ ( -- ) depth test-start ! ;
: -> ( results -- )
  depth test-start @ - dup actual-count !
  dup 128 > abort" too many test results"
  0 ?do actual-stack i cells + ! loop ;
: }T ( expected -- )
  depth test-start @ - actual-count @ <> abort" wrong result count"
  actual-count @ 0 ?do
    actual-stack i cells + @ <> abort" wrong result value"
  loop ;
create prefix-test 2 cells allot
: countdown-product ( -- u ) 1 1 3 do i * -1 +loop ;
: run-helper-tests ( -- )
  T{ 1 cells -> 8 }T
  T{ 0 is-prime? -> false }T
  T{ 1 is-prime? -> false }T
  T{ 2 is-prime? -> true }T
  T{ 49 is-prime? -> false }T
  T{ 97 is-prime? -> true }T
  T{ 0 integer-sqrt -> 0 }T
  T{ 1 integer-sqrt -> 1 }T
  T{ 2 integer-sqrt -> 1 }T
  T{ 4 integer-sqrt -> 2 }T
  T{ 5 integer-sqrt -> 2 }T
  T{ 9999 integer-sqrt -> 99 }T
  T{ 10000 integer-sqrt -> 100 }T
  T{ 2147395600 integer-sqrt -> 46340 }T
  T{ 4150 5 digit-pow-sum -> 4150 }T
  T{ 0 5 digit-pow-sum -> 0 }T
  T{ 999 5 digit-pow-sum -> 177147 }T
  T{ 28 divisor-count -> 6 }T
  T{ 1 proper-divisor-sum -> 0 }T
  T{ 220 proper-divisor-sum -> 284 }T
  T{ 342 count-letters -> 23 }T
  T{ 115 count-letters -> 20 }T
  T{ 0 pentagonal? -> false }T
  T{ 1 pentagonal? -> true }T
  T{ 5 pentagonal? -> true }T
  T{ 12 pentagonal? -> true }T
  T{ 2 pentagonal? -> false }T
  T{ countdown-product -> 6 }T
  12345 0 prefix-test prefix!
  67890 1 prefix-test prefix!
  T{ 0 prefix-test prefix@ -> 12345 }T
  T{ 1 prefix-test prefix@ -> 67890 }T
  T{ 3 4 5 multiply-mod -> 2 }T
  T{ 2 10 1000 power-mod -> 24 }T ;
run-helper-tests
.( Forth solver-helper regression tests passed.) cr
bye
