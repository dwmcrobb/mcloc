%{
  //=========================================================================
  // @(#) $DwmPath: dwm/mcplex/mcloc/trunk/classes/src/DwmMclocSqlFileScanner.lex 11829 $
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
  //!  \file DwmMclocSqlFileScanner.lex
  //!  \brief Dwm::Mcloc::SqlFileScanner implementation and lexical scanner
  //-------------------------------------------------------------------------

  #include "DwmMclocSqlFileScanner.hh"

  static const std::string svnid("@(#) $DwmPath: dwm/mcplex/mcloc/trunk/classes/src/DwmMclocSqlFileScanner.lex 11829 $");
%}

 //  flex options
%option prefix="sql_"
%option 8bit full reentrant noyywrap
%option outfile="DwmMclocSqlFileScanner.cc"
%option extra-type="Dwm::Mcloc::SourceFileInfo *"

 //  patterns
m_blankLine      ^[ \t]*\n
m_sqlCommentLine ^[ \t]*[-][-].*\n
m_cCommentLine   ^[ \t]*[/][*].*[*][/][ \t]*[\n]
   
 //  states
%x x_inComment
%x x_quote
   
%%

<INITIAL>{m_sqlCommentLine}     { yyextra->Counter().IncrementCommentLines(); }
<INITIAL>{m_cCommentLine}       { yyextra->Counter().IncrementCommentLines(); }
<INITIAL>[/][*]                 { BEGIN(x_inComment); }
<INITIAL>\"                     { BEGIN(x_quote); }
<INITIAL>{m_blankLine}
<INITIAL>[^"\-/*\n]+
<INITIAL>.
<INITIAL>[\n]                   { yyextra->Counter().IncrementCodeLines(); }

<x_inComment>[^*\n]*
<x_inComment>[^*\n]*\n          { yyextra->Counter().IncrementCommentLines(); }
<x_inComment>"*"+[^*/\n]*
<x_inComment>"*"+[^*/\n]*\n     { yyextra->Counter().IncrementCommentLines(); }
<x_inComment>\*\/[ \t]*\n       { yyextra->Counter().IncrementCommentLines();
                                  BEGIN(INITIAL); }
<x_inComment>"*"+"/"            { BEGIN(INITIAL); }

<x_quote>[\\]["]
<x_quote>\"                     { BEGIN(INITIAL); }                      
<x_quote>.
<x_quote>[\n]                   { yyextra->Counter().IncrementCodeLines(); }

<<EOF>>                         { return 0; }
  
%%

namespace Dwm {

  namespace Mcloc {

    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    void SqlFileScanner::Scan(const std::filesystem::path & path,
                              SourceFileInfo & sfi) const
    {
      sfi.Counter().Reset();
      yyscan_t  scanner;
      sql_lex_init_extra(&sfi, &scanner);
      // sql_set_debug(1, scanner);
      FILE  *inFile = fopen(path.string().c_str(), "r");
      if (inFile) {
        sql_set_in(inFile, scanner);
        sql_lex(scanner);
        fclose(inFile);
      }
      sql_lex_destroy(scanner);
      return;
    }

  }  // namespace Mcloc

}  // namespace Dwm
