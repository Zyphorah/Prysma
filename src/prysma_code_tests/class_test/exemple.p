Interface IAnimal 
{
    fn void makeSound();
}

class Dog implements IAnimal
{
    dec int32 age = 5;
    fn void Dog()
    {
        
    }

    fn void makeSound()
    {
        call print("Woof!");
    }
}

class Cat implements IAnimal
{
    dec int32 age = 3;
    fn void Cat()
    {
        
    }

    fn void makeSound()
    {
        call print("Meow!");
    }
}


fn int32 main() 
{
    dec IAnimal dog = new Dog();
    dec IAnimal cat = new Cat();

    call dog.makeSound(); // Output: Woof!
    call cat.makeSound(); // Output: Meow!

    return 0;
}