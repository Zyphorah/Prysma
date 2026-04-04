//===-- malloc.cpp -------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

// Copyright (C) 2026 Raphael Arseneault
//
// This file is part of Prysma.
//
// Prysma is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

#include <cstdlib>

extern "C" {
    auto prysma_malloc(size_t size) -> void* {
        return std::malloc(size); 
    }

    void prysma_free(void* ptr) {
        if (ptr != nullptr) { 
            std::free(ptr);
        }
    }
}
