! Native Factor solvers added for the 121..255 extension.
!
! Each word retains the mathematical reduction used by the C and Rust
! implementations.  Large numeric literals below are problem inputs or
! moduli, never reference answers.
USING: arrays assocs combinators euler.shared generalizations hashtables kernel locals math
math.bitwise math.constants math.functions math.order math.primes ranges
sequences sets sorting vectors ;
IN: euler.extended

! Problem 121: dp[r] is the weighted number of histories containing r red
! draws.  On turn t, red has weight t and blue has weight one.
:: p121 ( -- n )
    16 0 <array> :> ways
    1 0 ways set-nth
    1 15 [a..b] [| turn |
        turn 1 [a..b] [| reds |
            reds ways nth reds 1 - ways nth turn * +
            reds ways set-nth
        ] each
    ] each
    0 :> wins!
    0 7 [a..b] [| reds | reds ways nth wins + wins! ] each
    16 factorial-number wins /i ;

! Problem 123: for odd n, the remainder collapses to 2*n*p_n modulo p_n^2.
:: p123 ( -- n )
    1 :> index! 2 :> candidate! 0 :> answer!
    [ answer zero? ] [
        candidate 1 + candidate!
        candidate prime? [
            index 1 + index!
            index odd? index 2 * candidate * 10000000000 > and
            [ index answer! ] when
        ] when
    ] while answer ;

! Problem 125: prefix sums enumerate consecutive square sums; a hash table
! removes values having more than one representation.
:: p125 ( -- n )
    10001 0 <array> :> prefix
    1 10000 [a..b] [| n | n 1 - prefix nth n sq + n prefix set-nth ] each
    H{ } clone :> found
    1 9999 [a..b] [| first |
        first 1 + :> last!
        [ last 10000 <= ] [
            last prefix nth first 1 - prefix nth - :> value
            value 100000000 >= [ 10001 last! ] [
                value decimal-palindrome? [ t value found set-at ] when
                last 1 + last!
            ] if
        ] while
    ] each found keys sum ;

! Problem 128: only the first and last tile in each hexagonal ring can have
! three prime differences; the six differences reduce to these tests.
:: p128 ( -- n )
    1 :> count! 1 :> answer! 1 :> ring!
    [ count 2000 < ] [
        ring 6 * 1 - prime? ring 6 * 1 + prime? and
        ring 12 * 5 + prime? and [
            count 1 + count!
            ring 3 * ring 1 - * 2 + answer!
        ] when
        ring 1 > [
            ring 6 * 1 - prime? ring 6 * 5 + prime? and
            ring 12 * 7 - prime? and [
                count 1 + count!
                ring 3 * ring 1 + * 1 + answer!
            ] when
        ] when
        ring 1 + ring!
    ] while answer ;

! Repunit length modulo n.  Keeping only the remainder avoids constructing
! million-digit repunits and is shared by problems 129 and 130.
:: repunit-length ( n -- length )
    1 n mod :> remainder! 1 :> length!
    [ remainder zero? not ] [
        remainder 10 * 1 + n mod remainder!
        length 1 + length!
    ] while length ;
:: p129 ( -- n )
    1000001 :> n!
    [ n 10 gcd nip 1 = [ n repunit-length 1000000 > ] [ f ] if not ]
    [ n 1 + n! ] while n ;
:: p130 ( -- n )
    91 :> n! 0 :> found! 0 :> total!
    [ found 25 < ] [
        n 10 gcd nip 1 = n prime? not and [
            n 1 - n repunit-length mod zero? [
                total n + total! found 1 + found!
            ] when
        ] when
        n 2 + n!
    ] while total ;

! Problem 131: consecutive cubes yield p=3n^2+3n+1.
:: p131 ( -- n )
    0 :> count! 1 :> n!
    [ n sq 3 * n 3 * + 1 + 1000000 < ] [
        n sq 3 * n 3 * + 1 + prime? [ count 1 + count! ] when
        n 1 + n!
    ] while count ;

! Problems 137 and 138 are the Pell recurrences obtained after eliminating
! the square-root conditions.  The loops still derive every requested term.
:: p137 ( -- n )
    2 :> a! 15 :> b!
    13 [
        b :> next-a
        b 7 * a - 1 + :> next-b
        next-a a! next-b b!
    ] times b ;
:: p138 ( -- n )
    1 :> a! 17 :> b! 17 :> total!
    11 [
        b :> next-a
        b 18 * a - :> next-b
        next-a a! next-b b!
        total b + total!
    ] times total ;

! Problem 139: primitive Euclidean triples tile exactly when c is divisible
! by the difference of the legs; scaled perimeters are then counted at once.
:: p139 ( -- n )
    0 :> total! 2 :> m!
    [ m m 1 + * 2 * 100000000 < ] [
        1 m 1 - [a..b] [| n |
            m n - odd? m n gcd nip 1 = and [
                m sq n sq - :> a
                m n * 2 * :> b
                m sq n sq + :> c
                c a b - abs mod zero? [
                    total 99999999 a b + c + /i + total!
                ] when
            ] when
        ] each
        m 1 + m!
    ] while total ;

