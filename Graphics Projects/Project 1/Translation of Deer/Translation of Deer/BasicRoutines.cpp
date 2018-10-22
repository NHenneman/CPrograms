#include <glew.h>
#include <freeglut.h>
#include <string>
#include <iostream>
#include <cstdlib>
#include "LoadShaders.h"
#include "vmath.h"
#include "vgl.h"
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <array>

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
GLuint vertexBufferObj, elementBufferObj, arrayBufferObj;
/*
* Global variables
*   The location for the transformation and the current rotation
*   angle are set up as globals since multiple methods need to
*   access them.
*/
float rotationAngle, time = 0.0;
bool elements;
int nbrTriangles, materialToUse = 0;

map<string, GLuint> locationMap;

// Prototypes
GLuint buildProgram(string vertexShaderName, string fragmentShaderName);
int glutStartUp(int & argCount, char *argValues[],
	string windowTitle = "No Title", int width = 500, int height = 500);
void setAttributes(float lineWidth = 1.0, GLenum face = GL_FRONT_AND_BACK,
	GLenum fill = GL_FILL);
void buildObjects();
void getLocations();
void init(string vertexShader, string fragmentShader);
vmath::mat4 ortho(float znear, float zfar, float left, float right, float bottom, float top);
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
	//Start of code to read in deer object
	string line;
	int h = 0, i = 0, j = 0;
	GLfloat vertices[2496];
	GLfloat colors[3328];
	GLushort indices[4710];

	ifstream file("DeerRaw.obj");
	if (!file.is_open())
	{
		cerr << "File 'DeerRaw.obj' cannot be opened." << endl;
		exit(1);
	}

	while (getline(file, line))
	{
		if (line.substr(0, 2) == "v ")
		{
			istringstream s(line.substr(2));
			s >> vertices[i] >> vertices[i + 1] >> vertices[i + 2];
			colors[h] = 0.0f; colors[h + 1] = 1.0f; colors[h + 2] = 0.0f; colors[h + 3] = 1.0f;
			i += 3;
			h += 4;
		}
		else if (line.substr(0, 2) == "f ")
		{
			istringstream s(line.substr(2));
			GLushort a, b, c;
			s >> a; s >> b; s >> c;
			a--; b--; c--;
			indices[j] = a;
			indices[j + 1] = b;
			indices[j + 2] = c;
			j += 3;
		}
		else if (line[0] == '#')
		{
			/* ignoring this line */
		}
		else
		{
			/* ignoring this line */
		}
	}
	//End of code to read in deer object

	glGenBuffers(1, &elementBufferObj);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObj);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),
		indices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vertexBufferObj);
	glBindVertexArray(vertexBufferObj);

	// Alternately...
	// GLuint   vaoID;
	// glGenVertexArrays(1, &vaoID);
	// glBindVertexArray(vaoID);
	//

	/*
	* Test code for internal object.
	*/
	nbrTriangles = 832;
	glGenBuffers(1, &arrayBufferObj);
	glBindBuffer(GL_ARRAY_BUFFER, arrayBufferObj);
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(vertices) + sizeof(colors),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors),
		colors);
	/*
	* Set up variables into the shader programs (Note:  We need the
	* shaders loaded and built into a program before we do this)
	*/
	GLuint vPosition = glGetAttribLocation(programID, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(programID, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(vertices)));

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
	time += 0.005f;
	if (time >= (6.28))
		time -= 6.28;
	if (rotationAngle >= 360.0)
		rotationAngle -= 360.0;
	glutPostRedisplay();
	glutTimerFunc(1000 / 60, timer, 1);
}

/*
* The display routine is basically unchanged at this point.
*/
void display() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// needed
	vmath::mat4 rotate = vmath::rotate(rotationAngle, 1.0f, (0.5f * (2 * cos(time) + cos(2 * time))), (0.5f * (2 * sin(time) - sin(2 * time))));
	vmath::mat4 translate = vmath::translate(0.0f, 0.0f, 0.0f);
	vmath::mat4 scale = vmath::scale(0.25f, 0.25f, 0.25f);
	//vmath::mat4 scale = ortho(-100.0f, 100.0f, -1.0f, 1.0f, -1.0f, 1.0f);
	vmath::mat4 currentMatrix = scale * rotate * translate;
	glUniformMatrix4fv(locationMap["modelingTransform"], 1, GL_TRUE,
		currentMatrix);

	glBindVertexArray(vertexBufferObj);

	glBindBuffer(GL_ARRAY_BUFFER, arrayBufferObj);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObj);
	glDrawElements(GL_TRIANGLES,
		5000,
		GL_UNSIGNED_SHORT,
		NULL);

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
	glutTimerFunc(1000 / 60, timer, 1);
	glutKeyboardFunc(keypress);
	glutMainLoop();
}

/*
* routine to generate an orthographic projection matrix.
* author:  Jerry Heuring
* Date:  September 28, 2018
*
* The routine sets up an orthonormal projection.
*
* Bugs:
*    None Known.
*
* Revisions:
*
*/
vmath::mat4 ortho(float znear, float zfar, float left, float right, float bottom, float top) {
	vmath::mat4 orthoMatrix = vmath::scale(1.0f);
	orthoMatrix[0][0] = 2.0f / (right - left);
	orthoMatrix[1][1] = 2.0f / (top - bottom);
	orthoMatrix[2][2] = 2.0f / (zfar - znear);
	orthoMatrix[0][3] = -(right + left) / (right - left);
	orthoMatrix[1][3] = -(top + bottom) / (top - bottom);
	orthoMatrix[2][3] = -(zfar + znear) / (zfar - znear);
	return orthoMatrix;
}