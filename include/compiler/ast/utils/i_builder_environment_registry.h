//===-- i_builder_environment_registry.h -------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

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
