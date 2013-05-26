//  integer power function
int powi(int base, int exp){
  if (exp == 0) return 1;
  if (exp <  0) return 0;
  int i,res = 1;
    for(i=0; i<exp; i++){
      res *= base;
    }
  return res;
}
