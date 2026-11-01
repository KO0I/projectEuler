//! Project Euler 33: native Rust via the shared project_euler library.
fn main() {
    let answer = project_euler::solve(33).expect("implemented Euler problem");
    println!("{answer}");
}
