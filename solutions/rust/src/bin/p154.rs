//! Test v2 and v5 of trinomial coefficients using factorial valuation prefixes.
fn choose(v: &[u32], n: usize, k: usize) -> u32 {
    v[n] - v[k] - v[n - k]
}
fn solve() -> u64 {
    const N: usize = 200_000;
    let (mut twos, mut fives) = (vec![0u32; N + 1], vec![0u32; N + 1]);
    for n in 1..=N {
        let mut x = n;
        twos[n] = twos[n - 1];
        fives[n] = fives[n - 1];
        while x % 2 == 0 {
            twos[n] += 1;
            x /= 2
        }
        while x % 5 == 0 {
            fives[n] += 1;
            x /= 5
        }
    }
    let mut answer = 0;
    for i in 0..=N {
        let (a, b) = (choose(&twos, N, i), choose(&fives, N, i));
        if a >= 12 && b >= 12 {
            answer += (i + 1) as u64;
            continue;
        }
        for j in 0..=(i + 1) / 2 {
            if a + choose(&twos, i, j) >= 12 && b + choose(&fives, i, j) >= 12 {
                answer += 1;
                if j < i / 2 {
                    answer += 1
                }
            }
        }
    }
    answer
}
fn main() {
    println!("{}", solve());
}
