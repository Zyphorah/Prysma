#ifndef BB090689_06EE_4A0F_92F1_0614195D6776
#define BB090689_06EE_4A0F_92F1_0614195D6776
#include <string>

class IEntre 
{
    public: 
    IEntre(const IEntre&) = delete;
    auto operator=(const IEntre&) -> IEntre& = delete;
    IEntre(IEntre&&) = delete;
    auto operator=(IEntre&&) -> IEntre& = delete;
        IEntre() = default;
    virtual ~IEntre() = default;
    virtual auto entrer() -> std::string = 0 ;
};

#endif /* BB090689_06EE_4A0F_92F1_0614195D6776 */
