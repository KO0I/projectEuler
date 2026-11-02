// Project Euler 59: XOR Decryption.
import euler from "../../support/goose";

fn solve() -> i64 {
    let cipher = numbers("tests/data/p059_cipher.txt");
    var best = -1000000000;
    var answer = 0;
    for a in 97..123 {
        for b in 97..123 {
            for c in 97..123 {
                let key = [a, b, c];
                var score = 0;
                var sum = 0;
                var plain: u8[>..] = [];
                for i in cipher.len {
                    let ch = cipher[i] ^ key[i % 3];
                    if ch < 32 || ch > 126 {
                        score -= 1000;
                    }
                    if ch == 32 {
                        score += 5;
                    }
                    if contains("etaoinETAOIN", ch as! u8) {
                        score++;
                    }
                    sum += ch;
                    plain.push(ch as! u8);
                }
                for i in 0..plain.len - 4 {
                    if plain[i..i + 5] == " the " {
                        score += 20;
                    }
                }
                if score > best {
                    best = score;
                    answer = sum;
                }
            }
        }
    }
    return answer;
}

fn main() {
    print(solve());
}
