%{
  //=========================================================================
  // @(#) $DwmPath: dwm/mcplex/mcloc/trunk/classes/src/DwmMclocRustFileScanner.lex 11829 $
  //=========================================================================
  //  Copyright (c) Daniel W. McRobb 2019, 2020
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
  //!  \file DwmMclocRustFileScanner.lex
  //!  \brief Dwm::Mcloc::RustFileScanner implementation and lexical scanner
  //-------------------------------------------------------------------------

  #include "DwmMclocRustFileScanner.hh"

  static const std::string svnid("@(#) $DwmPath: dwm/mcplex/mcloc/trunk/classes/src/DwmMclocRustFileScanner.lex 11829 $");

  //   INITIAL
  //       x_initComLine
  //       x_initBlkCom
  //       x_code
  //           x_comLine
  //           x_blkCom
  //           x_string
  //           x_char

  struct RustExtra {
    std::string                   fname;
    Dwm::Mcloc::SourceFileInfo  & sfi;
  };
  
%}

 //  flex options
%option prefix="rust_"
%option 8bit full reentrant stack noyywrap
%option outfile="DwmMclocRustFileScanner.cc"
%option extra-type="RustExtra *"

 //  patterns
   
 //  states
%x x_code
%x x_initComLine
%x x_comLine
%x x_initBlkCom
%x x_blkCom
%x x_string
%x x_char
   
%%

<INITIAL>"//"                 { yy_push_state(x_initComLine, yyscanner); }
<INITIAL>"/*"                 { yy_push_state(x_initBlkCom, yyscanner); }
<INITIAL>[*/"]                { yy_push_state(x_code, yyscanner);
                                 yyless(0); }
<INITIAL>[^/* "\t\n]          { yy_push_state(x_code, yyscanner); }
<INITIAL>[ \t]
<INITIAL>\n

<x_initComLine>.+
<x_initComLine>\n             { yyextra->sfi.Counter().IncrementCommentLines();
                                yy_pop_state(yyscanner); }

<x_initBlkCom>"/*"            { yy_push_state(x_initBlkCom, yyscanner); }
<x_initBlkCom>^[ \t]*\n
<x_initBlkCom>[^*/\n]+
<x_initBlkCom>"*/"[ \t]*\n    { yyextra->sfi.Counter().IncrementCommentLines();
                                yy_pop_state(yyscanner);
                                unput('\n'); }
<x_initBlkCom>"*/"            { yy_pop_state(yyscanner); }
<x_initBlkCom>.
<x_initBlkCom>\n              { yyextra->sfi.Counter().IncrementCommentLines(); }

<x_code>[^*/\"'\n]+
<x_code>"//"                  { yy_push_state(x_comLine, yyscanner); }
<x_code>"/*"                  { yy_push_state(x_blkCom, yyscanner); }
<x_code>\'\"\'
<x_code>\"                    { yy_push_state(x_string, yyscanner); }
<x_code>\'                    { yy_push_state(x_char, yyscanner); }
<x_code>[*/]
<x_code>\n                    { yyextra->sfi.Counter().IncrementCodeLines();
                                yy_pop_state(yyscanner); }

<x_comLine>.+
<x_comLine>\n                 { yyless(0);
                                yy_pop_state(yyscanner); }

<x_blkCom>"/*"                { yy_push_state(x_blkCom, yyscanner); }
<x_blkCom>^[ \t]*\n
<x_blkCom>[^*/\n]+
<x_blkCom>"*/"                { yy_pop_state(yyscanner); }
<x_blkCom>.
<x_blkCom>\n                  { yyextra->sfi.Counter().IncrementCommentLines(); }

<x_string>\\\"
<x_string>\"                  { yy_pop_state(yyscanner); }
<x_string>[^"\\\n]+
<x_string>\\
<x_string>\n                  { yyextra->sfi.Counter().IncrementCodeLines(); }

<x_char>[^'\\]+
<x_char>\\\'
<x_char>\'                    { yy_pop_state(yyscanner); }
<x_char>\\

<<EOF>>                       { return 0; }
  
%%

namespace Dwm {

  namespace Mcloc {

    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    void RustFileScanner::Scan(const std::filesystem::path & path,
                               SourceFileInfo & sfi) const
    {
      sfi.Counter().Reset();
      RustExtra  extra({ path.filename().string(), sfi });
      yyscan_t  scanner;
      rust_lex_init_extra(&extra, &scanner);
      rust_set_debug(1, scanner);
      FILE  *inFile = fopen(path.string().c_str(), "r");
      if (inFile) {
        rust_set_in(inFile, scanner);
        rust_lex(scanner);
        fclose(inFile);
      }
      rust_lex_destroy(scanner);
      return;
    }

  }  // namespace Mcloc

}  // namespace Dwm
