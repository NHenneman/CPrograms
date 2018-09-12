#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string>
#include <iostream>
#include <cstdlib>
#include "LoadShaders.h"
#include "vmath.h"
#include "vgl.h"
#include <map>

using namespace std;

#define BUFFER_OFFSET(x)  ((const void*) (x))

GLuint programID;
/*
* Arrays to store the indices/names of the Vertex Array Objects and
* Buffers.  Rather than using the books enum approach I've just
* gone out and made a bunch of them and will use them as needed.
*
* Not the best choice I'm sure.
*/

GLuint vertexBuffers[10], arrayBuffers[10], elementBuffers[10];
/*
* Global variables
*   The location for the transformation and the current rotation
*   angle are set up as globals since multiple methods need to
*   access them.
*/
float rotationAngle;
bool elements;
int nbrTriangles, materialToUse = 0;

map<string, GLuint> locationMap;

// Prototypes
GLuint buildProgram(string vertexShaderName, string fragmentShaderName);
int glutStartUp(int & argCount, char *argValues[],
	string windowTitle = "No Title", int width = 500, int height = 500);
void setAttributes(float lineWidth = 1.0, GLenum face = GL_FRONT_AND_BACK,
	GLenum fill = GLU_FILL);
void buildObjects();
void getLocations();
void init(string vertexShader, string fragmentShader);

/*
* Routine to encapsulate some of the startup routines for GLUT.  It returns the window ID of the
* single window that is created.
*/
int glutStartUp(int & argCount, char *argValues[], string title, int width, int height) {
	int windowID;
	GLenum glewErrCode;

	glutInit(&argCount, argValues);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	windowID = glutCreateWindow(title.c_str());

	glutInitContextVersion(3, 1);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glewErrCode = glewInit();
	if (glewErrCode != GLEW_OK) {
		cerr << "Glew init failed " << glewErrCode << endl;
		exit(EXIT_FAILURE);
	}
	return windowID;
}


/*
* Use the author's routines to build the program and return the program ID.
*/
GLuint buildProgram(string vertexShaderName, string fragmentShaderName) {

	/*
	*  Use the Books code to load in the shaders.
	*/
	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, vertexShaderName.c_str() },
		{ GL_FRAGMENT_SHADER, fragmentShaderName.c_str() },
		{ GL_NONE, NULL }
	};
	GLuint program = LoadShaders(shaders);
	if (program == 0) {
		cerr << "GLSL Program didn't load.  Error \n" << endl
			<< "Vertex Shader = " << vertexShaderName << endl
			<< "Fragment Shader = " << fragmentShaderName << endl;
	}
	glUseProgram(program);
	return program;
}

/*
* Set up the clear color, lineWidth, and the fill type for the display.
*/
void setAttributes(float lineWidth, GLenum face, GLenum fill) {
	/*
	* I'm using wide lines so that they are easier to see on the screen.
	* In addition, this version fills in the polygons rather than leaving it
	* as lines.
	*/
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glLineWidth(lineWidth);
	glPolygonMode(face, fill);
	glEnable(GL_DEPTH);

}

/*
* read and/or build the objects to be displayed.  Also sets up attributes that are
* vertex related.
*/

