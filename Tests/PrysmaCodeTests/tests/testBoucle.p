scope {

    fn bool testBoucleConditionExtern()
    {
        dec int i = 0; 

        dec bool condition = i < 5;

        while(condition)
        {
            aff i = i + 1; 
            aff condition = i < 5;
        }
        if (i == 5)
        {
            return true;    
        }
        return false;
    }

   fn bool testBoucleNesPasEgale()
   {
        dec int i = 0; 
        while(i != 5)
        {
            aff i = i +1; 
        }
        if (i == 5)
        {
            return true;
        }
        return false; 
   }

   fn bool testBoucle()
    {
        dec int i = 0;

        while (i < 5) {
            aff i = i + 1;
        }
        if (i == 5) {
            return true;
        } 
        return false;
   }

   
   fn bool testBoucleImbrique()
   {
        dec int i = 0;
        dec int j = 0;
        dec int total = 0;
        while(i < 5)
        {
            aff j = 0;
            while(j < 5)
            {
                aff total = total + 1;
                aff j = j + 1;
            }
            aff i = i + 1;
        }
        if(total == 25)
        {
            return true;
        }
        return false;
   }

   fn bool testBoucleConditionComplexe()
   {
        dec int i = 0;
        dec int j = 10;
        while(i < 5 && j > 5)
        {
            aff i = i + 1;
            aff j = j - 1;
        }
        if(i == 5 && j == 5)
        {
            return true;
        }
        return false;
   }

   fn bool testBoucleNonExecute()
   {
       dec int i = 5;
       while(i < 5)
       {
           aff i = i + 1;
       }
       if(i == 5)
       {
           return true;
       }
       return false;
   }

   fn int main()
   {
        call printBool(call testBoucle());
        call printBool(call testBoucleNesPasEgale());
        call printBool(call testBoucleConditionExtern());
        call printBool(call testBoucleImbrique());
        call printBool(call testBoucleConditionComplexe());
        call printBool(call testBoucleNonExecute());

      return 1;
   }
}
