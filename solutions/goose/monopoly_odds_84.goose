// Project Euler 84: Monopoly Odds.
import euler from "../../support/goose";

// Independent uniformly distributed card draws.
// Doubles persist unless sent to jail.
recursive fn land(out: f64[120]&, p: i64, d: i64, m: f64) {
    if p == 30 {
        out[10] += m;
        return;
    }
    if p == 2 || p == 17 || p == 33 {
        out[d * 40] += m / 16.0;
        out[10] += m / 16.0;
        out[d * 40 + p] += m * 14.0 / 16.0;
        return;
    }
    if p == 7 || p == 22 || p == 36 {
        for q in [0, 11, 24, 39, 5] {
            out[d * 40 + q] += m / 16.0;
        }
        out[10] += m / 16.0;
        let rail = if p == 7 {
            15
        } else if p == 22 {
            25
        } else {
            5
        };
        let util = if p == 22 {
            28
        } else {
            12
        };
        out[d * 40 + rail] += m * 2.0 / 16.0;
        out[d * 40 + util] += m / 16.0;
        land(out, p - 3, d, m / 16.0);
        out[d * 40 + p] += m * 6.0 / 16.0;
        return;
    }
    out[d * 40 + p] += m;
}

fn solve() -> i64 {
    var dist = [0.0; 120];
    dist[0] = 1.0;
    for _ in 400 {
        var next = [0.0; 120];
        for state in 120 {
            for a in 1..5 {
                for b in 1..5 {
                    let doubles = if a == b {
                        state / 40 + 1
                    } else {
                        0
                    };
                    let mass = dist[state] / 16.0;
                    if doubles == 3 {
                        next[10] += mass;
                    } else {
                        land(next, (state % 40 + a + b) % 40, doubles, mass);
                    }
                }
            }
        }
        dist = next;
    }
    var odds = [0.0; 40];
    for s in 120 {
        odds[s % 40] += dist[s];
    }
    var answer = 0;
    for _ in 3 {
        var best = 0;
        for i in 1..40 {
            if odds[i] > odds[best] {
                best = i;
            }
        }
        answer = answer * 100 + best;
        odds[best] = -1.0;
    }
    return answer;
}

fn main() {
    print(solve());
}
