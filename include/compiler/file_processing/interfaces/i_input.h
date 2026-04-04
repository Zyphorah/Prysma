//===-- i_input.h -------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#ifndef BB090689_06EE_4A0F_92F1_0614195D6776
#define BB090689_06EE_4A0F_92F1_0614195D6776
#include <string>

class IInput 
{
public: 
    IInput(const IInput&) = delete;
    auto operator=(const IInput&) -> IInput& = delete;
    IInput(IInput&&) = delete;
    auto operator=(IInput&&) -> IInput& = delete;
    IInput() = default;
    virtual ~IInput() = default;
    virtual auto getInput() -> std::string = 0;
};

#endif /* BB090689_06EE_4A0F_92F1_0614195D6776 */
