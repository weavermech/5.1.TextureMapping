// Includes
#include <GL/glew.h>	//OpenGL Related Functions
#include <GL/glut.h>	//OpenGL Related Functions
#include <Shader.h>		//Functions for compiling shaders
#include <Vector.h>		//Functions for Vectors
#include <iostream>		//Writing to the Console using std::cout 
#include <math.h>       // Includes Math functions
#include <Matrix.h>
#include <Mesh.h>
#include <SphericalCameraManipulator.h>
#include <Texture.h>	//texture class


//Function Prototypes
bool initGL(int argc, char** argv);		        	//Function to init OpenGL
void initShader();					                //Function to init Shader
void initGeometry();					            //Function to init Geometry 
void drawGeometry();					            //Function to draw Geometry
void display(void);					                //OpenGL Display Function
void keyboard(unsigned char key, int x, int y);		//Keyboard Function
void mouse(int button, int state, int x, int y);	//Mouse Function
void motion(int x, int y);				            //Mouse Motion Function
void initTexture(std::string filename, GLuint & textureID);


//Global Variables
GLuint shaderProgramID;			                    // Shader Program ID
GLuint vertexPositionAttribute;		                // Vertex Position Attribute Location
GLuint vertexTexcoordAttribute; 					// Vertex Texcoord Attribute Location
GLuint TextureMapUniformLocation;					// Texture Map Location
GLuint texture; 									// OpenGL Texture

//Viewing
SphericalCameraManipulator cameraManip;
Matrix4x4 ModelViewMatrix;		                    // ModelView Matrix
GLuint MVMatrixUniformLocation;		                // ModelView Matrix Uniform
Matrix4x4 ProjectionMatrix;		                    // Projection Matrix
GLuint ProjectionUniformLocation;	                // Projection Matrix Uniform Location

//Mesh
Mesh mesh;				                            // Mesh


//Main Program Entry
int main(int argc, char** argv)
{	
	//init OpenGL
	if(!initGL(argc, argv))
		return -1;
    
    //Init OpenGL Shader
    initShader();
	
	//Init Mesh Geometry
	//mesh.initCube();
    mesh.loadOBJ("../models/cube.obj");    

    //Init Camera Manipultor
	cameraManip.setPanTiltRadius(0.f,0.f,2.f);
	cameraManip.setFocus(mesh.getMeshCentroid());

	//load texture models
	initTexture("../models/Crate.bmp", texture);

	//Start main loop
	glutMainLoop();

	//Clean-Up
	glDeleteProgram(shaderProgramID);

    return 0;
}

//! Function to Initlise OpenGL
bool initGL(int argc, char** argv)
{
	//Init GLUT
    glutInit(&argc, argv);
    
	//Set Display Mode: single buffered
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH); //added depth to the end

	//Set Window Size
    glutInitWindowSize(500, 500);
    
    // Window Position
    glutInitWindowPosition(200, 200);

	//Create Window
    glutCreateWindow("First Window");
    
    // Init GLEW
	if (glewInit() != GLEW_OK) 
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return false;
	}
	
	//Set Display function
    glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
    glutMotionFunc(motion);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);  //add depth testing

	return true;
}

//Init Shader
void initShader()
{
	//Create shader
    shaderProgramID = Shader::LoadFromFile("shader.vert","shader.frag");
    
    // Get a handle for our vertex position buffer
	vertexPositionAttribute = glGetAttribLocation(shaderProgramID, "aVertexPosition");

    //!
	MVMatrixUniformLocation = glGetUniformLocation(shaderProgramID,     "MVMatrix_uniform"); 
	ProjectionUniformLocation = glGetUniformLocation(shaderProgramID,   "ProjMatrix_uniform");

	//initialise global variables for texture
	vertexTexcoordAttribute = glGetAttribLocation(shaderProgramID,
												  "aVertexTexcoord");
	TextureMapUniformLocation = glGetUniformLocation(shaderProgramID,
													 "TextureMap_uniform");

}


void initTexture(std::string filename, GLuint & textureID)
{
	glGenTextures(1, &textureID);				//generate an openGL texture
	glBindTexture(GL_TEXTURE_2D, textureID);	//make it the current openGL texture

	//set texture parameters
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);


	//Get texture Data
	int width, height;
	char * data;
	//load texture data from file
	Texture::LoadBMP(filename, width, height, data);
	//copy to gpu
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	//Cleanup data as copied to GPU
	delete[] data;
}

//Display Loop
void display(void)
{
	// Clear the screen
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   //added depth to end

  	//Use shader
	glUseProgram(shaderProgramID);

	//Set Colour after program is in use
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(TextureMapUniformLocation, 0);
	
    //Projection Matrix - Perspective Projection
    ProjectionMatrix.perspective(90, 1.0, 0.0001, 100.0);
   
    //Set Projection Matrix
    glUniformMatrix4fv(	
		ProjectionUniformLocation,  //Uniform location
		1,							//Number of Uniforms
		false,						//Transpose Matrix
		ProjectionMatrix.getPtr());	//Pointer to ModelViewMatrixValues

	//Apply Camera Manipluator to Set Model View Matrix on GPU
    ModelViewMatrix.toIdentity();
    Matrix4x4 m = cameraManip.apply(ModelViewMatrix);
	glUniformMatrix4fv(	
		MVMatrixUniformLocation,  	//Uniform location
		1,					        //Number of Uniforms
		false,				        //Transpose Matrix
	    m.getPtr());	        //Pointer to Matrix Values
    


    //Call Draw Geometry Function
	mesh.Draw(vertexPositionAttribute,-1,vertexTexcoordAttribute);  //overwrote previous mesh.draw
	//Unuse Shader
	glUseProgram(0);
	
	//Redraw frame
	glutPostRedisplay();
    glutSwapBuffers();
}

// Keyboard Interaction
void keyboard(unsigned char key, int x, int y)
{
	//Quits program when esc is pressed
	if (key == 27)	//esc key code
	{
		exit(0);
	}
	else if(key == 'a')
	{
		std::cout << "a key pressed" << std::endl;
	}
	else if(key == 'b')
	{
		glClearColor(1.0,1.0,1.0,1.0);
	}
	else if(key == 'B')
	{
		glClearColor(0.0,0.0,0.0,1.0);
	}
    else if(key == 'w')
    {
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    }
    else if(key == 'W')
    {
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    }
    
	//Tell opengl to redraw frame
	glutPostRedisplay();
}

// Mouse Interaction
void mouse(int button, int state, int x, int y)
{
    cameraManip.handleMouse(button, state,x,y);
    glutPostRedisplay(); 
}

// Mouse Interaction
void motion(int x, int y)
{
    cameraManip.handleMouseMotion(x,y);
    glutPostRedisplay(); 
}




