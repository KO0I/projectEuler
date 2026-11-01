//! Project Euler 61: native Rust via the shared project_euler library.
fn main() {
    let answer = project_euler::solve(61).expect("implemented Euler problem");
    println!("{answer}");
}
