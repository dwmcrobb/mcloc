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
  //!  \file DwmMclocCocomoConfigParser.y
  //!  \author Daniel W. McRobb
  //!  \brief Cocomo project config parser
  //---------------------------------------------------------------------------

  #include <cstdio>
  #include <iostream>
  #include <string>

  extern struct yy_buffer_state *
    mcloccocomocfg_scan_buffer(char *, size_t);
  extern struct yy_buffer_state *
    mcloccocomocfg_scan_string(const char *);
  
  extern void mcloccocomocfg_switch_to_buffer(struct yy_buffer_state *);
  extern void mcloccocomocfg_delete_buffer(struct yy_buffer_state *);
  extern void mcloccocomocfg_flush_buffer(struct yy_buffer_state *);
  extern void mcloccocomocfgrestart(FILE *);
  
  extern "C" {
    extern void mcloccocomocfgerror(const char *arg, ...);
    extern FILE *mcloccocomocfgin;
  }

  #include "DwmMclocCocomo1IntermediateConfig.hh"

  std::string                                 g_cocomoConfigPath;
  Dwm::Mcloc::Cocomo1::Intermediate::Config  *g_cocomoConfig = nullptr;
%}

%define api.prefix {mcloccocomocfg}

%union {
    Dwm::Mcloc::Cocomo1::Intermediate::ProjectClassEnum  classEnumVal;
    Dwm::Mcloc::Cocomo1::Intermediate::Rating            ratingVal;
}

%code provides
{
  // Tell Flex the expected prototype of yylex.
  #define YY_DECL                             \
    int mcloccocomocfglex ()

  // Declare the scanner.
  YY_DECL;
}

%type <classEnumVal>  ProjectClass Class
%type <ratingVal>     Rating
%type <ratingVal>     VeryLowToVeryHighRating
%type <ratingVal>     VeryLowToHighRating
%type <ratingVal>     LowToVeryHighRating
%type <ratingVal>     NominalToExtraHighRating

%token APPLICATIONS APPLICATION CONSTRAINTS DEVELOPMENT ENGINEERING
%token ENVIRONMENT PERFORMANCE PROGRAMMING RELIABILITY CAPABILITY
%token COMPLEXITY EXPERIENCE VOLATILITY EXTRAHIGH PERSONNEL TURNABOUT
%token DATABASE ENGINEER HARDWARE LANGUAGE REQUIRED SCHEDULE SOFTWARE
%token VERYHIGH NOMINAL PRODUCT PROJECT RUNTIME VERYLOW VIRTUAL
%token MACHINE ANALYST METHODS MEMORY CLASS TOOLS HIGH SIZE TIME LOW
%token USE THE OF ORGANIC SEMIDETACHED EMBEDDED

%%

Entries: Entry
{}
| Entries Entry
{};

Entry: ProjectClass
{
  g_cocomoConfig->ProjectClass($1);
}
| Product
{}
| Hardware
{}
| Personnel
{}
| Project
{};

ProjectClass: PROJECT CLASS ':' Class ';'
{
  $$ = $4;
};

Class: ORGANIC
{
  $$ = Dwm::Mcloc::Cocomo1::Intermediate::e_organic;
}
| SEMIDETACHED
{
  $$ = Dwm::Mcloc::Cocomo1::Intermediate::e_semiDetached;
}
| EMBEDDED
{
  $$ = Dwm::Mcloc::Cocomo1::Intermediate::e_embedded;
};

Product: PRODUCT '{' ProductValues '}'
{};

ProductValues: ProductValue
| ProductValues ProductValue
{};

ProductValue: REQUIRED SOFTWARE RELIABILITY ':' VeryLowToVeryHighRating ';'
{
  g_cocomoConfig->Product().RequiredReliability($5);
}
| SIZE OF APPLICATION DATABASE ':' LowToVeryHighRating ';'
{
  g_cocomoConfig->Product().SizeOfApplicationDatabase($6);
}
| COMPLEXITY OF THE PRODUCT ':' Rating ';'
{
  g_cocomoConfig->Product().ComplexityOfProduct($6);
};

Hardware: HARDWARE '{' HardwareValues '}'
{};

HardwareValues: HardwareValue
{}
| HardwareValues HardwareValue
{};

HardwareValue: RUNTIME PERFORMANCE CONSTRAINTS ':' NominalToExtraHighRating ';'
{
  g_cocomoConfig->Hardware().RuntimePerformanceConstraints($5);
}
| MEMORY CONSTRAINTS ':' NominalToExtraHighRating ';'
{
  g_cocomoConfig->Hardware().MemoryContraints($4);
}
| VOLATILITY OF THE VIRTUAL MACHINE ENVIRONMENT ':' LowToVeryHighRating ';'
{
  g_cocomoConfig->Hardware().VolatilityOfVMEnvironment($8);
}
| REQUIRED TURNABOUT TIME ':' LowToVeryHighRating ';'
{
  g_cocomoConfig->Hardware().RequiredTurnaboutTime($5);
};

Personnel: PERSONNEL '{' PersonnelValues '}'
{};

PersonnelValues: PersonnelValue
{}
| PersonnelValues PersonnelValue
{};

