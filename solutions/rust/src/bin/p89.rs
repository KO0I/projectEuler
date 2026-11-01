//! Project Euler 89: native Rust via the shared project_euler library.
fn main() {
    let answer = project_euler::solve(89).expect("implemented Euler problem");
    println!("{answer}");
}
