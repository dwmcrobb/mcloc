%{
  //=========================================================================
  // @(#) $DwmPath: dwm/mcplex/mcloc/trunk/classes/src/DwmMclocClojureFileScanner.lex 11829 $
  //=========================================================================
  //  Copyright (c) Daniel W. McRobb 2019
  //  All rights reserved.
  //
  //  Redistribution and use in source and binary forms, with or without
  //  modification, are permitted provided that the following conditions
  //  are met:
  //
  //  1. Redistributions of source code must retain the above copyright
  //     notice, this list of conditions and the following disclaimer.
  //  2. Redistributions in binary form must reproduce the above copyright
  //     notice, this list of conditions and the following disclaimer in the
  //     documentation and/or other materials provided with the distribution.
  //  3. The names of the authors and copyright holders may not be used to
  //     endorse or promote products derived from this software without
  //     specific prior written permission.
  //
  //  IN NO EVENT SHALL DANIEL W. MCROBB BE LIABLE TO ANY PARTY FOR
  //  DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES,
  //  INCLUDING LOST PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE,
  //  EVEN IF DANIEL W. MCROBB HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH
  //  DAMAGE.
  //
  //  THE SOFTWARE PROVIDED HEREIN IS ON AN "AS IS" BASIS, AND
  //  DANIEL W. MCROBB HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT,
  //  UPDATES, ENHANCEMENTS, OR MODIFICATIONS. DANIEL W. MCROBB MAKES NO
  //  REPRESENTATIONS AND EXTENDS NO WARRANTIES OF ANY KIND, EITHER
  //  IMPLIED OR EXPRESS, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  //  WARRANTIES OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE,
  //  OR THAT THE USE OF THIS SOFTWARE WILL NOT INFRINGE ANY PATENT,
  //  TRADEMARK OR OTHER RIGHTS.
  //=========================================================================
  
  //-------------------------------------------------------------------------
  //!  \file DwmMclocClojureFileScanner.lex
  //!  \brief Dwm::Mcloc::ClojureFileScanner implementation and lexical scanner
  //-------------------------------------------------------------------------

  #include <iostream>
  
  #include "DwmMclocClojureFileScanner.hh"

  static const std::string svnid("@(#) $DwmPath: dwm/mcplex/mcloc/trunk/classes/src/DwmMclocClojureFileScanner.lex 11829 $");

  typedef struct {
    int                           depth;
    Dwm::Mcloc::SourceFileInfo  & sfi;
  } MyExtra;

%}

 //  flex options
%option prefix="clojure_"
%option 8bit full reentrant stack noyywrap
%option outfile="DwmMclocClojureFileScanner.cc"
%option extra-type="MyExtra *"

 //  patterns
m_commentLine  ^[ \t]*[\;].*\n

 // states
%x x_comment
%x x_nestedParen
%x x_nestedBracket
%x x_quote

%%

<INITIAL>{m_commentLine}    { yyextra->sfi.Counter().IncrementCommentLines(); }
<INITIAL>\(comment          { ++yyextra->depth; BEGIN(x_comment); }
<INITIAL>\#\_[ \t]*\(       { yy_push_state(x_nestedParen, yyscanner); }
<INITIAL>\#\_[ \t]*\[       { yy_push_state(x_nestedBracket, yyscanner); }

<x_comment>\\\"
<x_comment>\"               { yy_push_state(x_quote, yyscanner); }
<x_comment>^[ \t]*\n
<x_comment>\([ \t]*         { ++yyextra->depth; }
<x_comment>\)[ \t]*\n       { yyextra->sfi.Counter().IncrementCommentLines();
                              --yyextra->depth;
                              if (yyextra->depth == 0) {
                                BEGIN(INITIAL);
                              }
                            }
<x_comment>\)[ \t]*         { --yyextra->depth;
                              if (yyextra->depth == 0) {
                                BEGIN(INITIAL);
                              }
                            }
<x_comment>.
<x_comment>\n               { yyextra->sfi.Counter().IncrementCommentLines(); }

<x_quote>\\\"
<x_quote>\"                 { yy_pop_state(yyscanner); }
<x_quote>[^"\n]
<x_quote>\n                 { yyextra->sfi.Counter().IncrementCommentLines(); }

<x_nestedParen>\\\"
<x_nestedParen>\"           { yy_push_state(x_quote, yyscanner); }
<x_nestedParen>^[ \t]*\n
<x_nestedParen>\([ \t]*     { yy_push_state(x_nestedParen, yyscanner); }
<x_nestedParen>\)[ \t]*\n   { yy_pop_state(yyscanner);
                              yyextra->sfi.Counter().IncrementCommentLines();
                            }
<x_nestedParen>\)[ \t]*     { yy_pop_state(yyscanner); }
<x_nestedParen>[^\(\)"\n]
<x_nestedParen>\n           { yyextra->sfi.Counter().IncrementCommentLines(); }

<x_nestedBracket>\\\"
<x_nestedBracket>\"         { yy_push_state(x_quote, yyscanner); }
<x_nestedBracket>^[ \t]*\n
<x_nestedBracket>\[[ \t]*   { yy_push_state(x_nestedBracket, yyscanner); }
<x_nestedBracket>\][ \t]*\n { yyextra->sfi.Counter().IncrementCommentLines();
                              yy_pop_state(yyscanner);
                            }
<x_nestedBracket>\][ \t]*   { yy_pop_state(yyscanner); }
<x_nestedBracket>[^\[\]\n]
<x_nestedBracket>\n         { yyextra->sfi.Counter().IncrementCommentLines(); }


<INITIAL>^[ \t]*\n
<INITIAL>[^\;#\n]+
<INITIAL>.
<INITIAL>[\n]               { yyextra->sfi.Counter().IncrementCodeLines(); }
<<EOF>>                     { return 0; }
  
%%

namespace Dwm {

  namespace Mcloc {

    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    void ClojureFileScanner::Scan(const std::filesystem::path & path,
                                SourceFileInfo & sfi) const
    {
      sfi.Counter().Reset();

      MyExtra   myextra = { 0, sfi };
      yyscan_t  scanner;
      clojure_lex_init_extra(&myextra, &scanner);
      FILE  *inFile = fopen(path.string().c_str(), "r");
      if (inFile) {
        clojure_set_in(inFile, scanner);
        clojure_lex(scanner);
        fclose(inFile);
      }
      clojure_lex_destroy(scanner);
      return;
    }

  }  // namespace Mcloc

}  // namespace Dwm
