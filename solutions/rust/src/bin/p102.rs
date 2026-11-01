//! Project Euler 102: native Rust via the shared project_euler library.
fn main() {
    let answer = project_euler::solve(102).expect("implemented Euler problem");
    println!("{answer}");
}
