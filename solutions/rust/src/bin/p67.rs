//! Project Euler 67: native Rust via the shared project_euler library.
fn main() {
    let answer = project_euler::solve(67).expect("implemented Euler problem");
    println!("{answer}");
}
