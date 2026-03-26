#ifndef B5028DC9_A4EF_4499_8345_FC361B37A0CC
#define B5028DC9_A4EF_4499_8345_FC361B37A0CC

class IBuilderEnvironmentRegistry
{
public:
    IBuilderEnvironmentRegistry() = default;
    virtual ~IBuilderEnvironmentRegistry() = default;
    IBuilderEnvironmentRegistry(const IBuilderEnvironmentRegistry&) = delete;
    auto operator=(const IBuilderEnvironmentRegistry&) -> IBuilderEnvironmentRegistry& = delete;
    IBuilderEnvironmentRegistry(IBuilderEnvironmentRegistry&&) = delete;
    auto operator=(IBuilderEnvironmentRegistry&&) -> IBuilderEnvironmentRegistry& = delete;
    virtual void fill() = 0;
};

#endif /* B5028DC9_A4EF_4499_8345_FC361B37A0CC */
