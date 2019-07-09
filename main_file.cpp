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
#include <cmath>
#include "constants.h"
#include "lodepng.h"
#include "shaderprogram.h"
#include "Object.h"
#include "Car.h"
#include "OBJLoader.h"
#include "Track.h"
#include <windows.h>
using namespace glm;
using namespace std;
const  int WIDTH = 800;
const  int HEIGHT= 800;
const  float RED =  0.529;
const  float GREEN = 0.808;
const  float BLUE = 0.808;
const char* title = "Racing Game";
const  int FPS = 60;
const  int amount_of_trees = 10;
const  int amount_of_lights = 4;
const  float max_angle = 10;
const  float changing_angle = 0.75;
const  int enemyCount =4;

float distance_to_player = 9;
float pitch_angle = 15;
//float yaw_angle = 0;


float angle_around_player = 0;


float aspectRatio=1;
float speed_angle = 0;

bool turnLeft  = false;
bool turnRight = false;
bool goPlayer = false;
bool backPlayer=false;

ShaderProgram *sp;


bool collision(Car &car, Object &object)
{
    if ( pow(car.getBody()->getRadius() - object.getRadius(), 2) <
        pow(car.getBody()->getPosition().x - object.getPosition().x, 2) +
        pow(car.getBody()->getPosition().z - object.getPosition().z, 2)
        )
    {
        if ( pow(car.getBody()->getRadius() + object.getRadius(), 2) >
            pow(car.getBody()->getPosition().x - object.getPosition().x, 2) +
            pow(car.getBody()->getPosition().z - object.getPosition().z, 2)
             )
                {
                    return true;
                }
    }

    return false;
}

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
void drawScene(GLFWwindow* window,mat4 &V, mat4 &P, Object &cube,Object &track, Car &player, Object tree[amount_of_trees], Car *enemy) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    sp->use();

//Swiatla
//----------------------------------------------------------------------------------------------------------------------

   float light_position[] = {
    0, 10, -10, 1,
    0, 10000, 0, 1,
    0, 10000, 10, 1,
    0, 10000,20, 1
    };

    glUniform4fv(sp->u("lp"), amount_of_lights, light_position);
//----------------------------------------------------------------------------------------------------------------------


//Renderowanie obiektów
//----------------------------------------------------------------------------------------------------------------------
    player.render(V, P, sp);
    player.getMarkup()->getArrow()->render(V, P, sp);


    for(int i=0;i<enemyCount;i++)
    {
    enemy[i].render(V, P, sp);
    }

    cube.render(V, P, sp);
    track.render(V, P, sp);

    if (player.checkpointReached())
    {
        Beep(1000, 200);
    }

    for(int i = 0 ; i < amount_of_trees; i++)
    {
        tree[i].render(V, P, sp);
    }

//----------------------------------------------------------------------------------------------------------------------


    glfwSwapBuffers(window); //Przerzuć tylny bufor na przedni
}

