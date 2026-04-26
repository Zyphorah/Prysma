//===-- visitor_class.cpp ---------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#include "compiler/visitor/code_gen/visitor_general_gen_code.h"
#include "compiler/ast/ast_genere.h"
#include "compiler/utils/prysma_cast.h"
#include <string>

/*
                                                                                                                        
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
                                                                                                                        
                                                                                        Struct                          
                                                                                                                        
                                                                          0        1        2         3        4        
                                        points to the data structure  ┌────────┬────────┬────────┬────────┬────────┐    
                            vPtr                      ┌───────────────►        │        │        │        │        │    
   ┌─────────────────────────────────┐                │               │ Memory │ Memory │ Memory │Memory  │Memory  │    
   │                                 │                │               └──┬─────┴───┬────┴────────┴────────┴────────┘    
   │ dec IAnimal dog = new Dog();    ┼────────────────┘                  │         │                                    
   │                                 │                                   │         └───────────────┐                    
   └─────────────────────────────────┘                          Points to│the vtable               ▼                    
                                                                         │                 dec int32 age = 5;           
      // The vtable contract is managed by the IAnimal interface         │                                              
                                                                         │                                              
         Interface IAnimal                                               │0        1        2         3        4        
         {                                                            ┌──▼─────┬────────┬────────┬────────┬────────┐    
             fn void makeSound(); ──────────────────────────►         │        │        │        │        │        │    
         }                                                     vtable │ Memory │Memory  │Memory  │Memory  │Memory  │    
                                                                      └───┬────┴────┬───┴────────┴────────┴────────┘    
                                                                          │         │                                   
                                                                          │         │                                   
                                                                          │         │                                   
                                                                          │         │                                   
                                                                          ▼         │                                   
                                                                     fn void Dog()  │                                   
                                                                         {          │                                   
                                                                                    ▼                                   
                                                                         }        fn void makeSound()                   
                                                                                     {                                  
                                                                                         call print("Woof!");           
                                                                                     }                                  
                                                                                                                        
                                                                                                                        
*/


void GeneralVisitorGenCode::visiter(NodeClass* nodeClass)
{
    std::string previousClassName = _contextGenCode->getCurrentClassName();
    _contextGenCode->setCurrentClassName(std::string(nodeClass->getNomClass().value));

    for (auto* member : nodeClass->getListMembers()) {
        if (prysma::isa<NodeDeclarationFunction>(member)) {
            member->accept(this);
        }
    }

    for (auto* builder : nodeClass->getBuilder()) {
        builder->accept(this);
    }

    _contextGenCode->setCurrentClassName(previousClassName);
}
