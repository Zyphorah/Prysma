class Data
{
   dec int32 a = 0;
   dec int32 b = 0;
   dec int32 c = 0;

   fn void Data(arg int32 pa, arg int32 pb, arg int32 pc)
   {
      aff a = pa;
      aff b = pb;
      aff c = pc;
   }

   fn int32 getA() { return a; }
   fn int32 getB() { return b; }
   fn int32 getC() { return c; }
}

fn bool verification()
{
    dec int32 a = 1;
    dec int32 b = 2;
    dec int32 c = 3;
    dec Data d = new Data(a, b, c);

    if (    d.getA() == a &&
            d.getB() == b &&
            d.getC() == c
            ) {
        return true;
    } else {
        return false;
    }
}

fn int32 main()
{
    call print(call verification());
    return 0;
}