#include <stdio.h>
#include <string>
#include <unistd.h>  // for usleep

#include <stdarg.h>


#include "../inc/minlog.h"

int test_minlog(int argc, char **argv);



int main(int argc, char **argv)
{

    std::string str(argv[0]);

    printf("Hello World! Lets create a minimal logger for applications!\n");

    // Inicializador do LOG
    //minlog_open(argv[0], MINLOG_LEVEL_DEBUG, "auxName");   // For log on files
    //    minlog_open(argv[0], MINLOG_LEVEL_DEBUG, "");   // For log on files
    //minlog_open(argv[0], MINLOG_LEVEL_DEBUG, NULL);   // For log on console
    if (argc > 1)
    {
        minlog_open(argv[0], MINLOG_LEVEL_DEBUG, argv[1]);   // For log on files
    }
    else
    {
        minlog_open(argv[0], MINLOG_LEVEL_DEBUG, NULL);   // For log on console
    }

    test_minlog(argc,argv);

    return 0;
}

int test_minlog(int argc, char **argv)
{
    /// Um log precisa de um inicializador

    /// Um log minimo poderia informar na mesma linha o seguinte:
    //  [timestamp][level][message+params][__LINE__][__FILE__]


    unsigned char c = ' ';

    for (int i = 0x20; i < 0x7f; i++, c++)
    {
        usleep(1000);

        //minlog(__FILE__, __LINE__, MINLOG_LEVEL_ERROR, "Mensagem com %d parametros. Valor Decimal %d. Valor Hexa 0x%x. Caracter %c", 4, (__uint64_t)(4), (__uint64_t)(i), (__uint64_t)(i), (__uint64_t)c );
        //MINLOG_ERROR("Mensagem com %d parametros. Valor Decimal %.3d. Valor Hexa 0x%x. Caracter %c", 4, i, i, c);
        MINLOG_ERROR("Mensagem com %d parametros. Valor Decimal %.3d. Valor Hexa 0x%x. Caracter %c. i negativo %d", 5, i, i, c, (-i));
    }


    MINLOG_WARNING("------------------- ABAIXO VALORES NÃO INICIALIZADOS ---------------------");

    __uint64_t p1,p2,p3,p4,p5,p6,p7;
    p1 = 1;
    p7 = 7;
    int i1=p1,i2=p2,i3=p3,i4=p4,i5=p5,i6=p6,i7=p7;

    minlog(__FILE__, __LINE__, MINLOG_LEVEL_DEBUG,     "Debug  com 7 parametros p. p1=%d p2=%d p3=%d p4=%d p5=%d p6=%d p7=%d", 7,p1,p2,p3,p4,p5,p6,p7);
    MINLOG("MINLOG com 7 parametros i. i1=%d i2=%d i3=%d i4=%d i5=%d i6=%d i7=%d", i1,i2,i3,i4,i5,i6,i7);


    MINLOG("No parameters.");

    __uint64_t *pointer1 = &p1;
    __uint64_t *pointer7 = &p7;
    __uint64_t result;
    __uint64_t *presult = &result;

    MINLOG_DEBUG("Address &p1=%p", pointer1);
    MINLOG_DEBUG("Address &p7=%p", pointer7);
    MINLOG_DEBUG("Address &result=%p Value=%X", presult, result);

    if (0x7fffffffdbF0 == (__uint64_t)presult) {
        // Run, Copy Adresses and Rebuild;
        MINLOG_WARNING("------- NOT DANGEROUS IF YOU KNOW THE FUTURE  -------");
        (*((__uint64_t *)(0x7fffffffdbF0))) = (*((__uint64_t *)(0x7fffffffdbE0)))+(*((__uint64_t *)(0x7fffffffdbE8)));
    }

    MINLOG_DEBUG("Address &result=%p Value=%X", presult, result);

//    [GMT 2019-10-04 18:12:19.017][E]{Mensagem com 5 parametros. Valor Decimal 120. Valor Hexa 0x78. Caracter x. i negativo -120}[232][src/minlog.c]
//    [GMT 2019-10-04 18:12:19.018][E]{Mensagem com 5 parametros. Valor Decimal 121. Valor Hexa 0x79. Caracter y. i negativo -121}[232][src/minlog.c]
//    [GMT 2019-10-04 18:12:19.020][E]{Mensagem com 5 parametros. Valor Decimal 122. Valor Hexa 0x7a. Caracter z. i negativo -122}[232][src/minlog.c]
//    [GMT 2019-10-04 18:12:19.022][E]{Mensagem com 5 parametros. Valor Decimal 123. Valor Hexa 0x7b. Caracter {. i negativo -123}[232][src/minlog.c]
//    [GMT 2019-10-04 18:12:19.023][E]{Mensagem com 5 parametros. Valor Decimal 124. Valor Hexa 0x7c. Caracter |. i negativo -124}[232][src/minlog.c]
//    [GMT 2019-10-04 18:12:19.024][E]{Mensagem com 5 parametros. Valor Decimal 125. Valor Hexa 0x7d. Caracter }. i negativo -125}[232][src/minlog.c]
//    [GMT 2019-10-04 18:12:19.026][E]{Mensagem com 5 parametros. Valor Decimal 126. Valor Hexa 0x7e. Caracter ~. i negativo -126}[232][src/minlog.c]
//    [GMT 2019-10-04 18:12:19.026][W]{------------------- ABAIXO VALORES NÃO INICIALIZADOS ---------------------}[237][src/minlog.c]
//    [GMT 2019-10-04 18:12:19.026][D]{Debug com 7 parametros. p1=1 p2=-134225560 p3=4 p4=4743539 p5=11008 p6=4748688 p7=7}[243][src/minlog.c]
//    [GMT 2019-10-04 18:12:19.026][I]{Mensagem sem parametros.}[244][src/minlog.c]
//    [GMT 2019-10-04 18:12:19.026][D]{Endereco &p1=0x7fffffffdbe0}[251][src/minlog.c]
//    [GMT 2019-10-04 18:12:19.026][D]{Endereco &p7=0x7fffffffdbe8}[252][src/minlog.c]
//    [GMT 2019-10-04 18:12:19.026][D]{Endereco &result=0x7fffffffdbf0 Valor=0}[253][src/minlog.c]
//    [GMT 2019-10-04 18:12:19.026][W]{--- JOGADA PERIGOSA ---}[256][src/minlog.c]
//    [GMT 2019-10-04 18:12:19.026][D]{Endereco &result=0x7fffffffdbf0 Valor=8}[260][src/minlog.c]

}

