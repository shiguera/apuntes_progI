# Utilización del juego de caracteres extendido

## Sistemas de codificación de caracteres

El sistema de codificación [ASCII](https://es.wikipedia.org/wiki/ASCII) comprende los valores entre 0 y 127. Del 0 al 31 son caracteres especiales, por ejemplo, el valor 10 corresponde al cambio de línea `\n` que usamos habitualmente en los programas. Desde el 32 hasta el 127 son los caracteres *imprimibles*. Los caracteres imprimibles incluyen todas las letras del alfabeto inglés, en mayúsculas y minúsculas, los dígitos numéricos y algunos caracteres especiales. No incluye las letras acentuadas o la letra *ñ*, por poner un ejemplo.

Se denomina **ASCII extendido** a cualquier *juego de caracteres* de 8 bits en el cual, los códigos 32 a 126 (0x20 a 0x7E) coinciden con los caracteres imprimibles de ASCII,
 así como los caracteres comúnmente llamados *de espacio*, que son los códigos de control de 8 a 13 (0x08 a 0x0D), ambos inclusive.

Las codificaciones de ASCII extendido utilizan además parte o la totalidad de los códigos superiores a 128 para codificar caracteres  adicionales a los caracteres imprimibles ASCII. 

Las codificaciones ASCII extendido más comunes son:

- [Página de códigos 437](https://es.wikipedia.org/wiki/P%C3%A1gina_de_c%C3%B3digos_437 "Página de códigos 437"), usual en las versiones en inglés del IBM PC y MS-DOS.
- [Página de códigos 850](https://es.wikipedia.org/wiki/P%C3%A1gina_de_c%C3%B3digos_850 "Página de códigos 850"), usual en las versiones de Europa occidental del IBM PC y MS-DOS.
- *Latin-1* ([ISO-8859-1](https://es.wikipedia.org/wiki/ISO-8859-1 "ISO-8859-1")), típico de Unix y, con modificaciones, en Microsoft Windows ([Windows-1252](https://es.wikipedia.org/wiki/Windows-1252 "Windows-1252")) y Macintosh.

Todas estas codificaciones utilizan un byte para representar un carácter y permiten representar las letras habituales en los idiomas de Europa occidental: Francia, España, Alemania, Italia y otros. En cambio, el juego de caracteres que ofrecen es limitado. Por ejemplo, no permiten representar la letra griega *alpha*, &alpha;, o el símbolo del Euro, &euro;.

Actualmente se impone la utilización del sistema de codificación **Unicode**. Este sistema utiliza entre 1 y 4 bytes para representar cada caracter. La codificación más extendida es Unicode UTF-8. UTF-8 es compatible con ASCII y es la codificación que utilizan los sistemas Linux, Mac y todas las páginas Web. En cambio, el sistema operativo Windows utiliza el sistema de codificación Unicode UTF-16 que, entre otras cosas, no es compatible con ASCII. Esto complica bastante la utilización del juego de caracteres extendido en los terminales Windows.

## El tipo *char* en C

En el lenguaje C, el tipo *char* que se utiliza para representar caracteres utiliza un byte de almacenamiento y, por tanto, no permite la utilización de la codificación Unicode. 

En realidad, el tipo *char* utiliza un entero con signo de un byte y permite guardar valores entre -128 y +127. Se puede utilizar el tipo *unsigned char*, en cuyo caso permitirá valores positivos entre 0 y 255. De esta forma, se pueden aprovechar los juegos de caracteres de las codificaciones ASCII Extendidas.

Las cadenas de caracteres son arrays de caracteres individuales cuyo último elemento es un `\0`.

Cuando se escribe un literal entrecomillado en el editor de texto, se guardará en el disco con la codificación de caracteres que utilice el editor. El compilador *GCC*, por defecto, supone que los ficheros fuente de los programas están escritos con codificación UTF-8. Por ello, mi recomendación es que se use esta codificación en los ficheros fuente que se escriben en VSCode.

Los terminales de Linux o Mac utilizan por defecto el sistema de codificación UTF-8. En cambio, en Windows, a la fecha de escribir estas líneas, el terminal no soporta la codificación UTF-8 para entrada de caracteres. 

Sí que se puede usar UTF-8 para salida de cadenas. Un ejemplo podría ser el siguiente código:

```c
#include <stdio.h>
#include <windows.h>

int main() {
    SetConsoleOutputCP(65001);
    char cadena[] = "Hola, ¿qué tal?";
    printf("%s\n", cadena);
}
```

La función *SetConsoleOutput(65001)* de la librería *windows.h* lo que hace es establecer el sistema de codificación UTF-8 en el terminal en el que se está ejecutando el programa. A continuación se declara y asigna una cadena con algunos caracteres especiales. Por último se imprime en pantalla. La salida del programa en mi ordenador es la siguiente:

<img title="" src="img/print_cadena_1.PNG" alt="" width="274" data-align="center">

Hay que entender que, cuando se utiliza UTF-8, los caracteres pueden utilizar entre 1 y 4 bytes. Por ejemplo, el carácter `ñ`, consta de dos bytes: el primero un 195 y el segundo 177.

Por la forma de actuar de la función *printf()* la cadena de caracteres UTF-8 se imprime correctamente en pantalla, incluso en Windows, pero si tratamos de contar caracteres con la función *strlen()*, lo que obtendremos es el número de bytes de la cadena, no el número de caracteres. De la misma forma, si intentamos acceder a caracteres individuales mediante el índice que ocupa el carácter en la cadena, también podemos obtener resultados inesperados. Observa el siguiente código:

```c
#include <stdio.h>
#include <windows.h>
#include <string.h>

void disp_bytes(char* cad) {
    printf("Cadena: ");
    for(int i=0; i<strlen(cad); i++) {
        printf("%hhu ", cad[i]);
    }
    printf("\n");
}
int main() {

    SetConsoleOutputCP(65001);
    unsigned char cadena[] = "Año";

    printf("%s\n", cadena);
    printf("Longitud: %d\n", strlen(cadena));
    printf("cadena[1]= %c\n", cadena[1]);
    disp_bytes(cadena);
}
```

La salida de este programa en mi ordenador con Windows es la siguiente:

<img title="" src="img/print_cadena_2.PNG" alt="" width="350" data-align="center">

Observa que los dos bytes centrales de la cadena son 195 y 177, que es el código UTF-8 de la letra `ñ`.

Así como incluir caracteres de más de un byte en una cadena puede funcionar para hacer salidas por pantalla, no funciona si uno de estos caracteres se intenta asignar a una variable *char*, que solo utiliza un byte. Si intentas compilar una línea de código como la siguiente, teniendo el editor configurado en UTF-8 obtendrás un error:

```c
char c = 'ñ';
```

Tampoco es posible hacer lecturas de cadenas que contengan caracteres especiales,  a través del terminal de Windows, utilizando la codificación UTF-8 y funciones como *fgets()* o *scanf()*.

### Utilización de la codificación Windows-1252

El terminal de Windows utiliza por defecto la página de códigos 850. Se trata de una codificación antigua heredada de los viejos sistemas MS-DOS. Con posterioridad, Windows creó la codificación denominada *Windows-1252*, que es muy similar a la codificación ISO-8859-1, pero no idéntica. Se trata de un juego de caracteres extendido, que utiliza un solo byte para cada carácter y aprovecha los códigos 128 a 255 para dar soporte a las letras especiales de los alfabetos de los paises de Europa Occidental. Incluye letras acentuadas, la letra ñ y otros caracteres.

Utilizando la codificación Windows-1252 sí que es posible hacer entradas de cadenas que contengan caracteres especiales utilizando el terminal de Windows. Observa el siguiente código:

```c
#include <stdio.h>
#include <string.h>
#include <windows.h>

void disp_bytes(char* cad) {
    printf("Bytes: ");
    for(int i=0; i<strlen(cad); i++) {
        printf("%hhu ", cad[i]);
    }
    printf("\n");
}
void limpia_endofline(char* cad) {
    if(cad[strlen(cad)-1] == '\n') {
        cad[strlen(cad)-1] = '\0';
    }
}
int main() {
    SetConsoleOutputCP(1252);

    char cad[80];
    printf("Cadena: ");
    fgets(cad, 80, stdin);
    limpia_endofline(cad);
    printf("%s\n", cad);
    printf("Longitud: %d\n", strlen(cad));
    disp_bytes(cad);
}
```

El programa establece la página de códigos Windows-1252 y después le pide una cadena al usuario. La cadena se limpia del `\n`residual y luego se imprime su longitud y los bytes de los que consta. 

La figura siguiente muestra la ejecución del programa cuando el usuario teclea la cadena *Año*. Ahora la longitud de la cadena es la correcta y el código de la letra *ñ* es el 241, que es el que le corresponde en la codificación 1252.

<img title="" src="img/fgets_cadena_1.PNG" alt="" width="319" data-align="center">

Si el editor VSCode lo tenemos configurado en UTF-8 y escribimos un literal entrecomillado que contenga caracteres especiales, no podremos verlo correctamente utilizando en el terminal la codificación 1252. Observa el código siguiente:

```c
#include <stdio.h>
#include <windows.h>

int main() {

    SetConsoleOutputCP(1252);

    char cad[] = "Hola, ¿qué tal?";
    printf("%s\n", cad);
}
```

Si ejecutas ese programa, la salida será similar a la siguiente:

<img title="" src="img/printf_1252.PNG" alt="" width="294" data-align="center">

¿Qué está sucediendo? El fichero fuente que se escribe en el disco no contiene las letras del literal entrecomillado, sino los códigos que les correspondan a dichas letras en la codificación de caracteres que utilice el editor. Como el editor VSCode está configurado con UTF-8, al escribir en el disco el literal entrecomillado, escribe los códigos UTF-8 y, al intentar imprimirlos utilizando la codificaión 1252 que se ha seleccionado para el terminal, se producen errores.

Una solución podría ser utilizar la codificación 1252 también en el editor de texto y guardar en el disco el fichero fuente con la codificación 1252. Si se hace así, la salida del programa anterior sería la siguiente:

<img title="" src="img/printf_1252_2.PNG" alt="" width="287" data-align="center">

Aunque la solución de utilizar la codificación 1252 en el editor y en el terminal podría resolver determinadas situaciones, personalmente creo que no es una buena solución. Por una parte, la codificación 1252 es muy limitada y hay muchos caracteres de uso habitual y que no están incluidos, por ejemplo la letra griega *alpha*, α, o el símbolo del Euro, €. Por otra parte, hacerse incompatible con otros programadores, a la hora de intercambiar ficheros fuente, o de subirlos a un repositorio de Guthub, por ejemplo, daría más quebraderos de cabeza que los que supone utilizar otras soluciones más adecuadas para utilizar caracteres especiales.



## El tipo *wchar_t* en C

### Situación en Windows

### Situación en Mac y Linux

En principio, no hay problemas para trabajar con codificación UTF-8 en los terminales de Mac o de Linux. En el caso de los caracteres individuales, si ocupan más de un byte, como sucede con los caracteres especiales, no se podrán guardar en una variable del tipo `char`.

El siguiente ejemplo se ha ejecutado en un entorno Ubuntu Linux. Observa la situación 4: para leer correctamente un caracter individual de más de un byte se utiliza una cadea, en lugar de un `char`.

```c
#include <stdio.h>

int main() {

    // Situación 1: Literal entrecomillado
    char s[] = "Cadenas: camión, año, ¿volví?";
    printf("%s\n", s);

    // Situación 2: Lectura de cadena desde el terminal
    char t[80];
    printf("Cadena:");
    fgets(t, 80, stdin);
    printf("%s\n", t);

    // Situación 3.- Lectura de carácter desde el terminal
    printf("Carácter:");
    char c = getchar(); // Teclea un carácter especial
    printf("%c\n", c);  // La salida no es correcta para 
                        // caracteres especiales

    // Situación 4.- Lectura de carácter especial a cadena
    char c2[5]; 
    printf("Carácter:");
    while(getchar()!='\n');
    fgets(c2, 5, stdin);
    printf("%s\n", c2);
}
```
