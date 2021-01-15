# mcloc
A lines-of-code counter.

Focused on C++ projects, which sometimes include code generated
by bison, flex, moc, rcc and uic.

Has support for many other languages, but since I'm not a user of many
of them, may be inaccurate on other languages.  If you use this code,
feel free to open an issue if you find problems.


When I first wrote mcloc more than 20 years ago, I needed a tool that
could ignore source code that was generated by flex or bison.  By
default, mcloc will ignore code generated by flex, bison, moc, rcc and
uic and instead just count the generator input.  That's my personal
preference; generated code is not code I maintain, and is usually much
larger than the generator input I maintain (and hence misleading).
Counters for generated code can be enabled on the command line if
desired.

Reasonably fast.  Speed has never been an issue in my usage.  As an example,
on a large code base which is orders of magnitude larger than my
typical use:

```
% time mcloc -l -Lc++ ~/gits/qt5
         CODE    COMMENTS LANGUAGE                                             
L  12,892,785   2,270,945 C++                                                  
   12,892,785   2,270,945 TOTAL  (208.4ms, 72.77M lines/s, 308% sys 1,361% usr)
3.231u 1.117s 0:01.07 405.6%    0+0k 0+0io 0pf+0w
```

More than 12 million lines/second in this case.  I don't need it to
be any faster.  I've seen it hit 80 million lines/second on some code bases
on a modern workstation (Threadripper 3960X with PCIe Gen4 storage).  For my
typical usage, on MUCH smaller code bases, it's essentially instantaneous.
And I'm not willing to gravely risk accuracy in the name of speed.  A few
lines here and there, no big deal.  But any time I find it getting lost and
grossly miscounting, I fix it.  This tends to be a problem with most LOC
counters and it's understandable.  There are limitations with what can be
done accurately for some languages without writing a full language front end.
