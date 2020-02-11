// Copyright 2019 Justin Hu
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This file is part of the T Language Compiler.

// implementation of simple functions for use in function pointers

#include "util/functional.h"

#include <stdio.h>
#include <stdlib.h>

void nullDtor(void *ignored) { (void)ignored; }
void invalidFunction(void) {
  fprintf(stderr,
          "tlc: internal compiler error: null function pointer called\n");
  abort();
}