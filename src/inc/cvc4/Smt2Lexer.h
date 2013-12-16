/** \file
 *  This C header file was generated by $ANTLR version 3.4
 *
 *     -  From the grammar source file : ../../../../src/parser/smt2/Smt2.g
 *     -                            On : 2013-12-06 18:07:54
 *     -                 for the lexer : Smt2LexerLexer
 *
 * Editing it, at least manually, is not wise.
 *
 * C language generator and runtime by Jim Idle, jimi|hereisanat|idle|dotgoeshere|ws.
 *
 *
 * The lexer 
Smt2Lexer

has the callable functions (rules) shown below,
 * which will invoke the code for the associated rule in the source grammar
 * assuming that the input stream is pointing to a token/text stream that could begin
 * this rule.
 *
 * For instance if you call the first (topmost) rule in a parser grammar, you will
 * get the results of a full parse, but calling a rule half way through the grammar will
 * allow you to pass part of a full token stream to the parser, such as for syntax checking
 * in editors and so on.
 *
 * The parser entry points are called indirectly (by function pointer to function) via
 * a parser context typedef pSmt2Lexer, which is returned from a call to Smt2LexerNew().
 *
 * As this is a generated lexer, it is unlikely you will call it 'manually'. However
 * the methods are provided anyway.
 *
 * The methods in pSmt2Lexer are  as follows:
 *
 *  - 
 void
      pSmt2Lexer->T__136(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->T__137(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->T__138(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->T__139(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->T__140(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->T__141(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->ASSERT_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->CHECKSAT_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->DECLARE_FUN_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->DECLARE_SORT_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->DEFINE_FUN_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->DEFINE_SORT_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->GET_VALUE_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->GET_ASSIGNMENT_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->GET_ASSERTIONS_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->GET_PROOF_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->GET_UNSAT_CORE_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->EXIT_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->ITE_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->LET_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->ATTRIBUTE_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->LPAREN_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->RPAREN_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->INDEX_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->SET_LOGIC_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->SET_INFO_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->GET_INFO_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->SET_OPTION_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->GET_OPTION_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->PUSH_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->POP_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->AS_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->DECLARE_DATATYPES_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->GET_MODEL_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->ECHO_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->REWRITE_RULE_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->REDUCTION_RULE_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->PROPAGATION_RULE_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->DECLARE_SORTS_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->DECLARE_FUNS_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->DECLARE_PREDS_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->DEFINE_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->DECLARE_CONST_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->SIMPLIFY_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->INCLUDE_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->ATTRIBUTE_PATTERN_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->ATTRIBUTE_NO_PATTERN_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->ATTRIBUTE_NAMED_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->AMPERSAND_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->AND_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->AT_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->DISTINCT_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->DIV_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->EQUAL_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->EXISTS_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->FORALL_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->GREATER_THAN_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->GREATER_THAN_EQUAL_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->IMPLIES_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->IS_INT_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->LESS_THAN_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->LESS_THAN_EQUAL_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->MINUS_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->NOT_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->OR_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->PLUS_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->POUND_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->SELECT_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->STAR_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->STORE_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->TO_INT_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->TO_REAL_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->XOR_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->INTS_DIV_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->INTS_MOD_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->ABS_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->DIVISIBLE_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->CONCAT_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->BVNOT_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->BVAND_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->BVOR_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->BVNEG_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->BVADD_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->BVMUL_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->BVUDIV_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->BVUREM_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->BVSHL_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->BVLSHR_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->BVULT_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->BVNAND_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->BVNOR_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->BVXOR_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->BVXNOR_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->BVCOMP_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->BVSUB_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->BVSDIV_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->BVSREM_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->BVSMOD_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->BVASHR_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->BVULE_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->BVUGT_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->BVUGE_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->BVSLT_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->BVSLE_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->BVSGT_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->BVSGE_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->BV2NAT_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->INT2BV_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->STRCON_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->STRLEN_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->STRSUB_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->STRINRE_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->STRTORE_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->RECON_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->REOR_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->REINTER_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->RESTAR_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->REPLUS_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->REOPT_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->RERANGE_TOK(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->QUOTED_SYMBOL(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->UNTERMINATED_QUOTED_SYMBOL(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->KEYWORD(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->SIMPLE_SYMBOL(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->WHITESPACE(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->INTEGER_LITERAL(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->NUMERAL(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->DECIMAL_LITERAL(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->HEX_LITERAL(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->BINARY_LITERAL(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->STRING_LITERAL(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->COMMENT(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->ALPHA(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->DIGIT(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->HEX_DIGIT(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->SYMBOL_CHAR_NOUNDERSCORE_NOATTRIBUTE(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->SYMBOL_CHAR(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->ESCAPE(pSmt2Lexer)
 *  - 
 void
      pSmt2Lexer->Tokens(pSmt2Lexer)
 *
 * The return type for any particular rule is of course determined by the source
 * grammar file.
 */
