
fn bool testNewDeleteInt64Basique()
{
   dec string[] functionnalite = "1.testNewDeleteInt64Basique : ";
   call print(ref functionnalite);
   dec ptr p = new int64;
   aff p = 100;
   dec int32 val = unref p;
   delete p;
   if (val == 100) {
      return true;
   } else {
      return false;
   }
   return false;
}

fn bool testNewDeleteInt64Zero()
{
   dec string[] functionnalite = "2.testNewDeleteInt64Zero : ";
   call print(ref functionnalite);
   dec ptr p = new int64;
   aff p = 0;
   dec int32 val = unref p;
   delete p;
   if (val == 0) {
      return true;
   } else {
      return false;
   }
   return false;
}

fn bool testNewDeleteInt64Negatif()
{
   dec string[] functionnalite = "3.testNewDeleteInt64Negatif : ";
   call print(ref functionnalite);
   dec ptr p = new int64;
   dec int32 valeur = 0 - 500;
   aff p = valeur;
   dec int32 val = unref p;
   delete p;
   if (val == valeur) {
      return true;
   } else {
      return false;
   }
   return false;
}

fn bool testNewDeleteInt64GrandeValeur()
{
   dec string[] functionnalite = "4.testNewDeleteInt64GrandeValeur : ";
   call print(ref functionnalite);
   dec ptr p = new int64;
   aff p = 9999999;
   dec int32 val = unref p;
   delete p;
   if (val == 9999999) {
      return true;
   } else {
      return false;
   }
   return false;
}

fn bool testNewDeleteInt64Reassignment()
{
   dec string[] functionnalite = "5.testNewDeleteInt64Reassignment : ";
   call print(ref functionnalite);
   dec ptr p = new int64;
   aff p = 1;
   aff p = 777;
   dec int32 val = unref p;
   delete p;
   if (val == 777) {
      return true;
   } else {
      return false;
   }
   return false;
}

fn int32 testNewDeleteInt64()
{
   call print(call testNewDeleteInt64Basique()); call backSlashN();
   call print(call testNewDeleteInt64Zero()); call backSlashN();
   call print(call testNewDeleteInt64Negatif()); call backSlashN();
   call print(call testNewDeleteInt64GrandeValeur()); call backSlashN();
   call print(call testNewDeleteInt64Reassignment()); call backSlashN();

   return 1;
}
