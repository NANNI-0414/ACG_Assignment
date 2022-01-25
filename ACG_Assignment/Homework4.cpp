
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include <GL/glew.h>
#include <GL/glut.h>


using namespace std;


GLuint LoadShaders(const char* vertex_file_path, const char* geometry_file_path, const char* fragment_file_path)
{
    //create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLint GeometryShaderID = glCreateShader(GL_GEOMETRY_SHADER);
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


    //Read the geometry shader code from the file
    string GeometryShaderCode;
    ifstream GeometryShaderStream(geometry_file_path, ios::in);
    if (GeometryShaderStream.is_open())
    {
        string Line = "";
        while (getline(GeometryShaderStream, Line))
            GeometryShaderCode += "\n" + Line;
        GeometryShaderStream.close();
    }

    //Compile Geometry Shader
    printf("Compiling shader : %s\n", geometry_file_path);
    char const* GeometrySourcePointer = GeometryShaderCode.c_str();
    glShaderSource(GeometryShaderID, 1, &GeometrySourcePointer, NULL);
    glCompileShader(GeometryShaderID);

    //Check Geometry Shader
    glGetShaderiv(GeometryShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(GeometryShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    vector<char> GeometryShaderErrorMessage(InfoLogLength);
    glGetShaderInfoLog(GeometryShaderID, InfoLogLength, NULL, GeometryShaderErrorMessage.data());
    fprintf(stdout, "%s\n", GeometryShaderErrorMessage.data());


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
    glAttachShader(ProgramID, GeometryShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    vector<char> ProgramErrorMessage(max(InfoLogLength, int(1)));
    glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, ProgramErrorMessage.data());
    fprintf(stdout, "%s\n", ProgramErrorMessage.data());

    glDeleteShader(VertexShaderID);
    glDeleteShader(GeometryShaderID);
    glDeleteShader(FragmentShaderID);

    return ProgramID;
}


void renderScene(void)
{
    //Clear all pixels
    glClear(GL_COLOR_BUFFER_BIT);
    //Let's draw something here

    //define the size of point and draw a point.
    glDrawArrays(GL_LINES_ADJACENCY, 0, 4);

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


int main(int argc, char** argv)
{
    GLfloat pointVertices[] = {
        -0.6f, -0.5f, 1.0f,
        -0.3f, 0.7f, 1.0f,
        0.3f, -0.5f, 1.0f,
        0.6f, 0.5f, 1.0f
    };
    GLfloat colorVertices[] = {
        1.0f, 1.0f, 0.0f
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
    glutCreateWindow("증거물2: 금발의 머리카락");

    //call initization function
    init();

    //1.
    //Generate VAO
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    //3. 
    GLuint programID = LoadShaders("VertexShader4.txt", "GeometryShader4.txt", "FragmentShader4.txt");
    glUseProgram(programID);


    //Create VAO
    GLuint VAO[1];
    glGenVertexArrays(1, VAO);
    //Bind VAO
    glBindVertexArray(VAO[0]);

    //Create VBO
    GLuint VBO[1];
    glGenBuffers(1, VBO);
    //Bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    //Put Data
    glBufferData(GL_ARRAY_BUFFER, sizeof(pointVertices), pointVertices, GL_STATIC_DRAW);
    //Set attribute pointer
    GLint posAttribLoc = glGetAttribLocation(programID, "position");
    glVertexAttribPointer(posAttribLoc, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(0));
    //Enable attrib arrays
    glEnableVertexAttribArray(posAttribLoc);

    //Create Color VBO
    GLuint color_VBO[1];
    glGenBuffers(1, color_VBO);	//color buffer
    //Bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, color_VBO[0]);
    //Put Data
    glBufferData(GL_ARRAY_BUFFER, sizeof(colorVertices), colorVertices, GL_STATIC_DRAW);
    //Set attribute pointer
    GLint fColor = glGetAttribLocation(programID, "color");
    glVertexAttribPointer(fColor, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(0));
    //Enable attrib arrays
    glEnableVertexAttribArray(fColor);

    GLuint tessNum = glGetUniformLocation(programID, "tess");
    glUniform1i(tessNum, 100);


    glutDisplayFunc(renderScene);

    //enter GLUT event processing cycle
    glutMainLoop();

    glDeleteVertexArrays(1, &VertexArrayID);

    return 1;
}
