#ifndef BB599809_0BCB_4424_9F67_25155C176A5E
#define BB599809_0BCB_4424_9F67_25155C176A5E

#include <string>

class IOutput
{
    public : 
        IOutput(const IOutput&) = delete;
        auto operator=(const IOutput&) -> IOutput& = delete;
        IOutput(IOutput&&) = delete;
        auto operator=(IOutput&&) -> IOutput& = delete;
        IOutput() = default;
        virtual ~IOutput() = default;
        virtual void output(const std::string& data) = 0;
};

#endif /* BB599809_0BCB_4424_9F67_25155C176A5E */
