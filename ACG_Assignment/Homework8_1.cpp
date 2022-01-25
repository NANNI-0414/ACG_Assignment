// Bezier Curve

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


using namespace std;
void myKeyboard(unsigned char key, int x, int y);

GLuint programID;

GLfloat u = 0.0f;       //시간에 따른 이동을 나타냄
GLfloat du = 0.015f;    //시간에 따른 변화량

GLfloat trans[16] = { 1.0f, 0.0f, 0.0f, 0.0f,
                    0.0f, 1.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, 1.0f, 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f };

GLfloat ctrlP0[] = { -0.6f, -0.5f, 1.0f, 1.0f };
GLfloat ctrlP1[] = { -0.3f, 0.3f, 1.0f, 1.0f };
GLfloat ctrlP2[] = { 0.3f, 0.3f, 1.0f, 1.0f };
GLfloat ctrlP3[] = { 0.6f, -0.5, 1.0f, 1.0f };


GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path)
{
    //create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    GLint Result = GL_FALSE;
    int InfoLogLength;

    //Read the vertex shader code from the file
    string VertexShaderCode;
    ifstream VertexShaderStream(vertex_file_path, ios::in);
    if (VertexShaderStream.is_open())
    {
        string Line = "";
        while (getline(VertexShaderStream, Line))
            VertexShaderCode += "\n" + Line;
        VertexShaderStream.close();
    }

    //Compile Vertex Shader
    printf("Compiling shader : %s\n", vertex_file_path);
    char const* VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
    glCompileShader(VertexShaderID);

    //Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    vector<char> VertexShaderErrorMessage(InfoLogLength);
    glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, VertexShaderErrorMessage.data());
    fprintf(stdout, "%s\n", VertexShaderErrorMessage.data());

    //Read the fragment shader code from the file
    string FragmentShaderCode;
    ifstream FragmentShaderStream(fragment_file_path, ios::in);
    if (FragmentShaderStream.is_open())
    {
        string Line = "";
        while (getline(FragmentShaderStream, Line))
            FragmentShaderCode += "\n" + Line;
        FragmentShaderStream.close();
    }

    //Compile Fragment Shader
    printf("Compiling shader : %s\n", fragment_file_path);
    char const* FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
    glCompileShader(FragmentShaderID);

    //Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    vector<char> FragmentShaderErrorMessage(InfoLogLength);
    glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, FragmentShaderErrorMessage.data());
    fprintf(stdout, "%s\n", FragmentShaderErrorMessage.data());

    //Link the program
    fprintf(stdout, "Linking program\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    vector<char> ProgramErrorMessage(max(InfoLogLength, int(1)));
    glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, ProgramErrorMessage.data());
    fprintf(stdout, "%s\n", ProgramErrorMessage.data());

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    return ProgramID;
}


void renderScene(void)
{
    //Clear all pixels
    glClear(GL_COLOR_BUFFER_BIT);

    //Let's draw something here
    //uniform으로 trans 행렬을 vertex shader로 넘기기
    GLuint matLoc = glGetUniformLocation(programID, "trans");
    glUniformMatrix4fv(matLoc, 1, GL_FALSE, trans);

    //define the size of point and draw a point.
    glDrawArrays(GL_TRIANGLES, 0, 36);

    //Double buffer
    glutSwapBuffers();
}


void init()
{
    //initilize the glew and check the errors.
    GLenum res = glewInit();
    if (res != GLEW_OK)
    {
        fprintf(stderr, "Error: '%s' \n", glewGetErrorString(res));
    }

    //select the background color
    glClearColor(0.373, 0.0, 1.0, 1.0);
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

}


void Timer(int value) {

    //dx, trans, ctrl point
    if (u <= 1.0f) {

        GLfloat b0 = (1.0f - u) * (1.0f - u) * (1.0f - u);
        GLfloat b1 = 3 * u * (1.0f - u);
        GLfloat b2 = 3 * u * u * (1.0f - u);
        GLfloat b3 = u * u * u;

        //x좌표 이동량, 변수 추가하기
        trans[12] = b0 * ctrlP0[0] + b1 * ctrlP1[0] + b2 * ctrlP3[0] + b3 * ctrlP3[0];
        trans[13] = b0 * ctrlP0[1] + b1 * ctrlP1[1] + b2 * ctrlP3[1] + b3 * ctrlP3[1];

        u += du;

    }

    glutPostRedisplay();
    glutTimerFunc(30, Timer, 1);
}


//'z'키를 누르면 ctrl point의 y좌표들의 부호가 반대가 된다.
void myKeyboard(unsigned char key, int x, int y) {
    if (key == 'z') {
        u = 0;

        ctrlP0[1] = -ctrlP0[1];
        ctrlP1[1] = -ctrlP1[1];
        ctrlP2[1] = -ctrlP2[1];
        ctrlP3[1] = -ctrlP3[1];

        glutPostRedisplay();
    }
}


