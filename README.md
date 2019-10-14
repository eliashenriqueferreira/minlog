# minlog

<br>Minimal Log for embedded aplications
<br>This library was writen in C to permit a wide range of applications.
<br>In this initial version I provide logs on screen and logs on file with process id and instance identification on filename.
<br>There are 6 levels of debug to know (Trace, Debug, Info, Warning, Error and Critical).
<br>All levels will print lines like this:

```bash
[GMT 2019-10-04 18:12:19.026][d]{Endereco &result=0x7fffffffdbf0 Valor=8}[260][src/minlog.c]
[GMT 2019-10-04 18:12:19.026] - Time Stamp that can be changed for local time.
[D] - Char to identify the level of the message.
{Endereco &result=0x7fffffffdbf0 Valor=8} a formated message using printf style.
[260] line of source file.
[src/minlog.c] source file where this log was generated.
```

## Using Docker

It can be built using the [official gcc image](https://hub.docker.com/_/gcc):

- At `make/conf/`, copy `config.mk` to a new local config file like `myconfig.local.mk`
- Change `CC` to `gcc` and `CXX` to `g++` (or whatever binary name your image uses)
- Compile executing:
  ```bash
  docker run --rm -v "$(pwd):/usr/local/src" -w /usr/local/src gcc:7 make -C make
  ```
- You should be seeing the `wrk/` directory with the compiled artifacts.

(It's a good way to cross-compile Linux binaries from a Windows host)

## Memory Dumper

<br>For me and old technicians that worked between 80's years a pointer is an address of memory.
<br>Mimdump routine show data around an address passed as parameter.
<br>Above we have 4 outputs configurations on Windows Machine;
<br>----------------------------------------------------------------------------
```bash
DEBUG 64 BITS
Hello World! Lets create a minimal logger for applications!
[GMT 2019-10-14 19:32:08.761][I]{INFO: Windows Aplication}[138][C:\ehf\myProjs\minlog-master\minlog\src\main.cpp]
[GMT 2019-10-14 19:32:08.761][I]{INFO: compiled in 64bits}[140][C:\ehf\myProjs\minlog-master\minlog\src\main.cpp]
[GMT 2019-10-14 19:32:08.762][I]{INFO: This is in DEBUG mode}[146][C:\ehf\myProjs\minlog-master\minlog\src\main.cpp]
[GMT 2019-10-14 19:32:08.762][D]{ - - - - - - - - - - - - SHOWING MEMORY STACK STRUCTURE - - - - - - - - - - - - - - - - - - }[53][C:\ehf\myProjs\minlog-master\minlog\src\main.cpp]
[00000061737DF994] -064 11 CC CC CC CC CC CC CC CC 00 00 19 C4 CC CC CC CC 00 00 19 C4 CC CC CC CC 00 00 19 C4 CC CC CC CC 00 00 19 C4 CC CC CC CC 00 00 19 C4 CC CC CC CC 00 00 19 C4 CC CC CC CC 00 00 19 C4 CC CC CC
[00000061737DF994] +064 11 11 11 11 CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC 22 22 22 22 CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC
[00000061737DF994] +004 11 11 11 11
[00000061737DF9B4] +004 22 22 22 22
[00000061737DF9D4] +002 33 33
[00000061737DF9F4] +002 44 44
[00000061737DFA14] +001 55
[00000061737DFA64] +001 66
[00000061737DFA88] +008 77 77 77 77 77 77 77 77
[00000061737DFAA8] +008 88 88 88 88 88 88 88 88
[00000061737DFA38] +015 BF 0F 0F 0F 0F 0F 0F 0F 0F 0F 0F 0F 0F 0F BF
[00000061737DFAC8] +008 A5 AB 33 33 00 00 00 00

RELEASE 64 BITS
Hello World! Lets create a minimal logger for applications!
[GMT 2019-10-14 19:34:11.616][I]{INFO: Windows Aplication}[138][C:\ehf\myProjs\minlog-master\minlog\src\main.cpp]
[GMT 2019-10-14 19:34:11.617][I]{INFO: compiled in 64bits}[140][C:\ehf\myProjs\minlog-master\minlog\src\main.cpp]
[GMT 2019-10-14 19:34:11.617][I]{INFO: This is NOT in DEBUG mode}[148][C:\ehf\myProjs\minlog-master\minlog\src\main.cpp]
[GMT 2019-10-14 19:34:11.617][D]{ - - - - - - - - - - - - SHOWING MEMORY STACK STRUCTURE - - - - - - - - - - - - - - - - - - }[53][C:\ehf\myProjs\minlog-master\minlog\src\main.cpp]
[000000132DD4FA9C] -064 11 B3 C2 33 33 00 00 44 44 00 00 55 66 00 00 7F F9 00 00 19 C4 00 00 00 13 00 00 19 C4 00 00 00 13 00 00 19 C4 00 00 00 00 00 00 19 C4 00 00 00 00 00 00 19 C4 00 00 00 00 00 00 19 C4 00 00 00
[000000132DD4FA9C] +064 11 11 11 11 22 22 22 22 00 00 00 00 88 88 88 88 88 88 88 88 77 77 77 77 77 77 77 77 F8 FA D4 2D 13 00 00 00 BF 0F 0F 0F 0F 0F 0F 0F 0F 0F 0F 0F 0F 0F BF 00 0E 3E DB BF 15 62 00 00 00 00 00 00
[000000132DD4FA9C] +004 11 11 11 11
[000000132DD4FAA0] +004 22 22 22 22
[000000132DD4FA98] +002 33 33
[000000132DD4FA94] +002 44 44
[000000132DD4FA91] +001 55
[000000132DD4FA90] +001 66
[000000132DD4FAB0] +008 77 77 77 77 77 77 77 77
[000000132DD4FAA8] +008 88 88 88 88 88 88 88 88
[000000132DD4FAC0] +015 BF 0F 0F 0F 0F 0F 0F 0F 0F 0F 0F 0F 0F 0F BF
[000000132DD4FAB8] +008 A5 AB 33 33 00 00 00 00

DEBUG 32 BITS
Hello World! Lets create a minimal logger for applications!
[GMT 2019-10-14 19:45:23.137][I]{INFO: Windows Aplication}[138][C:\ehf\myProjs\minlog-master\minlog\src\main.cpp]
[GMT 2019-10-14 19:45:23.141][I]{INFO: compiled in 32bits}[142][C:\ehf\myProjs\minlog-master\minlog\src\main.cpp]
[GMT 2019-10-14 19:45:23.141][I]{INFO: This is in DEBUG mode}[146][C:\ehf\myProjs\minlog-master\minlog\src\main.cpp]
[GMT 2019-10-14 19:45:23.142][D]{ - - - - - - - - - - - - SHOWING MEMORY STACK STRUCTURE - - - - - - - - - - - - - - - - - - }[53][C:\ehf\myProjs\minlog-master\minlog\src\main.cpp]
[0073F900] -064 11 CC CC CC CC CC CC CC CC 22 22 22 22 CC CC CC CC CC CC CC CC CC CC 33 33 CC CC CC CC CC CC CC CC CC CC 44 44 CC CC CC CC CC CC CC CC 55 CC CC CC CC CC CC CC CC CC CC CC CC BF 0F 0F 0F 0F 0F
[0073F900] +064 11 11 11 11 CC CC CC CC CB 43 28 45 24 FA 73 00 F5 62 27 00 E3 40 28 45 8C 15 27 00 8C 15 27 00 00 80 49 00 CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC CC
[0073F900] +004 11 11 11 11
[0073F8F4] +004 22 22 22 22
[0073F8E8] +002 33 33
[0073F8DC] +002 44 44
[0073F8D3] +001 55
[0073F8AF] +001 66
[0073F89C] +008 77 77 77 77 77 77 77 77
[0073F88C] +008 88 88 88 88 88 88 88 88
[0073F8B8] +015 BF 0F 0F 0F 0F 0F 0F 0F 0F 0F 0F 0F 0F 0F BF
[0073F87C] +008 A5 AB 33 33 00 00 00 00

RELEASE 32 BITS
Hello World! Lets create a minimal logger for applications!
[GMT 2019-10-14 19:48:02.990][I]{INFO: Windows Aplication}[138][C:\ehf\myProjs\minlog-master\minlog\src\main.cpp]
[GMT 2019-10-14 19:48:02.991][I]{INFO: compiled in 32bits}[142][C:\ehf\myProjs\minlog-master\minlog\src\main.cpp]
[GMT 2019-10-14 19:48:02.991][I]{INFO: This is NOT in DEBUG mode}[148][C:\ehf\myProjs\minlog-master\minlog\src\main.cpp]
[GMT 2019-10-14 19:48:02.991][D]{ - - - - - - - - - - - - SHOWING MEMORY STACK STRUCTURE - - - - - - - - - - - - - - - - - - }[53][C:\ehf\myProjs\minlog-master\minlog\src\main.cpp]
[0133FC2C] -064 11 00 00 33 33 22 22 22 22 00 00 44 44 55 66 00 5D 31 39 39 2E 32 30 3A 38 01 5D A0 50 00 00 00 3D 00 00 00 3F 00 00 19 C4 00 00 19 C4 00 00 19 C4 00 00 19 C4 00 00 19 C4 00 00 19 C4 00 00 19
[0133FC2C] +064 11 11 11 11 88 88 88 88 88 88 88 88 77 77 77 77 77 77 77 77 02 00 00 00 DC 32 25 00 07 00 00 00 BF 0F 0F 0F 0F 0F 0F 0F 0F 0F 0F 0F 0F 0F BF 00 2A 06 EE 38 A8 FC 33 01 BE 13 25 00 96 06 EE 38
[0133FC2C] +004 11 11 11 11
[0133FC24] +004 22 22 22 22
[0133FC28] +002 33 33
[0133FC20] +002 44 44
[0133FC1F] +001 55
[0133FC1E] +001 66
[0133FC38] +008 77 77 77 77 77 77 77 77
[0133FC30] +008 88 88 88 88 88 88 88 88
[0133FC4C] +015 BF 0F 0F 0F 0F 0F 0F 0F 0F 0F 0F 0F 0F 0F BF
[0133FC40] +008 A5 AB 33 33 00 00 00 00
```


