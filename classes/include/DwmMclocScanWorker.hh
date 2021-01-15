//===========================================================================
// @(#) $DwmPath: dwm/mcplex/mcloc/trunk/classes/include/DwmMclocScanWorker.hh 11828 $
//===========================================================================
//  Copyright (c) Daniel W. McRobb 2019
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
//!  \file DwmMclocScanWorker.hh
//!  \brief Dwm::Mcloc::ScanWorker class declaration
//---------------------------------------------------------------------------

#ifndef _DWMMCLOCSCANWORKER_HH_
#define _DWMMCLOCSCANWORKER_HH_

#include <filesystem>
#include <memory>
#include <thread>
#include <utility>

#include "DwmMclocThreadQueue.hh"
#include "DwmMclocSourceFileInfo.hh"

namespace Dwm {

  namespace Mcloc {

    //------------------------------------------------------------------------
    //!  Encapsulates a thread to parse source files from a work queue.
    //------------------------------------------------------------------------
    class ScanWorker
    {
    public:
      typedef Dwm::Mcloc::Thread::Queue<std::pair<std::filesystem::path,
                                                  SourceFileInfo *>>  Queue_t;
      
      //----------------------------------------------------------------------
      //!  Construct from a reference to the work queue.
      //----------------------------------------------------------------------
      ScanWorker(Queue_t & queue);

      //----------------------------------------------------------------------
      //!  Start the worker.
      //----------------------------------------------------------------------
      bool Start();

      //----------------------------------------------------------------------
      //!  Wait for thr worker to finish.
      //----------------------------------------------------------------------
      void Finish();
      
    private:
      Queue_t      &_queue;
      std::thread   _thread;
      
      void Run();
    };
    
  }  // namespace Mcloc

}  // namespace Dwm

#endif  // _DWMMCLOCSCANWORKER_HH_