void buildObjects() {
	GLfloat vertices[] = {	-0.5, -0.5,  0.5, 1.0,   // Triangle 1  --Side 1
							-0.5, -0.5, -0.5, 1.0,
							-0.5,  0.5,  0.5, 1.0,
							-0.5,  0.5, -0.5, 1.0,   // Triangle 2
							-0.5, -0.5, -0.5, 1.0,
							-0.5,  0.5,  0.5, 1.0,

							 0.5,  0.5,  0.5, 1.0,   // Triangle 3  --Side 2
							 0.5,  0.5, -0.5, 1.0,
							 0.5, -0.5, -0.5, 1.0,
							 0.5,  0.5,  0.5, 1.0,   // Triangle 4
							 0.5, -0.5,  0.5, 1.0,
							 0.5, -0.5, -0.5, 1.0,

							 0.5, -0.5,  0.5, 1.0,   // Triangle 5  --Side 3
							-0.5,  0.5,  0.5, 1.0,
							-0.5, -0.5,  0.5, 1.0,
							 0.5, -0.5,  0.5, 1.0,   // Triangle 6
							-0.5,  0.5,  0.5, 1.0,
							 0.5,  0.5,  0.5, 1.0,

							 0.5, -0.5, -0.5, 1.0,   // Triangle 7  -Side 4
							 0.5,  0.5, -0.5, 1.0,
							-0.5, -0.5, -0.5, 1.0,
							 0.5,  0.5, -0.5, 1.0,   // Triangle 8
							-0.5,  0.5, -0.5, 1.0,
							-0.5, -0.5, -0.5, 1.0,
	};
	
GLfloat colors[] = {	1.00, 1.00, 1.00, 1.00,   1.00, 1.00, 1.00, 1.00,  1.00, 1.00, 1.00, 1.00, // Triangle 1 -- WHITE
						0.00, 1.00, 0.00, 1.00,   0.00, 1.00, 0.00, 1.00,  0.00, 1.00, 0.00, 1.00, // Triangle 2 -- GREEN
						1.00, 1.00, 0.00, 1.00,   1.00, 1.00, 0.00, 1.00,  1.00, 1.00, 0.00, 1.00, // Triangle 3 -- YELLOW
						1.00, 0.00, 0.00, 1.00,   1.00, 0.00, 0.00, 1.00,  1.00, 0.00, 0.00, 1.00, // Triangle 4 -- RED
						0.00, 0.00, 0.00, 1.00,   0.00, 0.00, 0.00, 1.00,  0.00, 0.00, 0.00, 1.00, // Triangle 5 -- BLACK
						0.00, 0.24, 1.00, 1.00,   0.00, 0.24, 1.00, 1.00,  0.00, 0.24, 1.00, 1.00, // Triangle 6 -- Blue
						0.00, 1.00, 1.00, 1.00,   0.00, 1.00, 1.00, 1.00,  0.00, 1.00, 1.00, 1.00, // Triangle 7 -- CYAN
						1.00, 0.00, 1.00, 1.00,   1.00, 0.00, 1.00, 1.00,  1.00, 0.00, 1.00, 1.00, // Triangle 8 -- MAGENTA
	};

	float vertexColors[] = {
		//       r    g    b    a
		0.0, 0.0, 0.0, 1.0,   // The vertex colors of a unit cube.
		0.0, 0.0, 1.0, 1.0,
	};
	GLfloat normals[] = {
		-1.0, 0.0, 0.0,    -1.0, 0.0, 0.0,    -1.0, 0.0, 0.0,  // Triangle 1
		-1.0, 0.0, 0.0,    -1.0, 0.0, 0.0,    -1.0, 0.0, 0.0,  // Triangle 2
		-1.0, 0.0, 0.0,    -1.0, 0.0, 0.0,    -1.0, 0.0, 0.0,  // Triangle 3
		-1.0, 0.0, 0.0,    -1.0, 0.0, 0.0,    -1.0, 0.0, 0.0,  // Triangle 4
		-1.0, 0.0, 0.0,    -1.0, 0.0, 0.0,    -1.0, 0.0, 0.0,  // Triangle 5
		-1.0, 0.0, 0.0,    -1.0, 0.0, 0.0,    -1.0, 0.0, 0.0,  // Triangle 6
		-1.0, 0.0, 0.0,    -1.0, 0.0, 0.0,    -1.0, 0.0, 0.0,  // Triangle 7
		-1.0, 0.0, 0.0,    -1.0, 0.0, 0.0,    -1.0, 0.0, 0.0,  // Triangle 8
	};


	glGenVertexArrays(1, vertexBuffers);
	glBindVertexArray(vertexBuffers[0]);

	// Alternately...
	// GLuint   vaoID;
	// glGenVertexArrays(1, &vaoID);
	// glBindVertexArray(vaoID);
	//

	/*
	* Test code for internal object.
	*/
	nbrTriangles = 8;
	glGenBuffers(1, &(arrayBuffers[0]));
	glBindBuffer(GL_ARRAY_BUFFER, arrayBuffers[0]);
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(vertices) + sizeof(colors) + sizeof(normals),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors), sizeof(normals), normals);
	/*
	* Set up variables into the shader programs (Note:  We need the
	* shaders loaded and built into a program before we do this)
	*/
	GLuint vPosition = glGetAttribLocation(programID, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(programID, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertices)));

	GLuint vNormal = glGetAttribLocation(programID, "vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(nbrTriangles * 12 * sizeof(float)));

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/*
* This fills in the locations of most of the uniform variables for the program.
* there are better ways of handling this but this is good in going directly from
* what we had.
*
* Revised to get the locations and names of the uniforms from OpenGL.  These
* are then stored in a map so that we can look up a uniform by name when we
* need to use it.  The map is still global but it is a little neater than the
* version that used all the locations.  The locations are still there right now
* in case that is more useful for you.
*
*/

void getLocations() {
	/*
	* Find out how many uniforms there are and go out there and get them from the
	* shader program.  The locations for each uniform are stored in a global -- locationMap --
	* for later retrieval.
	*/
	GLint numberBlocks;
	char uniformName[1024];
	int nameLength;
	GLint size;
	GLenum type;
	glGetProgramiv(programID, GL_ACTIVE_UNIFORMS, &numberBlocks);
	for (int blockIndex = 0; blockIndex < numberBlocks; blockIndex++) {
		glGetActiveUniform(programID, blockIndex, 1024, &nameLength, &size, &type, uniformName);
		cout << uniformName << endl;
		locationMap[string(uniformName)] = blockIndex;
	}
}

void init(string vertexShader, string fragmentShader) {

	setAttributes(1.0f, GL_FRONT_AND_BACK, GL_FILL);

	programID = buildProgram(vertexShader, fragmentShader);

	buildObjects();

	getLocations();

}

/*
* Timer routine -- when called it increments the angle by 1 degree,
* tells Glut to redraw the screen, and sets a timer for 1/30 of a
* second later.
*/
void timer(int value) {
	rotationAngle += 1.0f;
	if (rotationAngle >= 360.0)
		rotationAngle -= 360.0;
	glutPostRedisplay();
	glutTimerFunc(1000 / 30, timer, 1);
}

/*
* The display routine is basically unchanged at this point.
*/
void display() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// needed
	vmath::mat4 currentMatrix = vmath::rotate(rotationAngle, 0.0f, 1.0f, 0.0f);

	glUniformMatrix4fv(locationMap["modelingTransform"], 1, GL_TRUE,
		currentMatrix);

	glBindVertexArray(vertexBuffers[0]);
	glBindBuffer(GL_ARRAY_BUFFER, arrayBuffers[0]);
	glDrawArrays(GL_TRIANGLES, 0, nbrTriangles * 3);

	glFlush();
}

/*
* Handle keypresses -- only one recognized in this version is q for quit.
*/

void keypress(unsigned char key, int x, int y) {
	switch (key) {
	case 'q':
	case 'Q':
		exit(0);
		break;

	default:
		// do nothing....
		break;
	}
}

/*
* Main program with calls for many of the helper routines.
*/
int main(int argCount, char *argValues[]) {

	glutStartUp(argCount, argValues, "My Test of New Routines");
	init("project0.vert", "project0.frag");
	glutDisplayFunc(display);
	glutTimerFunc(1000 / 30, timer, 1);
	glutKeyboardFunc(keypress);
	glutMainLoop();
}