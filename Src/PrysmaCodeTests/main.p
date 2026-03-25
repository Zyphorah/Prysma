class TestCallFunction
{
public: 
   fn void TestCallFunction() {}

   fn int32 test()
   {
      return 42;
   }
}

class TestAccesVariable
{
private: 
   dec int32 a_priv = 5;
   dec bool c_priv = true;
   dec int32[5] tab_priv = [1, 2, 3, 4, 5];
public:
   dec int32 a_pub = 5;
   dec bool c_pub = true;
   dec int32[5] tab_pub = [1, 2, 3, 4, 5]; 

   fn void TestAccesVariable() {}

   fn int32 getA()
   {
      return a_priv;
   }
   fn bool getC()
   {
      return c_priv;
   }
   fn int32 getTab(arg int32 index)
   {
      return tab_priv[index];
   }
}

fn bool testCallFunction()
{
   dec string[] msg = "1. Test de l'call d'une function dans une classe.";
   call print(msg);

   dec ptr executer = new TestCallFunction();

   if (call executer.test() == 42)
   {
      return true;
   }
   return false;
}

fn bool testAccesVariable()
{
   dec string[] msg = "2. Test de l'accès à une variable dans une classe.";
   call print(msg);

   dec ptr test = new TestAccesVariable();

   // Tester si les valeurs du array sont correctes array privé et public
   dec int32 index = 0;
   while (index < 5)
   {
      if (call test.getTab(index) != index + 1 || test.tab_pub[index] != index + 1)
      {
         return false;
      }
      aff index = index + 1;
   }

   if (call test.getA() == 5  && call test.getC() == true && test.a_pub == 5 && test.c_pub == true)
   {
      return true;
   }
   return false;
}

fn int32 main() 
{
    call print(call testCallFunction()); call backSlashN();
    call print(call testAccesVariable()); call backSlashN();

    return 0;
}
