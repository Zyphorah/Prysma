class IAnimal 
{
public:

    virtual ~IAnimal() = default;
    virtual void crier() = 0;
};

class Chat : public IAnimal
{
private: 
    int _age;
public: 
    Chat(int age) : _age(age) {}

    void crier() override {
       _age++;
    };

};

int main() {
    Chat monChat( 3);
    monChat.crier();
    return 0;
}