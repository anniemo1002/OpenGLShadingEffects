/* Lab 5 base code - transforms using local matrix functions
	to be written by students -
	CPE 471 Cal Poly Z. Wood + S. Sueda
*/
#include <iostream>
#include <glad/glad.h>

#include "GLSL.h"
#include "Program.h"
#include "Shape.h"
#include "WindowManager.h"

// used for helper in perspective
#include "glm/glm.hpp"

using namespace std;
using namespace glm;

class Matrix
{

public:

	static void printMat(float *A, const char *name = 0)
	{
		// OpenGL uses col-major ordering:
		// [ 0  4  8 12]
		// [ 1  5  9 13]
		// [ 2  6 10 14]
		// [ 3  7 11 15]
		// The (i, j)th element is A[i+4*j].

		if (name)
		{
			printf("%s=[\n", name);
		}

		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				printf("%- 5.2f ", A[i + 4*j]);
			}
			printf("\n");
		}

		if (name)
		{
			printf("];");
		}
		printf("\n");
	}

	static void createIdentityMat(float *M)
	{
		// set all values to zero
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				M[i + 4*j] = 0;
			}
		}

		// overwrite diagonal with 1s
		M[0] = M[5] = M[10] = M[15] = 1;
	}

	static void createTranslateMat(float *T, float x, float y, float z)
	{
		// IMPLEMENT ME
                
		T[ 0] = 1.0f;
		T[ 1] = 0.0f;
		T[ 2] = 0.0f;
		T[ 3] = 0.0f;

		T[ 4] = 0.0f;
                T[ 5] = 1.0f;
		T[ 6] = 0.0f;
		T[ 7] = 0.0f;

		T[ 8] = 0.0f;
                T[ 9] = 0.0f;
		T[10] = 1.0f;
		T[11] = 0.0f;

		T[12] = x;
                T[13] = y;
		T[14] = z;
		T[15] = 1.0f;
	}
	

	static void createScaleMat(float *S, float x, float y, float z)
	{
		// IMPLEMENT ME
		S[ 0] = x;
		S[ 1] = 0.0f;
		S[ 2] = 0.0f;
		S[ 3] = 0.0f;

		S[ 4] = 0.0f;
                S[ 5] = y;
		S[ 6] = 0.0f;
		S[ 7] = 0.0f;

		S[ 8] = 0.0f;
                S[ 9] = 0.0f;
		S[10] = z;
		S[11] = 0.0f;

		S[12] = 0.0f;
                S[13] = 0.0f;
		S[14] = 0.0f;
		S[15] = 1.0f;
	}

	static void createRotateMatX(float *R, float radians)
	{
		// IMPLEMENT ME
		R[ 0] = 1.0f;
		R[ 1] = 0.0f;
		R[ 2] = 0.0f;
		R[ 3] = 0.0f;

		R[ 4] = 0.0f;
                R[ 5] = cos(radians);
		R[ 6] = sin(radians);
		R[ 7] = 0.0f;

		R[ 8] = 0.0f;
                R[ 9] = -sin(radians);
		R[10] = cos(radians);
		R[11] = 0.0f;

		R[12] = 0.0f;
                R[13] = 0.0f;
		R[14] = 0.0f;
		R[15] = 1.0f;
	}

	static void createRotateMatY(float *R, float radians)
	{
		// IMPLEMENT ME
		R[ 0] = cos(radians);
		R[ 1] = 0.0f;
		R[ 2] = -sin(radians);
		R[ 3] = 0.0f;

		R[ 4] = 0.0f;
                R[ 5] = 1.0f;
		R[ 6] = 0.0f;
		R[ 7] = 0.0f;

		R[ 8] = sin(radians);
                R[ 9] = 0.0f;
		R[10] = cos(radians);
		R[11] = 0.0f;

		R[12] = 0.0f;
                R[13] = 0.0f;
		R[14] = 0.0f;
		R[15] = 1.0f;
	}

	static void createRotateMatZ(float *R, float radians)
	{
		// IMPLEMENT ME
		R[ 0] = cos(radians);
		R[ 1] = sin(radians);
		R[ 2] = 0.0f;
		R[ 3] = 0.0f;

		R[ 4] = -sin(radians);
                R[ 5] = cos(radians);
		R[ 6] = 0.0f;
		R[ 7] = 0.0f;

		R[ 8] = 0.0f;
                R[ 9] = 0.0f;
		R[10] = 1.0f;
		R[11] = 0.0f;

		R[12] = 0.0f;
                R[13] = 0.0f;
		R[14] = 0.0f;
		R[15] = 1.0f;
	}

	static void multMat(float *C, const float *A, const float *B)
	{
		float c = 0;

		for (int k = 0; k < 4; ++k)
		{
			// Process kth column of C
			for (int i = 0; i < 4; ++i)
			{
				// Process ith row of C.
				// The (i,k)th element of C is the dot product
				// of the ith row of A and kth col of B.
				c = 0.0;

				// vector dot product
				for (int j = 0; j < 4; ++j)
				{
					// IMPLEMENT ME
                                        c += A[i+(4*j)]*B[j+(4*k)];
				}
                                C[i+(4*k)] = c;

			}
		}
	}

	static void createPerspectiveMat(float *m, float fovy, float aspect, float zNear, float zFar)
	{
		// http://www-01.ibm.com/support/knowledgecenter/ssw_aix_61/com.ibm.aix.opengl/doc/openglrf/gluPerspective.htm%23b5c8872587rree
		float f = 1.0f / glm::tan(0.5f * fovy);

		m[ 0] = f / aspect;
		m[ 1] = 0.0f;
		m[ 2] = 0.0f;
		m[ 3] = 0.0f;
		m[ 4] = 0;

		m[ 5] = f;
		m[ 6] = 0.0f;
		m[ 7] = 0.0f;
		m[ 8] = 0.0f;

		m[ 9] = 0.0f;
		m[10] = (zFar + zNear) / (zNear - zFar);
		m[11] = -1.0f;
		m[12] = 0.0f;

		m[13] = 0.0f;
		m[14] = 2.0f * zFar * zNear / (zNear - zFar);
		m[15] = 0.0f;
	}

};

