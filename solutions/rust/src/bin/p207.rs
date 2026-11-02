// Project Euler 207: integer partition equations.
// With k=m(m-1), perfect partitions occur exactly at powers-of-two m.
fn solve() -> u64 {
    let (mut perfect, mut next) = (0u64, 2u64);
    for m in 2u64.. {
        if m == next {
            perfect += 1;
            next *= 2;
        }
        if perfect * 12345 < m - 1 {
            return m * (m - 1);
        }
    }
    unreachable!()
}
fn main() {
    println!("{}", solve());
}
