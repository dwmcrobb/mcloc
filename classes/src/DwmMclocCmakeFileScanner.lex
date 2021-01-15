%{
  //=========================================================================
  // @(#) $DwmPath: dwm/mcplex/mcloc/trunk/classes/src/DwmMclocCmakeFileScanner.lex 11829 $
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
  //!  \file DwmMclocCmakeFileScanner.lex
  //!  \author Daniel W. McRobb
  //!  \brief Dwm::Mcloc::CmakeFileScanner implementation and lexical scanner
  //-------------------------------------------------------------------------

  #include "DwmMclocCmakeFileScanner.hh"

  static const std::string svnid("@(#) $DwmPath: dwm/mcplex/mcloc/trunk/classes/src/DwmMclocCmakeFileScanner.lex 11829 $");
%}

 // flex options
%option prefix="cmake_"
%option 8bit full reentrant stack noyywrap
%option outfile="DwmMclocCmakeFileScanner.cc"
%option extra-type="Dwm::Mcloc::SourceFileInfo *"

 // patterns
m_cStart   ^[ \t]*[#]

 // states
%x x_oneLineComment
%x x_blockComment
%x x_quote

%%

<INITIAL>{m_cStart}\[\[        { yy_push_state(x_blockComment, yyscanner); }
<x_blockComment>\]\][ \t]*\n   { yyextra->Counter().IncrementCommentLines();
                                 yy_pop_state(yyscanner); }
<x_blockComment>\]\]           { yy_pop_state(yyscanner); }
<x_blockComment>.
<x_blockComment>\n             { yyextra->Counter().IncrementCommentLines(); }

<INITIAL>{m_cStart}            { yy_push_state(x_oneLineComment, yyscanner); }
<x_oneLineComment>.*\n         { yyextra->Counter().IncrementCommentLines();
                                 yy_pop_state(yyscanner); }
<INITIAL>^[ \t]*\n
<INITIAL>\n                    { yyextra->Counter().IncrementCodeLines(); }
<INITIAL>.

%%

namespace Dwm {

  namespace Mcloc {

    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    void CmakeFileScanner::Scan(const std::filesystem::path & path,
                                 SourceFileInfo & sfi) const
    {
      sfi.Counter().Reset();
      yyscan_t  scanner;
      cmake_lex_init_extra(&sfi, &scanner);
      FILE  *inFile = fopen(path.string().c_str(), "r");
      if (inFile) {
        cmake_set_in(inFile, scanner);
        cmake_lex(scanner);
        fclose(inFile);
      }
      cmake_lex_destroy(scanner);
      return;
    }

  }  // namespace Mcloc

}  // namespace Dwm
