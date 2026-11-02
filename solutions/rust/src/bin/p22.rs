//! Project Euler 22: Names Scores.

fn solve() -> i64 {
    {
        let mut names: Vec<&str> = include_str!("../../../../names.txt")
            .split(',')
            .map(|s| s.trim().trim_matches('"'))
            .collect();
        assert_eq!(names.len(), 5163);
        names.sort_unstable();
        names
            .iter()
            .enumerate()
            .map(|(i, name)| {
                let score: i64 = name
                    .bytes()
                    .filter(|c| c.is_ascii_uppercase())
                    .map(|c| i64::from(c - b'A' + 1))
                    .sum();
                score * (i as i64 + 1)
            })
            .sum()
    }
}

fn main() {
    println!("{}", solve());
}
