class ProfondeurDeux
{
   public:
      dec int32 entier = 42; 

      fn void ProfondeurDeux()
      {
        
      }

      fn void test()
      {
         call print(entier);
      }
}


class Profondeur
{
   public:
      dec ProfondeurDeux profondeurDeux = new ProfondeurDeux();

      fn void Profondeur()
      {
        
      }

      fn ptr getProfondeurDeux()
      {
         return profondeurDeux;
      }
}


class TestClass
{

   public:
      dec Profondeur profondeur = new Profondeur();

      fn void TestClass()
      {
      }

      fn ptr getProfondeur()
      {
         return profondeur;
      }
}


fn int32 main() 
{
   dec TestClass test = new TestClass();
   dec ProfondeurDeux profondeur = call test.getProfondeur().getProfondeurDeux();
   call profondeur.test();

   return 0;
}