// fibonaci
scope {

   fn int[10] teste(arg int[10] tableau)
   {
      aff tableau[0] = 10; 
      return tableau; 
   }

   fn int main() 
   {
      dec int[10] tableau = [1,2,3,4];

      dec int[10] tableauCopier = call teste(tableau);

      dec int i = 0;

      while(i < 10)
      {
         call printInt(tableauCopier[i]);
         aff i = i + 1;
      }
     
      
      return 0;
   }
}