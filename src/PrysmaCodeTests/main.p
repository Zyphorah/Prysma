// fibonaci
scope {
   fn int main() {
   
      dec int a = 0;
      dec int[5] tableau = [a, 2, 3,5];
      dec int i = 0; 
      
      while(i<4) {
         call printInt(tableau[i]);
         aff i = i + 1;
      }

      return 0;
   }
}

