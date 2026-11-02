// Project Euler 236: luxury hampers.
// Reduce the shared product ratio to coprime r/s, derive each product's base
// spoilage pair, and solve the remaining linear Diophantine equation.
fn gcd(mut a: i64, mut b: i64) -> i64 {
    while b != 0 {
        (a, b) = (b, a % b);
    }
    a
}
fn solve() -> String {
    let aa = [5248, 1312, 2624, 5760, 3936];
    let bb = [640, 1888, 3776, 3776, 5664];
    let (mut valid, mut bu, mut bv) = (0, 0i64, 1i64);
    for s in 1..=aa[1] {
        for r in 1..=bb[1] {
            if gcd(r, s) != 1 || 41 * r <= 59 * s {
                continue;
            }
            let (mut u, mut v) = (41 * r, 59 * s);
            let g = gcd(u, v);
            u /= g;
            v /= g;
            let (mut coef, mut max) = ([0i64; 5], [0i64; 5]);
            let mut feasible = true;
            for i in 0..5 {
                let g = gcd(u * bb[i], v * aa[i]);
                let base_a = v * aa[i] / g;
                let base_b = u * bb[i] / g;
                max[i] = (aa[i] / base_a).min(bb[i] / base_b);
                feasible &= max[i] >= 1;
                coef[i] = 15_744 * v * base_a - 18_880 * u * base_b;
            }
            if !feasible {
                continue;
            }
            let max_t = max[1] + max[2] + max[4];
            let mut found = false;
            for k1 in 1..=max[0] {
                for k4 in 1..=max[3] {
                    let rem = -(coef[0] * k1 + coef[3] * k4);
                    if (coef[1] == 0 && rem == 0)
                        || (coef[1] != 0
                            && rem % coef[1] == 0
                            && (3..=max_t).contains(&(rem / coef[1])))
                    {
                        found = true;
                        break;
                    }
                }
                if found {
                    break;
                }
            }
            if found {
                valid += 1;
                if u * bv > bu * v {
                    bu = u;
                    bv = v;
                }
            }
        }
    }
    assert_eq!(valid, 35);
    format!("{bu}/{bv}")
}
fn main() {
    println!("{}", solve());
}
