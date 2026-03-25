class IAnimal 
{
public:
    virtual ~IAnimal() = default;
    virtual void shout() = 0;
};

class Cat : public IAnimal
{
private: 
    int age_;
public: 
    Cat(int age) : age_(age) {}

    void shout() override {
       age_++;
    }
};

int main() {
    Cat myCat(3);
    myCat.shout();
    return 0;
}