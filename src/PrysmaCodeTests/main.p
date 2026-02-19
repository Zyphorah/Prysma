// fibonaci
scope {

  

   fn int teste(arg int[10] tableau)
   {
      call printInt(tableau[0]);
      return 0; 
   }

   fn int main() {

      dec int[10] tableau = [1,2,3,4];
      call teste(ref tableau);
   

      return 0;
   }
}