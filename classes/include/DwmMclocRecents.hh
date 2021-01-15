//===========================================================================
// @(#) $DwmPath: dwm/mcplex/mcloc/trunk/classes/include/DwmMclocRecents.hh 11828 $
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
//!  \file DwmMclocRecents.hh
//!  \author Daniel W. McRobb
//!  \brief Dwm::Mcloc::Recents class declaration
//---------------------------------------------------------------------------

#ifndef _DWMMCLOCRECENTS_HH_
#define _DWMMCLOCRECENTS_HH_

#include <ctime>
#include <deque>
#include <string>
#include <utility>

namespace Dwm {

  namespace Mcloc {

    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    class Recents
    {
    public:
      Recents();

      const std::deque<std::pair<time_t,std::string>> & Paths() const
      { return _paths; }

      void Add(const std::string & path);
      void Add(const std::pair<time_t,std::string> & timeAndPath);
      void Cull(size_t numToKeep = 8);

    private:
      // std::string                                _recentsFile;
      std::deque<std::pair<time_t,std::string>>  _paths;

      static bool EnsureRecentsDirExists();
      static std::string RecentsDir();
      void Save();
      bool Parse(const std::string & recentsFile);
    };
    
  }  // namespace Mcloc

}  // namespace Dwm

#endif  // _DWMMCLOCRECENTS_HH_
