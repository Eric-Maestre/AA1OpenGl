#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

struct ShaderProgram
{
	GLuint vertexShader = 0;
	GLuint geometryShader = 0;
	GLuint fragmentShader = 0;
};


struct Transform
{
	glm::vec3 position = glm::vec3(0.f);
	glm::vec3 forward = glm::vec3(1.f, 0.f, 0.f);
	glm::vec3 rotation = glm::vec3(0.f);
	float velocity = 0.0005f;
	float angularVelocity = -.05f;
};


void Resize_Window(GLFWwindow* window, int iFrameBufferWidth, int iFrameBufferHeight) {

	//Definir nuevo tamaño del viewport
	glViewport(0, 0, iFrameBufferWidth, iFrameBufferHeight);
}


glm::mat4 GenerateTranslationMatrix(glm::vec3 translation)
{
	return glm::translate(glm::mat4(1.f), translation);
}


glm::mat4 GenerateRotationMatrix(glm::vec3 axis, float degrees)
{
	return glm::rotate(glm::mat4(1.f), glm::radians(degrees), glm::normalize(axis));
}


glm::mat4 GenerateScaleMatrix(glm::vec3 scale)
{
	return glm::scale(glm::mat4(1.f), scale);
}


std::string Load_File(const std::string& filePath) {

	std::ifstream file(filePath);

	std::string fileContent;
	std::string line;

	//Lanzamos error si el archivo no se ha podido abrir
	if (!file.is_open()) {
		std::cerr << "No se ha podido abrir el archivo: " << filePath << std::endl;
		std::exit(EXIT_FAILURE);
	}

	//Leemos el contenido y lo volcamos a la variable auxiliar
	while (std::getline(file, line)) {
		fileContent += line + "\n";
	}

	//Cerramos stream de datos y devolvemos contenido
	file.close();

	return fileContent;
}


GLuint LoadVertexShader(const std::string& filePath) {

	// Crear un vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	//Usamos la funcion creada para leer el vertex shader y almacenarlo
	std::string sShaderCode = Load_File(filePath);
	const char* cShaderSource = sShaderCode.c_str();

	//Vinculamos el vertex shader con su código fuente
	glShaderSource(vertexShader, 1, &cShaderSource, nullptr);

	// Compilar el vertex shader
	glCompileShader(vertexShader);

	// Verificar errores de compilación
	GLint success;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	//Si la compilacion ha sido exitosa devolvemos el vertex shader
	if (success) {

		return vertexShader;

	}
	else {

		//Obtenemos longitud del log
		GLint logLength;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logLength);

		//Obtenemos el log
		std::vector<GLchar> errorLog(logLength);
		glGetShaderInfoLog(vertexShader, logLength, nullptr, errorLog.data());

		//Mostramos el log y finalizamos programa
		std::cerr << "Se ha producido un error al cargar el vertex shader:  " << errorLog.data() << std::endl;
		std::exit(EXIT_FAILURE);
	}
}


GLuint LoadGeometryShader(const std::string& filePath) {

	// Crear un vertex shader
	GLuint geometryShader = glCreateShader(GL_GEOMETRY_SHADER);

	//Usamos la funcion creada para leer el vertex shader y almacenarlo
	std::string sShaderCode = Load_File(filePath);
	const char* cShaderSource = sShaderCode.c_str();

	//Vinculamos el vertex shader con su código fuente
	glShaderSource(geometryShader, 1, &cShaderSource, nullptr);

	// Compilar el vertex shader
	glCompileShader(geometryShader);

	// Verificar errores de compilación
	GLint success;
	glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);

	//Si la compilacion ha sido exitosa devolvemos el vertex shader
	if (success) {

		return geometryShader;

	}
	else {

		//Obtenemos longitud del log
		GLint logLength;
		glGetShaderiv(geometryShader, GL_INFO_LOG_LENGTH, &logLength);

		//Obtenemos el log
		std::vector<GLchar> errorLog(logLength);
		glGetShaderInfoLog(geometryShader, logLength, nullptr, errorLog.data());

		//Mostramos el log y finalizamos programa
		std::cerr << "Se ha producido un error al cargar el geometry shader:  " << errorLog.data() << std::endl;
		std::exit(EXIT_FAILURE);
	}
}


GLuint LoadFragmentShader(const std::string& filePath) {

	// Crear un vertex shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	//Usamos la funcion creada para leer el vertex shader y almacenarlo
	std::string sShaderCode = Load_File(filePath);
	const char* cShaderSource = sShaderCode.c_str();

	//Vinculamos el vertex shader con su código fuente
	glShaderSource(fragmentShader, 1, &cShaderSource, nullptr);

	// Compilar el vertex shader
	glCompileShader(fragmentShader);

	// Verificar errores de compilación
	GLint success;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	//Si la compilacion ha sido exitosa devolvemos el vertex shader
	if (success) {

		return fragmentShader;

	}
	else {

		//Obtenemos longitud del log
		GLint logLength;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logLength);

		//Obtenemos el log
		std::vector<GLchar> errorLog(logLength);
		glGetShaderInfoLog(fragmentShader, logLength, nullptr, errorLog.data());

		//Mostramos el log y finalizamos programa
		std::cerr << "Se ha producido un error al cargar el fragment shader:  " << errorLog.data() << std::endl;
		std::exit(EXIT_FAILURE);
	}
}