! Problem 158: choose n letters, then choose any binary rise/fall pattern
! except the n+1 monotone/degenerate patterns.
:: p158 ( -- n )
    1 :> choose! 1 :> power! 0 :> best!
    1 26 [a..b] [| n |
        choose 27 n - * n /i choose!
        power 2 * power!
        choose power n - 1 - * best max best!
    ] each best ;

! Problem 162: inclusion-exclusion forces hexadecimal 0, 1 and A to occur.
:: p162 ( -- n )
    0 :> answer!
    3 16 [a..b] [| len |
        15 16 len 1 - ^ *
        15 len ^ -
        14 15 len 1 - ^ * 2 * -
        14 len ^ 2 * +
        13 14 len 1 - ^ * +
        13 len ^ -
        answer + answer!
    ] each answer ;

! Problem 182: CRT makes the unconcealed-message count a product of two gcds.
:: p182 ( -- n )
    1009 :> p 3643 :> q p 1 - q 1 - * :> phi
    1000000000 :> minimum! 0 :> answer!
    2 phi 1 - [a..b] [| e |
        e phi gcd nip 1 = [
            e 1 - p 1 - gcd nip 1 +
            e 1 - q 1 - gcd nip 1 + * :> fixed
            fixed minimum < [ fixed minimum! e answer! ] [
                fixed minimum = [ answer e + answer! ] when
            ] if
        ] when
    ] each answer ;

! Problem 183: the optimum number of parts is adjacent to n/e; termination
! depends only on whether the reduced denominator contains primes beyond 2,5.
:: p183 ( -- n )
    0 :> answer!
    5 10000 [a..b] [| n |
        n >float e / floor >integer :> low
        low 1 + :> high
        low >float n >float low / log *
        high >float n >float high / log * > [ low ] [ high ] if :> k
        k n k gcd nip /i :> denominator!
        [ denominator 2 mod zero? ] [ denominator 2 /i denominator! ] while
        [ denominator 5 mod zero? ] [ denominator 5 /i denominator! ] while
        denominator 1 = [ answer n - ] [ answer n + ] if answer!
    ] each answer ;

! Problems 197, 199 and 235 are numerical processes; formatting remains in
! their entrypoints so the reusable words expose numeric results.
: p197-next ( x -- y ) dup * neg 30.403243784 + 2 swap ^ floor 1e-9 * ;
:: p197 ( -- x ) -1.0 :> x! 2000 [ x p197-next x! ] times x x p197-next + ;
:: p199-fill ( a b c depth -- area )
    depth zero? [ 0.0 ] [
        a b + c + a b * b c * + c a * + sqrt 2.0 * + :> d
        1.0 d sq / a b d depth 1 - p199-fill +
        b c d depth 1 - p199-fill + c a d depth 1 - p199-fill +
    ] if ;
:: p199 ( -- x )
    3.0 sqrt 2.0 * 3.0 - :> radius
    1.0 radius / :> curvature
    1.0 radius sq 3.0 * -
    curvature curvature curvature 10 p199-fill -
    -1.0 curvature curvature 10 p199-fill 3.0 * - ;
:: p235-sum ( ratio -- total )
    1.0 :> power! 0.0 :> total!
    1 5000 [a..b] [| k |
        total 900.0 k 3.0 * - power * + total!
        power ratio * power!
    ] each total ;
:: p235 ( -- x )
    1.0 :> low! 1.01 :> high!
    200 [
        low high + 2.0 / :> middle
        middle p235-sum -600000000000.0 >
        [ middle low! ] [ middle high! ] if
    ] times low high + 2.0 / ;

! Problem 204: exponent choices for the 25 allowed primes give each Hamming
! number one unique nondecreasing factorization.
CONSTANT: hamming-primes { 2 3 5 7 11 13 17 19 23 29 31 37 41 43 47 53 59 61 67 71 73 79 83 89 97 }
:: p204-count ( index value -- count )
    index hamming-primes length = [ 1 ] [
        index hamming-primes nth :> prime
        0 :> result! value :> current!
        [ current 1000000000 <= ] [
            result index 1 + current p204-count + result!
            current prime * current!
        ] while result
    ] if ;
: p204 ( -- n ) 0 1 p204-count ;

! Problem 207: perfect partitions occur exactly at powers-of-two m.
:: p207 ( -- n )
    0 :> perfect! 2 :> next! 2 :> m! 0 :> answer!
    [ answer zero? ] [
        m next = [ perfect 1 + perfect! next 2 * next! ] when
        perfect 12345 * m 1 - < [ m m 1 - * answer! ] when
        m 1 + m!
    ] while answer ;

! Problem 209: decompose the 64-state Boolean permutation into cycles.  A
! cycle of length L permits F(L-1)+F(L+1) independent sets.
:: p209-transition ( value -- next )
    value 32 /i 2 mod :> a value 16 /i 2 mod :> b value 8 /i 2 mod :> c
    value 2 * 64 mod a b c * bitxor + ;
:: p209 ( -- n )
    66 0 <array> :> fib 1 1 fib set-nth
    2 65 [a..b] [| i | i 1 - fib nth i 2 - fib nth + i fib set-nth ] each
    64 f <array> :> visited 1 :> answer!
    0 63 [a..b] [| start |
        start visited nth not [
            0 :> length! start :> current!
            [ current visited nth not ] [
                t current visited set-nth length 1 + length!
                current p209-transition current!
            ] while
            answer length 1 - fib nth length 1 + fib nth + * answer!
        ] when
    ] each answer ;

