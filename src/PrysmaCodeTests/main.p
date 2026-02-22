scope {
   import "teste.p";
   
   fn void teste()
   {
      call print(1);
   }

   fn int64 main() 
   {
      dec int64 a = 10; 

      call teste();
      return 0;
   }
}