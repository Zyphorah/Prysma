// fibonaci
scope {
   fn int fib(arg int n) {
      
      if (n <= 1) {
         return n;
      }
      else
      {
         dec int un = n-1; 
         dec int deux = n-2;
         dec int fib_un = call fib(un);
         dec int fib_deux = call fib(deux);
         
         dec int res = fib_un + fib_deux;
      }
      return res;
   }

   fn int main() {
      dec int n = 7; 
      dec int res = call fib(n);
      call printInt(res);
      return 0;
   }
}