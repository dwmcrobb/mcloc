%{
  //=========================================================================
  // @(#) $DwmPath: dwm/mcplex/mcloc/trunk/classes/src/DwmMclocPerlFileScanner.lex 11829 $
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
  //!  \file DwmMclocPerlFileScanner.lex
  //!  \brief Dwm::Mcloc::PerlFileScanner implementation and lexical scanner
  //-------------------------------------------------------------------------

  #include "DwmMclocPerlFileScanner.hh"

  static const std::string svnid("@(#) $DwmPath: dwm/mcplex/mcloc/trunk/classes/src/DwmMclocPerlFileScanner.lex 11829 $");
%}

 //  flex options
%option prefix="perl_"
%option 8bit full reentrant noyywrap
%option outfile="DwmMclocPerlFileScanner.cc"
%option extra-type="Dwm::Mcloc::SourceFileInfo *"

 //  patterns
m_perlCommentLine ^[ \t]*[#].*\n
m_mlStart  \=
m_mlEnd    \=cut

 //  states
%x x_inComment
   
%%

<INITIAL>^[#][!][/].*\n
<INITIAL>{m_perlCommentLine}    { yyextra->Counter().IncrementCommentLines(); }
<INITIAL>^{m_mlStart}.*\n       { yyextra->Counter().IncrementCommentLines();
                                  BEGIN(x_inComment); }
<x_inComment>^{m_mlEnd}[ \t]*\n { yyextra->Counter().IncrementCommentLines();
                                  BEGIN(INITIAL); }
<x_inComment>.
<x_inComment>^[ \t]*\n
<x_inComment>\n                 { yyextra->Counter().IncrementCommentLines(); }
<INITIAL>^[ \t]*\n
<INITIAL>[^=#\n]+
<INITIAL>.
<INITIAL>[\n]                   { yyextra->Counter().IncrementCodeLines(); }
<<EOF>>                         { return 0; }
  
%%

namespace Dwm {

  namespace Mcloc {

    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    void PerlFileScanner::Scan(const std::filesystem::path & path,
                               SourceFileInfo & sfi) const
    {
      sfi.Counter().Reset();
      yyscan_t  scanner;
      perl_lex_init_extra(&sfi, &scanner);
      FILE  *inFile = fopen(path.string().c_str(), "r");
      if (inFile) {
        perl_set_in(inFile, scanner);
        perl_lex(scanner);
        fclose(inFile);
      }
      perl_lex_destroy(scanner);
      return;
    }

  }  // namespace Mcloc

}  // namespace Dwm