! Problem 218: exhaust the finite residue proof modulo lcm(6,28)=84.
:: p218 ( -- n )
    0 :> violations!
    0 83 [a..b] [| u | 0 83 [a..b] [| v |
        u v bitxor odd?
        u 3 mod zero? v 3 mod zero? and not and
        u 7 mod zero? v 7 mod zero? and not and [
            u sq v sq - :> m u v * 2 * :> n
            m n * 84 mod m sq n sq - 84 mod * 84 mod zero? not
            [ violations 1 + violations! ] when
        ] when
    ] each ] each violations ;

! Problem 219: cost-layer multiplicities replace a billion-leaf priority queue.
:: p219 ( -- n )
    128 0 <array> :> multiplicity 1 0 multiplicity set-nth
    999999999 :> needed 0 :> used! 0 :> internal! 0 :> cost!
    [ used needed < ] [
        cost 0 > [
            cost 1 - multiplicity nth
            cost 4 >= [ cost 4 - multiplicity nth + ] when
            cost multiplicity set-nth
        ] when
        cost multiplicity nth needed used - min :> take
        used take + used! internal cost take * + internal!
        cost 1 + cost!
    ] while internal needed 5 * + ;

! Problem 225: an invertible modular Tribonacci transition is purely periodic.
:: never-divides? ( modulus -- ? )
    1 :> a! 1 :> b! 1 :> c! f :> result! t :> running!
    [ running ] [
        a b + c + modulus mod :> next
        b a! c b! next c!
        c zero? [ f result! f running! ] when
        a 1 = b 1 = and c 1 = and [ t result! f running! ] when
    ] while result ;
:: p225 ( -- n )
    0 :> found! 1 :> candidate! 0 :> answer!
    [ answer zero? ] [
        candidate never-divides? [
            found 1 + found! found 124 = [ candidate answer! ] when
        ] when candidate 2 + candidate!
    ] while answer ;

! Problem 228: reduced edge-normal denominator d contributes phi(d) exactly
! when d divides one of the polygon sizes in the interval.
:: p228 ( -- n )
    1909 phi-sieve :> phi 1910 f <array> :> present
    t 1 present set-nth
    1864 1909 [a..b] [| n |
        1 n sqrt >integer [a..b] [| d | n d mod zero? [
            t d present set-nth t n d /i present set-nth
        ] when ] each
    ] each
    0 :> answer! 1 1909 [a..b] [| d |
        d present nth [ answer d phi nth + answer! ] when
    ] each answer ;

! Problem 234: sum arithmetic progressions in each consecutive-prime-square
! interval, subtracting common multiples twice.
:: multiple-sum ( divisor low high -- sum )
    low divisor 1 - + divisor /i :> first high divisor /i :> last
    first last > [ 0 ] [ divisor first last + * last first - 1 + * 2 /i ] if ;
:: p234 ( -- n )
    1010000 euler-sieve :> sieve V{ } clone :> primes
    2 1010000 [a..b] [| n | n sieve nth [ n primes push ] when ] each
    0 :> answer!
    0 primes length 2 - [a..b] [| i |
        i primes nth :> p i 1 + primes nth :> q p sq 1 + :> low
        low 999966663333 <= [
            q sq 1 - 999966663333 min :> high
            answer p low high multiple-sum + q low high multiple-sum +
            p q * low high multiple-sum 2 * - answer!
        ] when
    ] each answer ;

! Problem 239: normalized inclusion-exclusion terms avoid constructing 100!.
:: p239 ( -- x )
    1.0 100.0 99.0 * 98.0 * / :> term!
    term :> total!
    0 21 [a..b] [| j |
        term 22 j - neg >float *
        j 1 + >float 97 j - >float * / term!
        total term + total!
    ] each total 25.0 24.0 * 23.0 * 6.0 / * ;

! Problem 242: digit DP for sum 2^popcount(t) up to the binary bound.
:: p242 ( -- n )
    999999999999 4 /i :> upper
    40 1 <array> :> powers
    1 39 [a..b] [| i | i 1 - powers nth 3 * i powers set-nth ] each
    0 :> answer! 1 :> weight!
    39 0 [a..b] [| bit |
        upper 2 bit ^ /i 2 mod 1 = [
            answer weight bit powers nth * + answer!
            weight 2 * weight!
        ] when
    ] each answer weight + ;

! Problem 243: primorial construction plus exact totient comparison.
:: p243-phi ( value -- phi )
    value :> remaining! value :> result! 2 :> p!
    [ p sq remaining <= ] [
        remaining p mod zero? [
            result result p /i - result!
            [ remaining p mod zero? ] [ remaining p /i remaining! ] while
        ] when p 1 + p!
    ] while remaining 1 > [ result result remaining /i - ] [ result ] if ;
:: p243 ( -- n )
    { 2 3 5 7 11 13 17 19 23 29 31 37 } :> primes
    1 :> base! 1 :> phi! f :> crossed!
    primes [| p | crossed not [
        base p * base! phi p 1 - * phi!
        phi 94744 * base 15499 * < [ t crossed! ] when
    ] when ] each
    1 :> multiplier! 0 :> answer!
    [ answer zero? ] [
        base multiplier * :> d
        d p243-phi 94744 * d 1 - 15499 * < [ d answer! ] when
        multiplier 1 + multiplier!
    ] while answer ;

