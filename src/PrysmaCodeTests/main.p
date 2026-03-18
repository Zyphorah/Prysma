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

      fn void Chien(arg int32 test)
      {
         aff _test = test;
         call print(_test);
      }

      fn void crier()
      {   
         dec string[] crie = "Woof!";
         dec ptr animal = new Animal();
         call animal.crier(ref crie);
      }
}

fn int32 main() 
{
   dec int32 entier = 204; 
   dec ptr chien = new Chien(entier);

   call chien.crier();

   return 0;
}