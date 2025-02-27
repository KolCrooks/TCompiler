// Copyright 2021 Justin Hu
//
// This file is part of the T Language Compiler.
//
// The T Language Compiler is free software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or (at your
// option) any later version.
//
// The T Language Compiler is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
// Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// the T Language Compiler. If not see <https://www.gnu.org/licenses/>.
//
// SPDX-License-Identifier: GPL-3.0-or-later

/**
 * @file
 * types
 */

#ifndef TLC_AST_TYPE_H_
#define TLC_AST_TYPE_H_

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "util/container/vector.h"
#include "util/format.h"

/** A keyword type */
typedef enum {
  TK_VOID,
  TK_UBYTE,
  TK_BYTE,
  TK_CHAR,
  TK_USHORT,
  TK_SHORT,
  TK_UINT,
  TK_INT,
  TK_WCHAR,
  TK_ULONG,
  TK_LONG,
  TK_FLOAT,
  TK_DOUBLE,
  TK_BOOL,
} TypeKeyword;

/** the kind of a type */
typedef enum {
  TK_KEYWORD,
  TK_QUALIFIED,
  TK_POINTER,
  TK_ARRAY,
  TK_FUNPTR,
  TK_AGGREGATE,
  TK_REFERENCE,
} TypeKind;

struct SymbolTableEntry;
/** the type of a variable or value */
typedef struct Type {
  TypeKind kind;
  union {
    struct {
      TypeKeyword keyword;
    } keyword;
    struct {
      bool constQual;
      bool volatileQual;
      struct Type *base;
    } qualified;
    struct {
      struct Type *base;
    } pointer;
    struct {
      uint64_t length;
      struct Type *type;
    } array;
    struct {
      Vector argTypes; /**< vector of Type */
      struct Type *returnType;
    } funPtr;
    struct {
      Vector types; /**< vector of Type */
    } aggregate;
    struct {
      struct SymbolTableEntry *entry;
      char *id;
    } reference;
  } data;
} Type;

/**
 * create a keyword type
 */
Type *keywordTypeCreate(TypeKeyword keyword);
/**
 * create a qualified type
 */
Type *qualifiedTypeCreate(Type *base, bool constQual, bool volatileQual);
/**
 * create a pointer type
 */
Type *pointerTypeCreate(Type *base);
/**
 * create an array type
 */
Type *arrayTypeCreate(uint64_t length, Type *type);
/**
 * create a function pointer type
 *
 * argTypes is initialized as the empty vector
 */
Type *funPtrTypeCreate(Type *returnType);
/**
 * create a aggregate init type
 *
 * types is initialized as the empty vector
 */
Type *aggregateTypeCreate(void);
/**
 * create a reference type
 */
Type *referenceTypeCreate(struct SymbolTableEntry *entry, char *id);
/**
 * deep copies a type
 */
Type *typeCopy(Type const *);
/**
 * is a equal to b
 */
bool typeEqual(Type const *a, Type const *b);
/**
 * is from implicitly convertable to to
 */
bool typeImplicitlyConvertable(Type const *from, Type const *to);
/**
 * format a list of types
 */
char *typeVectorToString(Vector const *v);
/**
 * format a type
 */
char *typeToString(Type const *t);
/**
 * deinitializes a type
 *
 * @param t type to uninit
 */
void typeFree(Type *t);
/**
 * deinitializes a vector of types
 *
 * @param v vector to uninit
 */
void typeVectorFree(Vector *v);

#endif  // TLC_AST_TYPE_H_