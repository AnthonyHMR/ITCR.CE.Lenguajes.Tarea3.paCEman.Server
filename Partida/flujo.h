
struct jugador{
    int puntaje;
    int vidas;
};
struct partida{
    int ghosts;
    int nivel;
    int frutas;
    struct jugador jugador;
};

void setPartida(struct partida partida_, int ghosts, int frutas, int puntaje, int vidas);