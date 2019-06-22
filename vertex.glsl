#version 330

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform vec4 lp; //Współrzędne źródła światła w przestrzeni świata

//Atrybuty
in vec4 vertex; //wspolrzedne wierzcholka w przestrzeni modelu
in vec4 normal; //wspolrzedne wektora normalnego w przestrzeni modelu
in vec2 texCoord; //kolor wierzchołka

//Zmienne interpolowane
out vec2 iTexCoord;
out vec4 l;
out vec4 n;
out vec4 v;

void main(void) {

    l=normalize(V*(lp-M*vertex)); //Wektor "do œwiat³a" w przestrzeni oka
    n=normalize(V*M*normal); //Wektor normalny w przestrzeni oka
    v=normalize(vec4(0,0,0,1)-V*M*vertex); //Wektor "do obserwatora" w przestrzeni oka

    iTexCoord = texCoord;
    gl_Position=P*V*M*vertex;
}