! Problem 132: p divides R(k) precisely when 10^k is one modulo 9p.
:: p132 ( -- n )
    0 :> found! 0 :> answer! 2 :> p!
    [ found 40 < ] [
        p 2 = p 5 = or not p prime? and [
            10 1000000000 p 9 * modular-pow 1 = [
                answer p + answer! found 1 + found!
            ] when
        ] when p 1 + p!
    ] while answer ;

! Problem 133: repeated tenth powers test whether a prime can ever divide a
! repunit whose length is a power of ten.
:: p133 ( -- n )
    0 :> answer!
    2 99999 [a..b] [| p |
        p prime? [
            p 9 * :> modulus 10 modulus mod :> residue!
            20 [ residue 10 modulus modular-pow residue! ] times
            residue 1 = not [ answer p + answer! ] when
        ] when
    ] each answer ;

! Problem 157: isolate powers of two and five, then count divisor choices in
! the remaining coprime factor.  The second term covers both nonzero exponents.
:: divisor-count ( value -- count )
    value :> remaining! 2 :> p! 1 :> result!
    [ p sq remaining <= ] [
        remaining p mod zero? [
            1 :> exponent!
            [ remaining p mod zero? ] [
                remaining p /i remaining! exponent 1 + exponent!
            ] while result exponent * result!
        ] when p 1 + p!
    ] while remaining 1 > [ result 2 * ] [ result ] if ;
:: p157 ( -- n )
    0 :> answer! 1 :> ten!
    1 9 [a..b] [| n |
        ten 10 * ten! 1 :> two!
        0 n [a..b] [| a |
            1 :> five!
            0 n [a..b] [| b |
                two five * :> y
                ten y /i y 1 + * divisor-count answer + answer!
                a 0 > b 0 > and [
                    ten y /i two five + * divisor-count answer + answer!
                ] when
                five 5 * five!
            ] each two 2 * two!
        ] each
    ] each answer ;

! Problem 159: initialize each digital root, then relax every multiplicative
! split.  Increasing factors are sufficient because multiplication commutes.
:: p159 ( -- n )
    1000000 0 <array> :> best
    1 999999 [a..b] [| n | n 1 - 9 mod 1 + n best set-nth ] each
    2 999999 [a..b] [| a |
        2 :> b!
        [ a b * 1000000 < ] [
            a b * :> product
            product best nth a best nth b best nth + max product best set-nth
            b 1 + b!
        ] while
    ] each best 2 tail-slice sum ;

! Problem 171: a DP tracks both the number and numeric sum of digit strings
! for each square-digit total, reduced modulo the requested nine digits.
:: p171 ( -- n )
    1621 0 <array> :> counts! 1621 0 <array> :> sums!
    1 0 counts set-nth
    0 19 [a..b] [| position |
        1621 0 <array> :> next-counts
        1621 0 <array> :> next-sums
        0 position 81 * [a..b] [| square-sum |
            0 9 [a..b] [| digit |
                square-sum digit sq + :> target
                target next-counts nth square-sum counts nth + 1000000000 mod
                target next-counts set-nth
                target next-sums nth square-sum sums nth 10 * +
                square-sum counts nth digit * + 1000000000 mod
                target next-sums set-nth
            ] each
        ] each
        next-counts counts! next-sums sums!
    ] each
    0 :> answer! 1 40 [a..b] [| root |
        answer root sq sums nth + 1000000000 mod answer!
    ] each answer ;

! Problem 181: two-dimensional unbounded coin change over all nonempty group
! types; processing a type outside the totals removes group ordering.
:: p181-index ( black white -- index ) black 41 * white + ;
:: p181 ( -- n )
    2501 0 <array> :> ways
    1 0 ways set-nth
    0 60 [a..b] [| black |
        0 40 [a..b] [| white |
            black zero? white zero? and not [
                black 60 [a..b] [| b |
                    white 40 [a..b] [| w |
                        b w p181-index ways nth
                        b black - w white - p181-index ways nth +
                        b w p181-index ways set-nth
                    ] each
                ] each
            ] when
        ] each
    ] each 60 40 p181-index ways nth ;

! Problem 189: state is the base-three colouring of upward triangles in the
! current row.  Downward triangles contribute independent multiplicities.
:: trit ( state position -- digit ) state 3 position ^ /i 3 mod ;
:: p189 ( -- n )
    6561 0 <array> :> previous!
    1 0 previous set-nth 1 1 previous set-nth 1 2 previous set-nth
    2 8 [a..b] [| row |
        6561 0 <array> :> current
        0 3 row 1 - ^ 1 - [a..b] [| before |
            before previous nth zero? not [
                0 3 row ^ 1 - [a..b] [| after |
                    1 :> multiplicity!
                    0 row 2 - [a..b] [| j |
                        before j trit :> a after j trit :> b
                        after j 1 + trit :> c
                        1 :> distinct!
                        b a = not [ distinct 1 + distinct! ] when
                        c a = not c b = not and [ distinct 1 + distinct! ] when
                        multiplicity 3 distinct - * multiplicity!
                    ] each
                    after current nth before previous nth multiplicity * +
                    after current set-nth
                ] each
            ] when
        ] each current previous!
    ] each previous 3 8 ^ head-slice sum ;

