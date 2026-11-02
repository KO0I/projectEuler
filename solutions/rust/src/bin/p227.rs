// Project Euler 227: The Chase.
// Rotational symmetry leaves 99 separation states.  First-step expectation
// equations are solved by Gaussian elimination with partial pivoting.
fn solve() -> f64 {
    const N: usize = 99;
    let mut a = vec![[0f64; 100]; N];
    let shifts = [-2, -1, 0, 1, 2];
    let probs = [1.0 / 36.0, 2.0 / 9.0, 0.5, 2.0 / 9.0, 1.0 / 36.0];
    for k in 1..100 {
        let r = k - 1;
        a[r][r] = 1.0;
        a[r][N] = 1.0;
        for i in 0..5 {
            let next = (k as i32 + shifts[i]).rem_euclid(100) as usize;
            if next != 0 {
                a[r][next - 1] -= probs[i];
            }
        }
    }
    for col in 0..N {
        let pivot = (col..N)
            .max_by(|&x, &y| a[x][col].abs().total_cmp(&a[y][col].abs()))
            .unwrap();
        a.swap(col, pivot);
        let d = a[col][col];
        for j in col..=N {
            a[col][j] /= d;
        }
        for r in 0..N {
            if r == col {
                continue;
            }
            let f = a[r][col];
            for j in col..=N {
                a[r][j] -= f * a[col][j];
            }
        }
    }
    a[49][N]
}
fn main() {
    println!("{:.6}", solve());
}
