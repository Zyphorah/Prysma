source_filename = "Class.cpp"
target triple = "x86_64-pc-linux-gnu"

; Le système d'héritage multiple fonctionne en utilisant une technique visuel comme un onion
; on empile les classes les unes sur les autres, chaque classe enfant contient une instance de sa classe parent,
; ainsi on peut accéder aux méthodes et attributs de la classe parent à travers l'instance


; Les étapes à suivre pour l'instanciation d'un constructeur d'une classe avec héritage multiple sont les suivantes :
;Le constructeur de Chat appelle IAnimal_constructor.
;IAnimal_constructor appelle EtreVivant_constructor.
;EtreVivant_constructor s'exécute, puis rend la main à IAnimal.
;IAnimal installe sa VTable, puis rend la main à Chat.
;Chat installe enfin sa VTable finale.

%class.IAnimal = type { ptr }

%class.Chat = type <{ 
  %class.IAnimal,
  i32
}>

@VTable_IAnimal = constant [1 x ptr] [
  ptr @IAnimal_destructor,
  ptr @__crier_virtual   ; une méthode virtuelle pour la classe IAnimal qui 
  ; sera surchargé par la classe chat 
]


; La vtable est une table de pointeurs de fonctions qui permet de faire du polymorphisme en C++, elle contient les adresses 
; des méthodes virtuelles de la classe, ainsi on peut appeler les méthodes de la classe à travers la vtable,
; comme la méthode destructor crier chat tout ce qui ce trouve dans la classe chat 
; on fait une vtable pour chaque classe, ainsi la classe chat a sa propre vtable qui contient les méthodes de la classe chat, ainsi que les méthodes de la classe parent IAnimal.

@VTable_Chat = constant [2 x ptr] [ 
  ptr @Chat_destructor,
  ptr @Chat_crier
]

define i32 @main() {
entry:
  %monChat = alloca %class.Chat, align 8 ; on réserve de la mémoire pour un objet de type Chat

  call void @Chat_constructor(ptr %monChat, i32 3) ; on appelle le constructeur de la classe chat pour initialiser l'objet monChat avec une age de 3

  %vtable_ptr_ptr = getelementptr inbounds %class.Chat, ptr %monChat, i32 0, i32 0, i32 0 

  %vtable_ptr = load ptr, ptr %vtable_ptr_ptr, align 8 ; on charge l'adresse de la vtable de la classe chat dans la variable vtable_ptr, ainsi on peut accéder aux méthodes de la classe chat à travers la vtable

  %crier_func_ptr_ptr = getelementptr inbounds ptr, ptr %vtable_ptr, i64 1 
  %crier_func_ptr = load ptr, ptr %crier_func_ptr_ptr, align 8 ; on charge l'adresse de la méthode crier dans la variable crier_func_ptr 

  call void %crier_func_ptr(ptr %monChat) ; on peux maintenant appeler la méthode crier de la classe chat à travers la vtable stocké dans l'allocation de mémoire
  
  call void @Chat_destructor(ptr %monChat) 

  ret i32 0
}

define void @Chat_constructor(ptr %this, i32 %age) { ; comme le destructeur on utilise la méthode en onion pour gérer l'ordre d'instanciation
entry:
  call void @IAnimal_constructor(ptr %this)

  %vtable_ptr_ptr = getelementptr inbounds %class.Chat, ptr %this, i32 0, i32 0, i32 0
  store ptr getelementptr ([2 x ptr], ptr @VTable_Chat, i32 0, i32 0), ptr %vtable_ptr_ptr, align 8

  %age_ptr = getelementptr inbounds %class.Chat, ptr %this, i32 0, i32 1
  store i32 %age, ptr %age_ptr, align 8
  ret void
}

define void @Chat_crier(ptr %this) { 
entry:
  ret void
}

define void @Chat_destructor(ptr %this) { ; méthode de destruction de la classe chat
entry:
  call void @IAnimal_destructor(ptr %this) ; on utilise une méthode de destruction car 
  ; c'est une technique en onion on detruit tout de l'enfant vers le parent, 
  ; C'est ici qu'on gère l'ordre de destruction, on détruit d'abord les enfants puis les parents
  ; sinon on risque d'avoir des fuites de mémoire ou des comportements indéfinis si on détruit d'abord les parents
  ret void
}

define void @IAnimal_constructor(ptr %this) {
entry:
  %vtable_ptr_ptr = getelementptr inbounds %class.IAnimal, ptr %this, i32 0, i32 0
  store ptr getelementptr ([1 x ptr], ptr @VTable_IAnimal, i32 0, i32 0), ptr %vtable_ptr_ptr, align 8
  ret void
}

; Chaque destructeur on une méthode qui lui est associé pour gérer l'ordre de destruction
define void @IAnimal_destructor(ptr %this) {
entry:
  ret void
}

