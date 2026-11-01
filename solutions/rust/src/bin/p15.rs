//! Project Euler 15: native Rust via the shared project_euler library.
fn main() {
    let answer = project_euler::solve(15).expect("implemented Euler problem");
    println!("{answer}");
}
