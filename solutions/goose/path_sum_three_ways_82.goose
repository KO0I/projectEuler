// Project Euler 82: Path Sum: Three Ways.
import euler from "../../support/goose";

fn solve() -> i64 {
    let a = numbers("tests/data/p082_matrix.txt");
    assert(a.len == 6400);
    var dist = [0; 80];
    for y in 80 {
        dist[y] = a[y * 80];
    }
    for x in 1..80 {
        for y in 80 {
            dist[y] += a[y * 80 + x];
        }
        for y in 1..80 {
            dist[y] = min(dist[y], dist[y - 1] + a[y * 80 + x]);
        }
        var y = 78;
        while y >= 0 {
            dist[y] = min(dist[y], dist[y + 1] + a[y * 80 + x]);
            y--;
        }
    }
    var best = dist[0];
    for d in dist {
        best = min(best, d);
    }
    return best;
}

fn main() {
    print(solve());
}
