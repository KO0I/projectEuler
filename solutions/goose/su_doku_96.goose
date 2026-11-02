// Project Euler 96: Su Doku.
import euler from "../../support/goose";

recursive fn search(cells: i64[81]&) -> bool {
    var chosen = -1;
    var options = 0;
    var minimum = 10;
    for i in 81 {
        if cells[i] != 0 {
            continue;
        }
        let r = i / 9;
        let c = i % 9;
        var mask = 0;
        for j in 9 {
            mask |= 1 << cells[9 * r + j];
            mask |= 1 << cells[9 * j + c];
            mask |= 1 << cells[(r / 3 * 3 + j / 3) * 9 + c / 3 * 3 + j % 3];
        }
        mask = (~mask) & 1022;
        let count = popcount(mask as u64);
        if count == 0 {
            return false;
        }
        if count < minimum {
            minimum = count;
            chosen = i;
            options = mask;
        }
    }
    if chosen < 0 {
        return true;
    }
    for d in 1..10 {
        if options & (1 << d) != 0 {
            cells[chosen] = d;
            if search(cells) {
                return true;
            }
        }
    }
    cells[chosen] = 0;
    return false;
}

fn solve() -> i64 {
    let text = file_text("tests/data/p096_sudoku.txt");
    let lines = split(text[..], '\n');
    var index = 0;
    var sum = 0;
    for _ in 50 {
        assert(index < lines.len && starts_with(lines[index], "Grid "));
        index++;
        var cells = [0; 81];
        for r in 9 {
            let line = trim(lines[index]);
            index++;
            assert(line.len == 9);
            for c in 9 {
                cells[r * 9 + c] = line[c] - '0';
            }
        }
        assert(search(cells));
        sum += 100 * cells[0] + 10 * cells[1] + cells[2];
    }
    return sum;
}

fn main() {
    print(solve());
}
