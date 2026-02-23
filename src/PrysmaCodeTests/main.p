scope {
  
   fn void teste()
   {
      call print(1);
      call print(2);
      dec string[] machaine = "string";
      call print(ref machaine); call backSlashN();
   }

   fn int64 main() 
   {
      dec int64 a = 10; 

      call teste();
      return 0;
   }
}