class Application : public EventCallbacks
{

public:

	WindowManager * windowManager = nullptr;

	// Our shader program
	std::shared_ptr<Program> prog;
	std::shared_ptr<Program> prog1;
	std::shared_ptr<Program> prog2;
	std::shared_ptr<Program> prog3;
        std::shared_ptr<Program> prog_ptr; 
	// Shape to be used (from obj file)
	shared_ptr<Shape> shape;

	// Contains vertex information for OpenGL
	GLuint VertexArrayID;
        std::string resourceDir;
	// Data necessary to give our triangle to OpenGL
	GLuint VertexBufferID;
        float light_pos_x = -2;
        int mode = 0;
        float angle = 0;
        int material = 0;
	void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		if (key == GLFW_KEY_Q && action == GLFW_PRESS)//move light to the left
		{
                   light_pos_x-=1;
		}
		if (key == GLFW_KEY_E && action == GLFW_PRESS)//move light to the right
		{
                   light_pos_x+=1;
		}
		if (key == GLFW_KEY_R && action == GLFW_PRESS)//rotate
		{
                   angle+=0.5;
		}
		if (key == GLFW_KEY_P && action == GLFW_PRESS)//change shaders
		{
                   if (mode==3) mode = 0;
                   else mode++;
		}
		if (key == GLFW_KEY_M && action == GLFW_PRESS)//change materials
		{
                   if (material==3) material = 0;
                   else material++;
		}
               
	}

	void mouseCallback(GLFWwindow *window, int button, int action, int mods)
	{
		double posX, posY;

		if (action == GLFW_PRESS)
		{
			glfwGetCursorPos(window, &posX, &posY);
			cout << "Pos X " << posX <<  " Pos Y " << posY << endl;
		}
	}

	void resizeCallback(GLFWwindow *window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	void init(const std::string& resourceDirectory)
	{
		GLSL::checkVersion();
                resourceDir = resourceDirectory;
		// Set background color.
		glClearColor(0.12f, 0.34f, 0.56f, 1.0f);

		// Enable z-buffer test.
		glEnable(GL_DEPTH_TEST);

		// Initialize the GLSL program.
		prog = make_shared<Program>();
		prog->setVerbose(true);
		   prog->setShaderNames(resourceDirectory + "/normal_vert.glsl", resourceDirectory + "/normal_frag.glsl");
		if (! prog->init())
		{
			std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
			exit(1);
		}
		prog->init();
		prog->addUniform("P");
		prog->addUniform("MV");
		prog->addAttribute("vertPos");
		prog->addAttribute("vertNor");


		prog1 = make_shared<Program>();
		prog1->setVerbose(true);
	        prog1->setShaderNames(resourceDirectory + "/gouraud_vert.glsl", resourceDirectory + "/gouraud_frag.glsl");
		if (! prog1->init())
		{
			std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
			exit(1);
		}
		prog1->init();
		prog1->addUniform("P");
		prog1->addUniform("MV");
		prog1->addAttribute("vertPos");
		prog1->addUniform("lightPos");
		prog1->addAttribute("vertNor");
                prog1->addUniform("MatDif");
                prog1->addUniform("MatAmb");
                prog1->addUniform("MatSpec");
                prog1->addUniform("Shine");


		prog2 = make_shared<Program>();
		prog2->setVerbose(true);
	        prog2->setShaderNames(resourceDirectory + "/phong_vert.glsl", resourceDirectory + "/phong_frag.glsl");
		if (! prog2->init())
		{
			std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
			exit(1);
		}
		prog2->init();
		prog2->addUniform("P");
		prog2->addUniform("MV");
		prog2->addAttribute("vertPos");
		prog2->addUniform("lightPos");
		prog2->addAttribute("vertNor");
                prog2->addUniform("MatDif");
                prog2->addUniform("MatAmb");
                prog2->addUniform("MatSpec");
                prog2->addUniform("Shine");
		
                prog3 = make_shared<Program>();
		prog3->setVerbose(true);
	        prog3->setShaderNames(resourceDirectory + "/sil_vert.glsl", resourceDirectory + "/sil_frag.glsl");
		if (! prog3->init())
		{
			std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
			exit(1);
		}
		prog3->init();
		prog3->addUniform("P");
		prog3->addUniform("MV");
		prog3->addAttribute("vertPos");
		prog3->addAttribute("vertNor");
	}

	void initGeom(const std::string& resourceDirectory)
	{
		// Initialize mesh.
		shape = make_shared<Shape>();
		shape->loadMesh(resourceDirectory + "/bunny_no_normals.obj");
		shape->resize();
		shape->init();
	}
        void set_res_dir(const std::string& resourceDirectory){
                resourceDir = resourceDirectory;
        }
        void SetMaterial(int i){
                switch(i){
                     case 0:
                        glUniform3f(prog_ptr->getUniform("MatAmb"),0.02,0.04,0.2);
                        glUniform3f(prog_ptr->getUniform("MatDif"),0.0,0.16,0.9);
                        glUniform3f(prog_ptr->getUniform("MatSpec"),0.14,0.2,0.8);
                        glUniform1f(prog_ptr->getUniform("Shine"),120.0);
                        break;
                     case 1:
                        glUniform3f(prog_ptr->getUniform("MatAmb"),0.13,0.13,0.14);
                        glUniform3f(prog_ptr->getUniform("MatDif"),0.3,0.3,0.4);
                        glUniform3f(prog_ptr->getUniform("MatSpec"),0.3,0.3,0.4);
                        glUniform1f(prog_ptr->getUniform("Shine"),4.0);
                        break;
                     case 2:
                        glUniform3f(prog_ptr->getUniform("MatAmb"),0.3294,0.2235,0.02745);
                        glUniform3f(prog_ptr->getUniform("MatDif"),0.7804,0.5686,0.11373);
                        glUniform3f(prog_ptr->getUniform("MatSpec"),0.9922,0.941176,0.80784);
                        glUniform1f(prog_ptr->getUniform("Shine"),27.9);
                        break;
                     case 3: 
                        glUniform3f(prog_ptr->getUniform("MatAmb"),0.1,0.2,0.2);
                        glUniform3f(prog_ptr->getUniform("MatDif"),0.8,0.2,0.1);
                        glUniform3f(prog_ptr->getUniform("MatSpec"),0.4,0.5,0.6);
                        glUniform1f(prog_ptr->getUniform("Shine"),7.9);
                        break;

                        
                }
        }
	void render()
	{
		// Local modelview matrix use this for lab 5
		float MV[16] = {0};
	        float P[16] = {0};
                float Temp[16] ={0};
                float Tg[16] ={0};
                float R[16] ={0};
                // Get current frame buffer size.
		int width, height;
		glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
		glViewport(0, 0, width, height);
		// Clear framebuffer.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use the local matrices for lab 5
		float aspect = width/(float)height;
		Matrix::createPerspectiveMat(P, 70.0f, aspect, 0.1f, 100.0f);
                if (mode==0) prog_ptr = prog;
                else if (mode==1) prog_ptr = prog1;
                else if (mode==2) prog_ptr = prog2;
                else prog_ptr = prog3;
                Matrix::createRotateMatY(R,angle);
                Matrix::createTranslateMat(Tg,1,0,0);
                Matrix::multMat(Temp,R,Tg);
                Matrix::createTranslateMat(Tg,0,0,-5);
                Matrix::multMat(MV,Tg,Temp);
                prog_ptr->bind();
	        glUniformMatrix4fv(prog_ptr->getUniform("P"), 1, GL_FALSE, P);
	        glUniformMatrix4fv(prog_ptr->getUniform("MV"), 1, GL_FALSE, MV);
                if (mode==2 || mode==1){
                     glUniform3f(prog_ptr->getUniform("MatDif"),0.0,1.0,0.9);
                     glUniform3f(prog_ptr->getUniform("lightPos"),light_pos_x,2.0,2.0);
                     glUniform3f(prog_ptr->getUniform("MatAmb"),0.0,0.1,0.0);
                     glUniform3f(prog_ptr->getUniform("MatSpec"),0.4922,0.641176,0.7784);
                     glUniform1f(prog_ptr->getUniform("Shine"),7.9);
                }
		shape->draw(prog_ptr);
		prog_ptr->unbind();

                Matrix::createTranslateMat(Tg,-1,0,0);
                Matrix::createRotateMatY(R,-1.0+angle);
                Matrix::multMat(Temp,R,Tg);
                Matrix::createTranslateMat(Tg,0,0,-5);   
                Matrix::multMat(MV,Tg,Temp);
                prog_ptr->bind();
		glUniformMatrix4fv(prog_ptr->getUniform("P"), 1, GL_FALSE, P);
		glUniformMatrix4fv(prog_ptr->getUniform("MV"), 1, GL_FALSE, MV);
                if (mode==2 || mode==1){
                     SetMaterial(material);
                     glUniform3f(prog_ptr->getUniform("lightPos"),light_pos_x,2.0,2.0);
                }
		shape->draw(prog_ptr);
		prog_ptr->unbind();

	}
};

int main(int argc, char **argv)
{

	// Where the resources are loaded from
	std::string resourceDir = "../resources";
	if (argc >= 2)
	{
		resourceDir = argv[1];
	}

	Application *application = new Application();

	// Your main will always include a similar set up to establish your window
	// and GL context, etc.

	WindowManager *windowManager = new WindowManager();
	windowManager->init(640, 480);
	windowManager->setEventCallbacks(application);
	application->windowManager = windowManager;

	// This is the code that will likely change program to program as you
	// may need to initialize or set up different data and state
        application->set_res_dir(resourceDir);
	application->init(resourceDir);
	application->initGeom(resourceDir);

	// Loop until the user closes the window.
	while (! glfwWindowShouldClose(windowManager->getHandle()))
	{
		// Render scene.
		application->render();

		// Swap front and back buffers.
		glfwSwapBuffers(windowManager->getHandle());
		// Poll for and process events.
		glfwPollEvents();
	}

	// Quit program.
	windowManager->shutdown();
	return 0;
}
