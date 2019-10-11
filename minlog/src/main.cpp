#include <stdio.h>
#include <string>
#include <stdarg.h>

#include "../inc/minlog.h"

int test_minlog(int argc, char **argv);



int main(int argc, char **argv)
{

    std::string str(argv[0]);

    printf("Hello World! Lets create a minimal logger for applications!\n");

    if (argc > 1)
    {
        minlog_open(argv[0], MINLOG_LEVEL_DEBUG, argv[1]);   // Initializing with logfiles
    }
    else
    {
        minlog_open(argv[0], MINLOG_LEVEL_DEBUG, NULL);   // Initializing on console
    }

    test_minlog(argc,argv);

    return 0;
}

#ifdef _WIN32
#pragma warning (disable: 4244)
#pragma warning (disable: 4700)
//#pragma warning (disable: 4996)
#endif

int test_minlog(int argc, char **argv)
{
    unsigned char c = ' ';

    for (int i = 0x20; i < 0x7f; i++, c++)
    {
        usleep(1000);
        MINLOG_ERROR("Message with %d parameters. Valor Decimal %.3d. Valor Hexa 0x%x. Caracter %c. i negativo %d", 5, i, i, c, (-i));
    }


    __uint64_t p1,p2,p3,p4,p5,p6,p7;
	p1 = 1;
#ifdef _MSC_VER
	p2 = 2;
	p3 = 3;
	p4 = 4;
	p5 = 5;
	p6 = 6;
#else
	MINLOG_WARNING("------------------- NOT INITIALIZED VALUES FOR TESTS ---------------------");
#endif
	p7 = 7;
    int i1=p1,i2=p2,i3=p3,i4=p4,i5=p5,i6=p6,i7=p7;

    minlog(__FILE__, __LINE__, MINLOG_LEVEL_DEBUG,     "Debug with 7 parameters p. p1=%d p2=%d p3=%d p4=%d p5=%d p6=%d p7=%d", 7,p1,p2,p3,p4,p5,p6,p7);
    MINLOG("MINLOG with 7 parameters i. i1=%d i2=%d i3=%d i4=%d i5=%d i6=%d i7=%d", i1,i2,i3,i4,i5,i6,i7);

	MINLOG("No parameters.");

    __uint64_t result = 0x65966596;
	__uint64_t diffP1Result = (__uint64_t)(&p1 - &result);

	MINLOG_DEBUG("Stack p1==> [%8.8p]=[%8.8X]", &p1, p1);
	MINLOG_DEBUG("Stack p2==> [%8.8p]=[%8.8X]", &p2, p2);
	MINLOG_DEBUG("Stack p3==> [%8.8p]=[%8.8X]", &p3, p3);
	MINLOG_DEBUG("Stack p4==> [%8.8p]=[%8.8X]", &p4, p4);
	MINLOG_DEBUG("Stack p5==> [%8.8p]=[%8.8X]", &p5, p5);
	MINLOG_DEBUG("Stack p6==> [%8.8p]=[%8.8X]", &p6, p6);
	MINLOG_DEBUG("Stack p7==> [%8.8p]=[%8.8X]", &p7, p7);
	MINLOG_DEBUG("Stack i1==> [%8.8p]=[%8.8X]", &i1, i1);
	MINLOG_DEBUG("Stack i2==> [%8.8p]=[%8.8X]", &i2, i2);
	MINLOG_DEBUG("Stack i3==> [%8.8p]=[%8.8X]", &i3, i3);
	MINLOG_DEBUG("Stack i4==> [%8.8p]=[%8.8X]", &i4, i4);
	MINLOG_DEBUG("Stack i5==> [%8.8p]=[%8.8X]", &i5, i5);
	MINLOG_DEBUG("Stack i6==> [%8.8p]=[%8.8X]", &i6, i6);
	MINLOG_DEBUG("Stack i7==> [%8.8p]=[%8.8X]", &i7, i7);
	MINLOG_DEBUG("  result==> [%8.8p]=[%8.8X]", &result, result);
	MINLOG_DEBUG("diffP1Result[%.16p]=[%.16x]", &diffP1Result, diffP1Result);

    if (0x007BFC00 == (__uint64_t)(&result)) {
        // Run, Copy Adresses and Rebuild;
        MINLOG_WARNING("------- NOT DANGEROUS IF YOU KNOW ADDRESS ALLOCATION  -------");
        (*((__uint64_t *)(0x7fffffffdbF0))) = (*((__uint64_t *)(0x7fffffffdbE0)))+(*((__uint64_t *)(0x7fffffffdbE8)));
    }
    MINLOG_DEBUG("New Result &result=%p Value=%X", &result, result);

	return 0;
}

