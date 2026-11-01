//! Project Euler 110: native Rust via the shared project_euler library.
fn main() {
    let answer = project_euler::solve(110).expect("implemented Euler problem");
    println!("{answer}");
}
