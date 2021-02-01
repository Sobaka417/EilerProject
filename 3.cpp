#include <iostream>
#include <math.h>

//we use simple solution, since number in tast is pretty low, for big number we need more efficient algo like pollard rho

int largest_prime_factor(long long number){
    long long max_prime_div = -1;
    //Exclude even numbers
    while(number%2==0){
      max_prime_div = 2;
      number /=2;}
    for (int i = 3; i < sqrt(number); i+=2) {
  while (number%i==0) {
    max_prime_div = i;
    number /=i;
  }
 }
    if (number>2) {
      max_prime_div = number;
    }
    return max_prime_div;

}

int main(int argc, char *argv[])
{
  long long result =largest_prime_factor(600851475143);
  std::cout << result << "\n";


  return 0;
}
