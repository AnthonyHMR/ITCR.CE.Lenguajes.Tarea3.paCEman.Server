#include "flujo.h"
void setPartida(struct partida partida_, int ghosts, int frutas, int puntaje, int vidas){
    struct partida partida = {ghosts, 1, frutas,  {0, 0}};
    partida_ = partida;
    partida_.jugador = {vidas, puntaje};
}