// [The "BSD license"]
// Copyright (c) 2005-2009 Jim Idle, Temporal Wave LLC
// http://www.temporal-wave.com
// http://www.linkedin.com/in/jimidle
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. The name of the author may not be used to endorse or promote products
//    derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
// NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef	_Smt2Lexer_H
#define _Smt2Lexer_H
/* =============================================================================
 * Standard antlr3 C runtime definitions
 */
#include    <antlr3.h>

/* End of standard antlr 3 runtime definitions
 * =============================================================================
 */


/** This suppresses warnings about the redefinition of token symbols between
  * different parsers. The redefinitions should be harmless as long as no
  * client: (a) #include's the lexer headers for two grammars AND (b) uses the
  * token symbol definitions.
  */
#pragma GCC system_header

#if defined(CVC4_COMPETITION_MODE) && !defined(CVC4_SMTCOMP_APPLICATION_TRACK)
/* This improves performance by ~10 percent on big inputs.
 * This option is only valid if we know the input is ASCII (or some 8-bit encoding).
 * If we know the input is UTF-16, we can use ANTLR3_INLINE_INPUT_UTF16.
 * Otherwise, we have to let the lexer detect the encoding at runtime.
 */
#  define ANTLR3_INLINE_INPUT_ASCII
#  define ANTLR3_INLINE_INPUT_8BIT
#endif /* CVC4_COMPETITION_MODE && !CVC4_SMTCOMP_APPLICATION_TRACK */

#include "parser/antlr_tracing.h"



#ifdef __cplusplus
extern "C" {
#endif

// Forward declare the context typedef so that we can use it before it is
// properly defined. Delegators and delegates (from import statements) are
// interdependent and their context structures contain pointers to each other
// C only allows such things to be declared if you pre-declare the typedef.
//
typedef struct Smt2Lexer_Ctx_struct Smt2Lexer, * pSmt2Lexer;



#ifdef	ANTLR3_WINDOWS
// Disable: Unreferenced parameter,							- Rules with parameters that are not used
//          constant conditional,							- ANTLR realizes that a prediction is always true (synpred usually)
//          initialized but unused variable					- tree rewrite variables declared but not needed
//          Unreferenced local variable						- lexer rule declares but does not always use _type
//          potentially unitialized variable used			- retval always returned from a rule
//			unreferenced local function has been removed	- susually getTokenNames or freeScope, they can go without warnigns
//
// These are only really displayed at warning level /W4 but that is the code ideal I am aiming at
// and the codegen must generate some of these warnings by necessity, apart from 4100, which is
// usually generated when a parser rule is given a parameter that it does not use. Mostly though
// this is a matter of orthogonality hence I disable that one.
//
#pragma warning( disable : 4100 )
#pragma warning( disable : 4101 )
#pragma warning( disable : 4127 )
#pragma warning( disable : 4189 )
#pragma warning( disable : 4505 )
#pragma warning( disable : 4701 )
#endif

/** Context tracking structure for 
Smt2Lexer

 */
struct Smt2Lexer_Ctx_struct
{
    /** Built in ANTLR3 context tracker contains all the generic elements
     *  required for context tracking.
     */
    pANTLR3_LEXER    pLexer;

     void
     (*mT__136)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mT__137)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mT__138)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mT__139)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mT__140)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mT__141)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mASSERT_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mCHECKSAT_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mDECLARE_FUN_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mDECLARE_SORT_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mDEFINE_FUN_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mDEFINE_SORT_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mGET_VALUE_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mGET_ASSIGNMENT_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mGET_ASSERTIONS_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mGET_PROOF_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mGET_UNSAT_CORE_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mEXIT_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mITE_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mLET_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mATTRIBUTE_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mLPAREN_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mRPAREN_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mINDEX_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mSET_LOGIC_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mSET_INFO_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mGET_INFO_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mSET_OPTION_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mGET_OPTION_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mPUSH_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mPOP_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mAS_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mDECLARE_DATATYPES_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mGET_MODEL_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mECHO_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mREWRITE_RULE_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mREDUCTION_RULE_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mPROPAGATION_RULE_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mDECLARE_SORTS_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mDECLARE_FUNS_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mDECLARE_PREDS_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mDEFINE_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mDECLARE_CONST_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mSIMPLIFY_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mINCLUDE_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mATTRIBUTE_PATTERN_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mATTRIBUTE_NO_PATTERN_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mATTRIBUTE_NAMED_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mAMPERSAND_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mAND_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mAT_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mDISTINCT_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mDIV_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mEQUAL_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mEXISTS_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mFORALL_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mGREATER_THAN_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mGREATER_THAN_EQUAL_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mIMPLIES_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mIS_INT_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mLESS_THAN_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mLESS_THAN_EQUAL_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mMINUS_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mNOT_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mOR_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mPLUS_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mPOUND_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mSELECT_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mSTAR_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mSTORE_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mTO_INT_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mTO_REAL_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mXOR_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mINTS_DIV_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mINTS_MOD_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mABS_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mDIVISIBLE_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mCONCAT_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mBVNOT_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mBVAND_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mBVOR_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mBVNEG_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mBVADD_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mBVMUL_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mBVUDIV_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mBVUREM_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mBVSHL_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mBVLSHR_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mBVULT_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mBVNAND_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mBVNOR_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mBVXOR_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mBVXNOR_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mBVCOMP_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mBVSUB_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mBVSDIV_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mBVSREM_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mBVSMOD_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mBVASHR_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mBVULE_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mBVUGT_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mBVUGE_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mBVSLT_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mBVSLE_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mBVSGT_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mBVSGE_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mBV2NAT_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mINT2BV_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mSTRCON_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mSTRLEN_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mSTRSUB_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mSTRINRE_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mSTRTORE_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mRECON_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mREOR_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mREINTER_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mRESTAR_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mREPLUS_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mREOPT_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mRERANGE_TOK)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mQUOTED_SYMBOL)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mUNTERMINATED_QUOTED_SYMBOL)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mKEYWORD)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mSIMPLE_SYMBOL)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mWHITESPACE)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mINTEGER_LITERAL)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mNUMERAL)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mDECIMAL_LITERAL)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mHEX_LITERAL)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mBINARY_LITERAL)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mSTRING_LITERAL)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mCOMMENT)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mALPHA)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mDIGIT)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mHEX_DIGIT)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mSYMBOL_CHAR_NOUNDERSCORE_NOATTRIBUTE)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mSYMBOL_CHAR)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mESCAPE)	(struct Smt2Lexer_Ctx_struct * ctx);

     void
     (*mTokens)	(struct Smt2Lexer_Ctx_struct * ctx);
    const char * (*getGrammarFileName)();
    void            (*reset)  (struct Smt2Lexer_Ctx_struct * ctx);
    void	    (*free)   (struct Smt2Lexer_Ctx_struct * ctx);
};

