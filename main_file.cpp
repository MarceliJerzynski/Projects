/*
Niniejszy program jest wolnym oprogramowaniem; możesz go
rozprowadzać dalej i / lub modyfikować na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundację Wolnego
Oprogramowania - według wersji 2 tej Licencji lub(według twojego
wyboru) którejś z późniejszych wersji.

Niniejszy program rozpowszechniany jest z nadzieją, iż będzie on
użyteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyślnej
gwarancji PRZYDATNOŚCI HANDLOWEJ albo PRZYDATNOŚCI DO OKREŚLONYCH
ZASTOSOWAŃ.W celu uzyskania bliższych informacji sięgnij do
Powszechnej Licencji Publicznej GNU.

Z pewnością wraz z niniejszym programem otrzymałeś też egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
jeśli nie - napisz do Free Software Foundation, Inc., 59 Temple
Place, Fifth Floor, Boston, MA  02110 - 1301  USA
*/

#define GLM_FORCE_RADIANS
#define GLM_FORCE_SWIZZLE

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "constants.h"
#include "lodepng.h"
#include "shaderprogram.h"
#include "Object.h"
#include "Car.h"
#include "OBJLoader.h"
#include "Markup.h"
using namespace glm;
using namespace std;
const int WIDTH = 800;
const int HEIGHT= 800;
const float RED =  0.529;
const float GREEN = 0.808;
const float BLUE = 0.808;
const char* title = "Racing Game";
const int FPS = 60;
const float max_angle = 90;
const float changing_angle = 0.315;

float distance_to_player = 9;
float pitch_angle = 15;
float yaw_angle = 0;


float angle_around_player = 0;


float aspectRatio=1;
float turn_angle=0.75;
float speed_angle = 0;

bool turnLeft  = false;
bool turnRight = false;
bool goPlayer = false;
bool backPlayer=false;

ShaderProgram *sp;

float toRadians(float angle)
{
    return angle*3.14f/180.0f;
}

//Procedura obsługi błędów
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}


void keyCallback(GLFWwindow* window,int key,int scancode,int action,int mods) {
    if (action==GLFW_PRESS) {
        if (key==GLFW_KEY_A) turnLeft = true;
        if (key==GLFW_KEY_D) turnRight = true;
        if (key==GLFW_KEY_W) goPlayer = true;
        if (key==GLFW_KEY_S) backPlayer = true;
        if (key==GLFW_KEY_V) speed_angle = 2* turn_angle;
    }
    if (action==GLFW_RELEASE) {
        if (key==GLFW_KEY_A) turnLeft = false;
        if (key==GLFW_KEY_D) turnRight = false;
        if (key==GLFW_KEY_W) goPlayer = false;
        if (key==GLFW_KEY_S) backPlayer = false;
         if (key==GLFW_KEY_V) speed_angle = 0;
    }
}

void windowResizeCallback(GLFWwindow* window,int width,int height) {
    if (height==0) return;
    aspectRatio=(float)width/(float)height;
    glViewport(0,0,width,height);
}

//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {
	//************Tutaj umieszczaj kod, który należy wykonać raz, na początku programu************
	glClearColor(RED,GREEN,BLUE,1);
	glEnable(GL_DEPTH_TEST);
	glfwSetWindowSizeCallback(window,windowResizeCallback);
	glfwSetKeyCallback(window,keyCallback);

	sp=new ShaderProgram("vertex.glsl",NULL,"fragment.glsl");
}


//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {
    //************Tutaj umieszczaj kod, który należy wykonać po zakończeniu pętli głównej************

    delete sp;
}

void setCamera(mat4 &V, Car player)
{
    float Camera_y = distance_to_player * sin(toRadians(pitch_angle));
    float Camera_x = distance_to_player * cos(toRadians(pitch_angle));

    float angle = player.getRotation() + toRadians(angle_around_player);
    float offsetX = Camera_x * sin(angle);
    float offsetZ = Camera_x * cos(angle);

    vec3 camera_position;
    camera_position.x = player.getPosition().x - offsetX;
    camera_position.y = player.getPosition().y + Camera_y;
    camera_position.z = player.getPosition().z - offsetZ;

    V = lookAt(
               camera_position,
               player.getPosition(),
               vec3(0, 1, 0 )
              );
}


//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window,mat4 &V, mat4 &P, Object &cube, Car &player, Object tree[20]) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    sp->use();

//Swiatla
//----------------------------------------------------------------------------------------------------------------------
    vec3 light_position = vec3(0,500,0);
    glUniform4f(sp->u("lp"),light_position.x,light_position.y,light_position.z,1); //Współrzędne źródła światła
//----------------------------------------------------------------------------------------------------------------------


//Renderowanie obiektów
//----------------------------------------------------------------------------------------------------------------------
    player.render(V, P, sp);
    player.getMarkup()->getArrow()->render(V, P, sp);
    cube.render(V, P, sp);

    if (player.checkpointReached())
    {
        cout<<"REACHED"<<endl;
    }

    for(int i = 0 ; i < 20; i++)
    {
        tree[i].render(V, P, sp);
    }
//----------------------------------------------------------------------------------------------------------------------

//   if ( markup.touched(player))
 //   {
  //     markup.changePosition(vec3(0,0,markup.getPosition().z-10));
 //   }


    glfwSwapBuffers(window); //Przerzuć tylny bufor na przedni
}

