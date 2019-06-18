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

// Type checking

#ifndef TLC_TYPECHECK_UTIL_H_
#define TLC_TYPECHECK_UTIL_H_

#include "parser/parser.h"
#include "typecheck/symbolTable.h"
#include "util/errorReport.h"
#include "util/options.h"

typedef HashMap ModuleSymbolTableMap;
ModuleSymbolTableMap *moduleSymbolTableMapCreate(void);
void moduleSymbolTableMapInit(ModuleSymbolTableMap *);
SymbolTable *moduleSymbolTableMapGet(ModuleSymbolTableMap const *,
                                     char const *key);
int moduleSymbolTableMapPut(ModuleSymbolTableMap *, char const *key,
                            SymbolTable *value);
void moduleSymbolTableMapUninit(ModuleSymbolTableMap *);
void moduleSymbolTableMapDestroy(ModuleSymbolTableMap *);

// pod struct holding two ModuleSymbolTableMaps
typedef struct {
  ModuleSymbolTableMap decls;
  ModuleSymbolTableMap codes;
} ModuleSymbolTableMapPair;
ModuleSymbolTableMapPair *moduleSymbolTableMapPairCreate(void);
void moduleSymbolTableMapPairInit(ModuleSymbolTableMapPair *);
void moduleSymbolTableMapPairUninit(ModuleSymbolTableMapPair *);
void moduleSymbolTableMapPairDestroy(ModuleSymbolTableMapPair *);

void typecheck(ModuleSymbolTableMapPair *, Report *, Options const *,
               ModuleAstMapPair const *asts);

#endif  // TLC_TYPECHECK_UTIL_H_