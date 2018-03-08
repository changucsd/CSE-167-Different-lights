#include "Window.h"

const char* window_title = "GLFW Starter Project";
Cube * dragon; //5
Cube * bear;  // 6
Cube * bunny;  //7

Cube * cone;
Cube * sphere;
GLint shaderProgram;

// On some systems you need to change this to the absolute path
#define VERTEX_SHADER_PATH "shader.vert"
#define FRAGMENT_SHADER_PATH "shader.frag"

// Default camera parameters
glm::vec3 cam_pos(0.0f, 0.0f, 20.0f);		// e  | Position of camera
glm::vec3 cam_look_at(0.0f, 0.0f, 0.0f);	// d  | This is where the camera looks at
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is

int Window::width;
int Window::height;

float lastX,lastY,yaw,pitch,fov;
float phong;

bool firstMouse;
bool leftPress,rightPress;

float ObjectNumber;
float lightMode;
float turnOn;

glm::mat4 Window::P;
glm::mat4 Window::V;

void Window::initialize_objects()
{
	dragon = new Cube("dragon.obj");    // dragon is very shiny, with no diffuse reflection
    dragon->setAmbient(glm::vec3(0.1f, 0.0f, 0.0f));
    dragon->setDiffuse(glm::vec3(0.0f, 0.0f, 0.0f));
    dragon->setSpecular(glm::vec3(0.5f, 0.5f, 0.5f));
    dragon->setShiness(128.0f);
    
    bear = new Cube("bear.obj");      // bear is only using diffuse reflection, with zero shininess
    bear->setAmbient(glm::vec3(0.1f, 0.0f, 0.0f));
    bear->setDiffuse(glm::vec3(50.0f, 0.0f, 0.0f));
    bear->setSpecular(glm::vec3(0.5f, 0.5f, 0.5f));
    bear->setShiness(0.0f);
    
    bunny = new Cube("bunny.obj");    // bunny is having significant diffuse and​ specular reflection components.
    bunny->setAmbient(glm::vec3(0.1f, 0.0f, 0.0f));
    bunny->setDiffuse(glm::vec3(100.0f, 0.0f, 100.0f));
    bunny->setSpecular(glm::vec3(0.9f, 0.9f, 0.9f));
    bunny->setShiness(32.0f);
    
    cone = new Cube("cone.obj");
    
    cone->changePosition(glm::vec3(0.0f,10.0f,0.0f));
    
    sphere = new Cube("sphere.obj");
    sphere->changePosition(glm::vec3(8.0f,0.0f,0.0f));
    


	// Load the shader program. Make sure you have the correct filepath up top
	shaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	delete(dragon);
    delete(bear);
    delete(bunny);
    delete(sphere);
    delete(cone);
	glDeleteProgram(shaderProgram);
}

