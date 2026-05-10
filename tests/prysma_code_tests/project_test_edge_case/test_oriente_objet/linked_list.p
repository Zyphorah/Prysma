// Démonstration expérimentale d'une implémentation d'une liste doublement chaînée en Prysma.

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

class Node
{
private:
   dec Data data = nullptr;
   dec Node next = nullptr;
   dec Node prev = nullptr;

public:
    fn void Node(arg Data pdata)
    {
        aff data = pdata;
    }

    fn void setNext(arg Node pnext)
    {
        aff next = pnext;
    }

    fn void setPrev(arg Node pprev)
    {
        aff prev = pprev;
    }

    fn Data getData() { return data; }
    fn Node getNext() { return next; }
    fn Node getPrev() { return prev; }
}

class LinkedList
{
private:
    dec int32 size = 0;
    dec Node head = nullptr;

public:

    fn void LinkedList()
    {
    }

    fn void addNode(arg Data pdata)
    {
       dec Node newNode = new Node(pdata);

       if (head == nullptr) {
           aff head = newNode;
       } else {
           dec Node current = head;
           while (call current.getNext() != nullptr) {
               aff current = call current.getNext();
           }
           call current.setNext(newNode);
           call newNode.setPrev(current);
       }
       aff size = size + 1;
    }

    fn void removeNode(arg int32 index)
    {
       if (head == nullptr) {
           return;
       }
       if (index == 0) {
           aff head = call head.getNext();
           if (head != nullptr) {
               call head.setPrev(nullptr);
           }
           aff size = size - 1;
           return;
       }
       dec Node current = head;
       while (index > 1 && call current.getNext() != nullptr) {
           aff current = call current.getNext();
           aff index = index - 1;
       }
       dec Node nodeToRemove = call current.getNext();
       if (nodeToRemove != nullptr) {
           dec Node nextNode = call nodeToRemove.getNext();
           call current.setNext(nextNode);
           if (nextNode != nullptr) {
               call nextNode.setPrev(current);
           }
           aff size = size - 1;
       }
    }

    fn void pop()
    {
       if (head == nullptr) {
          return;
       }
       if (call head.getNext() == nullptr) {
          aff head = nullptr;
          aff size = size - 1;
          return;
       }
       dec Node current = head;
       while (call current.getNext() != nullptr) {
          aff current = call current.getNext();
       }
       dec Node prevNode = call current.getPrev();
       if (prevNode != nullptr) {
          call prevNode.setNext(nullptr);
       }
       aff size = size - 1;
    }

    fn void addDataIndex(arg Data pdata, arg int32 index)
    {
       if (index <= 0 || head == nullptr) {
           call addNode(pdata);
           return;
       }
       dec Node newNode = new Node(pdata);
       dec Node current = head;
       while (index > 1 && call current.getNext() != nullptr) {
           aff current = call current.getNext();
           aff index = index - 1;
       }
       dec Node nextNode = call current.getNext();
       call current.setNext(newNode);
       call newNode.setPrev(current);
       call newNode.setNext(nextNode);
       if (nextNode != nullptr) {
           call nextNode.setPrev(newNode);
       }
       aff size = size + 1;
    }

    fn int32 getSize()
    {
        return size;
    }

    fn Data getHead()
    {
        return head.data;
    }

    fn Data getNodeByIndex(arg int32 index)
    {
       dec Node current = head;
       while (index > 0 && current != nullptr) {
           aff current = call current.getNext();
           aff index = index - 1;
       }
       if (current == nullptr) {
           return nullptr;
       }
       return current.data;
    }
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
