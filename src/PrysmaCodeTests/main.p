class TestAppelFonction
{
public: 
   fn void TestAppelFonction() {}

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
public:
   dec int32 a_pub = 5;
   dec bool c_pub = true;

   fn void TestAccesVariable() {}

   fn int32 getA()
   {
      return a_priv;
   }
   fn bool getC()
   {
      return c_priv;
   }
}

fn bool testAppelFonction()
{
   dec string[] msg = "1. Test de l'appel d'une fonction dans une classe.";
   call print(msg);

   dec ptr executer = new TestAppelFonction();

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

   if (call test.getA() == 5  && call test.getC() == true && test.a_pub == 5 && test.c_pub == true)
   {
      return true;
   }
   return false;
}

fn bool testAccesObjet()
{
   dec string[] msg = "3. Test de l'accès à un objet dans une classe.";
   call print(msg);

   dec ptr test = new TestAccesObjet();
   dec ptr a = call test.getA();

   if (call a.test() == 42)
   {
      return true;
   }
   return false;
}

fn int32 main() 
{
    call print(call testAppelFonction()); call backSlashN();
    call print(call testAccesVariable()); call backSlashN();

    return 0;
}
