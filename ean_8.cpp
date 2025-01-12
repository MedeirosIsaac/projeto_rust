#include <string>

using namespace std;

const string L_CODE[10] = {"0001101", "0011001", "0010011", "0111101", "0100011", "0110001", "0101111", "0111011", "0110111", "0001011"};
const string R_CODE[10] = {"1110010", "1100110", "1101100", "1000010", "1011100", "1001110", "1010000", "1000100", "1001000", "1110100"};

bool validar_digito_verificador(int identificador[8]) {
    int soma_impares = 0, soma_pares = 0;
    for (int i = 1; i < 8; i++) {
        if (i % 2 == 0) soma_pares += identificador[i-1];
        else soma_impares += identificador[i-1];
    }
    return (10 - (soma_pares * 1 + soma_impares * 3) % 10) % 10 == identificador[7];
}