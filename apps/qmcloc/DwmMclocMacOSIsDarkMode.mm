#include "DwmMclocMacOSIsDarkMode.hh"

#import <Foundation/NSString.h>
#import <AppKit/NSColor.h>

namespace Dwm {

  namespace Mcloc {

    namespace MacOS {

      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      bool IsDarkMode()
      {
        NSString *appearanceName = [[NSAppearance currentAppearance] name];
        return [appearanceName containsString:@"Dark"];
      }

    }  // namespace MacOS

  }  // namespace Mcloc

}  // namespace Dwm

