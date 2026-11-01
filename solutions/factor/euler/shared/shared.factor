! Reusable Euler operations shared by the native Factor solutions.
! Factor VM: -roots=<repository>/solutions/factor
USING: arrays io.encodings.utf8 io.files kernel locals math math.functions
math.order math.parser math.primes ranges sequences sorting splitting ;
IN: euler.shared

: decimal-digits ( n -- digits ) number>string length ;
: digit-sum ( n -- sum ) number>string [ 48 - ] map sum ;
: factorial-number ( n -- n! ) dup zero? [ drop 1 ] [ [1..b] product ] if ;
: decimal-palindrome? ( n -- ? ) number>string dup reverse = ;
: digit-signature ( n -- seq ) number>string >array sort ;
: pandigital-nine? ( n -- ? ) digit-signature "123456789" >array = ;
: pandigital-seven? ( n -- ? ) digit-signature "1234567" >array = ;
: digit-squares ( n -- sum ) number>string [ 48 - sq ] map sum ;
: pow10 ( n -- x ) 10 swap ^ ;
:: concat-number ( a b -- ab ) b decimal-digits pow10 a * b + ;
: decimal-cell ( str -- n ) 0 [ 48 - swap 10 * + ] reduce ;
: whitespace? ( ch -- ? ) { 32 10 13 9 } member? ;
: separator? ( ch -- ? ) { 32 10 13 9 44 } member? ;
: read-numbers ( path -- seq ) utf8 file-contents
    [ separator? ] split-when [ empty? not ] filter [ decimal-cell ] map ;
: read-signed-numbers ( path -- seq ) utf8 file-contents
    [ separator? ] split-when [ empty? not ] filter [ string>number ] map ;
:: euler-sieve ( bound -- flags )
    bound 1 + t <array> :> sieve
    f 0 sieve set-nth f 1 sieve set-nth
    2 bound sqrt >integer [a..b] [| p |
        p sieve nth [
            p sq bound p <range> [| x |
                f x sieve set-nth
            ] each
        ] when
    ] each
    sieve ;
:: divisor-sum-sieve ( bound -- arr )
    bound 1 + 0 <array> :> sums
    1 bound 2 /i [a..b] [| d |
        d 2 * bound d <range> [| n |
            n sums nth d + n sums set-nth
        ] each
    ] each sums ;
:: phi-sieve ( bound -- arr )
    0 bound [a..b] >array :> phi
    2 bound [a..b] [| p |
        p phi nth p = [
            p bound p <range> [| n |
                n phi nth :> old
                old old p /i - n phi set-nth
            ] each
        ] when
    ] each phi ;
:: euler-isqrt ( n -- root )
    n 2 < [ n ] [
        1 :> low!
        n :> high!
        [ low high < ] [
            low high + 1 + 2 /i :> mid
            mid n mid /i <= [ mid low! ] [ mid 1 - high! ] if
        ] while low
    ] if ;
:: polygonal? ( n sides -- ? )
    ! For pentagonal tests use the discriminant, avoiding floating-point rounding.
    sides 5 = [
        n 24 * 1 + euler-isqrt :> root
        root sq n 24 * 1 + = root 1 + 6 mod zero? and
    ] [ f ] if ;
:: modular-pow ( base exponent modulus -- residue )
    1 :> result!
    base modulus mod :> current!
    exponent :> power!
    [ power 0 > ] [
        power odd? [ result current * modulus mod result! ] when
        current sq modulus mod current!
        power 2 /i power!
    ] while result ;
: read-upper-words ( path -- words ) utf8 file-contents
    [ dup 65 >= swap 90 <= and not ] split-when
    [ empty? not ] filter ;
:: max-triangle-path ( path -- max )
    path read-numbers >array :> vals
    vals length 8 * 1 + euler-isqrt 1 - 2 /i :> rows
    rows 2 - 0 [a..b] [| row |
        0 row [a..b] [| col |
            row row 1 + * 2 /i col + :> index
            row 1 + row 2 + * 2 /i col + :> below
            below vals nth below 1 + vals nth max
            index vals nth + index vals set-nth
        ] each
    ] each vals first ;