GLFWwindow* Window::create_window(int width, int height)
{
    
    //lastX = (float)width/2;
    //lastY = (float)height/2;
	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ // Because Apple hates comforming to standards
	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		fprintf(stderr, "Either GLFW is not installed or your graphics card does not support modern OpenGL.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Get the width and height of the framebuffer to properly resize the window
	glfwGetFramebufferSize(window, &width, &height);
	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);
    
    //project 3

    yaw   = -90.0f;
    pitch = 0.0f;
    fov = 45.0f;
    
    phong = 1.0f;
    firstMouse = true;
    leftPress = false;
    rightPress = false;
    ObjectNumber = 5.0f;
    lightMode = 1.0f;
    turnOn = 1.0f;
    
    //ObjectNumber = 5.0f;
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	glfwGetFramebufferSize(window, &width, &height); // In case your Mac has a retina display
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size. This is the only matrix that OpenGL maintains for us in modern OpenGL!
	glViewport(0, 0, width, height);

	if (height > 0)
	{
		P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
}

void Window::idle_callback()
{
    glUseProgram(shaderProgram);
	// Call the update function the cube
    if(ObjectNumber == 5.0f)
    {
        dragon->draw(shaderProgram,phong,lightMode,turnOn);
    }
    else if(ObjectNumber == 6.0f)
    {
        bear->draw(shaderProgram,phong,lightMode,turnOn);
    }
    else
    {
        bunny->draw(shaderProgram,phong,lightMode,turnOn);
    }
    
    /*
    if(lightMode == 2.0f)
    {
        cone->draw(shaderProgram,-1.0f,3,1);
    }
    else if(lightMode == 3.0f)
    {
        
        sphere->draw(shaderProgram,-1.0f,3,1);
    }
     */

}

void Window::display_callback(GLFWwindow* window)
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Use the shader of programID
	glUseProgram(shaderProgram);
	
	// Render the cube
    if(ObjectNumber == 5.0f)
    {
        dragon->draw(shaderProgram,phong,lightMode,turnOn);
    }
    else if(ObjectNumber == 6.0f)
    {
        bear->draw(shaderProgram,phong,lightMode,turnOn);
    }
    else
    {
        bunny->draw(shaderProgram,phong,lightMode,turnOn);
    }
    
    /*
    if(lightMode == 2.0f)
    {
        cone->draw(shaderProgram,-1.0f,3,1);
    }
    else if(lightMode == 3.0f)
    {
        
        sphere->draw(shaderProgram,-1.0f,3,1);
    }
     */
    
	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check for a key press
	if (action == GLFW_PRESS)
	{
		// Check if escape was pressed
		if (key == GLFW_KEY_ESCAPE)
		{
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
        else if (key == GLFW_KEY_R)
        {
            // Close the window. This causes the program to also terminate.
            dragon->changePosition(glm::vec3(0.0f,0.0f,0.0f));
        }
        
        else if (key == GLFW_KEY_N)
        {
            phong = -phong;
        }
        else if(key == GLFW_KEY_5)
        {
            ObjectNumber = 5.0f;
        }
        else if(key == GLFW_KEY_6)
        {
            ObjectNumber = 6.0f;
        }
        else if(key == GLFW_KEY_7)
        {
            ObjectNumber = 7.0f;
        }
        
        else if (key == GLFW_KEY_1)
        {
            lightMode = 1.0f;
        }
        
        else if (key == GLFW_KEY_2)
        {
            lightMode = 2.0f;
        }
        else if (key == GLFW_KEY_3)
        {
            lightMode = 3.0f;
        }
        else if (key == GLFW_KEY_4)
        {
            turnOn = -turnOn;
        }
        
        else if(key ==  GLFW_KEY_S)
        {
            if(mods == GLFW_MOD_SHIFT) // P scale up glPointSize()
            {
                
                
            }
            else // p scale down
            {

                
                
            }
        }
	}
}

void Window::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    //printf("Mouse is called");
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    

    if(leftPress == true)
    {

        glm::vec3 lastPoint = trackBallmapping(lastX, lastY);
        glm::vec3 newPoint = trackBallmapping((float)xpos, (float)ypos);
       
        glm::vec3 axis = glm::cross(lastPoint,newPoint);
        float c = glm::dot(lastPoint,newPoint);
        float d = glm::acos(c);
        
        //cube->changeRotation(glm::rotate(glm::mat4(1.0f),deg,axis));
        //printf("I am here");
        
        if(ObjectNumber == 5.0f)
        {
            dragon->rotation= glm::rotate(glm::mat4(1.0f), d*2.0f, axis) * dragon->rotation;
        }
        else if(ObjectNumber == 6.0f)
        {
            bear->rotation= glm::rotate(glm::mat4(1.0f), d*2.0f, axis) * bear->rotation;
        }
        else
        {
            bunny->rotation= glm::rotate(glm::mat4(1.0f), d*2.0f, axis) * bunny->rotation;
        }
        
        lastX = xpos;
        lastY = ypos;
        
    }
    
    else if(rightPress == true)
    {
        //cube->changePosition(glm::vec3((float)xoffset,(float)yoffset,0.0f));
        
        xoffset *= 0.03f;
        yoffset *= 0.03f;
        
        
        if(ObjectNumber == 5.0f)
        {
            dragon->position.x =  dragon->position.x + (float)xoffset;
            dragon->position.y =  dragon->position.y + (float)yoffset;
            dragon->position.z =  dragon->position.z;

        }
        else if(ObjectNumber == 6.0f)
        {
            bear->position.x =  bear->position.x + (float)xoffset;
            bear->position.y =  bear->position.y + (float)yoffset;
            bear->position.z =  bear->position.z;

        }
        else
        {
            bunny->position.x =  bunny->position.x + (float)xoffset;
            bunny->position.y =  bunny->position.y + (float)yoffset;
            bunny->position.z =  bunny->position.z;
        }
        
        lastX = xpos;
        lastY = ypos;
        
    }
    
    else
    {
        lastX = xpos;
        lastY = ypos;
    }

    
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    
    float sensitivity = 0.8f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    
    
    if(ObjectNumber == 5.0f)
    {
        dragon->position.z =  dragon->position.z +  (float)yoffset;
    }
    else if(ObjectNumber == 6.0f)
    {
        bear->position.z =  bear->position.z +  (float)yoffset;
        
    }
    else
    {
        bunny->position.z =  bunny->position.z +  (float)yoffset;
    }
}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        rightPress = true;
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    {
        rightPress = false;
    }
    
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        //printf("hahaha\n");
        leftPress = true;
    }
    else if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        leftPress = false;
    }
}

glm::vec3 Window::trackBallmapping(float x,float y)
{
  
    glm::vec3 vector;
    float d;
    vector.x = (2.0*x - width)/width;
    vector.y = (height - 2.0*y)/height;
    vector.z = 0.0f;
    
    d = glm::length(vector);
    if(d > 1.0f)
    {
        d = 1.0f;
    }
    
    vector.z = sqrt(1.001 - d*d);
    vector = glm::normalize(vector);

    return vector;
}
