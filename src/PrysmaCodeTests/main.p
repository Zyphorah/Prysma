scope {
   fn int testArgDansIf(arg int val)
   {
      if (val == 100)
      {
         return val;
      }
      return 0;
   }
 
   fn int main() {
      dec int a = call testArgDansIf(100);
      call printInt(a);

      dec int b = call testArgDansIf(50);
      call printInt(b);

      return 0;
   }
}