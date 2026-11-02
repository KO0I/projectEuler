// Project Euler 222: sphere packing.
// Axial separation of radii a,b in the pipe is sqrt(200(a+b-50)).  Held-Karp
// subset DP finds the shortest Hamiltonian path including end-cap radii.
fn solve() -> i64 {
    const N: usize = 21;
    let states = 1usize << N;
    let mut sep = [[0f32; N]; N];
    for i in 0..N {
        for j in 0..N {
            sep[i][j] = (200.0 * ((30 + i + 30 + j - 50) as f32)).sqrt();
        }
    }
    let mut dp = vec![f32::MAX; states * N];
    for i in 0..N {
        dp[(1 << i) * N + i] = (30 + i) as f32;
    }
    for mask in 1..states {
        let mut bits = mask;
        while bits != 0 {
            let last = bits.trailing_zeros() as usize;
            bits &= bits - 1;
            let prev = mask ^ (1 << last);
            if prev == 0 {
                continue;
            }
            let mut pb = prev;
            let mut best = f32::MAX;
            while pb != 0 {
                let p = pb.trailing_zeros() as usize;
                pb &= pb - 1;
                best = best.min(dp[prev * N + p] + sep[p][last]);
            }
            dp[mask * N + last] = best;
        }
    }
    let full = states - 1;
    let mut last = (0..N)
        .min_by(|&a, &b| {
            (dp[full * N + a] + (30 + a) as f32).total_cmp(&(dp[full * N + b] + (30 + b) as f32))
        })
        .unwrap();
    let mut order = [0usize; N];
    let mut mask = full;
    for pos in (0..N).rev() {
        order[pos] = last;
        let prev = mask ^ (1 << last);
        if prev == 0 {
            break;
        }
        last = (0..N)
            .filter(|&p| prev >> p & 1 == 1)
            .min_by(|&a, &b| {
                (dp[prev * N + a] + sep[a][last]).total_cmp(&(dp[prev * N + b] + sep[b][last]))
            })
            .unwrap();
        mask = prev;
    }
    let mut length = (30 + order[0] + 30 + order[N - 1]) as f64;
    for i in 1..N {
        length += (200.0 * ((30 + order[i - 1] + 30 + order[i] - 50) as f64)).sqrt();
    }
    (length * 1000.0).round() as i64
}
fn main() {
    println!("{}", solve());
}
