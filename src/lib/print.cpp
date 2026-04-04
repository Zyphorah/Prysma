//===-- print.cpp -------------------------------------------*- C++ -*-===//
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

#include <cstdio>
#include <cstdarg> 

extern "C" {
    void print(int type, ...) {
        va_list args;
        va_start(args, type);

        switch (type) {
            case 'i': 
                printf("%d", va_arg(args, int));
                break;

            case 'f': 
                printf("%f", va_arg(args, double)); 
                break;

            case 's': 
                printf("%s", va_arg(args, char*));
                break;

            case 'b': {
                int val = va_arg(args, int);
                if (val != 0) {
                    printf("\033[32mtrue\033[0m");
                } else {
                    printf("\033[31mfalse\033[0m");
                }
            
                break;
            }
        }
        va_end(args);
    }

    void backSlashN() {
        printf("\n");
    } 
}
