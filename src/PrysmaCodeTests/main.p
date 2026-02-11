scope {
  
   fn int test()
   {
      return 10;
   }

   fn int main()
   {
      dec int resultat = call test();
      call printInt(resultat);
      return resultat;
   }
}