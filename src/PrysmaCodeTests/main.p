
class Animal
{

   public:

      fn void Animal()
      {

      }

      fn void crier(arg string[] crie)
      {
         call print(crie);
      }

   fin;
}

class Chien 
{ 
   private: 
     // dec ptr animal = new Animal();
      
      dec string[] crie = " ";

      fn void crierPriv()
      {
         call print("Woof!");
      }

   fin;

   public:

      fn void Chien()
      {
         aff crie = "Woof!";
      }

      fn void crier()
      {
         //animal.crier(crie);
      }
   fin;
}


fn int32 main() 
{
   //dec ptr Chien = new Chien();

  // call objet.methodePrint();

   return 0;
}
