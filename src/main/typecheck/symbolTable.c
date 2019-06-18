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

// Implementation of the symbol table

#include "typecheck/symbolTable.h"

#include "util/functional.h"
#include "util/nameUtils.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

char const *symbolKindToString(SymbolKind kind) {
  switch (kind) {
    case SK_VAR:
      return "a variable";
    case SK_TYPE:
      return "a type";
    case SK_FUNCTION:
      return "a function";
  }
  assert(false);  // not a valid enum - type safety violated
}

char const *typeDefinitionKindToString(TypeDefinitionKind kind) {
  switch (kind) {
    case TDK_STRUCT:
      return "a struct";
    case TDK_UNION:
      return "a union";
    case TDK_ENUM:
      return "an enumeration";
    case TDK_TYPEDEF:
      return "a type alias";
  }
  assert(false);  // not a valid enum - type safety violated
}

static SymbolInfo *symbolInfoCreate(SymbolKind kind) {
  SymbolInfo *si = malloc(sizeof(SymbolInfo));
  si->kind = kind;
  return si;
}
SymbolInfo *varSymbolInfoCreate(Type *type) {
  SymbolInfo *si = symbolInfoCreate(SK_VAR);
  si->data.var.type = type;
  return si;
}
SymbolInfo *structSymbolInfoCreate(void) {
  SymbolInfo *si = symbolInfoCreate(SK_TYPE);
  si->data.type.kind = TDK_STRUCT;
  si->data.type.data.structType.incomplete = true;
  typeVectorInit(&si->data.type.data.structType.fields);
  stringVectorInit(&si->data.type.data.structType.names);
  return si;
}
SymbolInfo *unionSymbolInfoCreate(void) {
  SymbolInfo *si = symbolInfoCreate(SK_TYPE);
  si->data.type.kind = TDK_UNION;
  si->data.type.data.unionType.incomplete = true;
  typeVectorInit(&si->data.type.data.unionType.fields);
  stringVectorInit(&si->data.type.data.unionType.names);
  return si;
}
SymbolInfo *enumSymbolInfoCreate(void) {
  SymbolInfo *si = symbolInfoCreate(SK_TYPE);
  si->data.type.kind = TDK_ENUM;
  si->data.type.data.enumType.incomplete = true;
  stringVectorInit(&si->data.type.data.enumType.fields);
  return si;
}
SymbolInfo *typedefSymbolInfoCreate(Type *what) {
  SymbolInfo *si = symbolInfoCreate(SK_TYPE);
  si->data.type.kind = TDK_TYPEDEF;
  si->data.type.data.typedefType.type = what;
  return si;
}
SymbolInfo *functionSymbolInfoCreate(Type *returnType) {
  SymbolInfo *si = symbolInfoCreate(SK_FUNCTION);
  si->data.function.returnType = returnType;
  vectorInit(&si->data.function.argumentTypeSets);
  return si;
}
void symbolInfoDestroy(SymbolInfo *si) {
  switch (si->kind) {
    case SK_VAR: {
      typeDestroy(si->data.var.type);
      break;
    }
    case SK_TYPE: {
      switch (si->data.type.kind) {
        case TDK_STRUCT: {
          typeVectorUninit(&si->data.type.data.structType.fields);
          stringVectorUninit(&si->data.type.data.structType.names, false);
          break;
        }
        case TDK_UNION: {
          typeVectorUninit(&si->data.type.data.unionType.fields);
          stringVectorUninit(&si->data.type.data.unionType.names, false);
          break;
        }
        case TDK_ENUM: {
          stringVectorUninit(&si->data.type.data.unionType.fields, false);
          break;
        }
        case TDK_TYPEDEF: {
          typeDestroy(si->data.type.data.typedefType.type);
          break;
        }
      }
      break;
    }
    case SK_FUNCTION: {
      typeDestroy(si->data.function.returnType);
      vectorUninit(&si->data.function.argumentTypeSets,
                   (void (*)(void *))typeVectorUninit);
      break;
    }
  }
  free(si);
}

