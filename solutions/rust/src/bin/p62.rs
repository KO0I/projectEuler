//! Project Euler 62: native Rust via the shared project_euler library.
fn main() {
    let answer = project_euler::solve(62).expect("implemented Euler problem");
    println!("{answer}");
}
