//! Project Euler 19: Counting Sundays.

fn leap(y: i32) -> bool {
    y % 4 == 0 && (y % 100 != 0 || y % 400 == 0)
}

fn solve() -> i64 {
    {
        let mut day = 1;
        let mut count = 0;
        let days = [31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31];
        for year in 1900..=2000 {
            for month in 0..12 {
                if year >= 1901 && day == 0 {
                    count += 1;
                }
                day = (day + days[month] + i32::from(month == 1 && leap(year))) % 7;
            }
        }
        count
    }
}

fn main() {
    println!("{}", solve());
}
