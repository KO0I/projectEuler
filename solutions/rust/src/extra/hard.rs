//! More involved native algorithms: prime cliques, finite Markov chains,
//! exact rational arithmetic, anagram squares, logarithm intervals,
//! exhaustive special-sum-set optimization.
use crate::extra_helpers::*;
use num_bigint::BigUint;
use num_traits::{One, Zero};
use std::collections::HashMap;

fn p60_concat(a:u64,b:u64)->u64 {let mut power=10;while power<=b {power*=10;}a*power+b}
struct PrimeGraph {primes:Vec<u64>, edges:Vec<Vec<bool>>}
impl PrimeGraph {
    fn new(limit:usize,best:u64)->Self {
        let primes:Vec<u64>=primes(limit).into_iter().filter(|&v|v!=2&&v!=5).collect();
        let n=primes.len();let mut edges=vec![vec![false;n];n];
        for i in 0..n {for j in i+1..n {
            let (a,b)=(primes[i],primes[j]);
            if a+b>=best || (a+b)%3==0 {continue;}
            if prime64(p60_concat(a,b))&&prime64(p60_concat(b,a)) {edges[i][j]=true;edges[j][i]=true;}
        }}Self{primes,edges}
    }
    fn search(&self,depth:usize,sum:u64,candidates:&[usize],best:&mut u64){
        let needed=5-depth;
        if candidates.len()<needed{return;}
        let lower=sum+candidates[..needed].iter().map(|&i|self.primes[i]).sum::<u64>();
        if lower>=*best {return;}
        if needed==1 {*best=lower;return;}
        for i in 0..=candidates.len()-needed {
            let bound=sum+candidates[i..i+needed].iter().map(|&j|self.primes[j]).sum::<u64>();
            if bound>=*best {break;}
            let head=candidates[i];let rest:Vec<usize>=candidates[i+1..].iter().copied().filter(|&j|self.edges[head][j]).collect();
            self.search(depth+1,sum+self.primes[head],&rest,best);
        }
    }
    fn minimum(&self,best:&mut u64){let candidates:Vec<usize>=(0..self.primes.len()).collect();
        self.search(0,0,&candidates,best);
    }
}
fn p60()->i64 {let mut best=u64::MAX;let mut limit=1000usize;
    loop {PrimeGraph::new(limit,best).minimum(&mut best);
        if best!=u64::MAX {break;}limit*=2;
    }
    PrimeGraph::new(best as usize-1,best).minimum(&mut best);best as i64
}
const STAY:i8=-1;
const NEXT_R:i8=-2;
const NEXT_U:i8=-3;
const BACK:i8=-4;
const SQUARES:usize=40;
const STATES:usize=SQUARES*3*16*16;
const CHEST:[i8;16]=[STAY,0,STAY,STAY,STAY,STAY,10,STAY,STAY,STAY,STAY,STAY,STAY,STAY,STAY,STAY];
const CHANCE:[i8;16]=[STAY,NEXT_R,24,STAY,0,NEXT_U,STAY,11,BACK,10,STAY,5,NEXT_R,STAY,39,STAY];
fn p84_index(square:usize,doubles:usize,cc:usize,ch:usize)->usize {((square*3+doubles)*16+cc)*16+ch}
fn p84_transition(mut state:usize,roll:(usize,bool),cc_deck:&[i8;16],ch_deck:&[i8;16])->usize {
    let mut ch=state%16;state/=16;let mut cc=state%16;state/=16;
    let mut doubles=state%3;let mut square=state/3;
    if roll.1 && doubles==2 {return p84_index(10,0,cc,ch);}
    doubles=if roll.1 {doubles+1}else{0};square=(square+roll.0)%40;
    let mut jailed=square==30;
    if [7,22,36].contains(&square) {let card=ch_deck[ch];ch=(ch+1)%16;
        if card>=0 {square=card as usize;jailed=card==10;}
        else if card==NEXT_R {square=match square {7=>15,22=>25,_=>5};}
        else if card==NEXT_U {square=if square==22 {28}else{12};}
        else if card==BACK {square-=3;}
    }
    if [2,17,33].contains(&square){let card=cc_deck[cc];cc=(cc+1)%16;
        if card>=0 {square=card as usize;jailed=card==10;}
    }
    if jailed {square=10;doubles=0;}
    p84_index(square,doubles,cc,ch)
}
fn p84()->i64 {
    let sides=4usize;let mut rolls:Vec<(usize,bool,usize)>=Vec::new();
    for a in 1..=sides {for b in 1..=sides {
        let sum=a+b;let is_double=a==b;
        if let Some(item)=rolls.iter_mut().find(|r|r.0==sum&&r.1==is_double){item.2+=1;}
        else {rolls.push((sum,is_double,1));}
    }}
    let count=rolls.len();let mut dest=vec![0u16;STATES*count];
    for state in 0..STATES {for (i,r) in rolls.iter().enumerate() {
        dest[state*count+i]=p84_transition(state,(r.0,r.1),&CHEST,&CHANCE) as u16;
    }}
    let mut probability=vec![1.0/STATES as f64;STATES];let mut next=vec![0.0f64;STATES];
    let mut converged=false;
    for _ in 0..30000 {next.fill(0.0);
        for state in 0..STATES {let mass=probability[state]/(sides*sides) as f64;
            for (roll,&(_,_,multiplicity)) in rolls.iter().enumerate() {
                next[dest[state*count+roll] as usize]+=mass*multiplicity as f64;
            }
        }
        let total:f64=next.iter().sum();let mut difference=0f64;
        for i in 0..STATES {next[i]/=total;difference+=(next[i]-probability[i]).abs();}
        std::mem::swap(&mut probability,&mut next);
        if difference<1e-13 {converged=true;break;}
    }
    assert!(converged,"Monopoly full-state power iteration did not converge");
    let mut marginal=[0.0f64;40];
    for (state,&mass) in probability.iter().enumerate(){marginal[state/(3*16*16)]+=mass;}
    let mut ranking:Vec<usize>=(0..40).collect();
    ranking.sort_by(|&a,&b|marginal[b].total_cmp(&marginal[a]).then(a.cmp(&b)));
    (10000*ranking[0]+100*ranking[1]+ranking[2]) as i64
}
#[derive(Clone,Copy)]struct Ratio{n:i64,d:i64}
impl Ratio {
    fn new(mut n:i64,mut d:i64)->Self {assert!(d!=0);
        if d<0 {n=-n;d=-d;}
        let divisor=gcd(n.unsigned_abs(),d as u64) as i64;
        Self{n:n/divisor,d:d/divisor}
    }
    fn add(a:Self,b:Self)->Self {Self::new(a.n*b.d+b.n*a.d,a.d*b.d)}
    fn sub(a:Self,b:Self)->Self {Self::new(a.n*b.d-b.n*a.d,a.d*b.d)}
    fn mul(a:Self,b:Self)->Self {Self::new(a.n*b.n,a.d*b.d)}
    fn div(a:Self,b:Self)->Option<Self> {if b.n==0 {None}else{Some(Self::new(a.n*b.d,a.d*b.n))}}
}
fn p93_score(d:[i64;4])->usize {
    fn evaluate(nums:&[Ratio],reached:&mut [bool;512]) {
        if nums.len()==1 {let v=nums[0];if v.d==1 && v.n>0 && v.n<reached.len() as i64 {reached[v.n as usize]=true;}
            return;
        }
        for i in 0..nums.len() {for j in i+1..nums.len() {
            let a=nums[i];let b=nums[j];let mut rest=Vec::with_capacity(nums.len()-1);
            for (k,&v) in nums.iter().enumerate(){if k!=i&&k!=j{rest.push(v);}}
            let mut choices=vec![Ratio::add(a,b),Ratio::sub(a,b),Ratio::sub(b,a),Ratio::mul(a,b)];
            if let Some(c)=Ratio::div(a,b){choices.push(c);}
            if let Some(c)=Ratio::div(b,a){choices.push(c);}
            for value in choices {rest.push(value);evaluate(&rest,reached);rest.pop();}
        }}
    }
    let data=d.map(|v|Ratio::new(v,1));let mut reached=[false;512];
    evaluate(&data,&mut reached);
    (1..reached.len()).find(|&i|!reached[i]).unwrap_or(512)-1
}
fn p93()->i64 {let mut best=(0usize,0i64);
    for a in 0..=6 {for b in a+1..=7 {for c in b+1..=8 {for d in c+1..=9 {
        let score=p93_score([a,b,c,d]);
        if score>best.0 {best=(score,1000*a+100*b+10*c+d);}
    }}}}best.1
}
fn p98()->i64 {
    let words:Vec<&str>=include_str!("../../../../tests/data/p098_words.txt")
        .split(',').map(|w|w.trim().trim_matches('"')).collect();
    let mut groups:HashMap<Vec<u8>,Vec<&str>>=HashMap::new();
    for word in &words {let mut key=word.as_bytes().to_vec();key.sort_unstable();groups.entry(key).or_default().push(word);}
    let mut best=0u64;
    for group in groups.values() {if group.len()<2 {continue;}
        let length=group[0].len() as u32;assert!(length<19);
        let lower=10u64.pow(length-1);let upper=10u64.pow(length);
        let mut start=isqrt(lower);if start*start<lower {start+=1;}
        let finish=isqrt(upper-1);
        for (i,&a) in group.iter().enumerate() {for &b in &group[i+1..] {
            for root in start..=finish {let square=root*root;let digits=square.to_string();
                let mut letters=[None::<u8>;26];let mut used=[None::<u8>;10];let mut valid=true;
                for (c,ch) in a.bytes().zip(digits.bytes()) {let index=(c-b'A') as usize;let digit=ch-b'0';
                    if letters[index].is_some_and(|v|v!=digit)||used[digit as usize].is_some_and(|v|v!=c){valid=false;break;}
                    letters[index]=Some(digit);used[digit as usize]=Some(c);
                }
                if !valid {continue;}
                let mut mapped=0u64;
                for c in b.bytes(){let digit=letters[(c-b'A') as usize].unwrap();
                    if mapped==0 && digit==0 {valid=false;break;}
                    mapped=mapped*10+digit as u64;
                }
                if valid && is_square(mapped) {best=best.max(square).max(mapped);}
            }
        }}
    }best as i64
}
/// An exact interval enclosing `scale * ln((den+num)/(den-num))`.
/// Each 2*z^(2j+1)/(2j+1) term is rounded downward; the omitted tail is
/// bounded by its first term times the geometric-series denominator.
fn logarithm_interval(num:u64,den:u64,scale:&BigUint)->(BigUint,BigUint){
    assert!(num<den&&3*num<=den);
    let (numerator,denominator)=(BigUint::from(num),BigUint::from(den));
    let nsq=&numerator*&numerator;let dsq=&denominator*&denominator;
    let mut power=numerator.clone();let mut power_den=denominator.clone();let mut lower=BigUint::zero();
    const TERMS:usize=24;
    for j in 0..TERMS {
        lower+=((&power*scale)<<1usize)/(&power_den*BigUint::from((2*j+1) as u64));
        power*=&nsq;power_den*=&dsq;
    }
    let divisor=&power_den*BigUint::from((2*TERMS+1) as u64)*(&dsq-&nsq);
    let remainder=(((&power*scale*&dsq)<<1usize)+&divisor-BigUint::one())/&divisor;
    let upper=&lower+BigUint::from(TERMS as u64)+remainder;
    (lower,upper)
}
fn p99()->i64 {
    let lines:Vec<(u64,u32)>=include_str!("../../../../tests/data/p099_base_exp.txt").lines().map(|line|{
        let (base,exponent)=line.split_once(',').expect("base,exponent");
        (base.parse().unwrap(),exponent.parse().unwrap())
    }).collect();assert_eq!(lines.len(),1000);
    let scale=BigUint::one()<<192usize;
    let (log2_low,log2_high)=logarithm_interval(1,3,&scale);
    let mut best=(0usize,BigUint::zero(),BigUint::zero());
    for (index,&(base,exponent)) in lines.iter().enumerate() {
        let power2=1u64 << (63-base.leading_zeros());
        let (mut low,mut high)=logarithm_interval(base-power2,base+power2,&scale);
        let k=power2.trailing_zeros() as u64;
        low+=&log2_low*k;high+=&log2_high*k;
        low*=exponent;high*=exponent;
        let larger= if best.0==0 {true} else if low>best.2 {true} else if high<best.1 {false}
            else {let (prev_base,prev_exp)=lines[best.0-1];
                BigUint::from(base).pow(exponent)>BigUint::from(prev_base).pow(prev_exp)
            };
        if larger {best=(index+1,low,high);}
    }best.0 as i64
}
fn p103()->i64 {
    fn cardinality_ok(values:&[u64;7],chosen:usize)->bool{
        for k in 1..=3 {if chosen<k+1 {continue;}
            let small:usize=(0..=k).map(|i|values[i] as usize).sum();
            let large:usize=(7-k..7).map(|i|if i<chosen {values[i] as usize}
                else {values[chosen-1] as usize+i-chosen+1}).sum();
            if small<=large {return false;}
        }true
    }
    fn dfs(chosen:usize,total:u64,current:&mut [u64;7],best:&mut [u64;7],best_sum:&mut u64,sums:&[u64],seen:&[bool;512]){
        if chosen==7 {*best_sum=total;*best=*current;return;}
        let remain=(7-chosen) as u64;let first=if chosen==0 {1}else{current[chosen-1]+1};
        for value in first.. {let minimum=total+remain*value+remain*(remain-1)/2;
            if minimum>=*best_sum {break;}
            current[chosen]=value;
            if !cardinality_ok(current,chosen+1) {continue;}
            if sums.iter().any(|&s|seen[(s+value) as usize]) {continue;}
            let mut next_sums=sums.to_vec();let mut next_seen=*seen;
            for &sum in sums {next_sums.push(sum+value);next_seen[(sum+value) as usize]=true;}
            dfs(chosen+1,total+value,current,best,best_sum,&next_sums,&next_seen);
        }
    }
    // The size-six optimal set is given in the problem statement, and the
    // middle-element construction provides a verified feasible upper bound.
    const PRIOR:[u64;6]=[11,18,19,20,22,25];let mut best=[0u64;7];best[0]=PRIOR[3];
    for i in 0..6 {best[i+1]=PRIOR[3]+PRIOR[i];}
    let mut best_sum=best.iter().sum();let mut current=[0u64;7];
    let mut seen=[false;512];seen[0]=true;
    dfs(0,0,&mut current,&mut best,&mut best_sum,&[0],&seen);
    best.iter().fold(0i64,|v,&part|{let magnitude=10i64.pow(part.ilog10()+1);v*magnitude+part as i64})
}
pub fn solve(n:u32)->Option<i64>{Some(match n {
    60=>p60(),84=>p84(),93=>p93(),98=>p98(),99=>p99(),103=>p103(),
    _=>return None,
})}
#[cfg(test)] mod tests {
    use super::*;
    #[test] fn rationals() {assert_eq!(Ratio::div(Ratio::new(3,2),Ratio::new(9,4)).unwrap().n,2);}
    #[test] fn primes64(){for n in 0..100_000 {assert_eq!(prime64(n),prime(n));}}
    #[test] fn log_enclosure(){let scale=BigUint::one()<<192usize;
        let (low,high)=logarithm_interval(1,3,&scale);assert!(low<high);
    }
}