GLuint CreateProgram(const ShaderProgram& shaders) {

	//Crear programa de la GPU
	GLuint program = glCreateProgram();

	//Verificar que existe un vertex shader y adjuntarlo al programa
	if (shaders.vertexShader != 0) {
		glAttachShader(program, shaders.vertexShader);
	}
	if (shaders.geometryShader != 0) {
		glAttachShader(program, shaders.geometryShader);
	}
	if (shaders.fragmentShader != 0) {
		glAttachShader(program, shaders.fragmentShader);
	}

	// Linkear el programa
	glLinkProgram(program);

	//Obtener estado del programa
	GLint success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);

	//Devolver programa si todo es correcto o mostrar log en caso de error
	if (success) {

		//Liberamos recursos
		if (shaders.vertexShader != 0) {
			glDetachShader(program, shaders.vertexShader);
		}
		if (shaders.geometryShader != 0) {
			glDetachShader(program, shaders.geometryShader);
		}
		if (shaders.fragmentShader != 0) {
			glDetachShader(program, shaders.fragmentShader);
		}

		return program;
	}
	else {

		//Obtenemos longitud del log
		GLint logLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

		//Almacenamos log
		std::vector<GLchar> errorLog(logLength);
		glGetProgramInfoLog(program, logLength, nullptr, errorLog.data());

		std::cerr << "Error al linkar el programa:  " << errorLog.data() << std::endl;
		std::exit(EXIT_FAILURE);
	}
}