! Problem 191: attendance DP state is (absence-used, consecutive-lates).
:: p191-index ( absent late -- index ) absent 3 * late + ;
:: p191 ( -- n )
    6 0 <array> :> dp! 1 0 dp set-nth
    30 [
        6 0 <array> :> next
        0 1 [a..b] [| absent | 0 2 [a..b] [| late |
            absent late p191-index dp nth :> ways
            absent 0 p191-index next nth ways + absent 0 p191-index next set-nth
            absent zero? [ 1 0 p191-index next nth ways + 1 0 p191-index next set-nth ] when
            late 2 < [ absent late 1 + p191-index next nth ways + absent late 1 + p191-index next set-nth ] when
        ] each ] each next dp!
    ] times dp sum ;

! Problem 194: chromatic polynomials for the two fixed-boundary unit graphs,
! Pascal recurrence for C(100,25), all modulo the composite 10^8.
: p194-a ( c -- n ) dup 9 - over * 34 + over * 69 - over * 77 + swap * 38 - ;
: p194-b ( c -- n ) dup 8 - over * 27 + over * 50 - over * 52 + swap * 24 - ;
:: p194 ( -- n )
    26 0 <array> :> row 1 0 row set-nth
    1 100 [a..b] [| i |
        i 25 min 1 [a..b] [| j |
            j row nth j 1 - row nth + 100000000 mod j row set-nth
        ] each
    ] each
    25 row nth 1984 1983 * 100000000 mod * 100000000 mod
    1984 p194-a 100000000 mod 25 100000000 modular-pow * 100000000 mod
    1984 p194-b 100000000 mod 75 100000000 modular-pow * 100000000 mod ;

! Problem 205: exact convolution of the two dice distributions.
:: dice-distribution ( dice sides -- distribution )
    64 0 <array> :> current! 1 0 current set-nth
    dice [
        64 0 <array> :> next
        0 63 sides - [a..b] [| total |
            1 sides [a..b] [| face |
                total face + next nth total current nth +
                total face + next set-nth
            ] each
        ] each next current!
    ] times current ;
:: p205 ( -- x )
    9 4 dice-distribution :> peter 6 6 dice-distribution :> colin
    0 :> favorable! 0 :> lower!
    0 63 [a..b] [| total |
        favorable total peter nth lower * + favorable!
        lower total colin nth + lower!
    ] each favorable >float 4 9 ^ 6 6 ^ * >float / ;

! Problem 208: profile DP over four direction counts and current heading; the
! step number determines the fifth count.
:: p208-index ( a b c d heading -- index )
    a 15 * b + 15 * c + 15 * d + 5 * heading + ;
:: p208 ( -- n )
    253125 0 <array> :> current!
    1 0 0 0 0 0 p208-index current set-nth
    0 69 [a..b] [| step |
        253125 0 <array> :> next
        0 14 [a..b] [| a | 0 14 [a..b] [| b |
        0 14 [a..b] [| c | 0 14 [a..b] [| d |
            a b + c + d + :> used
            used step <= step used - 14 <= and [
                0 4 [a..b] [| heading |
                    a b c d heading p208-index current nth :> ways
                    ways zero? not [
                        { 4 1 } [| turn |
                            heading turn + 5 mod :> h
                            { a b c d } clone :> counts
                            h 4 < [
                                h counts nth 14 < [
                                    h counts nth 1 + h counts set-nth
                                    counts first counts second counts third counts fourth h p208-index
                                    next nth ways +
                                    counts first counts second counts third counts fourth h p208-index next set-nth
                                ] when
                            ] [ step used - 14 < [
                                a b c d h p208-index next nth ways +
                                a b c d h p208-index next set-nth
                            ] when ] if
                        ] each
                    ] when
                ] each
            ] when
        ] each ] each ] each ] each next current!
    ] each 14 14 14 14 0 p208-index current nth ;

! Problem 217: for every digit sum, carry both the number of strings and the
! aggregate numeric value while extending a half.
:: append-layer ( counts sums old-max -- next-counts next-sums )
    208 0 <array> :> nc 208 0 <array> :> ns
    0 old-max [a..b] [| total | 0 9 [a..b] [| digit |
        total digit + :> target
        target nc nth total counts nth + 14348907 mod target nc set-nth
        target ns nth total sums nth 10 * + total counts nth digit * +
        14348907 mod target ns set-nth
    ] each ] each nc ns ;
:: p217 ( -- n )
    208 0 <array> :> rc! 208 0 <array> :> rs!
    208 0 <array> :> lc! 208 0 <array> :> ls!
    1 0 rc set-nth
    48 0 <array> :> p10 1 0 p10 set-nth
    1 47 [a..b] [| i | i 1 - p10 nth 10 * 14348907 mod i p10 set-nth ] each
    45 :> answer!
    1 23 [a..b] [| h |
        rc rs h 1 - 9 * append-layer rs! rc!
        h 1 = [ 1 9 [a..b] [| d | 1 d lc set-nth d d ls set-nth ] each ] [
            lc ls h 1 - 9 * append-layer ls! lc!
        ] if
        0 :> even! 0 :> odd!
        0 h 9 * [a..b] [| s |
            s lc nth s rc nth * 14348907 mod :> pairs
            s ls nth s rc nth * 14348907 mod h p10 nth *
            s rs nth s lc nth * + 14348907 mod even + 14348907 mod even!
            s ls nth 10 * 14348907 mod s rc nth * 14348907 mod
            h 1 + p10 nth * s rs nth 10 * s lc nth * + 14348907 mod
            pairs 45 * h p10 nth * + odd + 14348907 mod odd!
        ] each answer even + odd + 14348907 mod answer!
    ] each answer ;

