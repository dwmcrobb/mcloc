//===========================================================================
// @(#) $DwmPath: dwm/mcplex/mcloc/trunk/classes/include/DwmMclocEngNotate.hh 11828 $
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
//!  \file DwmMclocEngNotate.hh
//!  \author Daniel W. McRobb
//!  \brief Dwm::Mcloc::EngNotate class template
//---------------------------------------------------------------------------

#ifndef _DWMMCLOCENGNOTATE_HH_
#define _DWMMCLOCENGNOTATE_HH_

#include <array>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <utility>

namespace Dwm {

  namespace Mcloc {

    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    template <typename T>
    class EngNotate
    {
    public:
      EngNotate() = delete;
      
      EngNotate(T value, uint8_t precision, uint8_t width)
          : _value(value), _precision(precision), _width(width)
      { }

      friend std::ostream & 
      operator << (std::ostream & os, const EngNotate<T> & en)
      {
        std::streamsize  prevPrecision = os.precision();
        os.precision(en._precision);
        static constexpr std::array<std::pair<char,double>,17>
          k_multipliers = {
          {
            { 'Y', 1000000000000000000000000.0 },  // yotta
            { 'Z', 1000000000000000000000.0 },     // zeta
            { 'E', 1000000000000000000.0 },        // exa
            { 'P', 1000000000000000.0 },           // peta
            { 'T', 1000000000000.0 },              // tera
            { 'G', 1000000000.0 },                 // giga
            { 'M', 1000000.0 },                    // mega
            { 'K', 1000.0 },                       // kilo
            { ' ', 1.0 },
            { 'm', .001 },                         // milli
            { 'u', .000001 },                      // micro
            { 'n', .000000001 },                   // nano
            { 'p', .000000000001 },                // pico
            { 'f', .000000000000001 },             // femto
            { 'a', .000000000000000001 },          // atto
            { 'z', .000000000000000000001 },       // zepto
            { 'y', .000000000000000000000001 }     // yocto
          }
        };
        
        std::ostringstream  oss;
        oss.imbue(os.getloc());
        oss.precision(en._precision);
        
        auto  iter = std::find_if(std::begin(k_multipliers),
                                  std::end(k_multipliers),
                                  [&] (const std::pair<char,double> & km)
                                  { double  v = en._value / km.second;
                                    return ((v >= 1) || (v <= -1));
                                  });
        if (iter != std::end(k_multipliers)) {
          oss << std::setw(en._width) << (en._value / iter->second)
              << iter->first;
        }
        else {
          oss << std::setw(en._width) << en._value;
          if (en._value > 1 || en._value < 1)
            oss << " ";
        }
        os << oss.str();
        os.precision(prevPrecision);
        return os;
      }
      
    private:
      T        _value;
      uint8_t  _precision;
      uint8_t  _width;
    };
    
      
  }  // namespace Mcloc

}  // namespace Dwm

#endif  // _DWMMCLOCENGNOTATE_HH_