int main(int argc, char** argv)
{
    //cube position
    GLfloat cubeVertices[] = {
    0.1f, 0.1f,-0.1f, // triangle 2 : begin
    -0.1f,-0.1f,-0.1f,
    -0.1f, 0.1f,-0.1f, // triangle 2 : end
    0.1f, 0.1f,-0.1f,
    0.1f,-0.1f,-0.1f,
    -0.1f,-0.1f,-0.1f,

    -0.1f,-0.1f,-0.1f, // triangle 1 : begin
    -0.1f,-0.1f, 0.1f,
    -0.1f, 0.1f, 0.1f, // triangle 1 : end
    -0.1f,-0.1f,-0.1f,
    -0.1f, 0.1f, 0.1f,
    -0.1f, 0.1f,-0.1f,

    -0.1f, 0.1f, 0.1f,
    -0.1f,-0.1f, 0.1f,
    0.1f,-0.1f, 0.1f,
    0.1f, 0.1f, 0.1f,
    -0.1f, 0.1f, 0.1f,
    0.1f,-0.1f, 0.1f,

    0.1f, 0.1f, 0.1f,
    0.1f,-0.1f,-0.1f,
    0.1f, 0.1f,-0.1f,
    0.1f,-0.1f,-0.1f,
    0.1f, 0.1f, 0.1f,
    0.1f,-0.1f, 0.1f,

    0.1f,-0.1f, 0.1f,
    -0.1f,-0.1f,-0.1f,
    0.1f,-0.1f,-0.1f,
    0.1f,-0.1f, 0.1f,
    -0.1f,-0.1f, 0.1f,
    -0.1f,-0.1f,-0.1f,

    0.1f, 0.1f, 0.1f,
    0.1f, 0.1f,-0.1f,
    -0.1f, 0.1f,-0.1f,
    0.1f, 0.1f, 0.1f,
    -0.1f, 0.1f,-0.1f,
    -0.1f, 0.1f, 0.1f,
    };
    //cube color
    GLfloat cubeColors[] = {
        1.0, 0.0, 0.0, 1.0,
        1.0, 0.0, 0.0, 1.0,
        1.0, 0.0, 0.0, 1.0,
        1.0, 0.0, 0.0, 1.0,
        1.0, 0.0, 0.0, 1.0,
        1.0, 0.0, 0.0, 1.0,		//삼각형 2개로 이루어진 큐브 한 면의 점 색깔

        1.0, 0.0, 0.0, 1.0,
        1.0, 0.0, 0.0, 1.0,
        1.0, 0.0, 0.0, 1.0,
        1.0, 0.0, 0.0, 1.0,
        1.0, 0.0, 0.0, 1.0,
        1.0, 0.0, 0.0, 1.0,		//2

        1.0, 0.0, 0.0, 1.0,
        1.0, 0.0, 0.0, 1.0,
        1.0, 0.0, 0.0, 1.0,
        1.0, 0.0, 0.0, 1.0,
        1.0, 0.0, 0.0, 1.0,
        1.0, 0.0, 0.0, 1.0,		//3

        1.0, 0.0, 0.0, 1.0,
        1.0, 0.0, 0.0, 1.0,
        1.0, 0.0, 0.0, 1.0,
        1.0, 0.0, 0.0, 1.0,
        1.0, 0.0, 0.0, 1.0,
        1.0, 0.0, 0.0, 1.0,		//4

        1.0, 0.0, 0.0, 1.0,
        1.0, 0.0, 0.0, 1.0,
        1.0, 0.0, 0.0, 1.0,
        1.0, 0.0, 0.0, 1.0,
        1.0, 0.0, 0.0, 1.0,
        1.0, 0.0, 0.0, 1.0,		//5

        1.0, 0.0, 0.0, 1.0,
        1.0, 0.0, 0.0, 1.0,
        1.0, 0.0, 0.0, 1.0,
        1.0, 0.0, 0.0, 1.0,
        1.0, 0.0, 0.0, 1.0,
        1.0, 0.0, 0.0, 1.0		//6
    };


    //init GLUT and create Window
    //initialize the GLUT
    glutInit(&argc, argv);
    //GLUT_DOUBLE enables double buffering (drawing to a background buffer while the other buffer is displayed)
    glutInitDisplayMode(/* GLUT_3_2_CORE_PROFILE | */ GLUT_DOUBLE | GLUT_RGBA);
    //These two functions are used to define the position and size of the window. 
    glutInitWindowPosition(200, 200);
    glutInitWindowSize(480, 480);
    //This is used to define the name of the window.
    glutCreateWindow("Bezier Animation");

    //call initization function
    init();

    //1.
    //Generate VAO
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    //3. 
    programID = LoadShaders("VertexShader8_1.txt", "FragmentShader8_1.txt");
    glUseProgram(programID);


    //Create VAO
    GLuint VAO[1];
    glGenVertexArrays(1, VAO);
    //Bind VAO
    glBindVertexArray(VAO[0]);

    //cube관련
    //좌표전달
    //Create VBO
    GLuint cubeVBO[1];
    glGenBuffers(1, cubeVBO);	//vertex buffer
    //Bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO[0]);
    //Put data
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    //Set attribute pointer
    GLint cubeAttribLoc = glGetAttribLocation(programID, "cubePosition");
    glVertexAttribPointer(cubeAttribLoc, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(0));
    //Enable attribute arrays
    glEnableVertexAttribArray(cubeAttribLoc);


    //Create color VBO
    GLuint cubeColorVBO[1];
    glGenBuffers(1, cubeColorVBO);	//color buffer
    //Bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, cubeColorVBO[0]);
    //put data
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeColors), cubeColors, GL_STATIC_DRAW);
    //Set Attribute pointer
    GLint cubeColorLoc = glGetAttribLocation(programID, "cubeColor");
    glVertexAttribPointer(cubeColorLoc, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(0));
    //Enable attrib arrays
    glEnableVertexAttribArray(cubeColorLoc);

    glutDisplayFunc(renderScene);
    glutKeyboardFunc(myKeyboard);
    glutTimerFunc(30, Timer, 1);


    //enter GLUT event processing cycle
    glutMainLoop();

    glDeleteVertexArrays(1, &VertexArrayID);

    return 1;
}
