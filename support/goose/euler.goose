// Reusable integer, digit and input operations. No problem-specific solvers.
import std;

import os;

import math;

fn gcd(a0: i64, b0: i64) -> i64 {
    var a = a0;
    var b = b0;
    while b != 0 {
        let t = a % b;
        a = b;
        b = t;
    }
    return a;
}

fn prime(n: i64) -> bool {
    if n < 2 {
        return false;
    }
    if n % 2 == 0 {
        return n == 2;
    }
    var d = 3;
    while d <= n / d {
        if n % d == 0 {
            return false;
        }
        d += 2;
    }
    return true;
}

fn ipow(a: i64, b: i64) -> i64 {
    var r = 1;
    for _ in b {
        r *= a;
    }
    return r;
}

fn reverse(n0: i64, base: i64 = 10) -> i64 {
    var n = n0;
    var r = 0;
    while n > 0 {
        r = r * base + n % base;
        n /= base;
    }
    return r;
}

fn digits(n0: i64) -> i64 {
    var n = n0;
    var k = 1;
    while n >= 10 {
        n /= 10;
        k++;
    }
    return k;
}

fn digit_sum(n0: i64) -> i64 {
    var n = n0;
    var s = 0;
    while n > 0 {
        s += n % 10;
        n /= 10;
    }
    return s;
}

fn signature(n0: i64) -> i64 {
    var n = n0;
    var s = 0;
    while n > 0 {
        s += 1 << (4 * (n % 10));
        n /= 10;
    }
    return s;
}

fn pandigital(n0: i64, count: i64 = 9) -> bool {
    var n = n0;
    var mask = 0;
    var k = 0;
    while n > 0 {
        let b = 1 << (n % 10);
        if mask & b != 0 {
            return false;
        }
        mask |= b;
        n /= 10;
        k++;
    }
    return k == count && mask == (1 << (count + 1)) - 2;
}

fn divisor_sum(n: i64) -> i64 {
    if n < 2 {
        return 0;
    }
    var s = 1;
    var d = 2;
    while d <= n / d {
        if n % d == 0 {
            s += d;
            if d != n / d {
                s += n / d;
            }
        }
        d++;
    }
    return s;
}

fn divisor_count(n0: i64) -> i64 {
    var n = n0;
    var r = 1;
    var d = 2;
    while d <= n / d {
        var e = 0;
        while n % d == 0 {
            n /= d;
            e++;
        }
        r *= e + 1;
        d++;
    }
    if n > 1 {
        r *= 2;
    }
    return r;
}

fn sieve(limit: i64) -> bool[>..] {
    var a: bool[>..] = [];
    for _ in limit + 1 {
        a.push(true);
    }
    a[0] = false;
    a[1] = false;
    var p = 2;
    while p * p <= limit {
        if a[p] {
            var j = p * p;
            while j <= limit {
                a[j] = false;
                j += p;
            }
        }
        p++;
    }
    return a;
}

fn numbers(path: u8[:]) -> i64[>..] {
    var text: u8[>..] = [];
    if !read_file(path, text) {
        abort("cannot read input");
    }
    var a: i64[>..] = [];
    var v = 0;
    var active = false;
    for ch in text {
        if ch >= '0' && ch <= '9' {
            v = v * 10 + (ch - '0');
            active = true;
        } else if active {
            a.push(v);
            v = 0;
            active = false;
        }
    }
    if active {
        a.push(v);
    }
    return a;
}

fn file_text(path: u8[:]) -> u8[>..] {
    var text: u8[>..] = [];
    if !read_file(path, text) {
        abort("cannot read input");
    }
    return text;
}

fn read_words(path: u8[:]) -> u8[..64][>..] {
    let text = file_text(path);
    var out: u8[..64][>..] = [];
    var i = 0;
    while i < text.len {
        if text[i] >= 'A' && text[i] <= 'Z' {
            let start = i;
            while i < text.len && text[i] >= 'A' && text[i] <= 'Z' {
                i++;
            }
            out.push(text[start..i]);
        } else {
            i++;
        }
    }
    return out;
}

fn is_square(n: i64) -> bool {
    if n < 0 {
        return false;
    }
    let r = sqrt(n as f64) as! i64;
    return r * r == n || (r + 1) * (r + 1) == n;
}

// Base-ten fixed-capacity integers, least significant digit first.
struct Big {
    d: i64[1100], len: i64,
}

fn big(n0: i64) -> Big {
    var b = Big {
        d: [0; 1100], len: 0,
    };
    var n = n0;
    loop {
        b.d[b.len] = n % 10;
        b.len++;
        n /= 10;
        if n == 0 {
            break;
        }
    }
    return b;
}

fn multiply(b: Big&, n: i64) {
    if n == 0 {
        b = big(0);
        return;
    }
    var carry = 0;
    for i in b.len {
        let t = b.d[i] * n + carry;
        b.d[i] = t % 10;
        carry = t / 10;
    }
    while carry > 0 {
        assert(b.len < 1100);
        b.d[b.len] = carry % 10;
        b.len++;
        carry /= 10;
    }
}

fn add(a: Big&, b: Big) {
    let len = max(a.len, b.len);
    var carry = 0;
    for i in len {
        let t = a.d[i] + b.d[i] + carry;
        a.d[i] = t % 10;
        carry = t / 10;
    }
    a.len = len;
    if carry > 0 {
        assert(a.len < 1100);
        a.d[a.len] = carry;
        a.len++;
    }
}

fn big_sum(b: Big) -> i64 {
    var s = 0;
    for i in b.len {
        s += b.d[i];
    }
    return s;
}

fn next_perm(a: i64[:]) -> bool {
    var i = a.len - 2;
    while i >= 0 && a[i] >= a[i + 1] {
        i--;
    }
    if i < 0 {
        return false;
    }
    var j = a.len - 1;
    while a[j] <= a[i] {
        j--;
    }
    swap(a[i], a[j]);
    var lo = i + 1;
    var hi = a.len - 1;
    while lo < hi {
        swap(a[lo], a[hi]);
        lo++;
        hi--;
    }
    return true;
}

fn big_compare(a: Big, b: Big) -> i64 {
    if a.len != b.len {
        return sign(a.len - b.len);
    }
    var i = a.len - 1;
    while i >= 0 {
        if a.d[i] != b.d[i] {
            return sign(a.d[i] - b.d[i]);
        }
        i--;
    }
    return 0;
}

fn subtract(a: Big&, b: Big) {
    assert(big_compare(a, b) >= 0);
    var borrow = 0;
    for i in a.len {
        var t = a.d[i] - b.d[i] - borrow;
        borrow = 0;
        if t < 0 {
            t += 10;
            borrow = 1;
        }
        a.d[i] = t;
    }
    while a.len > 1 && a.d[a.len - 1] == 0 {
        a.len--;
    }
}
