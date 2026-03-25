class TestCallFunction
{
public: 

   fn void TestCallFunction()
   {
      
   }

   fn int32 test()
   {
      return 42;
   }
}

class TestAccesObject
{
private:
   dec ptr a_priv = new TestCallFunction();
public:
   fn void TestAccesObject() {}

   fn ptr getA()
   {
      return a_priv;
   }
}


class TestAccesVariable
{
private: 
   dec int32 a_priv = 5;
   dec string[] b_priv = "Hello";
   dec bool c_priv = true;
public:
   dec int32 a_pub = 5;
   dec string[] b_pub = "Hello";
   dec bool c_pub = true;
   fn void TestAccesVariable()
   {
      
   }

   fn int32 getA()
   {
      return a_priv;
   }
   fn string[] getB()
   {
      return b_priv;
   }
   fn bool getC()
   {
      return c_priv;
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

   dec string[] b_priv = call test.getB();
   dec string[] b_pub =  test.b_pub;

   dec int32 index = 0; 
   // b_priv on vérifi que les éléments de b_priv sont égaux à ceux de "Hello"
   while (index < 5)
   {
      dec string[] hello = "Hello";
      if (unref b_priv[index] != unref hello[index] ||  unref b_pub[index] != unref hello[index])
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
   dec int32 entier = 10; 
   call print(entier);
   call print(call testCallFunction()); call backSlashN();
   call print(call testAccesVariable()); call backSlashN();
   call print(call testAccesObject()); call backSlashN();
   return 0;
}