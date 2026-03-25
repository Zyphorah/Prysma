include "Comparison.p";

fn int32 valeurAbsolue(arg int32 n)
{
    if (n < 0) {
        dec int32 neg = 0 - n;
        return neg;
    }
    return n;
}

fn int32 maximum(arg int32 a, arg int32 b)
{
    dec bool aEstPlusGrand = call estPlusGrandInt(a, b);
    if (aEstPlusGrand == true) {
        return a;
    }
    return b;
}

fn int32 minimum(arg int32 a, arg int32 b)
{
    dec bool aEstPlusPetit = call estPlusPetitInt(a, b);
    if (aEstPlusPetit == true) {
        return a;
    }
    return b;
}
