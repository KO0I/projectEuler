! Native Factor implementations for additional Project Euler problems.
! The common number-theory, decimal, combinatoric and fixture words are in euler.shared.
USING: arrays assocs combinators hashtables euler.shared io.encodings.utf8 io.files
kernel locals math math.functions math.order math.parser math.primes
ranges sequences sorting splitting strings vectors ;
IN: euler.extra

:: p3 ( -- n )
    600851475143 :> remaining! 2 :> divisor! 1 :> largest!
    [ divisor sq remaining <= ] [
        remaining divisor mod zero?
        [ divisor largest! remaining divisor /i remaining! ]
        [ divisor 2 = [ 3 ] [ divisor 2 + ] if divisor! ] if
    ] while remaining 1 > [ remaining ] [ largest ] if ;

:: p4 ( -- n )
    0 :> best!
    100 999 [a..b] [| a |
        a 999 [a..b] [| b |
            a b * :> product
            product best > product decimal-palindrome? and
            [ product best! ] when
        ] each
    ] each best ;

:: p7 ( -- n )
    1 :> value! 0 :> found!
    [ found 10001 < ] [
        value 1 + value!
        value prime? [ found 1 + found! ] when
    ] while value ;

:: p8 ( -- n )
    "tests/data/p008.txt" utf8 file-contents
    [ dup 48 >= swap 57 <= and ] filter [ 48 - ] map :> digits
    0 :> best!
    0 digits length 13 - [a..b] [| at |
        1 :> product!
        0 12 [a..b] [| delta |
            at delta + digits nth product * product!
        ] each
        product best max best!
    ] each best ;

:: p12-divisors ( input -- count )
    input :> value! 2 :> divisor! 1 :> result!
    [ divisor sq value <= ] [
        value divisor mod zero?
        [ 1 :> exponent!
          [ value divisor mod zero? ] [
            value divisor /i value! exponent 1 + exponent!
          ] while
          result exponent * result!
        ] when divisor 1 + divisor!
    ] while
    value 1 > [ result 2 * ] [ result ] if ;
:: p12 ( -- n )
    1 :> n! 1 :> triangle!
    [ triangle p12-divisors 500 <= ] [
        n 1 + n! triangle n + triangle!
    ] while triangle ;

: p13 ( -- n ) "lsum" read-numbers sum
    number>string 0 10 rot subseq decimal-cell ;

:: p14-collatz ( n -- length )
    n :> current! 1 :> terms!
    [ current 1 > ] [
        current even? [ current 2 /i ] [ current 3 * 1 + ] if current!
        terms 1 + terms!
    ] while terms ;
:: p14 ( -- n )
    1 :> best! 1 :> index!
    2 999999 [a..b] [| n |
        n p14-collatz :> candidate
        candidate best > [ candidate best! n index! ] when
    ] each index ;

:: p19 ( -- n )
    ! 1900-01-01 was Monday (day-of-week 1); advance through months.
    1 :> dow! 0 :> answer!
    1900 2000 [a..b] [| year |
        1 12 [a..b] [| month |
            year 1901 >= dow 0 = and [ answer 1 + answer! ] when
            month { 4 6 9 11 } member?
              [ 30 ] [ month 2 = [
                year 4 mod zero?
                year 100 mod zero? not year 400 mod zero? or and [ 29 ] [ 28 ] if
              ] [ 31 ] if ] if
            :> length
            dow length + 7 mod dow!
        ] each
    ] each answer ;

:: p21 ( -- n )
    9999 divisor-sum-sieve :> sums
    0 :> total!
    2 9999 [a..b] [| a |
        a sums nth :> b
        b 0 > b 10000 < and a b = not and [
            b sums nth a = [ total a + total! ] when
        ] when
    ] each total ;

:: p22 ( -- n )
    "names.txt" read-upper-words sort :> names
    0 :> total!
    names [| name index |
        name [ 64 - ] map sum index 1 + * total + total!
    ] each-index total ;

