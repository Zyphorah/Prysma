
// Démonstration expérimentale d'une implémentation d'une liste doublement chaînée en Prysma.
class Node
{
private:
   dec ptr data = nullptr;
   dec Node next = nullptr;
   dec Node prev = nullptr;

public:
    fn void Node(arg ptr pdata)
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

    fn ptr getData() { return data; }
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

    fn void addNode(arg ptr pdata)
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

    fn void addDataIndex(arg ptr pdata, arg int32 index)
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

    fn ptr getHead()
    {
        return head.data;
    }

    fn ptr getNodeByIndex(arg int32 index)
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