void main() {

	//Definir semillas del rand según el tiempo
	srand(static_cast<unsigned int>(time(NULL)));

	//Inicializamos GLFW para gestionar ventanas e inputs
	glfwInit();

	//Configuramos la ventana
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	//Inicializamos la ventana
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "My Engine", NULL, NULL);

	//Asignamos función de callback para cuando el frame buffer es modificado
	glfwSetFramebufferSizeCallback(window, Resize_Window);

	//Definimos espacio de trabajo
	glfwMakeContextCurrent(window);

	//Permitimos a GLEW usar funcionalidades experimentales
	glewExperimental = GL_TRUE;

	//Activamos cull face
	glEnable(GL_CULL_FACE);

	//Indicamos lado del culling
	glCullFace(GL_BACK);

	//Inicializamos GLEW y controlamos errores
	if (glewInit() == GLEW_OK) {

		//Declarar instancia de Transform
		Transform cube;
		Transform cubo, piramide, ortoedro;

		//Declarar vec2 para definir el offset
		glm::vec2 offset = glm::vec2(0.f, 0.f);

		//Compilar shaders
		ShaderProgram cuboProgram, ciramideProgram, orotedroProgram;
		cuboProgram.vertexShader = LoadVertexShader("UpAndDownMovement.glsl");
		cuboProgram.fragmentShader = LoadFragmentShader("UpYellowDownOrange.glsl");

		//Compilar programa
		GLuint cuboCompiledProgram;
		cuboCompiledProgram = CreateProgram(cuboProgram);

		//Obtener referencia a offset
		GLint offsetReference = glGetUniformLocation(cuboCompiledProgram, "offset");

		//Definimos color para limpiar el buffer de color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		GLuint vaoCubo, vaoPiramide;
		GLuint vboHexaedro, vboPentaedro;
		//Definimos cantidad de vao a crear y donde almacenarlos
		glGenVertexArrays(1, &vaoCubo);

		//Indico que el VAO activo de la GPU es el que acabo de crear
		glBindVertexArray(vaoCubo);

		//Definimos cantidad de vbo a crear y donde almacenarlos
		glGenBuffers(1, &vboHexaedro);

		//Indico que el VBO activo es el que acabo de crear y que almacenará un array. Todos los VBO que genere se asignaran al último VAO que he hecho glBindVertexArray
		glBindBuffer(GL_ARRAY_BUFFER, vboHexaedro);

		//Posición X e Y del punto
		GLfloat hexa[] =
		{
			-0.8f, +0.2f, -0.5f, // 3
			-0.4f, +0.2f, -0.5f, // 2
			-0.8f, -0.2f, -0.5f, // 6
			-0.4f, -0.2f, -0.5f, // 7
			-0.4f, -0.2f, +0.5f, // 4
			-0.4f, +0.2f, -0.5f, // 2
			-0.4f, +0.2f, +0.5f, // 0
			-0.8f, +0.2f, -0.5f, // 3
			-0.8f, +0.2f, +0.5f, // 1
			-0.8f, -0.2f, -0.5f, // 6
			-0.8f, -0.2f, +0.5f, // 5
			-0.4f, -0.2f, +0.5f, // 4
			-0.8f, +0.2f, +0.5f, // 1
			-0.4f, +0.2f, +0.5f  // 0

		};

		//Definimos modo de dibujo para cada cara
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		//Ponemos los valores en el VBO creado
		glBufferData(GL_ARRAY_BUFFER, sizeof(hexa), hexa, GL_STATIC_DRAW);

		//Indicamos donde almacenar y como esta distribuida la información
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

		//Indicamos que la tarjeta gráfica puede usar el atributo 0
		glEnableVertexAttribArray(0);

		//Desvinculamos VBO
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//Desvinculamos VAOCubo
		glBindVertexArray(0);
		
		//VBOTriangle

		//Definimos cantidad de vao a crear y donde almacenarlos
		glGenVertexArrays(1, &vaoPiramide);

		//Indico que el VAO activo de la GPU es el que acabo de crear
		glBindVertexArray(vaoPiramide);

		//Definimos cantidad de vbo a crear y donde almacenarlos
		glGenBuffers(1, &vboPentaedro);

		//Indico que el VBO activo es el que acabo de crear y que almacenará un array. Todos los VBO que genere se asignaran al último VAO que he hecho glBindVertexArray
		glBindBuffer(GL_ARRAY_BUFFER, vboHexaedro);

		glGenBuffers(1, &vboPentaedro);

		glBindBuffer(GL_ARRAY_BUFFER, vboPentaedro);

		GLfloat penta[] =
		{
			+0.4f, 0.0f,-0.5f, //1
			+0.8f, 0.0f, -0.5f, //2
			+0.4f, 0.0f, 0.0f, //3
			+0.8f, 0.0f, 0.0f,//4
			+0.6f, +0.3f, -0.25f,//5
			+0.8f, 0.0f, -0.5f, //2
			+0.4f, 0.0f,-0.5f, //1
			+0.4f, 0.0f, 0.0f, //3
			+0.6f, +0.3f, -0.25f,//5
		};

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glBufferData(GL_ARRAY_BUFFER, sizeof(penta), penta, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//Desvinculamos VAO
		glBindVertexArray(0);

		//Indicar a la tarjeta GPU que programa debe usar
		//Preguntar
		glUseProgram(cuboCompiledProgram);
		//glUniformMatrix4fv(glGetUniformLocation(myFirstCompiledProgram, "transform"), 1, GL_FALSE, glm::value_ptr(modelMatrix));

		//Generamos el game loop
		while (!glfwWindowShouldClose(window)) {

			GLdouble screenSize[2];
			glfwGetWindowSize(window, (int*)&screenSize[0], (int*)&screenSize[1]);

			glUniform2dv(glGetUniformLocation(cuboCompiledProgram, "screenSize"), 1, &screenSize[0]);

			//Pulleamos los eventos (botones, teclas, mouse...)
			glfwPollEvents();

			//Limpiamos los buffers
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			//Definimos que queremos usar el VAO con los puntos
			glBindVertexArray(vaoCubo);

			//Matrices de transformacion

			//Cubo
			glBindVertexArray(vaoCubo);
			glUseProgram(cuboCompiledProgram);

			//Matrices de transformacion
			//CalculosCubo();
			//Paso los uniforms
			//UniformCubo();
			GLuint timeLocation = glGetUniformLocation(cuboCompiledProgram, "time");

			float time = 0.5f;
			glUniform1f(timeLocation, time);

			GLuint windowSizeLocation = glGetUniformLocation(cuboCompiledProgram, "windowSize");

			float windowSize = WINDOW_HEIGHT; 
			glUniform1f(windowSizeLocation, windowSize);

			//Dibujo cubo
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 22);




			//Piramide
			glBindVertexArray(vaoPiramide);
			//glUseProgram(programaPiramide);

			//Matrices de transformacion
			//CalculosPiramide();
			
			//Paso los uniforms
			//UniformPiramide();

			//Dibujo piramide
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 22);




			//cube rotation

		/*	glm::mat4 modelMatrix = glm::mat4(1.f);

			cube.position += cube.forward * cube.velocity;
			cube.rotation += glm::vec3(0.f, 1.f, 0.f) * cube.angularVelocity;

			if (cube.position.x >= 0.5f || cube.position.x <= -0.5f)
			{
				cube.forward = -cube.forward;
			}

			glm::mat4 cubeTransMatrix = GenerateTranslationMatrix(cube.position);
			glm::mat4 rotationMatrix = GenerateRotationMatrix(glm::vec3(0.f, 1.f, 0.f), cube.rotation.y);

			modelMatrix = cubeTransMatrix * rotationMatrix * modelMatrix;

			glUniformMatrix4fv(glGetUniformLocation(myFirstCompiledProgram, "transform"), 1, GL_FALSE, glm::value_ptr(modelMatrix));


			glDrawArrays(GL_TRIANGLE_STRIP, 0, 22); 
			glBindVertexArray(0); */




			//Cambiamos buffers
			glFlush();
			glfwSwapBuffers(window);
		}

		//Desactivar y eliminar programa
		//glUseProgram(0);
		//glDeleteProgram(myFirstCompiledProgram);

	}
	else {
		std::cout << "Ha petao." << std::endl;
		glfwTerminate();
	}

	//Finalizamos GLFW
	glfwTerminate();
}