:: p27 ( -- n )
    0 :> best-length! 0 :> best-product!
    -999 999 [a..b] [| a |
        2 1000 [a..b] [| b |
            b prime? [
                0 :> n!
                [ n sq a n * + b + dup 1 > swap prime? and ] [ n 1 + n! ] while
                n best-length > [ n best-length! a b * best-product! ] when
            ] when
        ] each
    ] each best-product ;

:: p29 ( -- n )
    H{ } clone :> powers
    2 100 [a..b] [| base |
        2 100 [a..b] [| exp |
            t base exp ^ powers set-at
        ] each
    ] each powers assoc-size ;

:: p30 ( -- n )
    0 :> total!
    2 354294 [a..b] [| n |
        n number>string [ 48 - 5 ^ ] map sum n =
        [ total n + total! ] when
    ] each total ;

:: p34 ( -- n )
    0 9 [a..b] [ factorial-number ] map :> facts
    0 :> total!
    3 2540160 [a..b] [| n |
        n number>string [ 48 - facts nth ] map sum n =
        [ total n + total! ] when
    ] each total ;

:: p35-rotate ( number shift -- rotated )
    number decimal-digits shift - pow10 :> power
    number power mod 10 shift ^ * number power /i + ;
:: p35 ( -- n )
    999999 euler-sieve :> primes
    0 :> answer!
    2 999999 [a..b] [| n |
        n primes nth [
            t :> valid!
            1 n decimal-digits 1 - [a..b] [| shift |
                n shift p35-rotate primes nth not [ f valid! ] when
            ] each
            valid [ answer 1 + answer! ] when
        ] when
    ] each answer ;

:: p40-digit ( ordinal -- digit )
    ordinal :> position!
    1 :> digits! 1 :> begin! 9 :> count!
    [ position count digits * > ] [
        position count digits * - position!
        digits 1 + digits!
        begin 10 * begin!
        count 10 * count!
    ] while
    begin position 1 - digits /i + number>string
    position 1 - digits mod swap nth 48 - ;
:: p40 ( -- n )
    { 1 10 100 1000 10000 100000 1000000 }
    [ p40-digit ] map product ;

:: p46 ( -- n )
    9 :> value! 0 :> answer!
    [ answer 0 = ] [
        value prime? not [
            t :> unrepresented!
            2 value 1 - [a..b] [| x |
                x prime? value x - 2 mod zero? and [
                    value x - 2 /i euler-isqrt :> root
                    root sq value x - 2 /i = [ f unrepresented! ] when
                ] when
            ] each
            unrepresented [ value answer! ] when
        ] when
        value 2 + value!
    ] while answer ;

:: p47-distinct ( number -- count )
    number :> remaining! 2 :> divisor! 0 :> count!
    [ divisor sq remaining <= ] [
        remaining divisor mod zero? [
            count 1 + count!
            [ remaining divisor mod zero? ] [ remaining divisor /i remaining! ] while
        ] when divisor 1 + divisor!
    ] while remaining 1 > [ count 1 + ] [ count ] if ;
:: p47 ( -- n )
    0 :> streak! 2 :> n!
    [ streak 4 < ] [
        n p47-distinct 4 = [ streak 1 + streak! ] [ 0 streak! ] if
        n 1 + n!
    ] while n 4 - ;

:: p48 ( -- n )
    0 :> total!
    1 1000 [a..b] [| n |
        n n 10000000000 modular-pow total + 10000000000 mod total!
    ] each total ;

:: p55-lychrel? ( n -- ? )
    n :> current!
    t :> likely!
    50 [| |
        current number>string reverse decimal-cell current + current!
        current decimal-palindrome? [ f likely! ] when
    ] times likely ;
:: p55 ( -- n ) 1 9999 [a..b] [ p55-lychrel? ] count ;

:: p69 ( -- n )
    1 :> product! 2 :> n!
    [ product n * 1000000 <= ] [
        n prime? [ product n * product! ] when
        n 1 + n!
    ] while product ;