PersonnelValue: ANALYST CAPABILITY ':' VeryLowToVeryHighRating ';'
{
  g_cocomoConfig->Personnel().AnalystCapability($4);
}
| APPLICATIONS EXPERIENCE ':' VeryLowToVeryHighRating ';'
{
  g_cocomoConfig->Personnel().ApplicationsExperience($4);
}
| SOFTWARE ENGINEER CAPABILITY ':' VeryLowToVeryHighRating ';'
{
  g_cocomoConfig->Personnel().SoftwareEngineerCapability($5);
}
| VIRTUAL MACHINE EXPERIENCE ':' VeryLowToHighRating ';'
{
  g_cocomoConfig->Personnel().VirtualMachineExperience($5);
}
| PROGRAMMING LANGUAGE EXPERIENCE ':' VeryLowToHighRating ';'
{
  g_cocomoConfig->Personnel().ProgrammingLanguageExperience($5);
};

Project: PROJECT '{' ProjectValues '}'
{};

ProjectValues: ProjectValue
{}
| ProjectValues ProjectValue
{};

ProjectValue: APPLICATION OF SOFTWARE ENGINEERING METHODS ':' VeryLowToVeryHighRating ';'
{
  g_cocomoConfig->Project().ApplicationOfSWEngineeringMethods($7);
}
| USE OF SOFTWARE TOOLS ':' VeryLowToVeryHighRating ';'
{
  g_cocomoConfig->Project().UseOfSoftwareTools($6);
}
| REQUIRED DEVELOPMENT SCHEDULE ':' VeryLowToVeryHighRating ';'
{
  g_cocomoConfig->Project().RequiredDevelopmentSchedule($5);
};

Rating: VERYLOW
{
  $$ = Dwm::Mcloc::Cocomo1::Intermediate::e_veryLow;
}
| LOW
{
  $$ = Dwm::Mcloc::Cocomo1::Intermediate::e_low;
}
| NOMINAL
{
  $$ = Dwm::Mcloc::Cocomo1::Intermediate::e_nominal;
}
| HIGH
{
  $$ = Dwm::Mcloc::Cocomo1::Intermediate::e_high;
}
| VERYHIGH
{
  $$ = Dwm::Mcloc::Cocomo1::Intermediate::e_veryHigh;
}
| EXTRAHIGH
{
  $$ = Dwm::Mcloc::Cocomo1::Intermediate::e_extraHigh;
};

VeryLowToVeryHighRating: VERYLOW
{
  $$ = Dwm::Mcloc::Cocomo1::Intermediate::e_veryLow;
}
| LOW
{
  $$ = Dwm::Mcloc::Cocomo1::Intermediate::e_low;
}
| NOMINAL
{
  $$ = Dwm::Mcloc::Cocomo1::Intermediate::e_nominal;
}
| HIGH
{
  $$ = Dwm::Mcloc::Cocomo1::Intermediate::e_high;
}
| VERYHIGH
{
  $$ = Dwm::Mcloc::Cocomo1::Intermediate::e_veryHigh;
};

VeryLowToHighRating: VERYLOW
{
  $$ = Dwm::Mcloc::Cocomo1::Intermediate::e_veryLow;
}
| LOW
{
  $$ = Dwm::Mcloc::Cocomo1::Intermediate::e_low;
}
| NOMINAL
{
  $$ = Dwm::Mcloc::Cocomo1::Intermediate::e_nominal;
}
| HIGH
{
  $$ = Dwm::Mcloc::Cocomo1::Intermediate::e_high;
};

LowToVeryHighRating: LOW
{
  $$ = Dwm::Mcloc::Cocomo1::Intermediate::e_low;
}
| NOMINAL
{
  $$ = Dwm::Mcloc::Cocomo1::Intermediate::e_nominal;
}
| HIGH
{
  $$ = Dwm::Mcloc::Cocomo1::Intermediate::e_high;
}
| VERYHIGH
{
  $$ = Dwm::Mcloc::Cocomo1::Intermediate::e_veryHigh;
};

NominalToExtraHighRating: NOMINAL
{
  $$ = Dwm::Mcloc::Cocomo1::Intermediate::e_nominal;
}
| HIGH
{
  $$ = Dwm::Mcloc::Cocomo1::Intermediate::e_high;
}
| VERYHIGH
{
  $$ = Dwm::Mcloc::Cocomo1::Intermediate::e_veryHigh;
}
| EXTRAHIGH
{
  $$ = Dwm::Mcloc::Cocomo1::Intermediate::e_extraHigh;
};

%%

namespace Dwm {

  namespace Mcloc {

    namespace Cocomo1 {

      namespace Intermediate {

        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        bool Config::Parse(const std::string & path)
        {
          bool  rc = false;
          g_cocomoConfigPath = path;
          mcloccocomocfgin = fopen(path.c_str(), "r");
          if (mcloccocomocfgin) {
            mcloccocomocfgrestart(mcloccocomocfgin);
            g_cocomoConfig = this;
            rc = (0 == mcloccocomocfgparse());
            fclose(mcloccocomocfgin);
          }
          else {
            std::cerr << "Failed to open cocomo config file '"
                      << path << "'\n";
          }
          return rc;
        }

        extern struct yy_buffer_state *ScanConfigString(const char *);
        extern void DeleteScanBuffer(struct yy_buffer_state *);
        
        //--------------------------------------------------------------------
        //!  
        //--------------------------------------------------------------------
        bool Config::Parse(std::istream & is)
        {
          bool  rc = false;
          std::string  confstr((std::istreambuf_iterator<char>(is)),
                               std::istreambuf_iterator<char>());
          struct yy_buffer_state  *bufferState =
            ScanConfigString(confstr.c_str());
          g_cocomoConfig = this;
          rc = (0 == mcloccocomocfgparse());
          DeleteScanBuffer(bufferState);
          return rc;
        }
        
            
      }  // namespace Intermediate

    }  // namespace Cocomo1

  }  // namespace Mcloc

}  // namespace Dwm

