%{
  //=========================================================================
  // @(#) $DwmPath: dwm/mcplex/mcloc/trunk/classes/src/DwmMclocConfigParse.y 11835 $
  //=========================================================================
  //  Copyright (c) Daniel W. McRobb 2020, 2021
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
  //!  \file DwmMclocConfigParse.y
  //!  \brief Dwm::Mcloc::Config::Config::Parse parser implementation
  //-------------------------------------------------------------------------

  #include <cstdio>
  #include <iostream>
  #include <map>
  #include <regex>
  #include <set>
  #include <string>

  extern "C" {
    extern void mcloccfgerror(const char *arg, ...);
    extern FILE *mcloccfgin;
  }

  #include "DwmMclocConfig.hh"

  using namespace std;

  std::string                  g_configPath;
  Dwm::Mcloc::Config::Config  *g_config = nullptr;
%}

%define api.prefix {mcloccfg}

%union {
    const std::string                     *stringVal;
    Dwm::Mcloc::Config::Config            *configVal;
    vector<Dwm::Mcloc::Config::Scanner>   *scannersVal;
    Dwm::Mcloc::Config::Scanner           *scannerVal;
    vector<Dwm::Mcloc::Config::Language>  *languagesVal;
    Dwm::Mcloc::Config::Language          *languageVal;
    set<string>                           *extensionsVal;
    map<string,regex>                     *expressionsVal;
    vector<string>                        *commentsVal;
}

%code provides
{
  // Tell Flex the expected prototype of yylex.
  #define YY_DECL                             \
    int mcloccfglex ()

  // Declare the scanner.
  YY_DECL;
}

%token ESCAPEDQUOTE EXPRESSIONS EXTENSIONS LANGUAGES SCANNERS

%token <stringVal>     COMMENTLINE QUOTEDSTR

%type <commentsVal>    Comments
%type <stringVal>      Expression ExpressionPart ExpressionString
%type <expressionsVal> Expressions ExpressionsClause
%type <stringVal>      Extension
%type <extensionsVal>  Extensions ExtensionsClause
%type <languageVal>    CommentedLanguage Language
%type <languagesVal>   Languages LanguagesClause
%type <configVal>      Config
%type <scannerVal>     CommentedScanner Scanner
%type <scannersVal>    Scanners ScannersClause

%right "notcommented" "commented"

%%

Config: CommentedScannersClause
{};

CommentedScannersClause: Comments ScannersClause %prec "commented"
{
    if (g_config) {
        g_config->Comments(*$1);
        g_config->Scanners(*$2);
    }
    delete $1;
    delete $2;
}
| ScannersClause %prec "uncommented"
{
    if (g_config) {
        g_config->Scanners(*$1);
    }
    delete $1;
};

ScannersClause: SCANNERS '{' Scanners '}'
{
    $$ = $3;
};

Scanners: CommentedScanner
{
    $$ = new vector<Dwm::Mcloc::Config::Scanner>();
    $$->push_back(*$1);
    delete $1;
}
| Scanners CommentedScanner
{
    if (std::find_if($$->begin(), $$->end(),
                     [&] (const Dwm::Mcloc::Config::Scanner & scanner) 
                     { return (scanner.Name() == $2->Name()); })
        == $$->end()) {
        $$->push_back(*$2);
    }
    delete $2;
};

CommentedScanner: Comments Scanner %prec "commented"
{
    $$ = $2;
    $$->Comments(*$1);
    delete $1;
}
| Scanner %prec "uncommented"
{
    $$ = $1;
};

Scanner: '"' QUOTEDSTR '"' '{' LanguagesClause '}'
{
    $$ = new Dwm::Mcloc::Config::Scanner(*$2, *$5);
    delete $2;
    delete $5;
};

LanguagesClause: LANGUAGES '{' Languages '}'
{
    $$ = $3;
};

