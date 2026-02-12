scope {
  
   fn int test()
   {
      return 2;
   }
   
   fn int main()
   {
      dec int resultat = call test();
  
      call printInt(resultat);
      return resultat;
   }
}
