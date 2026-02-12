scope {
   fn int main() {
      dec bool b = 10<=(1+50);
      call printBool(b);

      if (b)
      {
         call printBool(b);
      }
      
      return 0;
   }
}