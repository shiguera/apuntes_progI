#include <stdio.h>
#include <windows.h>

int main() {

    SetConsoleOutputCP(1252);

    char cad[] = "Hola, �qu� tal?";
    printf("%s\n", cad);
}