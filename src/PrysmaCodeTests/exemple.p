
// Teste simple de boucle récursive
scope 
{
   fn int teste(arg int a, arg int b)
   {
      call printInt(a);
      call printInt(b);
      return 0;
   }

   fn int main()
   {
        dec int a = 10 + 10; 
        dec int b = a + 10 + a;
        dec float c = 1;
        dec bool d = 1; 
        dec int teste_var = ref a;
        aff a = 10;
        dec int e = 14;
        dec int f = 15; 
        call teste(e, f);

        return a;
   }
}


// calculer une factoriel sans condition 
scope {
   fn int fact_accum(arg int produit, arg int n)
   {
      aff produit =  produit * n;     
      call printInt( produit);       
      dec int suivant = n + 1;
      call fact_accum( produit, n);
      return produit;
   }

   fn int main()
   {
      dec int resultat = 1;   
      dec int nombre = 2;             
      call fact_accum(resultat, nombre);    
      return resultat;
   }
}

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