! Problem 227: Gaussian elimination of the 99 transient separation states.
:: p227-index ( row column -- index ) row 100 * column + ;
:: p227 ( -- x )
    9900 0.0 <array> :> matrix
    { -2 -1 0 1 2 } :> shifts
    { 0.027777777777777776 0.2222222222222222 0.5 0.2222222222222222 0.027777777777777776 } :> probs
    1 99 [a..b] [| k |
        k 1 - :> row 1 row row p227-index matrix set-nth
        1.0 row 99 p227-index matrix set-nth
        0 4 [a..b] [| i |
            k i shifts nth + 100 + 100 mod :> next
            next zero? not [
                row next 1 - p227-index :> index
                index matrix nth i probs nth - index matrix set-nth
            ] when
        ] each
    ] each
    0 98 [a..b] [| column |
        column :> pivot!
        column 98 [a..b] [| row |
            row column p227-index matrix nth abs
            pivot column p227-index matrix nth abs > [ row pivot! ] when
        ] each
        pivot column = not [
            column 99 [a..b] [| j |
                column j p227-index matrix nth :> temporary
                pivot j p227-index matrix nth column j p227-index matrix set-nth
                temporary pivot j p227-index matrix set-nth
            ] each
        ] when
        column column p227-index matrix nth :> divisor
        column 99 [a..b] [| j |
            column j p227-index matrix nth divisor / column j p227-index matrix set-nth
        ] each
        0 98 [a..b] [| row | row column = not [
            row column p227-index matrix nth :> factor
            column 99 [a..b] [| j |
                row j p227-index matrix nth
                factor column j p227-index matrix nth * -
                row j p227-index matrix set-nth
            ] each
        ] when ] each
    ] each 49 99 p227-index matrix nth ;

! Problem 230: only Fibonacci-word lengths are stored.  A requested ordinal
! descends through concatenations into one of the two 100-digit seeds.
CONSTANT: p230-a "1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679"
CONSTANT: p230-b "8214808651328230664709384460955058223172535940812848111745028410270193852110555964462294895493038196"
:: p230-digit ( ordinal -- digit )
    100 0 <array> :> lengths
    100 0 lengths set-nth 100 1 lengths set-nth
    1 :> term! ordinal :> position!
    [ term lengths nth position < ] [
        term 1 + term!
        term 2 - lengths nth term 1 - lengths nth + term lengths set-nth
    ] while
    [ term 2 >= ] [
        position term 2 - lengths nth <= [ term 2 - term! ] [
            position term 2 - lengths nth - position! term 1 - term!
        ] if
    ] while
    position 1 - term zero? [ p230-a ] [ p230-b ] if nth 48 - ;
:: p230 ( -- n )
    0 :> answer! 1 :> power7! 1 :> place10!
    0 17 [a..b] [| n |
        n 19 * 127 + power7 * p230-digit place10 * answer + answer!
        power7 7 * power7! place10 10 * place10!
    ] each answer ;

! Problem 232: descending score order solves the zero-score stochastic loop
! algebraically for each of player two's eight useful choices.
:: p232-index ( a b -- index ) a 228 * b + ;
:: p232 ( -- x )
    22800 0.0 <array> :> p1
    23028 0.0 <array> :> p2
    0 99 [a..b] [| a | 100 227 [a..b] [| b |
        1.0 a b p232-index p1 set-nth 1.0 a b p232-index p2 set-nth
    ] each ] each
    99 0 [a..b] [| a | 99 0 [a..b] [| b |
        a 1 + b p232-index p2 nth :> after-head
        0.0 :> best! 1 :> gain! 0.5 :> probability!
        8 [
            1.0 probability - :> failure
            probability a b gain + p232-index p1 nth *
            failure 0.5 * after-head * + 1.0 failure 0.5 * - /
            best max best!
            gain 2 * gain! probability 0.5 * probability!
        ] times
        best a b p232-index p2 set-nth
        after-head best + 0.5 * a b p232-index p1 set-nth
    ] each ] each 0 p1 nth ;

! Problem 240: face-descending DP counts distinguishable dice by binomial
! choices and records assigned dice, top slots and constrained sum.
:: p240-index ( used top total -- index ) used 11 * top + 71 * total + ;
:: p240 ( -- n )
    21 21 * 0 <array> :> choose
    0 20 [a..b] [| n |
        1 n 21 * choose set-nth 1 n 21 * n + choose set-nth
        n 1 > [ 1 n 1 - [a..b] [| k |
            n 1 - 21 * k 1 - + choose nth n 1 - 21 * k + choose nth +
            n 21 * k + choose set-nth
        ] each ] when
    ] each
    16401 0 <array> :> dp! 1 0 dp set-nth
    12 1 [a..b] [| face |
        16401 0 <array> :> next
        0 20 [a..b] [| used | 0 10 [a..b] [| top |
        0 70 [a..b] [| total |
            used top total p240-index dp nth :> ways
            ways zero? not [ 0 20 used - [a..b] [| count |
                count 10 top - min :> enters
                total face enters * + :> new-total
                new-total 70 <= [
                    used count + top enters + new-total p240-index :> index
                    index next nth ways 20 used - 21 * count + choose nth * +
                    index next set-nth
                ] when
            ] each ] when
        ] each ] each ] each next dp!
    ] each 20 10 70 p240-index dp nth ;

