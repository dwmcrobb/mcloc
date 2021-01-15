%{
  //------------------------------------------------------------------------

  #include <iostream>
  
  static int i = 0;

  /*
    Block comment.  Note we count separators like '%{' and '%%' as code,
    not comments or blanks.
  */
%}

%%

<INITIAL>.                { /* do nothing. */ }
<INITIAL>"/*"             { /*  counts as code. */ }
 /* counts as comment */
<INITIAL>"//"             { /*  also counts as code. */ }
 // counts as comment
<INITIAL>^[ \t]*\/\*      { /*  code */ }
<INITIAL>^a               {  /*  counts as comment (not ideal)
                             counts as comment
                             counts as code */ }
<INITIAL>["]
<INITIAL>\\               { std::cout << '"' << '\n'; }
<INITIAL>\n               { ++i; }

%%

//----------------------------------------------------------------------------
//!  
//----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
  return 0;
}
