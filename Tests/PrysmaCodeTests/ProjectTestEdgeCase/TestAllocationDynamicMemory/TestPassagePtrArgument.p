
fn bool verifierInt32(arg ptr pointeur)
{
   dec int32 val = unref pointeur;
   if (val == 50) {
      return true;
   } else {
      return false;
   }
   return false;
}

fn bool testPasserPtrInt32ParArgument()
{
   dec string[] functionnalite = "1.testPasserPtrInt32ParArgument : ";
   call print(ref functionnalite);
   dec ptr p = new int32;
   aff p = 50;
   dec bool resultat = call verifierInt32(p);
   delete p;
   if (resultat == true) {
      return true;
   } else {
      return false;
   }
   return false;
}

fn bool verifierValeurZero(arg ptr pointeur)
{
   dec int32 val = unref pointeur;
   if (val == 0) {
      return true;
   } else {
      return false;
   }
   return false;
}

fn bool testPasserPtrValeurZeroParArgument()
{
   dec string[] functionnalite = "2.testPasserPtrValeurZeroParArgument : ";
   call print(ref functionnalite);
   dec ptr p = new int32;
   aff p = 0;
   dec bool resultat = call verifierValeurZero(p);
   delete p;
   if (resultat == true) {
      return true;
   } else {
      return false;
   }
   return false;
}

fn bool verifierBool(arg ptr pointeur)
{
   dec bool val = unref pointeur;
   if (val == true) {
      return true;
   } else {
      return false;
   }
   return false;
}

fn bool testPasserPtrBoolParArgument()
{
   dec string[] functionnalite = "3.testPasserPtrBoolParArgument : ";
   call print(ref functionnalite);
   dec ptr p = new bool;
   aff p = true;
   dec bool resultat = call verifierBool(p);
   delete p;
   if (resultat == true) {
      return true;
   } else {
      return false;
   }
   return false;
}

fn int32 lireEtAdditionner(arg ptr ptrA, arg ptr ptrB)
{
   dec int32 a = unref ptrA;
   dec int32 b = unref ptrB;
   dec int32 somme = a + b;
   return somme;
}

fn bool testPasserDeuxPtrParArgument()
{
   dec string[] functionnalite = "4.testPasserDeuxPtrParArgument : ";
   call print(ref functionnalite);
   dec ptr p1 = new int32;
   dec ptr p2 = new int32;
   aff p1 = 30;
   aff p2 = 70;
   dec int32 resultat = call lireEtAdditionner(p1, p2);
   delete p1;
   delete p2;
   if (resultat == 100) {
      return true;
   } else {
      return false;
   }
   return false;
}

fn int32 lireEtReturnner(arg ptr pointeur)
{
   dec int32 val = unref pointeur;
   return val;
}

fn bool verifierReturn(arg ptr pointeur)
{
   dec int32 val = call lireEtReturnner(pointeur);
   if (val == 999) {
      return true;
   } else {
      return false;
   }
   return false;
}

fn bool testPasserPtrADeuxFunctions()
{
   dec string[] functionnalite = "5.testPasserPtrADeuxFunctions : ";
   call print(ref functionnalite);
   dec ptr p = new int32;
   aff p = 999;
   dec bool resultat = call verifierReturn(p);
   delete p;
   if (resultat == true) {
      return true;
   } else {
      return false;
   }
   return false;
}

fn int32 testPassagePtrArgument()
{
   call print(call testPasserPtrInt32ParArgument()); call backSlashN();
   call print(call testPasserPtrValeurZeroParArgument()); call backSlashN();
   call print(call testPasserPtrBoolParArgument()); call backSlashN();
   call print(call testPasserDeuxPtrParArgument()); call backSlashN();
   call print(call testPasserPtrADeuxFunctions()); call backSlashN();

   return 1;
}
