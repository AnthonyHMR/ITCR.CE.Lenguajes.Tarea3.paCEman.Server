/*
*jugador es una estructura que almacena los valores de
* vidas y puntaje que posee un jugador a lo largo del juego
*/
struct jugador{
    int puntaje;
    int vidas;
};
/*
*partida es una estructura que almacena las cantidades de
* inicio para el cliente: ghosts, nivel, frutas, jugador.
*/
struct partida{
    int ghosts;
    int nivel;
    int frutas;
    struct jugador jugador;
};
//Metodo para inicializar una partida
void setPartida(struct partida partida_, int ghosts, int frutas, int puntaje, int vidas);