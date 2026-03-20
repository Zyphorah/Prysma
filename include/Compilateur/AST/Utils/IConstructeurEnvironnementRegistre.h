#ifndef B5028DC9_A4EF_4499_8345_FC361B37A0CC
#define B5028DC9_A4EF_4499_8345_FC361B37A0CC

class IConstructeurEnvironnementRegistre
{
public:
    IConstructeurEnvironnementRegistre() = default;
    virtual ~IConstructeurEnvironnementRegistre() = default;
    IConstructeurEnvironnementRegistre(const IConstructeurEnvironnementRegistre&) = delete;
    auto operator=(const IConstructeurEnvironnementRegistre&) -> IConstructeurEnvironnementRegistre& = delete;
    IConstructeurEnvironnementRegistre(IConstructeurEnvironnementRegistre&&) = delete;
    auto operator=(IConstructeurEnvironnementRegistre&&) -> IConstructeurEnvironnementRegistre& = delete;
    virtual void remplir() = 0;
};

#endif /* B5028DC9_A4EF_4499_8345_FC361B37A0CC */