void moving(mat4 &V,  Car &player)
{
    bool camera_back = true;
    //obsluga kamery przy poruszaniu sie
    if (player.isMoving() == 1)
    {
        if (turnLeft)   //i jednoczesnie A
        {
            player.turnLeft();    //skrec gracza
            if ( angle_around_player >= -max_angle)  //skrec kamere
            {
                if ( speed_angle == 0)
                angle_around_player -= changing_angle;
            }
        camera_back = false;
        } else
	    if (turnRight) //i jednoczesnie D
        {
            player.turnRight();   //skrec gracza
            if (angle_around_player <= max_angle)    //skrec kamere
            {
                if ( speed_angle == 0)
                angle_around_player += changing_angle;
            }
        camera_back = false;
        }
    } else
    if (player.isMoving() == -1)
    {
        if (turnLeft)   //i jednoczesnie A
        {
            player.turnRight();   //skrec gracza
            if (angle_around_player < max_angle)    //skrec kamere
            {
                if ( speed_angle == 0)
                angle_around_player += changing_angle;
            }
            camera_back = false;
        } else
        if (turnRight)  //i jednoczesnie D
        {
            player.turnLeft();
            if ( angle_around_player > -max_angle)  //skrec kamere
            {
                if ( speed_angle == 0)
                angle_around_player -=  changing_angle;
            }
        camera_back = false;
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
            player.turnWheelLeft();
        }
        if (turnRight)
        {
            player.turnWheelRight();
        }
        if (!turnLeft && !turnRight)  //prostuj koła
        {
            if (player.getWheelRotation() > 0)
            {
                player.turnWheelRight();
            }
            if (player.getWheelRotation() < 0)
            {
                player.turnWheelLeft();
            }
        }

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



    setCamera(V, player);

}


void game( Object &cube, Object &track,Car &player,Object tree[amount_of_trees], Car *enemy)

{

    float help;

    for(int i=0;i<enemyCount;i++)
    {
    enemy[i].AI();
    enemy[i].checkpointReached();
    }

    for(int i = 0 ; i < amount_of_trees; i++)   //kolizja z drzewami
    {
        if ( collision(player, tree[i])   )
        {

            player.setV(-0.2);
        }
    }

    for(int i = 0 ; i < enemyCount; i++)
    {
        if ( collision(player, *enemy[i].getBody() )  ) //kolizja gracza z enemy
        {
            help = player.getV();
            player.setV( enemy[i].getV() );
            enemy[i].setV(help);
        }
    }

    for(int i = 0; i < enemyCount -1; i++)
    {
        for(int j = 0; j < enemyCount -1; j++)
        {
            if ( collision( enemy[j], *enemy[j+1].getBody()))   //kolizja enemy z enemy
            {
            help = enemy[j].getV();
            enemy[j].setV( enemy[j+1].getV() );
            enemy[j+1].setV(help);
            }
        }
    }


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
    Object track;
    track.loadFromPath("track.obj","tracktexture.png", vec3(0.0f,-2.0f,0.0f), -90.0f, 0.0f, 0.0f, 2.0f);


    Object cube;
    cube.loadFromPath("cube.obj","Grass.png", vec3(0.0f,-102.0f,0.0f), -90.0f, 0.0f, 0.0f, 200.0f);

    Car *enemy=new Car[enemyCount];

    for(int i=0;i<enemyCount;i = i + 2)
    {
    enemy[i].loadFromPath("body.obj","chassis.obj","headlit.obj","license.obj", "wheel.obj","test.png","as3.png","s3cos.png","license.png","texWhee1.png", 0.01,0.05 ,vec3(25.0f,0.0f,0-5.0f*i), 0.0f,0.0f,0.0f,1.0f);
    enemy[i].getMarkup()->loadMarkup(0.2);
    }
    for(int i=1;i<enemyCount;i = i + 2)
    {
    enemy[i].loadFromPath("body.obj","chassis.obj","headlit.obj","license.obj", "wheel.obj","test.png","as3.png","s3cos.png","license.png","texWhee1.png", 0.01,0.05 ,vec3(30.0f,0.0f,0-5.0f*(i-1)), 0.0f,0.0f,0.0f,1.0f);
    enemy[i].getMarkup()->loadMarkup(0.2);
    }

    Car player;
    player.loadFromPath("body.obj","chassis.obj","headlit.obj","license.obj", "wheel.obj","test.png","as3.png","s3cos.png","license.png","texWhee1.png", 0.01,0.05 ,vec3(25.0f,0.0f,10.0f), 0.0f,0.0f,0.0f,1.0f);
    player.getMarkup()->loadMarkup(0.2);


    OBJLoader loader;
    loader.load("Tree.obj");

    Object tree[amount_of_trees];
    int i = 0;
    for(i = 0 ; i < amount_of_trees; i++)
    {
        tree[i].loadFromLoader(loader,"bricks.png", vec3(0,-2,65-15*i), 0,0,0,2);
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
		drawScene(window, V, P, cube,track, player, tree, enemy); //Wykonaj procedurę rysującą
        moving(V, player);                                   //wykonaj procedurę odpowiajająca za poruszanie graczem oraz kamerą
        game(cube,track, player, tree, enemy);
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
