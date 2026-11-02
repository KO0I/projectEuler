// Project Euler 54: Poker Hands.
import euler from "../../support/goose";

fn rank(c: u8) -> i64 {
    let s = "23456789TJQKA";
    for i in s.len {
        if s[i] == c {
            return i + 2;
        }
    }
    abort("bad card");
}

fn score(v0: i64[5], suits: i64[5]) -> i64 {
    var v = v0;
    sort(v[..]);
    var counts = [0; 15];
    for r in v {
        counts[r]++;
    }
    var flush = true;
    for s in suits {
        if s != suits[0] {
            flush = false;
        }
    }
    var straight = 0;
    if v[4] - v[0] == 4 {
        var unique = true;
        for i in 1..5 {
            if v[i] == v[i - 1] {
                unique = false;
            }
        }
        if unique {
            straight = v[4];
        }
    }
    if v == [2, 3, 4, 5, 14] {
        straight = 5;
    }
    if flush && straight > 0 {
        return 8 * ipow(15, 5) + straight;
    }
    var groups = [0; 5];
    var k = 0;
    for count in [4, 3, 2, 1] {
        var r = 14;
        while r >= 2 {
            if counts[r] == count {
                groups[k] = r;
                k++;
            }
            r--;
        }
    }
    var category = 0;
    if counts[groups[0]] == 4 {
        category = 7;
    } else if counts[groups[0]] == 3 && counts[groups[1]] == 2 {
        category = 6;
    } else if flush {
        category = 5;
    } else if straight > 0 {
        category = 4;
    } else if counts[groups[0]] == 3 {
        category = 3;
    } else if counts[groups[0]] == 2 && counts[groups[1]] == 2 {
        category = 2;
    } else if counts[groups[0]] == 2 {
        category = 1;
    }
    var value = category * ipow(15, 5);
    if category == 4 {
        return value + straight;
    }
    if category == 5 || category == 0 {
        for i in 5 {
            value += v[4 - i] * ipow(15, 4 - i);
        }
    } else {
        for i in k {
            value += groups[i] * ipow(15, 4 - i);
        }
    }
    return value;
}

fn solve() -> i64 {
    let text = file_text("tests/data/p054_poker.txt");
    let lines = split(text[..], '\n');
    var wins = 0;
    var hands = 0;
    for line in lines {
        if trim(line).len == 0 {
            continue;
        }
        let cards = split(trim(line), ' ');
        assert(cards.len == 10);
        var left = [0; 5];
        var right = [0; 5];
        var ls = [0; 5];
        var rs = [0; 5];
        for i in 5 {
            left[i] = rank(cards[i][0]);
            ls[i] = cards[i][1];
            right[i] = rank(cards[i + 5][0]);
            rs[i] = cards[i + 5][1];
        }
        if score(left, ls) > score(right, rs) {
            wins++;
        }
        hands++;
    }
    assert(hands == 1000);
    return wins;
}

fn main() {
    print(solve());
}
