scope {

   fn int test(arg int[10] tableau)
   {
       call printInt(tableau[2]);
       return 0;
   }

   fn int main() 
   {
      dec int i = 10; 
      dec int[10] tableau = [1,2,4,5]; 

      call test(tableau);
     
      return 0;
   }

}