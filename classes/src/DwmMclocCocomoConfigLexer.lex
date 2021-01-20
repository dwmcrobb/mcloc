%{
  //===========================================================================
  // @(#) $DwmPath$
  //===========================================================================
  //  Copyright (c) Daniel W. McRobb 2021
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
  //!  \file DwmMclocCocomoConfigLexer.lex
  //!  \author Daniel W. McRobb
  //!  \brief Dwm::Mcloc::Cocomo::Config lexer
  //---------------------------------------------------------------------------

  #include "DwmMclocCocomo1IntermediateConfig.hh"
  #include "DwmMclocCocomoConfigParser.hh"

  extern std::string  g_cocomoConfigPath;

  extern struct yy_buffer_state *mcloccocomocfg_scan_buffer(char *, size_t);
  
  extern "C" {
    #include <stdarg.h>

    void mcloccocomocfgerror(const char *arg, ...)
    {
      va_list  ap;
      va_start(ap, arg);
      vfprintf(stderr, arg, ap);
      fprintf(stderr, ": '%s' at line %d of %s\n",
              yytext, yylineno, g_cocomoConfigPath.c_str());
      return;
    }
  }

%}

%option noyywrap full yylineno
%option prefix="mcloccocomocfg"
%option outfile="DwmMclocCocomoConfigLexer.cc"

%x x_error

%%

<INITIAL>#.*
<INITIAL>[Ss]emi\-[Dd]etached   { return SEMIDETACHED; }
<INITIAL>[Aa]pplications        { return APPLICATIONS; }
<INITIAL>[Aa]pplication         { return APPLICATION; }
<INITIAL>[Cc]onstraints         { return CONSTRAINTS; }
<INITIAL>[Dd]evelopment         { return DEVELOPMENT; }
<INITIAL>[Ee]ngineering         { return ENGINEERING; }
<INITIAL>[Ee]nvironment         { return ENVIRONMENT; }
<INITIAL>[Pp]erformance         { return PERFORMANCE; }
<INITIAL>[Pp]rogramming         { return PROGRAMMING; }
<INITIAL>[Rr]eliability         { return RELIABILITY; }
<INITIAL>[Cc]apability          { return CAPABILITY; }
<INITIAL>[Cc]omplexity          { return COMPLEXITY; }
<INITIAL>[Ee]xperience          { return EXPERIENCE; }
<INITIAL>[Vv]olatility          { return VOLATILITY; }
<INITIAL>[Ee]xtra[Hh]igh        { return EXTRAHIGH; }
<INITIAL>[Pp]ersonnel           { return PERSONNEL; }
<INITIAL>[Tt]urnabout           { return TURNABOUT; }
<INITIAL>[Dd]atabase            { return DATABASE; }
<INITIAL>[Ee]ngineer            { return ENGINEER; }
<INITIAL>[Ee]mbedded            { return EMBEDDED; }
<INITIAL>[Hh]ardware            { return HARDWARE; }
<INITIAL>[Ll]anguage            { return LANGUAGE; }
<INITIAL>[Rr]equired            { return REQUIRED; }
<INITIAL>[Ss]chedule            { return SCHEDULE; }
<INITIAL>[Ss]oftware            { return SOFTWARE; }
<INITIAL>[Vv]ery[Hh]igh         { return VERYHIGH; }
<INITIAL>[Nn]ominal             { return NOMINAL; }
<INITIAL>[Oo]rganic             { return ORGANIC; }
<INITIAL>[Pp]roduct             { return PRODUCT; }
<INITIAL>[Pp]roject             { return PROJECT; }
<INITIAL>[Rr]untime             { return RUNTIME; }
<INITIAL>[Vv]ery[Ll]ow          { return VERYLOW; }
<INITIAL>[Vv]irtual             { return VIRTUAL; }
<INITIAL>[Mm]achine             { return MACHINE; }
<INITIAL>[Aa]nalyst             { return ANALYST; }
<INITIAL>[Mm]ethods             { return METHODS; }
<INITIAL>[Mm]emory              { return MEMORY; }
<INITIAL>[Cc]lass               { return CLASS; }
<INITIAL>[Tt]ools               { return TOOLS; }
<INITIAL>[Hh]igh                { return HIGH; }
<INITIAL>[Ss]ize                { return SIZE; }
<INITIAL>[Tt]ime                { return TIME; }
<INITIAL>[Ll]ow                 { return LOW; }
<INITIAL>[Uu]se                 { return USE; }
<INITIAL>[Tt]he                 { return THE; }
<INITIAL>[Oo]f                  { return OF; }
<INITIAL>\{                     { return '{'; }
<INITIAL>\}                     { return '}'; }
<INITIAL>\;                     { return ';'; }
<INITIAL>\:                     { return ':'; }
<INITIAL>[ \t]
<INITIAL>[^{};: \t\n]           { unput(yytext[0]); BEGIN(x_error); }
<INITIAL>\n
<x_error>[^ \t\n]+              { mcloccocomocfgerror("invalid text");
                                  exit(1); }
<x_error>[\t\n]                 { mcloccocomocfgerror("invalid text");
                                  exit(1); }
<<EOF>>                         { return 0; }

%%

namespace Dwm {

  namespace Mcloc {

    namespace Cocomo1 {

      namespace Intermediate {

        struct yy_buffer_state *ScanConfigString(const char *str)
        {
          yy_buffer_state  *buf = yy_scan_string(str);
          yy_switch_to_buffer(buf);
          return buf;
        }

        void DeleteScanBuffer(struct yy_buffer_state *buf)
        {
          yy_delete_buffer(YY_CURRENT_BUFFER);
          return;
        }
        
        
      }  // namespace Intermediate

    }  // namespace Cocomo1

  }  // namespace Mcloc

}  // namespace Dwm
