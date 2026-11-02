// Project Euler 53: Combinatoric Selections.
import euler from "../../support/goose";

fn solve() -> i64 {
    var row = [0; 101];
    row[0] = 1;
    var count = 0;
    for n in 1..101 {
        var r = n;
        while r > 0 {
            row[r] = min(1000001, row[r] + row[r - 1]);
            if row[r] > 1000000 {
                count++;
            }
            r--;
        }
    }
    return count;
}

fn main() {
    print(solve());
}
