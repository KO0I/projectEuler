// Project Euler 224: almost-right triangles II.
// Three Lorentz transformations preserve c^2-a^2-b^2=1 and form a complete
// increasing ternary tree rooted at (2,2,3); perimeter prunes subtrees.
fn visit(a: i64, b: i64, c: i64) -> u64 {
    if a + b + c > 75_000_000 {
        return 0;
    }
    let mut n = (a <= b) as u64;
    n += visit(a - 2 * b + 2 * c, 2 * a - b + 2 * c, 2 * a - 2 * b + 3 * c);
    n += visit(a + 2 * b + 2 * c, 2 * a + b + 2 * c, 2 * a + 2 * b + 3 * c);
    n + visit(
        -a + 2 * b + 2 * c,
        -2 * a + b + 2 * c,
        -2 * a + 2 * b + 3 * c,
    )
}
fn solve() -> u64 {
    visit(2, 2, 3)
}
fn main() {
    println!("{}", solve());
}
