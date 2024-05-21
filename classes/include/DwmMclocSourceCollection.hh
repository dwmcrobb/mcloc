//===========================================================================
// @(#) $DwmPath: dwm/mcplex/mcloc/trunk/classes/include/DwmMclocSourceCollection.hh 11828 $
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
//!  \file DwmMclocSourceCollection.hh
//!  \brief Dwm::Mcloc::SourceCollection class declaration
//---------------------------------------------------------------------------

#ifndef _DWMMCLOCSOURCECOLLECTION_HH_
#define _DWMMCLOCSOURCECOLLECTION_HH_

extern "C" {
  #include <sys/resource.h>
}

#include <chrono>
#include <filesystem>
#include <iostream>
#include <map>
#include <memory>
#include <unordered_map>
#include <vector>

#include "DwmMclocHowToSort.hh"
#include "DwmMclocSourceFileInfo.hh"
#include "DwmMclocCodeCounterMap.hh"
#include "DwmMclocScanWorker.hh"

namespace Dwm {

  namespace Mcloc {

    class SourceCollection;
    
    //------------------------------------------------------------------------
    //!  Encapsulates source files from a single directory.  We only keep a
    //!  container of pairs of (filename, info) in this class, and only the
    //!  files in a single directory, NOT including its subdirectories.  And
    //!  since we don't store the full path of the files, this class is only
    //!  useful to us in the context of a containing directory.  i.e. we
    //!  don't use it standalone, but instead as a value in a
    //!  SourceCollection.
    //------------------------------------------------------------------------
    class SourceDirFiles
    {
    public:
      typedef std::unordered_map<std::string,SourceFileInfo> SourceFilesMap;

      //----------------------------------------------------------------------
      //!  Default constructor.
      //----------------------------------------------------------------------
      SourceDirFiles() = default;
      
      //----------------------------------------------------------------------
      //!  Add the file at the given path @c fspath.
      //----------------------------------------------------------------------
      bool AddFile(const std::filesystem::path & fspath);

      //----------------------------------------------------------------------
      //!  Returns a const reference to the contained files.  Note that the
      //!  paths in the entries are only filenames, not full paths.  This is
      //!  mainly in the interest of conserving memory, and having full paths
      //!  in the file entries would just duplicate information from the
      //!  containing directory.
      //----------------------------------------------------------------------
      const SourceFilesMap & Files() const  { return _files; }

      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      void GetTotalsByExtension(CodeCounterMap<std::string> & byExt,
                                bool includeGenerated = false) const;

      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      void GetTotalsByLanguage(CodeCounterMap<std::string> & byLanguage,
                               bool includeGenerated = false) const;

      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      void GetTotalsByScanner(CodeCounterMap<std::string> & byScanner,
                              bool includeGenerated = false) const;

      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      void GetTotal(CodeCounter & total, bool includeGenerated = false) const;
      
      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      std::ostream & Print(std::ostream & os, bool showGenerated,
                           HowToSort howToSort, bool isLastDir);

      friend class SourceCollection;
      
    private:
      SourceFilesMap  _files;

      void QueueForWork(const std::filesystem::path & dirPath,
                        ScanWorker::Queue_t & queue);

      typedef std::vector<const std::pair<const std::string,
                                          SourceFileInfo> *>  PointerVector;
      
      bool CopyToVector(PointerVector & v) const;
      
      void SortBySize(PointerVector & v, HowToSort howToSort);
      
      void SortByName(PointerVector & v);

      PointerVector::const_iterator
      LastPrintable(const PointerVector & fileVec, bool showGenerated) const;
      
      std::ostream & PrintEntry(PointerVector::value_type f,
                                std::ostream & os, bool showGenerated);
      
      std::ostream & PrintLastEntry(PointerVector::value_type f,
                                    std::ostream & os, bool showGenerated);
    };

    //------------------------------------------------------------------------
    //!  Encapsulates a collection of source files, organized by directory.
    //!  Note that the collection is not a single-rooted filesystem tree;
    //!  we want to be able to handle disconnected heirarchies in the same
    //!  collection, and a deep tree requires more time to process.  One
    //!  of the objectives of mcloc is reasonably speedy processing.
    //------------------------------------------------------------------------
    class SourceCollection
    {
    public:
      //----------------------------------------------------------------------
      //!  Default constructor.
      //----------------------------------------------------------------------
      SourceCollection() = default;
      
