//===========================================================================
// @(#) $DwmPath: dwm/mcplex/mcloc/trunk/classes/src/DwmMclocSourceCollection.cc 11829 $
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
//!  \file DwmMclocSourceCollection.cc
//!  \brief Dwm::Mcloc::SourceCollection class implementation
//---------------------------------------------------------------------------

extern "C" {
  #include <sys/types.h>
  #include <sys/time.h>
  #include <unistd.h>
}

#include <algorithm>
#include <iomanip>
#include <set>
#include <sstream>
#include <vector>

#include "DwmMclocCodeUtils.hh"
#include "DwmMclocEngNotate.hh"
#include "DwmMclocLanguage.hh"
#include "DwmMclocSourceCollection.hh"
#include "DwmMclocTerminalTricks.hh"

using namespace std;

static const string svnid("@(#) $DwmPath: dwm/mcplex/mcloc/trunk/classes/src/DwmMclocSourceCollection.cc 11829 $");

namespace Dwm {

  namespace Mcloc {

    namespace fs = std::filesystem;

    static TerminalTricks  tt;

    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    bool SourceDirFiles::AddFile(const fs::path & fspath)
    {
      bool  rc = false;
      if (CodeUtils::IsSourceFile(fspath)) {
        auto  it = _files.find(fspath.filename());
        if (it == _files.end()) {
          _files[fspath.filename()] = SourceFileInfo();
        }
        rc = true;
      }
      return rc;
    }

    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    void SourceDirFiles::QueueForWork(const fs::path & dirPath,
                                      ScanWorker::Queue_t & queue)
    {
      for (auto & f : _files) {
        fs::path  p(dirPath);
        p += "/";
        p += f.first;
        queue.PushBack(make_pair(p,&f.second));
      }
      return;
    }

    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    bool
    SourceDirFiles::CopyToVector(PointerVector & v) const
    {
      v.clear();
      for (auto it = _files.begin(); it != _files.end(); ++it) {
        v.push_back(&(*it));
      }
      return (! v.empty());
    }
    
    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    void SourceDirFiles::SortBySize(PointerVector & v, HowToSort howToSort)
    {
      if (CopyToVector(v)) {
        if (sortBySizeReversed == howToSort) {
          sort(v.begin(), v.end(),
               [] (const auto & f1, const auto & f2) -> bool
               { return (f1->second.Counter() > f2->second.Counter()); });
        }
        else {
          sort(v.begin(), v.end(),
               [] (const auto & f1, const auto & f2) -> bool
               { return (f1->second.Counter() < f2->second.Counter()); });
        }
      }
      return;
    }

    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    void SourceDirFiles::SortByName(PointerVector & v)
    {
      if (CopyToVector(v)) {
        sort(v.begin(), v.end(),
             [&] (const auto & f1, const auto & f2) -> bool
             { return (f1->first < f2->first); });
      }
      return;
    }

    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    static ostream &
    PrintSourceDirEntry(const pair<const string,SourceFileInfo> *f,
                        ostream & os, bool showGenerated, bool lastEntry)
    {
      if (! lastEntry) {
        if (! f->second.IsGenerated()) {
          os << "F " << f->second.Counter() << "   " << f->first << endl;
        }
        else {
          if (showGenerated) {
            os << "G " << f->second.Counter() << "   " << f->first
               << " (" << f->second.GeneratedBy().string() << ")\n";
          }
        }
      }
      else {
        if (! f->second.IsGenerated()) {
          os << "F " << tt.Underscore(f->second.Counter())
             << ' ' << tt.Underscore("  ") << tt.Underscore(f->first) << endl;
        }
        else {
          if (showGenerated) {
            os << tt.Underscore("G ") << tt.Underscore(f->second.Counter())
               << ' ' << tt.Underscore("  ") << tt.Underscore(f->first)
               << tt.Underscore(" (")
               << tt.Underscore(f->second.GeneratedBy().string())
               << tt.Underscore(")") << '\n';
          }
        }
      }
      return os;
    }

    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    SourceDirFiles::PointerVector::const_iterator
    SourceDirFiles::LastPrintable(const PointerVector & fileVec,
                                  bool showGenerated) const
    {
      auto  rc = fileVec.end();
      for (auto it = fileVec.begin(); it != fileVec.end(); ++it) {
        if (showGenerated || (! (*it)->second.IsGenerated())) {
          rc = it;
        }
      }
      return rc;
    }

    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    ostream &
    SourceDirFiles::PrintEntry(PointerVector::value_type f,
                               ostream & os, bool showGenerated)
    {
      if (! f->second.IsGenerated()) {
        os << "F " << f->second.Counter() << "   " << f->first << endl;
      }
      else {
        if (showGenerated) {
          os << "G " << f->second.Counter() << "   " << f->first
             << " (" << f->second.GeneratedBy().string() << ")\n";
        }
      }
      return os;
    }

    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    ostream &
    SourceDirFiles::PrintLastEntry(PointerVector::value_type f,
                                   ostream & os, bool showGenerated)
    {
      os << (f->second.IsGenerated() ? "G " : "F ");
      ostringstream  oss;
      oss.imbue(std::locale(""));
      oss << f->second.Counter() << "   " << f->first;
      if (f->second.IsGenerated()) {
        oss << " (" << f->second.GeneratedBy().string() << ")";
      }
      string  s(oss.str());
      int  trailerSize = tt.Columns() - (s.size() + 3);
      string  trailer;
      if (trailerSize > 0) {
        trailer.assign(trailerSize, ' ');
      }
      tt.Underscore(os, true);
      os << s << trailer;
      tt.Underscore(os, false);
      os << '\n';
      return os;
    }
    
    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    ostream & SourceDirFiles::Print(ostream & os, bool showGenerated,
                                    HowToSort howToSort, bool isLastDir)
    {
      PointerVector  fileVec;
      switch (howToSort) {
        case sortBySize:
        case sortBySizeReversed:
          SortBySize(fileVec, howToSort);
          break;
        default:
          SortByName(fileVec);
          break;
      }

      if (! fileVec.empty()) {
        PointerVector::const_iterator  lastPrintable = fileVec.end();
        if (isLastDir) {
          lastPrintable = LastPrintable(fileVec, showGenerated);
        }
        for (auto it = fileVec.begin(); it != lastPrintable; ++it) {
          PrintEntry(*it, os, showGenerated);
        }
        if (lastPrintable != fileVec.end()) {
          PrintLastEntry(*lastPrintable, os, showGenerated);
        }
      }
      return os;
    }

    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    void SourceDirFiles::GetTotal(CodeCounter & total,
                                  bool includeGenerated) const
    {
      total.Reset();
      for (const auto & f : _files) {
        if (includeGenerated || (! f.second.IsGenerated())) {
          total += f.second.Counter();
        }
      }
      return;
    }

    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    void SourceDirFiles::GetTotalsByScanner(CodeCounterMap<string> & byScanner,
                                            bool includeGenerated) const
    {
      using path = std::filesystem::path;
      for (const auto & f : _files) {
        if (includeGenerated || (! f.second.IsGenerated())) {
          const Language  *lang = CodeUtils::PathToLanguage(fs::path(f.first));
          if (nullptr != lang) {
            const FileScanner  *scanner = lang->Scanner();
            if (nullptr != scanner) {
              byScanner.Increment(scanner->CommentStyle(),
                                  f.second.Counter());
            }
            else {
              byScanner.Increment("UNKNOWN", f.second.Counter());
            }
          }
          else {
            byScanner.Increment("UNKNOWN", f.second.Counter());
          }
        }
      }
      return;
    }
      
    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    void
    SourceDirFiles::GetTotalsByLanguage(CodeCounterMap<string> & byLanguage,
                                        bool includeGenerated) const
    {
      using path = std::filesystem::path;
      for (const auto & f : _files) {
        if (includeGenerated || (! f.second.IsGenerated())) {
          const Language  *lang = CodeUtils::PathToLanguage(fs::path(f.first));
          if (nullptr != lang) {
            byLanguage.Increment(lang->Name(), f.second.Counter());
          }
          else {
            byLanguage.Increment("UNKNOWN", f.second.Counter());
          }
        }
      }
      return;
    }
    
    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    void
    SourceDirFiles::GetTotalsByExtension(CodeCounterMap<string> & byExt,
                                         bool includeGenerated) const
    {
      for (const auto & f : _files) {
        if (includeGenerated || (! f.second.IsGenerated())) {
          string  ext = std::filesystem::path(f.first).extension();
          if ((! ext.empty())
              && (CodeUtils::ExtensionLanguage(ext) != "UNKNOWN")) {
            byExt.Increment(std::filesystem::path(f.first).extension(),
                            f.second.Counter());
          }
          else {
            //  need to look up by expression
            string  expr = CodeUtils::PathToExpression(f.first);
            byExt.Increment(string("\"") + expr + "\"", f.second.Counter());
          }
        }
      }
      return;
    }
    
    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    void SourceCollection::AddFile(const fs::path & fspath)
    {
      fs::path  dirPath = fspath.parent_path();
      if (dirPath.empty()) {
        dirPath = ".";
      }
      if (_sources.find(dirPath) == _sources.end()) {
        _sources[dirPath] = make_unique<SourceDirFiles>();
      }
      _sources[dirPath]->AddFile(fspath);
      return;
    }

    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    void SourceCollection::AddDirectory(const fs::path & fspath, bool recurse)
    {
      auto it = _sources.find(fspath);
      if (it == _sources.end()) {
        unique_ptr<SourceDirFiles>  newFiles = make_unique<SourceDirFiles>();
        fs::directory_iterator  di(fspath);
        fs::directory_iterator  dirEnd;
        for ( ; di != dirEnd; ++di) {
          if (fs::is_regular_file(di->path())) {
            newFiles->AddFile(di->path());
          }
          else if (recurse && (! fs::is_symlink(di->path()))
                   && fs::is_directory(di->path())) {
            AddDirectory(di->path(), recurse);
          }
        }
        if (! newFiles->Files().empty()) {
          _sources[fspath] = std::move(newFiles);
        }
      }
      
      return;
    }

    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    static uint64_t CalculateTimeDiff(const struct timeval & starttv,
                                      const struct timeval & endtv)
    {
      uint64_t  endUsecs = endtv.tv_sec * 1000000 + endtv.tv_usec;
      uint64_t  startUsecs = starttv.tv_usec * 1000000 + starttv.tv_usec;
      return (endUsecs - startUsecs);
    }
    
    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    void SourceCollection::CalculateCpuTimes(struct rusage & usageStart)
    {
      struct rusage   usageEnd;
      getrusage(RUSAGE_SELF, &usageEnd);
      _userCpuUsecs = CalculateTimeDiff(usageStart.ru_utime,
                                        usageEnd.ru_utime);
      _sysCpuUsecs = CalculateTimeDiff(usageStart.ru_stime,
                                      usageEnd.ru_stime);
      return;
    }
    
    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    void SourceCollection::Parse(int numThreads)
    {
      using namespace std::chrono;
      
      auto            startTime = steady_clock::now();
      struct rusage   usageStart;
      getrusage(RUSAGE_SELF, &usageStart);

      ScanWorker::Queue_t  queue;
      for (auto & d : _sources) {
        d.second->QueueForWork(d.first, queue);
      }
      vector<unique_ptr<ScanWorker>>   workers;
      for (int i = 0; i < numThreads; ++i) {
        workers.push_back(make_unique<ScanWorker>(queue));
      }
      for (auto & worker : workers) { worker->Start();  }
      for (auto & worker : workers) { worker->Finish(); }
      
      auto  endTime = steady_clock::now();
      _parseTime = duration_cast<microseconds>(endTime - startTime);
      CalculateCpuTimes(usageStart);
      
      return;
    }

    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    std::string SourceCollection::KeyTitle(WhatToPrint whatToPrint)
    {
      static const map<WhatToPrint,std::string>  keyTitles = {
        { byDirectory, "DIR" },
        { byFile,      "PATH" },
        { byExtension, "EXTENSION/EXPRESSION" },
        { byLanguage,  "LANGUAGE" },
        { byScanner,   "SCANNER" }
      };
      
      std::string  rc;
      auto  it = keyTitles.find(whatToPrint);
      if (it != keyTitles.end()) {
        rc = it->second;
      }
      return rc;
    }

    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    ostream & SourceCollection::PrintHeader(ostream & os,
                                            WhatToPrint whatToPrint,
                                            bool quiet) const
    {
      if (! quiet) {
        string  hdrstr("       CODE    COMMENTS ");
        hdrstr += KeyTitle(whatToPrint);
        int  numColumns = tt.Columns();
        for (int i = hdrstr.size(); i < numColumns - 3; ++i) {
          hdrstr += ' ';
        }
        os << "  " << tt.Underscore(hdrstr) << '\n';
      }
      return os;
    }
    
    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    ostream & SourceCollection::Print(ostream & os, WhatToPrint whatToPrint,
                                      bool showGenerated,
                                      HowToSort howToSort, bool quiet)
    {
      if (isatty(STDOUT_FILENO)) {
        os.imbue(std::locale(""));
      }
      else {
        tt.On(false);
      }
      size_t  boldLen = tt.Bold("").size();
      
      if (HasAnyFiles()) {
        if (! quiet) {
          PrintHeader(os, whatToPrint, quiet);
          switch (whatToPrint) {
            case byDirectory:
              PrintByPath(os, showGenerated, howToSort, false);
              break;
            case byFile:
              PrintByPath(os, showGenerated, howToSort, true);
              break;
            case byExtension:
              PrintByExtension(os, showGenerated, howToSort);
              break;
            case byLanguage:
              PrintByLanguage(os, showGenerated, howToSort);
              break;
            case byScanner:
              PrintByScanner(os, showGenerated, howToSort);
              break;
            default:
              break;
          }
        }
        if (HasMultipleFiles() || quiet) {
          CodeCounter  total;
          GetTotal(total, showGenerated);
          os << setw(13 + boldLen) << tt.Bold(total.CodeLines()) << ' '
             << setw(11 + boldLen) << tt.Bold(total.CommentLines())
             << tt.Bold(" TOTAL  ") << '('
             << EngNotate((_parseTime.count() / 1000000.0), 4, 4) << "s";
          if (_parseTime.count() > 0) {
            long  linesPerSec = (total.CodeLines() + total.CommentLines())
              / (_parseTime.count() / 1000000.0);
            os << ", " << EngNotate(linesPerSec, 4, 4) << " lines/s, ";
          }
          os << (_sysCpuUsecs * 100) / _parseTime.count() << "% sys "
             << (_userCpuUsecs * 100) / _parseTime.count() << "% usr)\n";
        }
      }
      return os;
    }

    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    void SourceCollection::GetTotal(CodeCounter & total,
                                    bool includeGenerated) const
    {
      CodeCounter  dirTotal;
      for (const auto & d : _sources) {
        d.second->GetTotal(dirTotal, includeGenerated);
        total += dirTotal;
      }
      return;
    }
    
    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    void
    SourceCollection::GetTotalsByExtension(CodeCounterMap<string> & byExt,
                                           bool includeGenerated) const
    {
      for (const auto & d : _sources) {
        d.second->GetTotalsByExtension(byExt, includeGenerated);
      }
      return;
    }

    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    void
    SourceCollection::GetTotalsByLanguage(CodeCounterMap<string> & byLanguage,
                                          bool includeGenerated) const
    {
      for (const auto & d : _sources) {
        d.second->GetTotalsByLanguage(byLanguage, includeGenerated);
      }
      
      return;
    }

    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    void
    SourceCollection::GetTotalsByScanner(CodeCounterMap<string> & byScanner,
                                         bool includeGenerated) const
    {
      for (const auto & d : _sources) {
        d.second->GetTotalsByScanner(byScanner, includeGenerated);
      }
      return;
    }
    
    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    void SourceCollection::GetTotalsByExtension(StringCounterPairVector & ctrs,
                                                HowToSort howToSort)
    {
      CodeCounterMap<string>  byExt;
      for (const auto & d : _sources) {
        d.second->GetTotalsByExtension(byExt, false);
      }
      byExt.Transform(ctrs);
      switch (howToSort) {
        case sortBySize:
          SortBySize(ctrs, false);
          break;
        case sortBySizeReversed:
          SortBySize(ctrs, true);
          break;
        case sortByName:
          SortByName(ctrs);
          break;
        default:
          break;
      }
      return;
    }

    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    void SourceCollection::GetTotalsByLanguage(StringCounterPairVector & ctrs,
                                               HowToSort howToSort)
    {
      CodeCounterMap<string>  byLanguage;
      for (const auto & d : _sources) {
        d.second->GetTotalsByLanguage(byLanguage, false);
      }
      byLanguage.Transform(ctrs);
      switch (howToSort) {
        case sortBySize:
          SortBySize(ctrs, false);
          break;
        case sortBySizeReversed:
          SortBySize(ctrs, true);
          break;
        case sortByName:
          SortByName(ctrs);
          break;
        default:
          break;
      }
      return;
    }

    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    void SourceCollection::GetTotalsByScanner(StringCounterPairVector & ctrs,
                                              HowToSort howToSort)
    {
      CodeCounterMap<string>  byScanner;
      for (const auto & d : _sources) {
        d.second->GetTotalsByScanner(byScanner, false);
      }
      byScanner.Transform(ctrs);
      //  xxx TODO: sort
      return;
    }

    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    void SourceCollection::SortBySize(StringCounterPairVector & ctrs,
                                      bool reversed)
    {
      sort(ctrs.begin(), ctrs.end(),
           [&] (const auto & d1, const auto & d2)
           { return (reversed ?
                     (d1.second.CodeLines() > d2.second.CodeLines())
                     : (d1.second.CodeLines() < d2.second.CodeLines())); });
      return;
    }

    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    void SourceCollection::SortByName(StringCounterPairVector & ctrs)
    {
      sort(ctrs.begin(), ctrs.end(),
           [&] (const auto & d1, const auto & d2)
           { return d1.first < d2.first; });
      return;
    }
    
    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    void SourceCollection::SortBySize(map<string,CodeCounter> & dirTotals,
                                      PointerVector & v,
                                      HowToSort howToSort) const
    {
      v.clear();
      for_each(_sources.begin(), _sources.end(),
               [&] (const auto & d)
               { v.push_back(&d); } );
      sort(v.begin(), v.end(),
           [&] (const auto & d1, const auto & d2)
           { return ((sortBySizeReversed == howToSort) ?
                     dirTotals[d2->first] < dirTotals[d1->first]
                     : dirTotals[d1->first] < dirTotals[d2->first]); });
      return;
    }

    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    void SourceCollection::SortByName(PointerVector & v) const
    {
      v.clear();
      for_each(_sources.begin(), _sources.end(),
               [&] (const auto & d)
               { v.push_back(&d); } );
      sort(v.begin(), v.end(),
           [] (const auto & d1, const auto & d2)
           { return (d1->first < d2->first); });
      return;
    }
    
    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    ostream & SourceCollection::PrintByPath(ostream & os,
                                            bool showGenerated,
                                            HowToSort howToSort,
                                            bool showFiles)
    {
      map<string,CodeCounter>  dirTotals;
      for (const auto & d : _sources) {
        d.second->GetTotal(dirTotals[d.first], showGenerated);
      }
      PointerVector  v;
      switch (howToSort) {
        case sortBySize:
        case sortBySizeReversed:
          SortBySize(dirTotals, v, howToSort);
          break;
        default:
          SortByName(v);
          break;
      }

      if (! v.empty()) {
        size_t  i = 0;
        for ( ; i < v.size() - 1; ++i) {
          const auto & d = v[i];
          os << "D " << setw(11) << dirTotals[d->first].CodeLines() << ' '
             << setw(11) << dirTotals[d->first].CommentLines() << ' '
             << d->first << '\n';
          if (showFiles) {
            d->second->Print(os, showGenerated, howToSort, false);
          }
        }
        const auto & d = v[i];
        if (showFiles) {
          os << "D " << setw(11) << dirTotals[d->first].CodeLines() << ' '
             << setw(11) << dirTotals[d->first].CommentLines() << ' '
             << d->first << '\n';
          d->second->Print(os, showGenerated, howToSort, true);
        }
        else {
          ostringstream  oss;
          oss.imbue(std::locale(""));
          oss << setw(11) << dirTotals[d->first].CodeLines() << ' '
              << setw(11) << dirTotals[d->first].CommentLines() << ' '
              << d->first;
          string  s(oss.str());
          int  trailerSize = tt.Columns() - (s.size() + 3);
          string  trailer;
          if (trailerSize > 0) {
            trailer.assign(trailerSize, ' ');
          }
          os << "D " << tt.Underscore(s + trailer) << '\n';
        }
      }
      
      return os;
    }
    
    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    ostream &
    SourceCollection::PrintByExtension(ostream & os,
                                       bool includeGenerated,
                                       HowToSort howToSort) const
    {
      CodeCounterMap<string>  byExt;
      GetTotalsByExtension(byExt, includeGenerated);
      byExt.Print(os, howToSort, tt, "E");
      return os;
    }

    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    ostream &
    SourceCollection::PrintByLanguage(ostream & os,
                                      bool includeGenerated,
                                      HowToSort howToSort) const
    {
      CodeCounterMap<string>  countersByLanguage;
      GetTotalsByLanguage(countersByLanguage, includeGenerated);
      countersByLanguage.Print(os, howToSort, tt, "L");
      return os;
    }
    
    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    ostream &
    SourceCollection::PrintByScanner(ostream & os,
                                     bool includeGenerated,
                                     HowToSort howToSort) const
    {
      CodeCounterMap<string>  countersByScanner;
      GetTotalsByScanner(countersByScanner, includeGenerated);
      countersByScanner.Print(os, howToSort, tt, "S");
      return os;
    }

    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    bool SourceCollection::HasAnyFiles() const
    {
      bool  rc = false;
      for (const auto & d : _sources) {
        if (! d.second->Files().empty()) {
          rc = true;
          break;
        }
      }
      return rc;
    }
    
    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    bool SourceCollection::HasMultipleFiles() const
    {
      bool  rc = true;
      if (_sources.empty()) {
        rc = false;
      }
      else {
        if (_sources.size() == 1) {
          if (_sources.begin()->second->Files().size() <= 1) {
            rc = false;
          }
        }
      }
      return rc;
    }

    
  }  // namespace Mcloc

}  // namespace Dwm
