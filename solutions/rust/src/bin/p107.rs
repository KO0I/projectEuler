//! Project Euler 107: native Rust via the shared project_euler library.
fn main() {
    let answer = project_euler::solve(107).expect("implemented Euler problem");
    println!("{answer}");
}
