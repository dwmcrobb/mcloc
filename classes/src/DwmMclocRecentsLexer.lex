%{
  //===========================================================================
  // @(#) $DwmPath: dwm/mcplex/mcloc/trunk/classes/src/DwmMclocRecentsLexer.lex 11829 $
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
  //!  \file DwmMclocRecentsLexer.lex
  //!  \author Daniel W. McRobb
  //!  \brief Recents file lexer
  //---------------------------------------------------------------------------

  #include <cstdio>
  #include <cstdlib>
  #include <string>
  #include <utility>
  
  using namespace std;
  
  #include "DwmMclocRecents.hh"
  #include "DwmMclocRecentsParser.hh"

  extern std::string  g_recentsPath;
  
  extern "C" {
    #include <stdarg.h>

    void mclocrecentserror(const char *arg, ...)
    {
      va_list  ap;
      va_start(ap, arg);
      vfprintf(stderr, arg, ap);
      fprintf(stderr, ": '%s' at line %d of %s\n",
              yytext, yylineno, g_recentsPath.c_str());
      return;
    }
  }
  
%}

%option noyywrap
%option prefix="mclocrecents"
%option yylineno

%x x_path
%x x_quotedPath
   
%%

<INITIAL>[0-9]+			     { BEGIN(x_path);
                           mclocrecentslval.timeVal =
                           strtoull(yytext, nullptr, 10);
                           return TIME; }
<INITIAL>.
<INITIAL>\n
<x_path>["]              { BEGIN(x_quotedPath); }
<x_path>.
<x_path>\n               { BEGIN(INITIAL); }
<x_quotedPath>[^"]+      { mclocrecentslval.stringVal =
                             new std::string(yytext);
                           return PATH; }
<x_quotedPath>["]        { BEGIN(x_path); }

%%
