%{
  //===========================================================================
  // @(#) $DwmPath: dwm/mcplex/mcloc/trunk/classes/src/DwmMclocFlexFileScanner.lex 11829 $
  //===========================================================================
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
  //===========================================================================
  
  //---------------------------------------------------------------------------
  //!  \file DwmMclocFlexFileScanner.lex
  //!  \author Daniel W. McRobb
  //!  \brief flex file scanner
  //---------------------------------------------------------------------------
  
  #include "DwmMclocFlexFileScanner.hh"

  static const std::string svnid("@(#) $DwmPath: dwm/mcplex/mcloc/trunk/classes/src/DwmMclocFlexFileScanner.lex 11829 $");

  //   INITIAL (definitions)
  //       x_initComLine
  //       x_initBlkCom
  //       x_pctBlk
  //           x_initComLine
  //           x_initBlkCom
  //           x_code
  //               x_comLine
  //               x_blkCom
  //               x_string
  //               x_char
  //       x_rules
  //           x_initComLine
  //           x_initBlkCom
  //           x_user
  //               x_initComLine
  //               x_initBlkCom
  //               x_code
  //                   x_comLine
  //                   x_blkCom
  //                   x_string
  //                   x_char
  //           x_code
  //               x_comLine
  //               x_blkCom
  //               x_string
  //               x_char
  
%}

 //  flex options
%option prefix="flex_"
%option 8bit full reentrant stack noyywrap
%option outfile="DwmMclocFlexFileScanner.cc"
%option extra-type="Dwm::Mcloc::CodeCounter *"

 //  states
%x x_rules
%x x_lrMtch
%x x_lrBrkt
%x x_lrStr
%x x_lrCode
%x x_user
%x x_pctBlk
%x x_code
%x x_initComLine
%x x_comLine
%x x_initBlkCom
%x x_blkCom
%x x_string
%x x_char

%%

 // Note that the INITIAL state is used in the 'definitions' section, which
 // is syntactically different than the 'rules' and the 'user code' sections
 // of a flex input file.

<INITIAL>^\%top\{[ \t]*       { yy_push_state(x_pctBlk, yyscanner);
                                yyextra->IncrementCodeLines(); }
<INITIAL>^\%\{[ \t]*          { yy_push_state(x_pctBlk, yyscanner);
                                yyextra->IncrementCodeLines(); }
<INITIAL>^\%\%\n              { yy_push_state(x_rules, yyscanner);
                                yyextra->IncrementCodeLines(); }
<INITIAL>^[ \t]*\/\/          { yy_push_state(x_initComLine, yyscanner); }
<INITIAL>^[ \t]*\/\*          { yy_push_state(x_initBlkCom, yyscanner); }
<INITIAL>^[^% \t\n]+.*
<INITIAL>^\%[^t\{\n]+.*
<INITIAL>.
<INITIAL>^[ \t]*\n
<INITIAL>\n                   { yyextra->IncrementCodeLines(); }

<x_pctBlk>^\%\}[ \t]*\n       { yy_pop_state(yyscanner);
                                yyextra->IncrementCodeLines(); }
<x_pctBlk>^\%\}               { yy_pop_state(yyscanner); }
<x_pctBlk>"//"                { yy_push_state(x_initComLine, yyscanner); }
<x_pctBlk>"/*"                { yy_push_state(x_initBlkCom, yyscanner); }
<x_pctBlk>[^/*%} \t\n]        { yy_push_state(x_code, yyscanner); }
<x_pctBlk>[*/%}]              { yy_push_state(x_code, yyscanner);
                                unput(yytext[0]); }
<x_pctBlk>[ \t]
<x_pctBlk>\n

<x_rules>^\%\%\n              { yy_push_state(x_user, yyscanner);
                                yyextra->IncrementCodeLines(); }
<x_rules>^[ \t]+\/\/          { yy_push_state(x_initComLine, yyscanner); }
<x_rules>^[ \t]+\/\*          { yy_push_state(x_initBlkCom, yyscanner); }
<x_rules>^[^ %\t\n]           { yy_push_state(x_lrMtch, yyscanner);
                                yyless(0); /* unput(yytext[0]); */ }
<x_rules>.
<x_rules>^[ \t]*\n
<x_rules>\n                   { yyextra->IncrementCodeLines(); }

