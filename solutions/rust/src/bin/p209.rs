// Project Euler 209: circular logic.
// The Boolean-state transition is a permutation.  Each cycle permits binary
// circular strings with no adjacent ones: F(L-1)+F(L+1).
fn transition(v: usize) -> usize {
    let (a, b, c) = ((v >> 5) & 1, (v >> 4) & 1, (v >> 3) & 1);
    ((v << 1) & 63) | (a ^ (b & c))
}
fn solve() -> u64 {
    let mut f = [0u64; 66];
    f[1] = 1;
    for i in 2..66 {
        f[i] = f[i - 1] + f[i - 2];
    }
    let mut seen = [false; 64];
    let mut answer = 1;
    for start in 0..64 {
        if seen[start] {
            continue;
        }
        let (mut len, mut v) = (0, start);
        loop {
            seen[v] = true;
            len += 1;
            v = transition(v);
            if v == start {
                break;
            }
        }
        answer *= f[len - 1] + f[len + 1];
    }
    answer
}
fn main() {
    println!("{}", solve());
}
