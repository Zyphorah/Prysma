scope 
{
   fn int main()
   {
        dec int a = 10 + 10; 
        dec int b =  unref a + 10;
        aff a = 10;
        
        call printInt( unref a ); 

        return 1;
   }
}
