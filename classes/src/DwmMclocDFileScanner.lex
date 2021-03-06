%{
  //=========================================================================
  // @(#) $DwmPath: dwm/mcplex/mcloc/trunk/classes/src/DwmMclocDFileScanner.lex 11829 $
  //=========================================================================
  //  Copyright (c) Daniel W. McRobb 2020
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
  //!  \file DwmMclocDFileScanner.lex
  //!  \author Daniel W. McRobb
  //!  \brief Dwm::Mcloc::DFileScanner implementation and lexical scanner
  //-------------------------------------------------------------------------

  #include "DwmMclocDFileScanner.hh"

  static const std::string svnid("@(#) $DwmPath: dwm/mcplex/mcloc/trunk/classes/src/DwmMclocDFileScanner.lex 11829 $");
%}

 // flex options
%option prefix="d_"
%option 8bit full reentrant stack noyywrap
%option outfile="DwmMclocDFileScanner.cc"
%option extra-type="Dwm::Mcloc::SourceFileInfo *"

 // patterns
m_cppCmtLine  ^[ \t]*[/][/].*\n
m_c89CmtLine  ^[ \t]*[/][*].*[*][/][ \t]*[\n]

 // states
%x x_c89
%x x_nested
%x x_quote

%%

<INITIAL>{m_cppCmtLine}  { yyextra->Counter().IncrementCommentLines(); }
<INITIAL>{m_c89CmtLine}  { yyextra->Counter().IncrementCommentLines(); }

<INITIAL>\\\"
<INITIAL>\/\*           { yy_push_state(x_c89, yyscanner); }
<INITIAL>\/\+           { yy_push_state(x_nested, yyscanner); }
<INITIAL>\"             { yy_push_state(x_quote, yyscanner); }

<x_c89>^[ \t]*\n
<x_c89>\*\/[ \t]*\n     { yyextra->Counter().IncrementCommentLines();
                          yy_pop_state(yyscanner); }
<x_c89>\*\/             { yy_pop_state(yyscanner); }
<x_c89>\n               { yyextra->Counter().IncrementCommentLines(); }
<x_c89>.

<x_nested>^[ \t]*\n
<x_nested>\/\+          { yy_push_state(x_nested, yyscanner); }
<x_nested>\+\/[ \t]*\n  { yyextra->Counter().IncrementCommentLines();
                          yy_pop_state(yyscanner); }
<x_nested>\+\/          { yy_pop_state(yyscanner); }
<x_nested>\n            { yyextra->Counter().IncrementCommentLines(); }
<x_nested>.

<x_quote>\\\"
<x_quote>\"             { yy_pop_state(yyscanner); }
<x_quote>\n             { yyextra->Counter().IncrementCodeLines(); }
<x_quote>.

<INITIAL>^[ \t]*\n
<INITIAL>[^/\+\*"\n]+
<INITIAL>\n             { yyextra->Counter().IncrementCodeLines(); }
<INITIAL>.

%%

namespace Dwm {

  namespace Mcloc {

    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    void DFileScanner::Scan(const std::filesystem::path & path,
                            SourceFileInfo & sfi) const
    {
      sfi.Counter().Reset();
      yyscan_t  scanner;
      d_lex_init_extra(&sfi, &scanner);
      FILE  *inFile = fopen(path.string().c_str(), "r");
      if (inFile) {
        d_set_in(inFile, scanner);
        d_lex(scanner);
        fclose(inFile);
      }
      d_lex_destroy(scanner);
      return;
    }

  }  // namespace Mcloc

}  // namespace Dwm