:: p92-end ( n -- endpoint )
    n :> value!
    [ value 1 = value 89 = or ] [ value digit-squares value! ] until
    value ;
:: p92 ( -- n )
    0 :> answer!
    1 9999999 [a..b] [| n |
        n p92-end 89 = [ answer 1 + answer! ] when
    ] each answer ;

: p15 ( -- n ) 40 factorial-number 20 factorial-number sq /i ;
: p18 ( -- n ) "tests/data/p018_triangle.txt" max-triangle-path ;

:: p23 ( -- n )
    28123 divisor-sum-sieve :> sums
    12 28123 [a..b] [| n | n sums nth n > ] filter :> abundant
    28124 f <array> :> representable
    abundant [| a |
        abundant [| b |
            a b + 28123 <= [ t a b + representable set-nth ] when
        ] each
    ] each
    1 28123 [a..b] [| n | n representable nth not ] filter sum ;

:: p24 ( -- n )
    999999 :> rank! 0 :> answer!
    10 f <array> :> used
    0 9 [a..b] [| index |
        9 index - factorial-number :> block
        rank block /i :> wanted
        rank block mod rank!
        0 :> found!
        0 9 [a..b] [| digit |
            digit used nth not [
                found wanted = [
                    answer 10 * digit + answer!
                    t digit used set-nth
                    found 1 + found!
                ] [ found 1 + found! ] if
            ] when
        ] each
    ] each answer ;

:: p26 ( -- n )
    0 :> longest! 0 :> best!
    2 999 [a..b] [| d |
        d -1 <array> :> seen
        0 :> pos! 1 d mod :> remainder!
        [ remainder 0 > remainder seen nth -1 = and ] [
            pos remainder seen set-nth
            pos 1 + pos!
            remainder 10 * d mod remainder!
        ] while
        remainder 0 = [ 0 ] [ pos remainder seen nth - ] if :> cycle
        cycle longest > [ cycle longest! d best! ] when
    ] each best ;

:: p31 ( -- n )
    201 0 <array> :> ways
    1 0 ways set-nth
    { 1 2 5 10 20 50 100 200 } [| coin |
        coin 200 [a..b] [| amount |
            amount ways nth amount coin - ways nth + amount ways set-nth
        ] each
    ] each 200 ways nth ;

:: p32 ( -- n )
    10000 f <array> :> seen
    0 :> total!
    1 99 [a..b] [| a |
        a 1 + 9999 a /i [a..b] [| b |
            a b * :> product
            a b concat-number product concat-number pandigital-nine?
            product 9999 <= and [
                product seen nth not [
                    total product + total!
                    t product seen set-nth
                ] when
            ] when
        ] each
    ] each total ;

:: p33-cancel? ( n d -- ? )
    n 10 /i :> a  n 10 mod :> b
    d 10 /i :> c  d 10 mod :> e
    a c = a 0 > and b d * n e * = and
    a e = a 0 > and b d * n c * = and or
    b c = b 0 > and a d * n e * = and or
    b e = b 0 > and a d * n c * = and or ;
:: p33 ( -- n )
    1 :> numerator! 1 :> denominator!
    10 99 [a..b] [| n |
        n 1 + 99 [a..b] [| d |
            n d p33-cancel? [ numerator n * numerator! denominator d * denominator! ] when
        ] each
    ] each
    denominator dup numerator gcd nip /i ;

:: reverse-base ( value base -- reversal )
    0 :> out!
    value :> n!
    [ n 0 > ] [
        out base * n base mod + out!
        n base /i n!
    ] while out ;
:: p36 ( -- n )
    0 :> total!
    1 999999 [a..b] [| n |
        n decimal-palindrome? n 2 reverse-base n = and
        [ total n + total! ] when
    ] each total ;

