#ifndef BB599809_0BCB_4424_9F67_25155C176A5E
#define BB599809_0BCB_4424_9F67_25155C176A5E

#include <string>

class ISortie
{
    public : 
        ISortie(const ISortie&) = delete;
        auto operator=(const ISortie&) -> ISortie& = delete;
        ISortie(ISortie&&) = delete;
        auto operator=(ISortie&&) -> ISortie& = delete;
        ISortie() = default;
        virtual ~ISortie() = default;
        virtual void sort(const std::string& data) = 0;
};

#endif /* BB599809_0BCB_4424_9F67_25155C176A5E */
