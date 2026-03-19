class Animal
{
   public:

      fn void Animal()
      {
      }

      fn void crier(arg string[] crie)
      {
         call print(unref crie);
      }
}

class Chien 
{ 
   private: 

   dec int64 _test = 0; 

   public:
   dec int32 valeurPublic = 2134;

      fn void Chien(arg int32 test, arg int32 test2)
      {
         aff _test = test;
  
         call print(_test);
         call print(test2);
      }

      fn void crier()
      {   
         dec string[] crie = "Woof!";
         dec int32 test = 42;
         dec ptr animal = new Animal();
         call animal.crier(ref crie);
      }
}

fn int32 main() 
{
   dec int32 entier = 204; 

   dec ptr chien = new Chien(entier, 10);

   call chien.crier();
   //aff chien.valeurPublic = 105;
   call print(chien.valeurPublic);

   return 0;
}