SymbolTable *symbolTableCreate(void) { return hashMapCreate(); }
SymbolInfo *symbolTableGet(SymbolTable const *table, char const *key) {
  return hashMapGet(table, key);
}
int symbolTablePut(SymbolTable *table, char const *key, SymbolInfo *value) {
  return hashMapPut(table, key, value, (void (*)(void *))symbolInfoDestroy);
}
void symbolTableDestroy(SymbolTable *table) {
  hashMapDestroy(table, (void (*)(void *))symbolInfoDestroy);
}

ModuleTableMap *moduleTableMapCreate(void) { return hashMapCreate(); }
void moduleTableMapInit(ModuleTableMap *map) { hashMapInit(map); }
SymbolTable *moduleTableMapGet(ModuleTableMap const *table, char const *key) {
  return hashMapGet(table, key);
}
int moduleTableMapPut(ModuleTableMap *table, char const *key,
                      SymbolTable *value) {
  return hashMapPut(table, key, value, nullDtor);
}
void moduleTableMapUninit(ModuleTableMap *map) { hashMapUninit(map, nullDtor); }
void moduleTableMapDestroy(ModuleTableMap *table) {
  hashMapDestroy(table, nullDtor);
}

void environmentInit(Environment *env, SymbolTable *currentModule,
                     char const *currentModuleName) {
  env->currentModule = currentModule;
  env->currentModuleName = currentModuleName;
  moduleTableMapInit(&env->imports);
  stackInit(&env->scopes);
}
TernaryValue environmentIsType(Environment const *env, Report *report,
                               TokenInfo const *token, char const *filename) {
  if (isScoped(token->data.string)) {
    char *moduleName;
    char *shortName;
    splitName(token->data.string, &moduleName, &shortName);
    if (strcmp(moduleName, env->currentModuleName) == 0) {
      SymbolInfo *info = symbolTableGet(env->currentModule, shortName);
      if (info != NULL) {
        free(moduleName);
        free(shortName);
        return info->kind == SK_TYPE ? YES : NO;
      }
    } else {
      for (size_t idx = 0; idx < env->imports.capacity; idx++) {
        if (strcmp(moduleName, env->imports.keys[idx]) == 0) {
          SymbolInfo *info =
              symbolTableGet(env->imports.values[idx], shortName);
          if (info != NULL) {
            free(moduleName);
            free(shortName);
            return info->kind == SK_TYPE ? YES : NO;
          }
        }
      }
    }

    if (isScoped(moduleName)) {
      char *enumModuleName;
      char *enumName;
      splitName(moduleName, &enumModuleName, &enumName);
      // TODO: check to see if it's a enum type
    }

    free(moduleName);
    free(shortName);
    reportError(report, "%s:%zu:%zu: error: undefined identifier '%s'\n",
                filename, token->line, token->character, token->data.string);
    return INDETERMINATE;
  } else {
    for (size_t idx = env->scopes.size; idx-- > 0;) {
      SymbolInfo *info =
          symbolTableGet(env->scopes.elements[idx], token->data.string);
      if (info != NULL) return info->kind == SK_TYPE ? YES : NO;
    }
    SymbolInfo *info = symbolTableGet(env->currentModule, token->data.string);
    if (info != NULL) return info->kind == SK_TYPE ? YES : NO;

    char const *foundModule = NULL;
    for (size_t idx = 0; idx < env->imports.capacity; idx++) {
      if (env->imports.keys[idx] != NULL) {
        SymbolInfo *current =
            symbolTableGet(env->imports.values[idx], token->data.string);
        if (current != NULL) {
          if (info == NULL) {
            info = current;
            foundModule = env->imports.keys[idx];
          } else {
            reportError(report,

                        "%s:%zu:%zu: error: identifier '%s' is ambiguous\n",
                        filename, token->line, token->character,
                        token->data.string);
            reportMessage(report, "\tcandidate module: %s\n",
                          env->imports.keys[idx]);
            reportMessage(report, "\tcandidate module: %s\n", foundModule);
            return INDETERMINATE;
          }
        }
      }
    }
    if (info != NULL) {
      return info->kind == SK_TYPE ? YES : NO;
    } else {
      reportError(report, "%s:%zu:%zu: error: undefined identifier '%s'\n",
                  filename, token->line, token->character, token->data.string);
      return INDETERMINATE;
    }
  }
}
void environmentUninit(Environment *env) {
  moduleTableMapUninit(&env->imports);
  stackUninit(&env->scopes, (void (*)(void *))symbolTableDestroy);
}