:: p37-truncatable? ( n sieve -- ? )
    n :> left!
    t :> okay!
    [ left 0 > ] [
        left sieve nth not [ f okay! ] when
        left 10 /i left!
    ] while
    n decimal-digits 1 - pow10 :> pow!
    n :> right!
    [ pow 1 > ] [
        right pow mod right!
        right sieve nth not [ f okay! ] when
        pow 10 /i pow!
    ] while okay ;
:: p37 ( -- n )
    999999 euler-sieve :> sieve
    0 :> found! 0 :> answer! 11 :> n!
    [ found 11 < ] [
        n sieve p37-truncatable? [ answer n + answer! found 1 + found! ] when
        n 2 + n!
    ] while answer ;

:: p38 ( -- n )
    0 :> best!
    1 9999 [a..b] [| start |
        0 :> number! 1 :> factor!
        [ number decimal-digits 9 < ] [
            number start factor * concat-number number!
            factor 1 + factor!
        ] while
        factor 2 > number pandigital-nine? and
        [ number best max best! ] when
    ] each best ;

:: p41 ( -- n )
    7654321 :> n! 0 :> answer!
    [ n 1234567 >= answer 0 = and ] [
        n pandigital-seven? n prime? and [ n answer! ] when
        n 2 - n!
    ] while answer ;

:: p42 ( -- n )
    "tests/data/p042_words.txt" read-upper-words
    [| word | word [ 64 - ] map sum :> score
      score 8 * 1 + euler-isqrt :> root
      root sq score 8 * 1 + = ] count ;

:: p45 ( -- n )
    144 :> n! 0 :> answer!
    [ answer zero? ] [
        n 2 * 1 - n * :> hexagonal
        hexagonal 5 polygonal? [ hexagonal answer! ] when
        n 1 + n!
    ] while answer ;

:: p49 ( -- n )
    9999 euler-sieve :> sieve
    0 :> answer!
    1000 9999 [a..b] [| a |
        a sieve nth [
            a 1 + 9999 [a..b] [| b |
                b 2 * a - :> c
                c 10000 < [
                    b sieve nth c sieve nth and
                    a digit-signature b digit-signature = and
                    a digit-signature c digit-signature = and
                    a 1487 = b 4817 = and not and [
                        a 100000000 * b 10000 * + c + answer!
                    ] when
                ] when
            ] each
        ] when
    ] each answer ;

:: p52 ( -- n )
    1 :> magnitude! 0 :> answer!
    [ answer zero? ] [
        magnitude magnitude 10 * 1 - [a..b] [| n |
            n 6 * magnitude 10 * < answer zero? and [
                n digit-signature :> signature
                2 6 [a..b] [| factor |
                    n factor * digit-signature signature =
                ] all? [ n answer! ] when
            ] when
        ] each
        magnitude 10 * magnitude!
    ] while answer ;

:: p53 ( -- n )
    101 0 <array> :> coeff
    1 0 coeff set-nth
    0 :> answer!
    1 100 [a..b] [| n |
        n 1 [a..b] [| k |
            k coeff nth k 1 - coeff nth + 1000001 min
            k coeff set-nth
        ] each
        0 n [a..b] [| k |
            k coeff nth 1000000 > [ answer 1 + answer! ] when
        ] each
    ] each answer ;

:: p56 ( -- n )
    0 :> best!
    1 99 [a..b] [| a |
        a :> value!
        1 99 [a..b] [| b |
            value a * value!
            value digit-sum best max best!
        ] each
    ] each best ;

:: p58 ( -- n )
    0 :> prime-count! 3 :> side! 0 :> answer!
    [ answer zero? ] [
        side sq :> square
        side 1 - :> step
        1 3 [a..b] [| k |
            square k step * - prime?
            [ prime-count 1 + prime-count! ] when
        ] each
        prime-count 10 * side 2 * 1 - <
        [ side answer! ] when
        side 2 + side!
    ] while answer ;

:: p63 ( -- n )
    0 :> answer!
    1 9 [a..b] [| base |
        1 :> value!
        1 25 [a..b] [| exp |
            value base * value!
            value decimal-digits exp = [ answer 1 + answer! ] when
        ] each
    ] each answer ;

