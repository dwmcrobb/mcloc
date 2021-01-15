%{
  //=========================================================================
  // @(#) $DwmPath: dwm/mcplex/mcloc/trunk/classes/src/DwmMclocConfigLex.lex 11829 $
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
  //!  \file DwmMclocConfigLex.lex
  //!  \brief mcloc configuration file lexer implementation
  //-------------------------------------------------------------------------

  #include <cstdio>
  #include <map>
  #include <regex>
  #include <set>
  #include <string>

  using namespace std;

  #include "DwmMclocConfig.hh"
  #include "DwmMclocConfigParse.hh"

  extern std::string  g_configPath;

  extern "C" {
    #include <stdarg.h>

    void mcloccfgerror(const char *arg, ...)
    {
      va_list  ap;
      va_start(ap, arg);
      vfprintf(stderr, arg, ap);
      fprintf(stderr, ": '%s' at line %d of %s\n",
              yytext, yylineno, g_configPath.c_str());
      return;
    }
  }

  static const std::string svnid("@(#) $DwmPath: dwm/mcplex/mcloc/trunk/classes/src/DwmMclocConfigLex.lex 11829 $");

%}

%option noyywrap full yylineno
%option prefix="mcloccfg"

%x x_quotedString

%%

<INITIAL>Scanners                { return SCANNERS;    }
<INITIAL>Languages               { return LANGUAGES;   }
<INITIAL>Extensions              { return EXTENSIONS;  }
<INITIAL>Expressions             { return EXPRESSIONS; }
<INITIAL>^[ \t]*#.*\n            { mcloccfglval.stringVal =
                                     new std::string(yytext);
                                   return COMMENTLINE; }
<INITIAL>#.*
<INITIAL>\"                      { BEGIN(x_quotedString); return '"'; }
<INITIAL>\{                      { return '{'; }
<INITIAL>\}                      { return '}'; }
<INITIAL>\,                      { return ','; }
<INITIAL>[ \t\n]
<INITIAL>[^"{},# \t\n]+          { mcloccfgerror("invalid text in config");
                                   exit(1); }

<x_quotedString>\\\"             { return ESCAPEDQUOTE; }
<x_quotedString>[^\"]+           { mcloccfglval.stringVal = 
                                     new std::string(yytext);
                                   return QUOTEDSTR; }
<x_quotedString>\"               { BEGIN(INITIAL); return '"'; }

%%

