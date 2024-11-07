# Lectura de caracteres y cadenas en C

## Lectura de caracteres

Se puede utilizar *scanf()*, con el formato `%c`, utilizando un espacio delante del formato para evitar el problema de los `\n`residuales que pudiera haber en el búfer de lectura:

```c
char caracter;
scanf(" %c", &caracter);
```

Se puede no poner el espacio delante del formato `%c`, pero si no es la primera lectura habrá que hacer un *getchar()* previo para descartar el `\n`. 

```c
#include <stdio.h>

int main() {
   char c, d;

   printf("Primer caracter: ");
   scanf("%c", &c);

   printf("Segundo caracter: ");
   getchar();
   scanf("%c", &d);

   printf("Primero: %c Segundo: %c \n", c, d);
}
```

También se puede usar *getchar()* directamente pero, si no es la primera lectura, habrá que tener cuidado con los `\n`residuales del búfer. 

```c
#include <stdio.h>

int main() {
   char c, d;

   printf("Primer caracter: ");
   c = getchar();

   printf("Segundo caracter: ");
   getchar();
   d = getchar();
      
   printf("Primero: %c Segundo: %c \n", c, d);
} 
```

## Lectura de cadenas

Se pueden hacer con *scanf()* y formato `%s`, pero habrá que tener en cuenta que para *scanf()*, los espacios indican el fin de la lectura y, por tanto, no se podrán leer cadenas que contengan espacios.

El formato `%s`funciona como los formatos numéricos y descarta directamente los `\n`residuales. No hace falta el truco del espacio delante del formato.

También hay que señalar que con *scanf()*, no hay que poner `&`delante del nombre de la variable, pues ya es un puntero:

```c
#include <stdio.h>

int main() {
   char c[40], d[40];

   printf("Primera cadena: ");
   scanf("%s", c);

   printf("Segunda cadena: ");
   scanf("%s", d);

   printf("Primera: %s Segunda: %s \n", c, d);
}
```

También se pueden leer cadenas con *gets()*. En este caso, se pueden leer cadenas con espacios, el final de la cadena lo marcan los `\n`o los `EOF` (*CTRL+Z*). 

```c
#include <stdio.h>

int main() {
   char c[40], d[40];

   printf("Primera cadena: ");
   gets(c);

   printf("Segunda cadena: ");
   gets(d);

   printf("Primera: %s Segunda: %s \n", c, d);
}

```

La función *gets()* lee el último `\n`y lo descarta, añadiendo un `\0`en su lugar a la cadena leída. Por ello, se pueden hacer varias lecturas consecutivas con *gets()* sin que haya que tomar precauciones con los `\n`residuales.

```c
#include <stdio.h>

#define DIM 8

void print_cadena(char* c) {
   printf("Cadena: ");
   for(int i=0; i<DIM; i++) {
      printf("%d ", c[i]);
   }
   printf("\n");
}

int main() {
   
   char c[DIM]="ZZZZZZZ";
   printf("Teclea cadena:");
   gets(c);

   char d[DIM]="ZZZZZZZ";
   printf("Teclea cadena: ");
   gets(d);

   printf("Cadena 1: -%s- Cadena 2: -%s-\n", c, d);

   print_cadena(c);
   print_cadena(d);
}
```

La salida de este programa muestra cómo se han leído las cadenas, añadiendo un `\0`al final de lo leído y sin dejar un `\n`residual en el bufer:

<img title="" src="file:///home/shiguera/ownCloud/Apuntes/img/2024-11-07-20-26-16-Captura%20desde%202024-11-07%2020-26-06.png" alt="" width="380" data-align="center">

Ahora bien, si ya existían `\n`residuales en el bufer, porque las anteriores lecturas se hicieron con *scanf()*, la función *gets()* leerá una cadena vacía:

```c
#include <stdio.h>

#define DIM 8

void print_cadena(char* c) {
   printf("Cadena: ");
   for(int i=0; i<DIM; i++) {
      printf("%d ", c[i]);
   }
   printf("\n");
}

int main() {
   
   char c[DIM]="ZZZZZZZ";
   printf("Teclea cadena:");
   scanf("%s", c);

   char d[DIM]="ZZZZZZZ";
   printf("Teclea cadena: ");
   gets(d);

   printf("Cadena 1: -%s- Cadena 2: -%s-\n", c, d);

   print_cadena(c);
   print_cadena(d);
}
```

<img title="" src="file:///home/shiguera/ownCloud/Apuntes/img/2024-11-07-20-30-26-Captura%20desde%202024-11-07%2020-30-07.png" alt="" width="387" data-align="center">

En la salida se observa que el *scanf()* también añade el carácter `\0`al final de lo leído. Pero deja el `\n`residual y el siguiente *gets()* lee una cadena vacía. En la segunda cadena se ve que se ha añadido un `\0`como primer carácter de la cadena leída.

Otro de los problemas de *gets()* es que se puede producir desbordamiento del búfer, si la cadena tecleada es mayor que la cadena que había preparada en memoria. En estos casos, *gets()* escribirá más alla de las posiciones de la cadena en memoria, pudiendo sobreescribir cualquier cosa que hubiera en esas posiciones de memoria.

En el siguiente código se ha modificado la función *print_cadena()* para que escriba un carácter más alla de la longitud de la cadena:

```c
#include <stdio.h>

#define DIM 4

void print_cadena(char* c) {
   // Modificado para que escriba un carácter 
   // más allá del final de la cadena
   printf("Cadena: ");
   for(int i=0; i<=DIM; i++) {
      printf("%d ", c[i]);
   }
   printf("\n");
}

int main() {
   
   char c[DIM]="ZZZ";
   printf("Teclea cadena:");
   gets(c);

   printf("Cadena: -%s- \n", c);

   print_cadena(c);
}

```

Si ejecutamos este código y tecleamos una cadena de 4 caracteres, se observa que *gets()* ha escrito el `\0`en la posición de memoria que hay a continuación del final de la cadena `c`. La cadena la podemos imprimir, porque sigue teniendo el `\0`final, pero lo que hubiera ahí, se lo ha cargado:

<img title="" src="file:///home/shiguera/ownCloud/Apuntes/img/2024-11-07-20-38-50-Captura%20desde%202024-11-07%2020-38-36.png" alt="" width="400" data-align="center">

Por este y otro motivos, la función *gets()* se descartó de C a partir de C11 y, a día de hoy, se recomienda utilizar *fgets()*. De hecho, cuando se compila un programa usando *gets()* el compilador emite un warning.










