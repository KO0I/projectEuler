//! Project Euler 88: native Rust via the shared project_euler library.
fn main() {
    let answer = project_euler::solve(88).expect("implemented Euler problem");
    println!("{answer}");
}
