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

// The parser

#ifndef TLC_PARSER_PARSER_H_
#define TLC_PARSER_PARSER_H_

#include "symbolTable/symbolTable.h"
#include "util/container/hashMap.h"
#include "util/errorReport.h"
#include "util/fileList.h"
#include "util/options.h"

// specialization of a generic
typedef HashMap ModuleAstMap;
ModuleAstMap *moduleAstMapCreate(void);
void moduleAstMapInit(ModuleAstMap *);
Node *moduleAstMapGet(ModuleAstMap const *, char const *key);
int moduleAstMapPut(ModuleAstMap *, char const *key, Node *value);
void moduleAstMapUninit(ModuleAstMap *);
void moduleAstMapDestroy(ModuleAstMap *);

// pod struct holding two ModuleAstMaps
typedef struct {
  ModuleAstMap decls;
  ModuleAstMap codes;
} ModuleAstMapPair;
ModuleAstMapPair *moduleAstMapPairCreate(void);
void moduleAstMapPairInit(ModuleAstMapPair *);
void moduleAstMapPairUninit(ModuleAstMapPair *);
void moduleAstMapPairDestroy(ModuleAstMapPair *);

// specialization of a generic
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

// parses the files, producing SOME_TYPE
void parse(ModuleAstMapPair *, ModuleSymbolTableMapPair *, Report *, Options *,
           FileList *);

#endif  // TLC_PARSER_PARSER_H_