Languages: CommentedLanguage
{
    $$ = new vector<Dwm::Mcloc::Config::Language>();
    $$->push_back(*$1);
    delete $1;
}
| Languages CommentedLanguage
{
    if (std::find_if($$->begin(), $$->end(),
                     [&] (const Dwm::Mcloc::Config::Language & lang) 
                     { return (lang.Name() == $2->Name()); }) == $$->end()) {
        $$->push_back(*$2);
    }
    delete $2;
};

CommentedLanguage: Comments Language %prec "commented"
{
    $$ = $2;
    $$->Comments(*$1);
    delete $1;
}
| Language %prec "uncommented"
{
    $$ = $1;
};

Language: '"' QUOTEDSTR '"' '{' ExtensionsClause ExpressionsClause '}'
{
    $$ = new Dwm::Mcloc::Config::Language(*$2, *$5, *$6);
    delete $2;
    delete $5;
    delete $6;
}
| '"' QUOTEDSTR '"' '{' ExpressionsClause ExtensionsClause '}'
{
    $$ = new Dwm::Mcloc::Config::Language(*$2, *$6, *$5);
    delete $2;
    delete $5;
    delete $6;
}
| '"' QUOTEDSTR '"' '{' ExpressionsClause '}'
{
    std::set<std::string>  emptyExtensions;
    $$ = new Dwm::Mcloc::Config::Language(*$2, emptyExtensions, *$5);
    delete $2;
    delete $5;
}
| '"' QUOTEDSTR '"' '{' ExtensionsClause '}'
{
    std::map<std::string,std::regex>  emptyExpressions;
    $$ = new Dwm::Mcloc::Config::Language(*$2, *$5, emptyExpressions);
    delete $2;
    delete $5;
};

ExtensionsClause: EXTENSIONS '{' Extensions '}'
{
    $$ = $3;
};

Extensions: Extension
{
    $$ = new std::set<std::string>();
    $$->insert(*$1);
    delete $1;
}
| Extensions ',' Extension
{
    $$->insert(*$3);
    delete $3;
};

Extension: '"' QUOTEDSTR '"'
{
    $$ = $2;
};

ExpressionsClause: EXPRESSIONS '{' Expressions '}'
{
    $$ = $3;
};

Expressions: Expression
{
    $$ = new std::map<std::string,std::regex>();
    (*$$)[*($1)] =
        std::regex(*$1, std::regex::ECMAScript|std::regex::optimize);
    delete $1;
}
| Expressions ',' Expression
{
    (*$$)[*$3] = std::regex(*$3, regex::ECMAScript|regex::optimize);
    delete $3;
};

Expression: '"' ExpressionString '"'
{
    $$ = $2;
};

ExpressionString: ExpressionPart
{
    $$ = $1;
}
| ExpressionString ExpressionPart
{
    $$ = new std::string(*$1 + *$2);
    delete $1;
    delete $2;
};

ExpressionPart: QUOTEDSTR
{
    $$ = $1;
}
| ESCAPEDQUOTE
{
    $$ = new std::string("\"");
};

Comments: Comments COMMENTLINE  %prec "commented"
{
    $$ = $1;
    $$->push_back(*$2);
    delete $2;
}
| COMMENTLINE %prec "notcommented"
{
    $$ = new std::vector<std::string>({ *$1 });
    delete $1;
};

%%

static const std::string svnid("@(#) $DwmPath: dwm/mcplex/mcloc/trunk/classes/src/DwmMclocConfigParse.y 11835 $");

namespace Dwm {

  namespace Mcloc {

    namespace Config {

      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      bool Config::Parse(const std::string & path)
      {
        bool  rc = false;
        g_configPath = path;
        mcloccfgin = fopen(path.c_str(), "r");
        if (mcloccfgin) {
          g_config = this;
          rc = (0 == mcloccfgparse());
          fclose(mcloccfgin);
        }
        else {
            std::cerr << "Failed to open config file '" << path << "'\n";
        }
        return rc;
      }

    }  // namespace Config

  }  // namespace Mcloc

}  // namespace Dwm
