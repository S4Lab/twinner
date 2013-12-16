/*********************                                                        */
/*! \file smt1_input.h
 ** \verbatim
 ** Original author: Morgan Deters
 ** Major contributors: Christopher L. Conway
 ** Minor contributors (to current version): none
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2013  New York University and The University of Iowa
 ** See the file COPYING in the top-level source directory for licensing
 ** information.\endverbatim
 **
 ** \brief [[ Add file-specific comments here ]].
 **
 ** [[ Add file-specific comments here ]]
 **/

#include "cvc4parser_private.h"

#ifndef __CVC4__PARSER__SMT1_INPUT_H
#define __CVC4__PARSER__SMT1_INPUT_H

#include "parser/antlr_input.h"
#include "parser/smt1/generated/Smt1Lexer.h"
#include "parser/smt1/generated/Smt1Parser.h"

// extern void Smt1ParserSetAntlrParser(CVC4::parser::AntlrParser* newAntlrParser);

namespace CVC4 {

class Command;
class Expr;
class ExprManager;

namespace parser {

class Smt1Input : public AntlrInput {

  /** The ANTLR3 SMT lexer for the input. */
  pSmt1Lexer d_pSmt1Lexer;

  /** The ANTLR3 CVC parser for the input. */
  pSmt1Parser d_pSmt1Parser;

public:

  /**
   * Create an input.
   *
   * @param inputStream the input stream to use
   */
  Smt1Input(AntlrInputStream& inputStream);

  /** Destructor. Frees the lexer and the parser. */
  virtual ~Smt1Input();

  /** Get the language that this Input is reading. */
  InputLanguage getLanguage() const throw() {
    return language::input::LANG_SMTLIB_V1;
  }

protected:

  /**
   * Parse a command from the input. Returns <code>NULL</code> if
   * there is no command there to parse.
   *
   * @throws ParserException if an error is encountered during parsing.
   */
  Command* parseCommand();

  /**
   * Parse an expression from the input. Returns a null
   * <code>Expr</code> if there is no expression there to parse.
   *
   * @throws ParserException if an error is encountered during parsing.
   */
  Expr parseExpr();

private:

  /**
   * Initialize the class. Called from the constructors once the input
   * stream is initialized.
   */
  void init();

};/* class Smt1Input */

}/* CVC4::parser namespace */
}/* CVC4 namespace */

#endif /* __CVC4__PARSER__SMT1_INPUT_H */
