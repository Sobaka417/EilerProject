//Just use standart euclidian algo and gcd based lcm (lcm(a,b)=a*b/gcd(a,b))
//Note: lcm(a,b,c) = lcm(lcm(a,b),c)

#include <iostream>
#include <vector>
#include <math.h>
#include <string>

int gcd(int a, int b)
{
  if (a == 0)
    return b;
  return gcd(b % a, a);
}

long long lcm(long long a, int b){
  return (a*b)/gcd(a,b);
}

long long lcm_for_range(int min, int max){
  std::vector<int> a_lcm(max-min,0);
  for (int i = 0; i <= max-min; ++i) {
    a_lcm[i]=i+min;
  }
  std::cout << "range is " << a_lcm[0]<< " "<<  a_lcm[19] << "\n";

  long long cur_lcm =a_lcm[0];
  for (int i=1;i<=max-min; i++) {
    long long prev_lcm = cur_lcm;
    cur_lcm = lcm(cur_lcm,a_lcm[i]);
    std::cout << "lcm for "<< prev_lcm << " " <<  a_lcm[i] << " is "<< cur_lcm << "\n";
  }
  return cur_lcm;
}


int main(int argc, char *argv[])
{
  // int table_test[] = {4,7,6,21,42};
  // int table_test_size = 5;
  // lcm_table_method(table_test, table_test_size);

  int res = lcm_for_range(1,20);
  std::cout << "result is " << res << "\n";

  return 0;
}