      //----------------------------------------------------------------------
      //!  Adds the file at the given path @c fspath to the collection.
      //----------------------------------------------------------------------
      void AddFile(const std::filesystem::path & fspath);

      //----------------------------------------------------------------------
      //!  Adds all source files in the directory at the given path @c fspath
      //!  to the collection.  Recursively adds all source files in all
      //!  subdirectories of @c fspath if @c recurse is @c true.
      //----------------------------------------------------------------------
      void AddDirectory(const std::filesystem::path & fspath,
                        bool recurse = true);

      //----------------------------------------------------------------------
      //!  Parse all of the contained source files and populate their
      //!  code counters.
      //----------------------------------------------------------------------
      void Parse(int numThreads = 4);

      //----------------------------------------------------------------------
      //!  Choices of what to print (mutually exclusive).
      //----------------------------------------------------------------------
      typedef enum {
        byDirectory = 1,  // print per-directory counters
        byFile      = 2,  // print per-directory and per-file counters
        byExtension = 3,  // print per-extension counters
        byLanguage  = 4,  // print per-language counters
        byScanner   = 5   // print per-scanner counters
      } WhatToPrint;

      //----------------------------------------------------------------------
      //!  Print the counters, organized by @c whatToPrint.  If @c quiet is
      //!  true, will only print the total.
      //----------------------------------------------------------------------
      std::ostream & Print(std::ostream & os,
                           WhatToPrint whatToPrint = byFile,
                           bool showGenerated = false,
                           HowToSort howToSort = sortByName,
                           bool quiet = false);

      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      void GetTotal(CodeCounter & total, bool includeGenerated = false) const;
      
      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      void GetTotalsByExtension(CodeCounterMap<std::string> & byExt,
                                bool includeGenerated = false) const;

      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      void GetTotalsByLanguage(CodeCounterMap<std::string> & byLanguage,
                               bool includeGenerated = false) const;

      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      void GetTotalsByScanner(CodeCounterMap<std::string> & byScanner,
                              bool includeGenerated = false) const;

      using StringCounterPairVector =
        std::vector<std::pair<std::string,CodeCounter>>;
      
      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      void GetTotalsByExtension(StringCounterPairVector & ctrs,
                                HowToSort howToSort = sortBySize);
      
      void GetTotalsByLanguage(StringCounterPairVector & ctrs,
                               HowToSort howToSort = sortBySize);
      
      void GetTotalsByScanner(StringCounterPairVector & ctrs,
                              HowToSort howToSort = sortBySize);

      std::chrono::duration<long,std::micro> ParseTime() const
      { return _parseTime; }
        
      
    private:
      std::unordered_map<std::string,
                         std::unique_ptr<SourceDirFiles>>    _sources;
      std::chrono::duration<long,std::micro>                 _parseTime;
      uint64_t                                               _userCpuUsecs;
      uint64_t                                               _sysCpuUsecs;
      
      bool HasAnyFiles() const;
      bool HasMultipleFiles() const;
      static std::string KeyTitle(WhatToPrint whatToPrint);

      using PointerVector = 
        std::vector<const std::pair<const std::string,
                                    std::unique_ptr<SourceDirFiles>> *>;
      
      void SortBySize(std::map<std::string,CodeCounter> & dirTotals,
                      PointerVector & v, HowToSort howToSort) const;
      void SortByName(PointerVector & v) const;
      void SortBySize(StringCounterPairVector & ctrs, bool reversed);
      void SortByName(StringCounterPairVector & ctrs);
      void CalculateCpuTimes(struct rusage & usageStart);
      std::ostream & PrintHeader(std::ostream & os,
                                 WhatToPrint whatToPrint,
                                 bool quiet) const;
      std::ostream & PrintByPath(std::ostream & os,
                                 bool includeGenerated = false,
                                 HowToSort howToSort = sortByName,
                                 bool showFiles = true);
      std::ostream & PrintByExtension(std::ostream & os,
                                      bool includeGenerated = false,
                                      HowToSort howToSort = sortByName) const;
      std::ostream & PrintByLanguage(std::ostream & os,
                                     bool includeGenerated = false,
                                     HowToSort howToSort = sortByName) const;
      std::ostream & PrintByScanner(std::ostream & os,
                                    bool includeGenerated = false,
                                    HowToSort howToSort = sortByName) const;
    };

  }  // namespace Mcloc


}  // namespace Dwm

#endif  // _DWMMCLOCSOURCECOLLECTION_HH_
