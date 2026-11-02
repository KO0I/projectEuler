// Project Euler 198: ambiguous numbers.
// Stern-Brocot intervals enumerate Farey-neighbour pairs.  The long zero-left
// spine is counted algebraically; DFS visits only bounded off-spine subtrees.
#[derive(Clone, Copy)]
struct I {
    a: u64,
    b: u64,
    c: u64,
    d: u64,
}
const L: u64 = 50_000_000;
fn midpoint(i: I) -> bool {
    100 * (i.a * i.d + i.b * i.c) < 2 * i.b * i.d
}
fn left(i: I) -> bool {
    100 * i.a < i.b
}
fn solve() -> u64 {
    let mut count = L - 50;
    let mut stack = Vec::new();
    let mut d = 100;
    while d * (d + 1) <= L {
        stack.push(I {
            a: 1,
            b: d + 1,
            c: 1,
            d,
        });
        d += 1;
    }
    while let Some(i) = stack.pop() {
        if i.b * i.d > L || !left(i) {
            continue;
        }
        if midpoint(i) {
            count += 1;
        }
        let (n, q) = (i.a + i.c, i.b + i.d);
        let l = I {
            a: i.a,
            b: i.b,
            c: n,
            d: q,
        };
        let r = I {
            a: n,
            b: q,
            c: i.c,
            d: i.d,
        };
        if l.b * l.d <= L {
            stack.push(l);
        }
        if r.b * r.d <= L && left(r) {
            stack.push(r);
        }
    }
    count
}
fn main() {
    println!("{}", solve());
}
