//lets du a trick
//pldrm = first_num * second_num = (10^n-x)*(10^n-y) where n=digits count, x,y int and we want keep is small as possible for finding largest pldrm
//also pldrm  = "10^n — (x+y)"left + "xy"right
// some simple algebra and we get quadratic equation where z=x+y
//x^2 - zx + right = 0
//notice: its work oly for digit_count<8


#include <iostream>
#include <string>
#include <math.h>
#include <algorithm>


long long largest_palinfrome(int digit_count){
  if (digit_count==1) {
    return 9;
  }

  for (int z=2; z<pow(10,digit_count)-pow(10, digit_count-1)-1;z++){
    int left = int(pow(10,digit_count)) - z;
    std::string s_right = std::to_string(left);
    std::reverse(s_right.begin(),s_right.end());
    int right = std::stoi(s_right);

    float root1, root2 = 0;
    double delta = 0.000000001;

    // no root
    if(int(pow(z,2)) - 4*right < 0){
      continue;
    }
    //have root
    else{
      root1 = 0.5 * (z+(sqrt( (z*z-4*right)) ));
      root2 = 0.5 * (z-(sqrt( (z*z-4*right)) ));
    }

    if (abs(root1-round(root1))<delta || abs(root2-round(root2))<delta){
      return (int(pow(10,digit_count)))*left+right;}
  }
  return 0;//something going wrong
}


int main(int argc, char *argv[])
{
  std::cout << largest_palinfrome(3) << "\n";

  return 0;
}
