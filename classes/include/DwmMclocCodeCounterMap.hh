//===========================================================================
// @(#) $DwmPath: dwm/mcplex/mcloc/trunk/classes/include/DwmMclocCodeCounterMap.hh 11828 $
//===========================================================================
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
//===========================================================================

//---------------------------------------------------------------------------
//!  \file DwmMclocCodeCounterMap.hh
//!  \brief Dwm::Mcloc::CodeCounterMap class template
//---------------------------------------------------------------------------

#ifndef _DWMMCLOCCODECOUNTERMAP_HH_
#define _DWMMCLOCCODECOUNTERMAP_HH_

#include <algorithm>
#include <functional>
#include <iostream>
#include <mutex>
#include <sstream>
#include <unordered_map>

#include "DwmMclocCodeCounter.hh"
#include "DwmMclocHowToSort.hh"
#include "DwmMclocTerminalTricks.hh"

namespace Dwm {

  namespace Mcloc {

    //------------------------------------------------------------------------
    //!  Utility class template to encapsulate an unordered_map of CodeCounter
    //!  objects keyed by the type @c T.  Since I'm using @c unordered_map,
    //!  @c T must be hashable.
    //------------------------------------------------------------------------
    template <typename T>
    class CodeCounterMap
    {
    public:
      //----------------------------------------------------------------------
      //!  Default constructor.
      //----------------------------------------------------------------------
      CodeCounterMap()
          : _mtx(), _counters()
      {}

      //----------------------------------------------------------------------
      //!  Increments the CodeCounter for the given @c key.
      //----------------------------------------------------------------------
      void Increment(const T & key, const CodeCounter & counter)
      {
        std::lock_guard<std::mutex>  lck(_mtx);
        _counters[key].Increment(counter.CodeLines(), counter.CommentLines());
        return;
      }
      
      //----------------------------------------------------------------------
      //!  Clears the contents.
      //----------------------------------------------------------------------
      void Clear()
      {
        std::lock_guard<std::mutex>  lck(_mtx);
        _counters.clear();
        return;
      }

      //----------------------------------------------------------------------
      //!  Populates @c output from the contents of @c *this, remapping
      //!  the keys via @c remapFn.  @c remapFn must accept a key of type
      //!  T as it's sole argument, adn return an instance of type U (the
      //!  key type for the output map).
      //----------------------------------------------------------------------
      template <class U>
      void Transform(std::function<U(const T &)> remapFn,
                     CodeCounterMap<U> & output) const
      {
        output.Clear();
        std::lock_guard<std::mutex>  lck(_mtx);
        for (const auto & entry : _counters) {
          output.Increment(remapFn(entry.first), entry.second);
        }
        return;
      }

      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      void Transform(std::vector<std::pair<std::string,CodeCounter>> & v)
      {
        v.clear();
        std::lock_guard<std::mutex>  lck(_mtx);
        for (const auto & entry : _counters) {
          v.push_back(entry);
        }
        return;
      }
      
      //----------------------------------------------------------------------
      //!  Prints the contents to an ostream and returns the ostream.
      //!  @c howToSort determines how entries are sorted, and @c prefix
      //!  is printed in the first column of each row.
      //----------------------------------------------------------------------
      std::ostream & Print(std::ostream & os, HowToSort howToSort,
                           TerminalTricks & tt,
                           const std::string & prefix = "")
      {
        //  Declare temporary vector to store pointers to our internal data.
        std::vector<const std::pair<const T,CodeCounter> *>  counters;

        //  Populate the vector of pointers to our data so we can sort.
        //  In nested scope because we only need a lock while copying.
        {
          std::lock_guard<std::mutex>  lck(_mtx);
          for (const auto & c : _counters) {
            counters.push_back(&c);
          }
        }

        //  Sort our temporary vector.
        switch (howToSort) {
          case sortBySize:
            std::sort(counters.begin(), counters.end(), _orderBySize);
            break;
          case sortBySizeReversed:
            std::sort(counters.begin(), counters.end(), _orderBySizeReverse);
            break;
          default:
            std::sort(counters.begin(), counters.end(), _orderByName);
            break;
        }

        if (! counters.empty()) {
          //  Print our temporary vector.
          size_t  i = 0;
          for ( ; i < counters.size() - 1; ++i) {
            const auto & entry = counters[i];
            os << prefix << ' ' << entry->second << ' '
               << entry->first << '\n';
          }
          const auto & entry = counters[i];
          std::ostringstream  oss;
          oss.imbue(std::locale(""));
          oss << entry->second << ' ' << entry->first;
          std::string  s(oss.str());
          std::string  trailer(tt.Columns() - (s.size() + 3), ' ');
          os << prefix << ' ' << tt.Underscore(s + trailer) << '\n';
        }
        
        return os;
      }
      
    private:
      mutable std::mutex                 _mtx;
      std::unordered_map<T,CodeCounter>  _counters;

      //  order-by-ascending-size lambda for std::sort()
      static constexpr auto  _orderBySize =
          [] (const std::pair<const T,CodeCounter> * e1,
              const std::pair<const T,CodeCounter> * e2)
          { return (e1->second < e2->second); };

      //  order-by-descending-size lambda for std::sort()
      static constexpr auto  _orderBySizeReverse =
          [] (const std::pair<const T,CodeCounter> * e1,
              const std::pair<const T,CodeCounter> * e2)
          { return (e1->second > e2->second); };

      //  order-by-name lambda for std::sort()
      static constexpr auto  _orderByName =
          [] (const std::pair<const T,CodeCounter> * e1,
              const std::pair<const T,CodeCounter> * e2)
          { return (e1->first < e2->first); };
    };
    
  }  // namespace Mcloc

}  // namespace Dwm

#endif  // _DWMMCLOCCODECOUNTERMAP_HH_
