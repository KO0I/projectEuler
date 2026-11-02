// Project Euler 83: Path Sum: Four Ways.
import euler from "../../support/goose";

fn solve() -> i64 {
    let a = numbers("tests/data/p083_matrix.txt");
    assert(a.len == 6400);
    var dist = [1000000000; 6400];
    var done = [false; 6400];
    dist[0] = a[0];
    for _ in 6400 {
        var v = -1;
        for i in 6400 {
            if !done[i] && (v < 0 || dist[i] < dist[v]) {
                v = i;
            }
        }
        if v == 6399 {
            return dist[v];
        }
        done[v] = true;
        let x = v % 80;
        let y = v / 80;
        for k in 4 {
            let dx = [-1, 1, 0, 0];
            let dy = [0, 0, -1, 1];
            let xx = x + dx[k];
            let yy = y + dy[k];
            if xx >= 0 && xx < 80 && yy >= 0 && yy < 80 {
                let j = yy * 80 + xx;
                dist[j] = min(dist[j], dist[v] + a[j]);
            }
        }
    }
    abort("no path");
}

fn main() {
    print(solve());
}