:: p64 ( -- n )
    0 :> answer!
    2 10000 [a..b] [| n |
        n euler-isqrt :> root
        root sq n = not [
            0 :> m! 1 :> d! root :> a! 0 :> period! t :> continuing!
            [ continuing ] [
                d a * m - m!
                n m sq - d /i d!
                root m + d /i a!
                period 1 + period!
                a root 2 * = [ f continuing! ] when
            ] while
            period odd? [ answer 1 + answer! ] when
        ] when
    ] each answer ;

: p67 ( -- n ) "tests/data/p067_triangle.txt" max-triangle-path ;

:: p71 ( -- n )
    0 :> best-n! 1 :> best-d!
    2 1000000 [a..b] [| d |
        d 3 * 1 - 7 /i :> numerator
        numerator best-d * best-n d * >
        [ numerator best-n! d best-d! ] when
    ] each best-n ;

:: p72 ( -- n )
    1000000 phi-sieve :> phi
    2 1000000 [a..b] [| n | n phi nth ] map sum ;

:: p73-count ( a b c d -- n )
    b d + 12000 > [ 0 ] [
        a b a c + b d + p73-count
        a c + b d + c d p73-count + 1 +
    ] if ;
: p73 ( -- n ) 1 3 1 2 p73-count ;

:: p75 ( -- n )
    1500001 0 <array> :> perimeters
    2 866 [a..b] [| m |
        1 m 1 - [a..b] [| n |
            m n - odd? m n gcd nip 1 = and [
                m m n + * 2 * :> perimeter
                perimeter 1500000 <= [
                    perimeter 1500000 perimeter <range> [| multiple |
                        multiple perimeters nth 1 + 2 min
                        multiple perimeters set-nth
                    ] each
                ] when
            ] when
        ] each
    ] each
    perimeters [ 1 = ] count ;

:: p76 ( -- n )
    101 0 <array> :> ways 1 0 ways set-nth
    1 99 [a..b] [| coin |
        coin 100 [a..b] [| n |
            n ways nth n coin - ways nth + n ways set-nth
        ] each
    ] each 100 ways nth ;

:: p77 ( -- n )
    2 :> n! 0 :> answer!
    [ answer zero? ] [
        n 1 + 0 <array> :> ways
        1 0 ways set-nth
        2 n [a..b] [| p |
            p prime? [
                p n [a..b] [| amount |
                    amount ways nth amount p - ways nth +
                    amount ways set-nth
                ] each
            ] when
        ] each
        n ways nth 5000 > [ n answer! ] when
        n 1 + n!
    ] while answer ;

:: p78 ( -- n )
    100001 0 <array> :> p
    1 0 p set-nth
    1 :> n! 0 :> answer!
    [ answer zero? ] [
        0 :> total! 1 :> k!
        [ k 3 k * 1 - * 2 /i n <= ] [
            k 3 k * 1 - * 2 /i :> g1
            k 3 k * 1 + * 2 /i :> g2
            k odd? [ 1 ] [ -1 ] if :> sign
            total sign n g1 - p nth * + total!
            g2 n <= [ total sign n g2 - p nth * + total! ] when
            k 1 + k!
        ] while
        total 1000000 mod :> residue
        residue n p set-nth
        residue zero? [ n answer! ] when
        n 1 + n!
    ] while answer ;

:: p85 ( -- n )
    2000000 :> diff! 0 :> best!
    1 2000 [a..b] [| width |
        1 width [a..b] [| height |
            width width 1 + * height height 1 + * * 4 /i :> rectangles
            rectangles 2000000 - abs :> error
            error diff < [ error diff! width height * best! ] when
        ] each
    ] each best ;

:: p86 ( -- n )
    0 :> count! 1 :> length!
    [ count 1000000 <= ] [
        2 length 2 * [a..b] [| joined |
            length sq joined sq + euler-isqrt :> root
            root sq length sq joined sq + = [
                joined length - 1 max :> low
                joined 2 /i length min :> high
                high low >= [ count high low - 1 + + count! ] when
            ] when
        ] each
        length 1 + length!
    ] while length 1 - ;

