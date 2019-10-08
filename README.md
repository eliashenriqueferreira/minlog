# minlog
<br>Minimal Log for embedded aplications
<br>This library was writen in C to permit a wide range of applications.
<br>In this initial version I provide logs on screen and logs on file with process id and instance identification on filename.
<br>There are 6 levels of debug to know(Trace, Debug, Info, Warning, Error and Critical).
<br>All levels will print lines like this:
<br>   [GMT 2019-10-04 18:12:19.026][D]{Endereco &result=0x7fffffffdbf0 Valor=8}[260][src/minlog.c]
<br>   [GMT 2019-10-04 18:12:19.026] - Time Stamp that can be changed for local time.
<br>   [D] - Char to identify the level of the message.
<br>   {Endereco &result=0x7fffffffdbf0 Valor=8} a formated message using printf style.
<br>   [260] line of source file.
<br>   [src/minlog.c] source file where this log was generated.
