// Project Euler 89: Roman Numerals.
import euler from "../../support/goose";

fn value(c: u8) -> i64 {
    if c == 'I' {
        return 1;
    }
    if c == 'V' {
        return 5;
    }
    if c == 'X' {
        return 10;
    }
    if c == 'L' {
        return 50;
    }
    if c == 'C' {
        return 100;
    }
    if c == 'D' {
        return 500;
    }
    if c == 'M' {
        return 1000;
    }
    abort("bad Roman digit");
}

fn solve() -> i64 {
    let text = file_text("tests/data/p089_roman.txt");
    let lines = split(text[..], '\n');
    var saved = 0;
    for line in lines {
        let s = trim(line);
        if s.len == 0 {
            continue;
        }
        var n = 0;
        for i in s.len {
            let v = value(s[i]);
            if i + 1 < s.len && v < value(s[i + 1]) {
                n -= v;
            } else {
                n += v;
            }
        }
        var shortest = 0;
        let vs = [1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1];
        let lens = [1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1];
        for i in vs.len {
            shortest += (n / vs[i]) * lens[i];
            n %= vs[i];
        }
        saved += s.len - shortest;
    }
    return saved;
}

fn main() {
    print(solve());
}