:: p91 ( -- n )
    0 :> answer!
    0 50 [a..b] [| ax |
        0 50 [a..b] [| ay |
            ax 0 = ay 0 = and not [
                0 50 [a..b] [| bx |
                    0 50 [a..b] [| by |
                        bx 0 = by 0 = and not
                        ax bx = ay by = and not and [
                            ax bx * ay by * + zero?
                            ax ax bx - * ay ay by - * + zero? or
                            bx bx ax - * by by ay - * + zero? or
                            [ answer 1 + answer! ] when
                        ] when
                    ] each
                ] each
            ] when
        ] each
    ] each answer 2 /i ;

: p97 ( -- n ) 2 7830457 10000000000 modular-pow 28433 * 1 + 10000000000 mod ;

:: p100 ( -- n )
    15 :> blue! 21 :> total!
    [ total 1000000000000 <= ] [
        blue 3 * total 2 * + 2 - :> next-blue
        blue 4 * total 3 * + 3 - :> next-total
        next-blue blue! next-total total!
    ] while blue ;

:: p106 ( -- n )
    0 :> total!
    2 6 [a..b] [| k |
        2 k * factorial-number k factorial-number sq /i :> choices
        choices k 1 + /i :> catalan
        12 factorial-number
        2 k * factorial-number 12 2 k * - factorial-number * /i
        choices 2 /i catalan - * total + total!
    ] each total ;

:: p112-bouncy? ( value -- ? )
    value number>string >array :> chars
    t :> inc! t :> dec!
    1 chars length 1 - [a..b] [| index |
        index 1 - chars nth index chars nth > [ f inc! ] when
        index 1 - chars nth index chars nth < [ f dec! ] when
    ] each
    inc not dec not and ;
:: p112 ( -- n )
    100 :> n! 0 :> count!
    0 :> answer!
    [ answer zero? ] [
        n p112-bouncy? [ count 1 + count! ] when
        count 100 * n 99 * = [ n answer! ] when
        n 1 + n!
    ] while answer ;

:: p113-binomial ( n k -- result )
    n factorial-number k factorial-number n k - factorial-number * /i ;
: p113 ( -- n ) 110 10 p113-binomial 109 9 p113-binomial + 1002 - ;

:: p114-ways ( size min-block -- ways )
    size 1 + 0 <array> :> ways
    1 0 ways set-nth
    1 size [a..b] [| n |
        n 1 - ways nth :> total!
        n min-block >= [
            min-block n [a..b] [| block |
                block n = [ 1 ] [ n block - 1 - ways nth ] if
                total + total!
            ] each
        ] when
        total n ways set-nth
    ] each size ways nth ;
: p114 ( -- n ) 50 3 p114-ways ;
:: p115 ( -- n )
    50 :> length!
    [ length 50 p114-ways 1000000 > ] [ length 1 + length! ] until
    length ;

:: p116 ( -- n )
    0 :> answer!
    2 4 [a..b] [| tile |
        51 0 <array> :> ways
        1 0 ways set-nth
        1 50 [a..b] [| n |
            n 1 - ways nth :> count!
            n tile >= [ count n tile - ways nth + count! ] when
            count n ways set-nth
        ] each
        50 ways nth 1 - answer + answer!
    ] each answer ;

:: p117 ( -- n )
    51 0 <array> :> ways
    1 0 ways set-nth
    1 50 [a..b] [| n |
        0 :> count!
        1 4 [a..b] [| width |
            n width >= [ count n width - ways nth + count! ] when
        ] each
        count n ways set-nth
    ] each 50 ways nth ;

:: p120 ( -- n )
    3 1000 [a..b] [| a |
        a odd? [ a a 1 - * ] [ a a 2 - * ] if
    ] map sum ;
