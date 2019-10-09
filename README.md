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