void moving(mat4 &V,  Car &player)
{
    bool camera_back = true;
    if (goPlayer)       //jesli trzyma W
    {
        player.move(1);  //rusz gracza
    } else
    if (backPlayer)     //jesli trzyma S
    {
        player.move(2);
    } else
    {
        player.move(0);
    }

    //obsluga kamery przy poruszaniu sie
    if (player.isMoving() == 1)
    {
        if (turnLeft)   //i jednoczesnie A
        {
            player.turn(turn_angle);    //skrec gracza
            if ( angle_around_player >= -max_angle)  //skrec kamere
            {
                if ( speed_angle == 0)
                angle_around_player -= changing_angle;
                if ( angle_around_player < 0)
                    camera_back = false;
            }
        } else
	    if (turnRight) //i jednoczesnie D
        {
            player.turn(-turn_angle);   //skrec gracza
            if (angle_around_player <= max_angle)    //skrec kamere
            {
                if ( speed_angle == 0)
                angle_around_player += changing_angle;
                if (angle_around_player > 0)
                    camera_back = false;
            }
        }
    } else
    if (player.isMoving() == -1)
    {
        if (turnLeft)   //i jednoczesnie A
        {
            player.turn(-turn_angle);   //skrec gracza
            if (angle_around_player < max_angle)    //skrec kamere
            {
                if ( speed_angle == 0)
                angle_around_player += changing_angle;
                if (angle_around_player > 0)
                    camera_back = false;
            }
        } else
        if (turnRight)  //i jednoczesnie D
        {
            player.turn(turn_angle);
            if ( angle_around_player > -max_angle)  //skrec kamere
            {
                if ( speed_angle == 0)
                angle_around_player -=  changing_angle;
                if ( angle_around_player < 0)
                    camera_back = false;
            }
        }
    }


         if (camera_back)
        {
            if (angle_around_player > 0)
            {
                angle_around_player -= 2*changing_angle;
            }
            if (angle_around_player < 0)
            {
                angle_around_player += 2*changing_angle;
            }
        }
        if (angle_around_player > 0)
            angle_around_player -= speed_angle;
        if (angle_around_player < 0)
            angle_around_player +=speed_angle;


        if (turnLeft)
        {
            player.turnWheel(3*turn_angle);
        }
        if (turnRight)
        {
            player.turnWheel(-3*turn_angle);
        }
        if (!turnLeft && !turnRight)  //prostuj koła
        {
            if (player.getWheelRotation() > 0)
            {
                player.turnWheel(-3*turn_angle);
            }
            if (player.getWheelRotation() < 0)
            {
                player.turnWheel(3*turn_angle);
            }
        }

    setCamera(V, player);

}

GLFWwindow* openGlstuff()
{
    GLFWwindow* window;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) { //Zainicjuj bibliotekę GLFW
		fprintf(stderr, "Nie można zainicjować GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(WIDTH, HEIGHT, title, NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.

	if (!window) //Jeżeli okna nie udało się utworzyć, to zamknij program
	{
		fprintf(stderr, "Nie można utworzyć okna.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje się aktywny i polecenia OpenGL będą dotyczyć właśnie jego.
	glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora

	if (glewInit() != GLEW_OK) { //Zainicjuj bibliotekę GLEW
		fprintf(stderr, "Nie można zainicjować GLEW.\n");
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window);

	return window;
}

int main(void)
{
	GLFWwindow* window; //Wskaźnik na obiekt reprezentujący okno
    window = openGlstuff();


//Tworzenie obiektów
//----------------------------------------------------------------------------------------------------------------------
	Car player;
    player.loadFromPath("BODY.obj", "wheel.obj",0.01,0.05 ,vec3(0.0f,0.0f,10.0f), 0.0f,0.0f,0.0f,1.0f);
    player.getMarkup()->loadMarkup(0.2);
    Object cube;
    cube.loadFromPath("cube.obj", vec3(0.0f,-5000.0f,0.0f), 0.0f, 0.0f, 0.0f, 10000.0f);

    OBJLoader loader;
    loader.load("Tree.obj");

    Object tree[20];
    int i = 0;
    for(i = 0 ; i < 20; i++)
    {
        tree[i].loadFromLoader(loader, vec3(5*i,0,20*i), 0,0,0,2);
    }

//----------------------------------------------------------------------------------------------------------------------

//macierze P i V
//----------------------------------------------------------------------------------------------------------------------
    mat4 P=perspective(50.0f*PI/180.0f, aspectRatio, 0.01f, 500.0f); //Wylicz macierz rzutowania
    mat4 V = mat4(1.0f);
//----------------------------------------------------------------------------------------------------------------------

    setCamera(V, player);
	glfwSetTime(0); //Zeruj timer

//Pętla główna gry
//----------------------------------------------------------------------------------------------------------------------
	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{
        glfwSetTime(0); //Zeruj timer
		drawScene(window, V, P, cube, player, tree); //Wykonaj procedurę rysującą
        moving(V, player);                                   //wykonaj procedurę odpowiajająca za poruszanie graczem oraz kamerą
		glfwPollEvents();                                    //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
		while(glfwGetTime() < 1/FPS) {}                      //Zapewnij stałe 60FPS
	}
//----------------------------------------------------------------------------------------------------------------------


//Popierdółki
//----------------------------------------------------------------------------------------------------------------------
	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
//----------------------------------------------------------------------------------------------------------------------
}
