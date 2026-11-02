// Project Euler 240: top dice.
// Descend face values.  State tracks assigned dice, occupied top-ten slots,
// and their sum; binomial choices compose the exact multinomial roll count.
fn solve() -> u128 {
    let mut choose = [[0u128; 21]; 21];
    for n in 0..=20 {
        choose[n][0] = 1;
        choose[n][n] = 1;
        for k in 1..n {
            choose[n][k] = choose[n - 1][k - 1] + choose[n - 1][k];
        }
    }
    let size = 21 * 11 * 71;
    let ix = |u: usize, t: usize, s: usize| (u * 11 + t) * 71 + s;
    let mut dp = vec![0u128; size];
    dp[ix(0, 0, 0)] = 1;
    for face in (1..=12).rev() {
        let mut next = vec![0u128; size];
        for used in 0..=20 {
            for top in 0..=10 {
                for sum in 0..=70 {
                    let ways = dp[ix(used, top, sum)];
                    if ways == 0 {
                        continue;
                    }
                    for count in 0..=20 - used {
                        let enters = count.min(10 - top);
                        let ns = sum + face * enters;
                        if ns <= 70 {
                            next[ix(used + count, top + enters, ns)] +=
                                ways * choose[20 - used][count];
                        }
                    }
                }
            }
        }
        dp = next;
    }
    dp[ix(20, 10, 70)]
}
fn main() {
    println!("{}", solve());
}