<x_user>"//"                  { yy_push_state(x_initComLine, yyscanner); }
<x_user>"/*"                  { yy_push_state(x_initBlkCom, yyscanner); }
<x_user>[^/* \t\n]            { yy_push_state(x_code, yyscanner);
                                unput(yytext[0]); }
<x_user>[*/]                  { yy_push_state(x_code, yyscanner);
                                unput(yytext[0]); }
<x_user>[ \t\n]

<x_initComLine>.+
<x_initComLine>\n             { yyextra->IncrementCommentLines();
                                yy_pop_state(yyscanner); }

<x_initBlkCom>[^*/\n]*
<x_initBlkCom>\*\/[ \t]*\n    { yyextra->IncrementCommentLines();
                                yy_pop_state(yyscanner); }
<x_initBlkCom>[^*\n]*
<x_initBlkCom>"*"+[^*/\n]*
<x_initBlkCom>\n              { yyextra->IncrementCommentLines(); }
<x_initBlkCom>\*\/            { yy_pop_state(yyscanner); }

<x_lrMtch>\\[^n]
<x_lrMtch>\[                  { yy_push_state(x_lrBrkt, yyscanner); }
<x_lrMtch>\"                  { yy_push_state(x_lrStr, yyscanner); }
<x_lrMtch>[^\[ \t\n]
<x_lrMtch>[ \t]               { yy_push_state(x_lrCode, yyscanner); }
<x_lrMtch>\n                  { yyextra->IncrementCodeLines();
                                yy_pop_state(yyscanner); }

<x_lrBrkt>\\[^\]]
<x_lrBrkt>\\\]
<x_lrBrkt>[^\]]
<x_lrBrkt>\]                  { yy_pop_state(yyscanner); }

<x_lrStr>\\\"
<x_lrStr>\"                   { yy_pop_state(yyscanner); }
<x_lrStr>[^\"]

<x_lrCode>^[ \t]*\n
<x_lrCode>[^*/\"'\n]+
<x_lrCode>"//"                { yy_push_state(x_comLine, yyscanner); }
<x_lrCode>"/*"                { yy_push_state(x_blkCom, yyscanner); }
<x_lrCode>\'\"\'
<x_lrCode>\"                  { yy_push_state(x_string, yyscanner); }
<x_lrCode>\'                  { yy_push_state(x_char, yyscanner); }
<x_lrCode>[*/]
<x_lrCode>\n                  { yy_pop_state(yyscanner);
                                unput('\n'); }

<x_code>^[ \t]*\n
<x_code>[^*/\"'\n]+
<x_code>"//"                  { yy_push_state(x_comLine, yyscanner); }
<x_code>"/*"                  { yy_push_state(x_blkCom, yyscanner); }
<x_code>\'\"\'
<x_code>\"                    { yy_push_state(x_string, yyscanner); }
<x_code>\'                    { yy_push_state(x_char, yyscanner); }
<x_code>[*/]
<x_code>\n                    { yyextra->IncrementCodeLines();
                                yy_pop_state(yyscanner); }

<x_comLine>.+
<x_comLine>\n                 { yy_pop_state(yyscanner);
                                unput('\n'); }
<x_blkCom>^[ \t]*\n
<x_blkCom>[^*/\n]+
<x_blkCom>"*/"                { yy_pop_state(yyscanner); }
<x_blkCom>.
<x_blkCom>\n                  { yyextra->IncrementCommentLines(); }

<x_string>\\\"
<x_string>\"                  { yy_pop_state(yyscanner); }
<x_string>[^"\\\n]+
<x_string>\\
<x_string>\n                  { yyextra->IncrementCodeLines(); }

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
    void FlexFileScanner::Scan(const std::filesystem::path & path,
                               SourceFileInfo & sfi) const
    {
      sfi.Counter().Reset();
      yyscan_t  scanner;
      flex_lex_init_extra(&(sfi.Counter()), &scanner);
      // flex_set_debug(1, scanner);
      FILE  *inFile = fopen(path.string().c_str(), "r");
      if (inFile) {
        flex_set_in(inFile, scanner);
        flex_lex(scanner);
        fclose(inFile);
      }
      flex_lex_destroy(scanner);
      return;
    }

  }  // namespace Mcloc

}  // namespace Dwm
