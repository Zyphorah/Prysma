include "list/linked_list.p";

class Data
{
private:
   dec int32 a = 0;
   dec int32 b = 0;
   dec int32 c = 0;

public:
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

// TODO : il me manque les destructeurs mais c'est pour le future, car ça n'existe pas encore dans le langage prysma, destructeur de class
fn bool verification()
{
    dec LinkedList list = new LinkedList();
    call list.addNode(new Data(1, 2, 3));
    call list.addNode(new Data(4, 5, 6));
    call list.addNode(new Data(7, 8, 9));

    if (list.getSize() != 3) { return false; }

    call list.pop();
    if (list.getSize() != 2) { return false; }

    dec Data d0 = call list.getNodeByIndex(0);
    if (d0 == nullptr) { return false; }
   
    call print(d0.getA()); call backSlashN();
    call print(d0.getB()); call backSlashN();
    call print(d0.getC()); call backSlashN();

    if (d0.getA() != 1 || d0.getB() != 2 || d0.getC() != 3) { return false; }

    dec Data d1 = call list.getNodeByIndex(1);

    call print(d1.getA()); call backSlashN();
    call print(d1.getB()); call backSlashN();
    call print(d1.getC()); call backSlashN();

    if (d1 == nullptr) { return false; }
    if (d1.getA() != 4 || d1.getB() != 5 || d1.getC() != 6) { return false; }

    dec Data d2 = call list.getNodeByIndex(2);

    if (d2 != nullptr) { return false; }

    return true;
}

fn int32 main()
{
    call print(call verification());
    return 0;
}
