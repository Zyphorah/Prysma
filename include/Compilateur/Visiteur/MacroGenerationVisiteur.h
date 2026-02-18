#ifndef D2023444_D6C2_4302_89E4_57775CF04AD5
#define D2023444_D6C2_4302_89E4_57775CF04AD5

#define DECLARER_METHODES_VISITEUR \
    void visiter(NoeudScope* NoeudScope) override; \
    void visiter(NoeudAffectationVariable* noeudAffectationVariable) override; \
    void visiter(NoeudDeclarationVariable* noeudDeclarationVariable) override; \
    void visiter(NoeudAppelFonction* noeudAppelFonction) override; \
    void visiter(NoeudRetour* noeudReturn) override; \
    void visiter(NoeudArgFonction* noeudArgFonction) override; \
    void visiter(NoeudDeclarationFonction* noeudDeclarationFonction) override; \
    void visiter(NoeudOperation* noeudOperation) override; \
    void visiter(NoeudLitteral* noeudLitteral) override; \
    void visiter(NoeudInstruction* instruction) override; \
    void visiter(NoeudUnRefVariable* noeudUnRefVariable) override; \
    void visiter(NoeudRefVariable* noeudRefVariable) override; \
    void visiter(NoeudIf* noeudIf) override; \
    void visiter(NoeudWhile* noeudWhile) override; \
    void visiter(NoeudNegation* noeudNegation) override; \
    void visiter(NoeudTableauInitialisation* noeudTableauInit) override;\
    void visiter(NoeudAffectationTableau* noeudAffectationTableau) override;\
    void visiter(NoeudLectureTableau* noeudLectureTableau) override;\

#define CLASS_NOEUD \
    class NoeudInstruction; \
    class NoeudScope; \
    class NoeudAffectationVariable; \
    class NoeudDeclarationVariable; \
    class NoeudAppelFonction; \
    class NoeudRetour; \
    class NoeudArgFonction; \
    class NoeudDeclarationFonction; \
    class NoeudLitteral; \
    class NoeudUnRefVariable; \
    class NoeudRefVariable; \
    class NoeudIf; \
    class NoeudElse; \
    class NoeudWhile; \
    class NoeudOperation; \
    class NoeudNegation; \
    class NoeudTableauInitialisation;\
    class NoeudAffectationTableau;\
    class NoeudLectureTableau;\
    
#endif /* D2023444_D6C2_4302_89E4_57775CF04AD5 */
