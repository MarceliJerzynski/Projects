#version 330


//Zmienne interpolowane
in vec4 ic;
in vec4 l;
in vec4 n;
in vec4 v;


out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela

void main(void) {
    vec4 kd=ic; //Kolor materia³u dla œwiat³a rozpraszanego
    vec4 ld=vec4(1,1,1,1); //Kolor œwiat³a rozpraszanwego
    vec4 ks=vec4(1,1,1,1); //Kolor odbiæ materia³u
    vec4 ls=vec4(1,1,1,1); //Kolor œwiat³a odbijanego

    vec4 ml=normalize(l);
    vec4 mn=normalize(n);
    vec4 mv=normalize(v);
    vec4 mr=reflect(-ml,mn); //Wektor kierunku odbicia w przestrzeni oka

    float nl=clamp(dot(mn,ml),0,1); //cos k¹ta pomiêdzy wektorami n i l
    float rv=pow(clamp(dot(mr,mv),0,1),25); //cos k¹ta pomiêdzy wektorami r i v podniesiony do potêgi (wyk³adnik Phonga)

	pixelColor=vec4(kd.rgb*ld.rgb*nl+ks.rgb*ls.rgb*rv,kd.a);
}