! Problem 135: n=u*v parameterizes the arithmetic progression; the parity and
! positivity constraints are u<3v and u+v divisible by four.
:: p135 ( -- n )
    1000000 0 <array> :> representations
    1 999999 [a..b] [| u |
        u 2 + 3 /i :> v!
        [ u v * 1000000 < ] [
            u 3 v * < u v + 4 mod zero? and [
                u v * :> product
                product representations nth 1 + 11 min product representations set-nth
            ] when v 1 + v!
        ] while
    ] each representations [ 10 = ] count ;

! Problem 140: all golden nuggets lie in six Pell orbits of x²-5y²=44.
:: p140 ( -- n )
    { { 7 1 } { 7 -1 } { 8 2 } { 8 -2 } { 13 5 } { 13 -5 } } :> seeds
    V{ } clone :> values
    seeds [| seed |
        seed first :> x! seed second :> y!
        35 [
            x 7 > x 7 - 5 mod zero? and [ x 7 - 5 /i values push ] when
            x 9 * y 20 * + :> next-x
            x 4 * y 9 * + :> next-y
            next-x x! next-y y!
        ] times
    ] each values sort members 30 head sum ;

! Problem 144: reflect the incoming vector across the ellipse normal and find
! the other quadratic intersection.  The aperture is tested after reflection.
:: p144 ( -- n )
    0.0 :> x0! 10.1 :> y0! 1.4 :> x! -9.6 :> y! 0 :> count!
    t :> running!
    [ running ] [
        x x0 - :> vx y y0 - :> vy x 4.0 * :> nx y :> ny
        vx nx * vy ny * + 2.0 * nx sq ny sq + / :> scale
        vx scale nx * - :> rx vy scale ny * - :> ry
        x rx * 8.0 * y ry * 2.0 * + neg
        rx sq 4.0 * ry sq + / :> time
        x x0! y y0! x time rx * + x! y time ry * + y!
        count 1 + count!
        x abs 0.01 <= y 0.0 > and [ f running! ] when
    ] while count ;

! Problem 145: paired-digit carry analysis gives closed counts by decimal
! length; the recurrences still generate every supported length class.
:: p145 ( -- n )
    0 :> total! 20 :> even!
    4 [ total even + total! even 30 * even! ] times
    100 :> odd! 2 [ total odd + total! odd 500 * odd! ] times total ;

! Problem 149: generate the 2000² lagged-Fibonacci grid, then run Kadane on
! every row, column and both diagonal directions.
:: p149-line ( grid row column dr dc -- best )
    row :> r! column :> c! -1000000000 :> best! 0 :> current!
    [ r 0 >= r 2000 < and c 0 >= and c 2000 < and ] [
        current r 2000 * c + grid nth + current!
        current best max best! current 0 max current!
        r dr + r! c dc + c!
    ] while best ;
:: p149 ( -- n )
    4000000 0 <array> :> grid
    1 4000000 [a..b] [| k |
        k 55 <= [
            100003 k 200003 * - k 3 ^ 300007 * + 1000000 mod
        ] [
            k 25 - grid nth k 56 - grid nth + 1000000 + 1000000 mod
        ] if :> value
        value 500000 - k 1 - grid set-nth
    ] each
    -1000000000 :> best!
    0 1999 [a..b] [| i |
        grid i 0 0 1 p149-line best max best!
        grid 0 i 1 0 p149-line best max best!
        grid i 0 1 1 p149-line best max best!
        grid 0 i 1 1 p149-line best max best!
        grid i 0 -1 1 p149-line best max best!
        grid 1999 i -1 1 p149-line best max best!
    ] each best ;

! Problem 163: orientation classification in an order-n cross-hatched grid
! reduces to this integer quasipolynomial.
:: p163 ( -- n )
    36 :> n
    n 3 ^ 1678 * n sq 3117 * + n 88 * +
    n 2 mod 345 * - n 3 mod 320 * - n 4 mod 90 * - n 5 mod 288 * -
    240 /i ;

! Problem 174: enumerate hollow square laminae and retain tile totals having
! from one through ten representations.
:: p174 ( -- n )
    1000001 0 <array> :> counts 3 :> outer!
    [ outer 1 - 4 * 1000000 <= ] [
        outer 2 - :> inner!
        t :> running!
        [ running ] [
            outer sq inner sq - :> tiles
            tiles 1000000 > [ f running! ] [
                tiles counts nth 1 + 11 min tiles counts set-nth
                inner 2 <= [ f running! ] [ inner 2 - inner! ] if
            ] if
        ] while outer 1 + outer!
    ] while counts [ dup 1 >= swap 10 <= and ] count ;

