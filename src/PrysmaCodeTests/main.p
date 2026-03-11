class NomClasse 
{ 
   private: 
      dec int64 attributPrive = 0;
      dec string[] attributPrive2 = " ";
      fn void methode(arg int64 parametre, arg string parametre2)
         {
            aff attributPrive = parametre;
            aff attributPrive2 = parametre2;
      }
   fin;

   public:
      fn void NomClasse(arg int64 parametre, arg string parametre2)
      {
            aff attributPrive = parametre;
            aff attributPrive2 = parametre2;
      }

      fn void methode(arg int64 parametre, arg string[] parametre2)
      {
         aff attributPrive = parametre;
            aff attributPrive2 = parametre2;
      }
   fin;
}

fn int32 main() 
{
  
   return 0;
}
