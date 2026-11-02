import euler from "../support/goose";
fn main() {
    assert(!prime(1) && prime(2) && prime(104729) && !prime(104730));
    assert(gcd(48, 18) == 6);
    assert(divisor_sum(220) == 284 && divisor_sum(284) == 220);
    assert(divisor_count(28) == 6);
    assert(pandigital(123456789) && !pandigital(123456788));
    var a = big(999);
    add(a, big(1));
    assert(a.len == 4 && a.d[3] == 1 && big_sum(a) == 1);
    subtract(a, big(1));
    assert(a.len == 3 && big_sum(a) == 27);
    multiply(a, 0);
    assert(a.len == 1 && a.d[0] == 0);
    a = big(1);
    for _ in 1000 { multiply(a, 2); }
    assert(a.len == 302 && big_sum(a) == 1366);
    var p = [0, 1, 2];
    var count = 1;
    while next_perm(p[..]) { count++; }
    assert(count == 6 && p == [2, 1, 0]);
    let primes = sieve(100);
    var found = 0;
    for n in 2..101 { if primes[n] { found++; } }
    assert(found == 25);
    print("Goose helper examples passed");
}