! Problem 178: last digit and used-digit mask are the complete step-number
! state.  Summing full masks after lengths 10..40 gives pandigital values.
:: p178-index ( digit mask -- index ) digit 1024 * mask + ;
:: p178 ( -- n )
    10240 0 <array> :> dp!
    1 9 [a..b] [| d | 1 d 2 d ^ p178-index dp set-nth ] each
    0 :> answer!
    1 40 [a..b] [| length |
        length 10 >= [ 0 9 [a..b] [| d |
            answer d 1023 p178-index dp nth + answer!
        ] each ] when
        10240 0 <array> :> next
        0 9 [a..b] [| d | 0 1023 [a..b] [| mask |
            d mask p178-index dp nth :> ways
            d 0 > [
                d 1 - :> nd mask 2 nd ^ bitor :> nm
                nd nm p178-index next nth ways + nd nm p178-index next set-nth
            ] when
            d 9 < [
                d 1 + :> nd mask 2 nd ^ bitor :> nm
                nd nm p178-index next nth ways + nd nm p178-index next set-nth
            ] when
        ] each ] each next dp!
    ] each answer ;

! Problem 202: unfolding reduces the path count to one residue class coprime
! to n.  Inclusion-exclusion over n's distinct prime factors is constant-size.
:: residue-count ( maximum residue -- count )
    residue zero? [ maximum 3 /i ] [
        maximum residue < [ 0 ] [ maximum residue - 3 /i 1 + ] if
    ] if ;
:: p202-multiples ( n divisor residue -- count )
    n 1 - divisor /i :> maximum divisor 3 mod :> dm
    dm zero? [ residue zero? [ maximum ] [ 0 ] if ] [
        maximum dm 1 = [ residue ] [ residue 2 * 3 mod ] if residue-count
    ] if ;
:: p202 ( -- n )
    12017639150 2 /i :> n n :> remaining!
    V{ } clone :> factors 2 :> p!
    [ p sq remaining <= ] [
        remaining p mod zero? [
            p factors push [ remaining p mod zero? ] [ remaining p /i remaining! ] while
        ] when p 2 = [ 3 ] [ p 2 + ] if p!
    ] while remaining 1 > [ remaining factors push ] when
    n 3 mod 2 * 3 mod :> target 0 :> total!
    0 2 factors length ^ 1 - [a..b] [| mask |
        1 :> divisor! f :> odd!
        factors [| prime i | mask 2 i ^ /i 2 mod 1 = [
            divisor prime * divisor! odd not odd!
        ] when ] each-index
        n divisor target p202-multiples :> count
        odd [ total count - ] [ total count + ] if total!
    ] each total ;

! Problem 237: fourth-order frontier recurrence as a 4x4 companion matrix,
! exponentiated modulo 10^8.
:: matrix-multiply ( left right -- result )
    16 0 <array> :> result
    0 3 [a..b] [| row | 0 3 [a..b] [| column |
        0 :> value!
        0 3 [a..b] [| k |
            value row 4 * k + left nth k 4 * column + right nth * + value!
        ] each value 100000000 mod row 4 * column + result set-nth
    ] each ] each result ;
:: matrix-power ( base exponent -- result )
    { 1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1 } clone :> result!
    base :> current! exponent :> power!
    [ power 0 > ] [
        power odd? [ result current matrix-multiply result! ] when
        current current matrix-multiply current!
        power 2 /i power!
    ] while result ;
:: p237 ( -- n )
    { 0 1 0 0 0 0 1 0 0 0 0 1 1 99999998 2 2 }
    999999999996 matrix-power :> jump
    { 1 1 4 8 } :> initial 0 :> answer!
    0 3 [a..b] [| i |
        answer 12 i + jump nth i initial nth * + 100000000 mod answer!
    ] each answer ;

! Problem 251: Cardano parameterization turns each coprime (q,r) pair into an
! arithmetic progression in p, counted by its exact linear sum bound.
:: inverse-mod ( a modulus -- inverse )
    a :> old-r! modulus :> r! 1 :> old-s! 0 :> s!
    [ r zero? not ] [
        old-r r /i :> quotient
        old-r quotient r * - :> next-r
        old-s quotient s * - :> next-s
        r old-r! next-r r! s old-s! next-s s!
    ] while old-s modulus mod dup 0 < [ modulus + ] when ;
:: p251 ( -- n )
    110000001 :> limit 110000000 sqrt >integer :> qmax 0 :> total!
    1 qmax [a..b] [| q |
        q sq :> q2
        total limit q2 3 * + q q2 * 8 * q 3 * + 1 + /i + total!
        q limit * 8 * >float 1.0 3.0 / ^ >integer 3 + :> rmax!
        rmax even? [ rmax 1 + rmax! ] when
        3 rmax 2 <range> [| r |
            q r gcd nip 1 = [
                r sq :> r2
                q 8 * r2 mod r2 inverse-mod 3 * r2 mod :> p0!
                p0 zero? [ r2 p0! ] when
                p0 q * 8 * 3 - r2 /i :> s0
                p0 q 3 * r + * s0 q2 * + :> base
                base limit <= [
                    r2 q 3 * r + * q q2 * 8 * + :> step
                    total limit base - step /i 1 + + total!
                ] when
            ] when
        ] each
    ] each total ;
