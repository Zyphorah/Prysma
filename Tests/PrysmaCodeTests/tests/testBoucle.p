scope {
   fn bool testBoucle()
   {
      dec int i = 0;

      while (i < 5) {
         aff i = i + 1;
      }
      if (i == 5) {
         return true;
      } else {
         return false;
      }
      return false;
   }

   fn int main()
   {
      call printBool(call testBoucle());
      //call printBool(call testRecursivite());
      return 1;
   }
}
