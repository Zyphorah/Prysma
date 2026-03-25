
fn bool testNewDeleteFloatBasique()
{
   dec string[] functionnalite = "1.testNewDeleteFloatBasique : ";
   call print(ref functionnalite);
   dec ptr p = new float;
   aff p = 3.14;
   dec float val = unref p;
   delete p;
   if (val == 3.14) {
      return true;
   } else {
      return false;
   }
   return false;
}

fn bool testNewDeleteFloatZero()
{
   dec string[] functionnalite = "2.testNewDeleteFloatZero : ";
   call print(ref functionnalite);
   dec ptr p = new float;
   aff p = 0.0;
   dec float val = unref p;
   delete p;
   if (val == 0.0) {
      return true;
   } else {
      return false;
   }
   return false;
}

fn bool testNewDeleteFloatNegatif()
{
   dec string[] functionnalite = "3.testNewDeleteFloatNegatif : ";
   call print(ref functionnalite);
   dec ptr p = new float;
   dec float valeur = 0.0 - 99.9;
   aff p = valeur;
   dec float val = unref p;
   delete p;
   if (val == valeur) {
      return true;
   } else {
      return false;
   }
   return false;
}

fn bool testNewDeleteFloatPetiteValeur()
{
   dec string[] functionnalite = "4.testNewDeleteFloatPetiteValeur : ";
   call print(ref functionnalite);
   dec ptr p = new float;
   aff p = 0.001;
   dec float val = unref p;
   delete p;
   if (val == 0.001) {
      return true;
   } else {
      return false;
   }
   return false;
}

fn bool testNewDeleteFloatReassignment()
{
   dec string[] functionnalite = "5.testNewDeleteFloatReassignment : ";
   call print(ref functionnalite);
   dec ptr p = new float;
   aff p = 1.1;
   aff p = 55.5;
   dec float val = unref p;
   delete p;
   if (val == 55.5) {
      return true;
   } else {
      return false;
   }
   return false;
}

fn int32 testNewDeleteFloat()
{
   call print(call testNewDeleteFloatBasique()); call backSlashN();
   call print(call testNewDeleteFloatZero()); call backSlashN();
   call print(call testNewDeleteFloatNegatif()); call backSlashN();
   call print(call testNewDeleteFloatPetiteValeur()); call backSlashN();
   call print(call testNewDeleteFloatReassignment()); call backSlashN();

   return 1;
}
