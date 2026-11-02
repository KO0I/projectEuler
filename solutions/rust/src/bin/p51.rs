//! Project Euler 51: Prime Digit Replacements.
use project_euler::extra_helpers::sieve;

fn solve() -> i64 {
    {
        let mut answer = None;
        'length: for len in 2..=7 {
            let upper = 10usize.pow(len as u32) - 1;
            let lower = 10usize.pow(len as u32 - 1);
            let p = sieve(upper);
            for n in lower..=upper {
                if !p[n] {
                    continue;
                }
                let d: Vec<usize> = (0..len).map(|k| (n / 10usize.pow(k as u32)) % 10).collect();
                for mask in 1..(1 << len) {
                    let selected: Vec<usize> = (0..len).filter(|k| mask & (1 << k) != 0).collect();
                    if !selected.iter().all(|&k| d[k] == d[selected[0]]) {
                        continue;
                    }
                    let coefficient: usize = selected.iter().map(|&k| 10usize.pow(k as u32)).sum();
                    let base = n - d[selected[0]] * coefficient;
                    let mut first = 0;
                    let mut count = 0;
                    for replacement in 0..10 {
                        if replacement == 0 && (mask & (1 << (len - 1)) != 0) {
                            continue;
                        }
                        let candidate = base + replacement * coefficient;
                        if p[candidate] {
                            if count == 0 {
                                first = candidate;
                            }
                            count += 1;
                        }
                    }
                    if count == 8 && first == n {
                        answer = Some(n as i64);
                        break 'length;
                    }
                }
            }
        }
        answer.expect("eight-prime family")
    }
}

fn main() {
    println!("{}", solve());
}
