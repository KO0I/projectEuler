//! Project Euler 81: native Rust via the shared project_euler library.
fn main() {
    let answer = project_euler::solve(81).expect("implemented Euler problem");
    println!("{answer}");
}
