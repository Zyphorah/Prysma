class TestAppelFonction
{
public: 

   fn void TestAppelFonction()
   {
      
   }

   fn int32 test()
   {
      return 42;
   }
}

fn bool testAppelFonction()
{
   dec string[] msg = "Test de l'appel d'une fonction dans une classe.";
   call print(msg);
   dec ptr executer = new TestAppelFonction();

   if (call executer.test() == 42)
   {
      return true;
   }
   return false;
}

fn int32 main() 
{
   call print(call testAppelFonction()); call backSlashN();

   return 0;
}