/*
[GMT 2019-10-11 20:15:45.081][D]{Stack p1==> [0136F7D0]=[00000001]}[71][C:\ehf\myProjs\minlog-master\minlog\src\main.cpp]
[GMT 2019-10-11 20:15:45.084][D]{Stack p2==> [0136F7C0]=[00000002]}[72][C:\ehf\myProjs\minlog-master\minlog\src\main.cpp]
[GMT 2019-10-11 20:15:45.093][D]{Stack p3==> [0136F7B0]=[00000003]}[73][C:\ehf\myProjs\minlog-master\minlog\src\main.cpp]
[GMT 2019-10-11 20:15:45.097][D]{Stack p4==> [0136F7A0]=[00000004]}[74][C:\ehf\myProjs\minlog-master\minlog\src\main.cpp]
[GMT 2019-10-11 20:15:45.106][D]{Stack p5==> [0136F790]=[00000005]}[75][C:\ehf\myProjs\minlog-master\minlog\src\main.cpp]
[GMT 2019-10-11 20:15:45.120][D]{Stack p6==> [0136F780]=[00000006]}[76][C:\ehf\myProjs\minlog-master\minlog\src\main.cpp]
[GMT 2019-10-11 20:15:45.126][D]{Stack p7==> [0136F770]=[00000007]}[77][C:\ehf\myProjs\minlog-master\minlog\src\main.cpp]
[GMT 2019-10-11 20:15:45.127][D]{Stack i1==> [0136F764]=[00000001]}[78][C:\ehf\myProjs\minlog-master\minlog\src\main.cpp]
[GMT 2019-10-11 20:15:45.135][D]{Stack i2==> [0136F758]=[00000002]}[79][C:\ehf\myProjs\minlog-master\minlog\src\main.cpp]
[GMT 2019-10-11 20:15:45.150][D]{Stack i3==> [0136F74C]=[00000003]}[80][C:\ehf\myProjs\minlog-master\minlog\src\main.cpp]
[GMT 2019-10-11 20:15:45.155][D]{Stack i4==> [0136F740]=[00000004]}[81][C:\ehf\myProjs\minlog-master\minlog\src\main.cpp]
[GMT 2019-10-11 20:15:45.156][D]{Stack i5==> [0136F734]=[00000005]}[82][C:\ehf\myProjs\minlog-master\minlog\src\main.cpp]
[GMT 2019-10-11 20:15:45.164][D]{Stack i6==> [0136F728]=[00000006]}[83][C:\ehf\myProjs\minlog-master\minlog\src\main.cpp]
[GMT 2019-10-11 20:15:45.177][D]{Stack i7==> [0136F71C]=[00000007]}[84][C:\ehf\myProjs\minlog-master\minlog\src\main.cpp]
[GMT 2019-10-11 20:15:45.189][D]{  result==> [0136F70C]=[65966596]}[85][C:\ehf\myProjs\minlog-master\minlog\src\main.cpp]
[GMT 2019-10-11 20:15:45.204][D]{diffP1Result[0136F6FC]=[00000018]}[86][C:\ehf\myProjs\minlog-master\minlog\src\main.cpp]
[GMT 2019-10-11 20:15:45.208][D]{New Result &result=0136F70C Value=65966596}[93][C:\ehf\myProjs\minlog-master\minlog\src\main.cpp]



[GMT 2019-10-11 20:03:56.266][I]{MINLOG with 7 parameters i. i1=1 i2=2 i3=3 i4=4 i5=5 i6=6 i7=7}[64][C:\ehf\myProjs\minlog-master\minlog\src\main.cpp]
[GMT 2019-10-11 20:03:56.281][I]{No parameters.}[66][C:\ehf\myProjs\minlog-master\minlog\src\main.cpp]
[GMT 2019-10-11 20:03:56.286][D]{Stack p1==> [007BFCC4]=[00000001]}[71][C:\ehf\myProjs\minlog-master\minlog\src\main.cpp]
[GMT 2019-10-11 20:03:56.294][D]{Stack p2==> [007BFCB4]=[00000002]}[72][C:\ehf\myProjs\minlog-master\minlog\src\main.cpp]
[GMT 2019-10-11 20:03:56.295][D]{Stack p3==> [007BFCA4]=[00000003]}[73][C:\ehf\myProjs\minlog-master\minlog\src\main.cpp]
[GMT 2019-10-11 20:03:56.299][D]{Stack p4==> [007BFC94]=[00000004]}[74][C:\ehf\myProjs\minlog-master\minlog\src\main.cpp]
[GMT 2019-10-11 20:03:56.307][D]{Stack p5==> [007BFC84]=[00000005]}[75][C:\ehf\myProjs\minlog-master\minlog\src\main.cpp]
[GMT 2019-10-11 20:03:56.311][D]{Stack p6==> [007BFC74]=[00000006]}[76][C:\ehf\myProjs\minlog-master\minlog\src\main.cpp]
[GMT 2019-10-11 20:03:56.326][D]{Stack p7==> [007BFC64]=[00000007]}[77][C:\ehf\myProjs\minlog-master\minlog\src\main.cpp]
[GMT 2019-10-11 20:03:56.328][D]{Stack i1==> [007BFC58]=[00000001]}[78][C:\ehf\myProjs\minlog-master\minlog\src\main.cpp]
[GMT 2019-10-11 20:03:56.330][D]{Stack i2==> [007BFC4C]=[00000002]}[79][C:\ehf\myProjs\minlog-master\minlog\src\main.cpp]
[GMT 2019-10-11 20:03:56.341][D]{Stack i3==> [007BFC40]=[00000003]}[80][C:\ehf\myProjs\minlog-master\minlog\src\main.cpp]
[GMT 2019-10-11 20:03:56.346][D]{Stack i4==> [007BFC34]=[00000004]}[81][C:\ehf\myProjs\minlog-master\minlog\src\main.cpp]
[GMT 2019-10-11 20:03:56.356][D]{Stack i5==> [007BFC28]=[00000005]}[82][C:\ehf\myProjs\minlog-master\minlog\src\main.cpp]
[GMT 2019-10-11 20:03:56.359][D]{Stack i6==> [007BFC1C]=[00000006]}[83][C:\ehf\myProjs\minlog-master\minlog\src\main.cpp]
[GMT 2019-10-11 20:03:56.367][D]{Stack i7==> [007BFC10]=[00000007]}[84][C:\ehf\myProjs\minlog-master\minlog\src\main.cpp]
[GMT 2019-10-11 20:03:56.373][D]{  result==> [007BFC00]=[65966596]}[85][C:\ehf\myProjs\minlog-master\minlog\src\main.cpp]
[GMT 2019-10-11 20:03:56.382][D]{diffP7Result[007BFBF0]=[0000000C]}[86][C:\ehf\myProjs\minlog-master\minlog\src\main.cpp]
[GMT 2019-10-11 20:03:56.393][D]{New Result &result=007BFC00 Value=65966596}[93][C:\ehf\myProjs\minlog-master\minlog\src\main.cpp]

*/
