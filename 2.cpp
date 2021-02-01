#include <iostream>
#include <math.h>


int even_fibbonachi(){
  int first_fibo = 1;
  int second_fibo = 1;
  int next = 0;
  int sequence_number = 3; //we begin with 3. member of fiboncahhi (2) 1,1,2
  int even_sum = 0;
  int max_fibo_num = 4*1000000;
 while(next < max_fibo_num){
   next  = first_fibo + second_fibo;
   first_fibo = second_fibo;
   second_fibo = next;
   if (sequence_number%3==0) {  //every third is even
     even_sum += next;
     std::cout << next << "\n";
   }
   sequence_number++;
 }
 return even_sum;
}


int main(int argc, char *argv[])
{
  std::cout << even_fibbonachi() << "\n";

  return 0;
}
