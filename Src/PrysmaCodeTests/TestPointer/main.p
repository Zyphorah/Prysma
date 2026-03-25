
fn void test(arg ptr test)
{
   call print(unref test);
}

fn int32 main() 
{
   dec ptr monPtr = new int32;

   aff monPtr = 120; 

   call test(monPtr);

   return 0;
}
