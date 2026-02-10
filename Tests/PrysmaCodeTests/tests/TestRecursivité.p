// Simple boucle récursive jusqu'au segmentation fault 

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