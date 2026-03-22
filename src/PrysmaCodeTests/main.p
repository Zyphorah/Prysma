class testAppelFonction
{
public: 

   fn void testAppelFonction()
   {
      
   }

   fn int64 test()
   {
      return 42;
   }
}

fn bool testAppelFonction()
{
   dec string[] msg = "Test de l'appel d'une fonction dans une classe.";
   call print(msg);

   dec ptr test = new testAppelFonction();

   if (test.test() == 42)
   {
      return true;
   }
   return false;
}

fn int32 main() 
{
   call print(testAppelFonction()); call backSlashN();

   return 0;
}