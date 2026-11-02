// Project Euler 44: Pentagon Numbers.
import euler from "../../support/goose";
fn pent(n: i64) -> bool {
    let s = sqrt((24 * n + 1) as f64) as! i64;
    return s * s == 24 * n + 1 && (s + 1) % 6 == 0;
}
fn solve() -> i64 {
    // Search differences in ascending order. For k > j,
    // 2(P_k-P_j) = (k-j)(3(k+j)-1). Enumerate its possible factors.
    var index = 1;
    loop {
        let difference = index * (3 * index - 1) / 2;
        var gap = 1;
        while 3 * gap * gap < 2 * difference {
            if (2 * difference) % gap == 0 {
                let quotient = 2 * difference / gap;
                if (quotient + 1) % 3 == 0 {
                    let sum = (quotient + 1) / 3;
                    if sum > gap && (sum - gap) % 2 == 0 {
                        let j = (sum - gap) / 2;
                        let k = (sum + gap) / 2;
                        if pent(j * (3 * j - 1) / 2 + k * (3 * k - 1) / 2) {
                            return difference;
                        }
                    }
                }
            }
            gap++;
        }
        index++;
    }
}
fn main() { print(solve());
}
