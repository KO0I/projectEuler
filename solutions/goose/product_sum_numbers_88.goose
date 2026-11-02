// Project Euler 88: Product-sum Numbers.
import euler from "../../support/goose";

recursive fn search(best: i64[12001]&, least: i64, product: i64, sum: i64, count: i64) {
    let k = product - sum + count;
    if k <= 12000 {
        best[k] = min(best[k], product);
    }
    var f = least;
    while f * product <= 24000 {
        search(best, f, product * f, sum + f, count + 1);
        f++;
    }
}

fn solve() -> i64 {
    var best = [24000; 12001];
    search(best, 2, 1, 0, 0);
    var seen = [false; 24001];
    var sum = 0;
    for k in 2..12001 {
        if !seen[best[k]] {
            seen[best[k]] = true;
            sum += best[k];
        }
    }
    return sum;
}

fn main() {
    print(solve());
}
