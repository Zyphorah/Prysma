//===-- class.cpp -------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

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
