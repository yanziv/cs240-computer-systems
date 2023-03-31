#include <stdio.h>
 
   long getAndSumValues() {
	 long x;
	 printf("Enter an integer: ");
	 scanf(" %ld",&x);
	 if (x == 0) {
   	   return 0;
	 } else {
      	   return x + getAndSumValues();
	}
 }
                    	  
int main() {
   long result;
   result = getAndSumValues();
   printf("The result = %ld\n",result);
   return 0;
}