// Function protoypes for the constructor functions that external translation units
// such as delegators and delegates may wish to call.
//
ANTLR3_API pSmt2Lexer Smt2LexerNew         (
pANTLR3_INPUT_STREAM
 instream);
ANTLR3_API pSmt2Lexer Smt2LexerNewSSD      (
pANTLR3_INPUT_STREAM
 instream, pANTLR3_RECOGNIZER_SHARED_STATE state);

/** Symbolic definitions of all the tokens that the 
lexer
 will work with.
 * \{
 *
 * Antlr will define EOF, but we can't use that as it it is too common in
 * in C header files and that would be confusing. There is no way to filter this out at the moment
 * so we just undef it here for now. That isn't the value we get back from C recognizers
 * anyway. We are looking for ANTLR3_TOKEN_EOF.
 */
#ifdef	EOF
#undef	EOF
#endif
#ifdef	Tokens
#undef	Tokens
#endif
#define EOF      -1
#define T__136      136
#define T__137      137
#define T__138      138
#define T__139      139
#define T__140      140
#define T__141      141
#define ABS_TOK      4
#define ALPHA      5
#define AMPERSAND_TOK      6
#define AND_TOK      7
#define ASSERT_TOK      8
#define AS_TOK      9
#define ATTRIBUTE_NAMED_TOK      10
#define ATTRIBUTE_NO_PATTERN_TOK      11
#define ATTRIBUTE_PATTERN_TOK      12
#define ATTRIBUTE_TOK      13
#define AT_TOK      14
#define BINARY_LITERAL      15
#define BV2NAT_TOK      16
#define BVADD_TOK      17
#define BVAND_TOK      18
#define BVASHR_TOK      19
#define BVCOMP_TOK      20
#define BVLSHR_TOK      21
#define BVMUL_TOK      22
#define BVNAND_TOK      23
#define BVNEG_TOK      24
#define BVNOR_TOK      25
#define BVNOT_TOK      26
#define BVOR_TOK      27
#define BVSDIV_TOK      28
#define BVSGE_TOK      29
#define BVSGT_TOK      30
#define BVSHL_TOK      31
#define BVSLE_TOK      32
#define BVSLT_TOK      33
#define BVSMOD_TOK      34
#define BVSREM_TOK      35
#define BVSUB_TOK      36
#define BVUDIV_TOK      37
#define BVUGE_TOK      38
#define BVUGT_TOK      39
#define BVULE_TOK      40
#define BVULT_TOK      41
#define BVUREM_TOK      42
#define BVXNOR_TOK      43
#define BVXOR_TOK      44
#define CHECKSAT_TOK      45
#define COMMENT      46
#define CONCAT_TOK      47
#define DECIMAL_LITERAL      48
#define DECLARE_CONST_TOK      49
#define DECLARE_DATATYPES_TOK      50
#define DECLARE_FUNS_TOK      51
#define DECLARE_FUN_TOK      52
#define DECLARE_PREDS_TOK      53
#define DECLARE_SORTS_TOK      54
#define DECLARE_SORT_TOK      55
#define DEFINE_FUN_TOK      56
#define DEFINE_SORT_TOK      57
#define DEFINE_TOK      58
#define DIGIT      59
#define DISTINCT_TOK      60
#define DIVISIBLE_TOK      61
#define DIV_TOK      62
#define ECHO_TOK      63
#define EQUAL_TOK      64
#define ESCAPE      65
#define EXISTS_TOK      66
#define EXIT_TOK      67
#define FORALL_TOK      68
#define GET_ASSERTIONS_TOK      69
#define GET_ASSIGNMENT_TOK      70
#define GET_INFO_TOK      71
#define GET_MODEL_TOK      72
#define GET_OPTION_TOK      73
#define GET_PROOF_TOK      74
#define GET_UNSAT_CORE_TOK      75
#define GET_VALUE_TOK      76
#define GREATER_THAN_EQUAL_TOK      77
#define GREATER_THAN_TOK      78
#define HEX_DIGIT      79
#define HEX_LITERAL      80
#define IMPLIES_TOK      81
#define INCLUDE_TOK      82
#define INDEX_TOK      83
#define INT2BV_TOK      84
#define INTEGER_LITERAL      85
#define INTS_DIV_TOK      86
#define INTS_MOD_TOK      87
#define IS_INT_TOK      88
#define ITE_TOK      89
#define KEYWORD      90
#define LESS_THAN_EQUAL_TOK      91
#define LESS_THAN_TOK      92
#define LET_TOK      93
#define LPAREN_TOK      94
#define MINUS_TOK      95
#define NOT_TOK      96
#define NUMERAL      97
#define OR_TOK      98
#define PLUS_TOK      99
#define POP_TOK      100
#define POUND_TOK      101
#define PROPAGATION_RULE_TOK      102
#define PUSH_TOK      103
#define QUOTED_SYMBOL      104
#define RECON_TOK      105
#define REDUCTION_RULE_TOK      106
#define REINTER_TOK      107
#define REOPT_TOK      108
#define REOR_TOK      109
#define REPLUS_TOK      110
#define RERANGE_TOK      111
#define RESTAR_TOK      112
#define REWRITE_RULE_TOK      113
#define RPAREN_TOK      114
#define SELECT_TOK      115
#define SET_INFO_TOK      116
#define SET_LOGIC_TOK      117
#define SET_OPTION_TOK      118
#define SIMPLE_SYMBOL      119
#define SIMPLIFY_TOK      120
#define STAR_TOK      121
#define STORE_TOK      122
#define STRCON_TOK      123
#define STRING_LITERAL      124
#define STRINRE_TOK      125
#define STRLEN_TOK      126
#define STRSUB_TOK      127
#define STRTORE_TOK      128
#define SYMBOL_CHAR      129
#define SYMBOL_CHAR_NOUNDERSCORE_NOATTRIBUTE      130
#define TO_INT_TOK      131
#define TO_REAL_TOK      132
#define UNTERMINATED_QUOTED_SYMBOL      133
#define WHITESPACE      134
#define XOR_TOK      135
#ifdef	EOF
#undef	EOF
#define	EOF	ANTLR3_TOKEN_EOF
#endif

#ifndef TOKENSOURCE
#define TOKENSOURCE(lxr) lxr->pLexer->rec->state->tokSource
#endif

/* End of token definitions for Smt2Lexer
 * =============================================================================
 */
/** } */

#ifdef __cplusplus
}
#endif

#endif

/* END - Note:Keep extra line feed to satisfy UNIX systems */
