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

struct GameObject {

	glm::vec3 position = glm::vec3(0.f);
	glm::vec3 rotation = glm::vec3(0.f);
	glm::vec3 scale = glm::vec3(1.f);

	glm::vec3 forward = glm::vec3(0.f);
	glm::vec3 forwardRotation = glm::vec3(0.f);
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

		//Declarar GameObjects
		GameObject cubo, piramide, ortoedro;

		//Declarar vec2 para definir el offset
		glm::vec2 offset = glm::vec2(0.f, 0.f);

		//Compilar shaders
		ShaderProgram cuboProgram, piramideProgram, ortoedroProgram;
		cuboProgram.vertexShader = LoadVertexShader("NormalVertexShader.glsl");
		cuboProgram.fragmentShader = LoadFragmentShader("UpYellowDownOrange.glsl");

		ortoedroProgram.vertexShader = LoadVertexShader("NormalVertexShader.glsl");
		ortoedroProgram.fragmentShader = LoadFragmentShader("UpYellowDownOrange.glsl");

		piramideProgram.vertexShader = LoadVertexShader("NormalVertexShader.glsl");
		piramideProgram.fragmentShader = LoadFragmentShader("RGBConstantChange.glsl");

		//Compilar programa
		GLuint cuboCompiledProgram, piramideCompiledProgram, ortoedroCompiledProgram;
		cuboCompiledProgram = CreateProgram(cuboProgram);
		piramideCompiledProgram = CreateProgram(piramideProgram);
		ortoedroCompiledProgram = CreateProgram(ortoedroProgram);

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
			-0.8f, +0.2f, -0.2f, // 3
			-0.4f, +0.2f, -0.2f, // 2
			-0.8f, -0.2f, -0.2f, // 6
			-0.4f, -0.2f, -0.2f, // 7
			-0.4f, -0.2f, +0.2f, // 4
			-0.4f, +0.2f, -0.2f, // 2
			-0.4f, +0.2f, +0.2f, // 0
			-0.8f, +0.2f, -0.2f, // 3
			-0.8f, +0.2f, +0.2f, // 1
			-0.8f, -0.2f, -0.2f, // 6
			-0.8f, -0.2f, +0.2f, // 5
			-0.4f, -0.2f, +0.2f, // 4
			-0.8f, +0.2f, +0.2f, // 1
			-0.4f, +0.2f, +0.2f  // 0
		};

		//Definimos modo de dibujo para cada cara
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

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

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glBufferData(GL_ARRAY_BUFFER, sizeof(penta), penta, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//Desvinculamos VAO
		glBindVertexArray(0);

		//Matrices de transformacion
		cubo.position = glm::vec3(0.f, 0.f, 0.f);
		cubo.rotation = glm::vec3(0.f, 1.f, 0.f);
		cubo.scale = glm::vec3(1.f, 1.f, 1.f);

		cubo.forward = glm::vec3(0.f, 1.f, 0.f);
		cubo.forwardRotation = cubo.forward;

		//matrices piramide
		piramide.position = glm::vec3(0.f, 0.f, 0.f);
		piramide.rotation = glm::vec3(0.f, 0.f, 1.f);
		piramide.scale = glm::vec3(1.f, 1.f, 1.f);

		piramide.forward = glm::vec3(0.f, 1.f, 0.f);
		piramide.forwardRotation = glm::vec3(1.f, 1.f, 0.f);

		//matrices ortoedro
		ortoedro.position = glm::vec3(0.5f, 0.f, 0.f);
		ortoedro.rotation = glm::vec3(1.f, 1.f, 0.f);
		ortoedro.scale = glm::vec3(1.f, 1.5f, 1.f);

		ortoedro.forward = glm::vec3(0.f,1.f,0.f);
		ortoedro.forwardRotation = glm::vec3(0.f,0.f,1.f);

		//bools for Inputs
		bool wireframeMode = false;

		bool cuboRenderizado = true;
		bool ortoedroRenderizado = true;
		bool piramideRenderizada = true;

		//Generamos el game loop
		while (!glfwWindowShouldClose(window)) {

			GLdouble screenSize[2];
			glfwGetWindowSize(window, (int*)&screenSize[0], (int*)&screenSize[1]);

			glUniform2dv(glGetUniformLocation(cuboCompiledProgram, "windowSize"), 1, &screenSize[0]);

			//Pulleamos los eventos (botones, teclas, mouse...)
			glfwPollEvents();

			if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
			{
				if (wireframeMode) {
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				}
				else {
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				}
				wireframeMode = !wireframeMode;
			}
			if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
				cuboRenderizado = !cuboRenderizado;
			if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
				ortoedroRenderizado = !ortoedroRenderizado;
			if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
				piramideRenderizada = !piramideRenderizada;
			if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
			{
				cubo.velocity += (cubo.velocity / 100) * 10;
				cubo.angularVelocity += (cubo.angularVelocity/100)*10;
				ortoedro.velocity += (ortoedro.velocity / 100) * 10;
				ortoedro.angularVelocity += (ortoedro.angularVelocity / 100) * 10;
				piramide.velocity += (piramide.velocity / 100) * 10;
				piramide.angularVelocity += (piramide.angularVelocity / 100) * 10;
			}
			if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
			{
				cubo.velocity -= (cubo.velocity / 100) * 10;
				cubo.angularVelocity -= (cubo.angularVelocity / 100) * 10;
				ortoedro.velocity -= (ortoedro.velocity / 100) * 10;
				ortoedro.angularVelocity -= (ortoedro.angularVelocity / 100) * 10;
				piramide.velocity -= (piramide.velocity / 100) * 10;
				piramide.angularVelocity -= (piramide.angularVelocity / 100) * 10;
			}



			//Limpiamos los buffers
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			//Cubo
			glBindVertexArray(vaoCubo);
			glUseProgram(cuboCompiledProgram);

			
			cubo.position += cubo.forward * cubo.velocity;
			cubo.rotation += cubo.forwardRotation * cubo.angularVelocity;

			if (cubo.position.y >= 0.9f || cubo.position.y <= -0.9f)
				cubo.forward = -cubo.forward;

			//Generar matrices
			glm::mat4 cuboTranslationMatrix = GenerateTranslationMatrix(cubo.position);
			glm::mat4 cuboRotationMatrix = GenerateRotationMatrix(cubo.rotation, cubo.rotation.y);
			glm::mat4 cuboScaleMatrix = GenerateScaleMatrix(cubo.scale);
			

			//Pasar matrices
			glUniformMatrix4fv(glGetUniformLocation(cuboCompiledProgram, "translationMatrix"), 1, GL_FALSE, glm::value_ptr(cuboTranslationMatrix));
			glUniformMatrix4fv(glGetUniformLocation(cuboCompiledProgram, "rotationMatrix"), 1, GL_FALSE, glm::value_ptr(cuboRotationMatrix));
			glUniformMatrix4fv(glGetUniformLocation(cuboCompiledProgram, "scaleMatrix"), 1, GL_FALSE, glm::value_ptr(cuboScaleMatrix));

			//Paso los uniforms
			glUniform2f(glGetUniformLocation(cuboCompiledProgram, "windowSize"), WINDOW_WIDTH, WINDOW_HEIGHT);
		

			//Dibujo cubo
			if(cuboRenderizado)
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 14);

			//ortoedro
			glUseProgram(ortoedroCompiledProgram);


			ortoedro.scale += ortoedro.forward * ortoedro.velocity;
			ortoedro.rotation += ortoedro.forwardRotation * ortoedro.angularVelocity;


			if (ortoedro.scale.y <= 1.f || ortoedro.scale.y >= 1.5f)
				ortoedro.forward = -ortoedro.forward;

			glm::mat4 ortoedroTranslationMatrix = GenerateTranslationMatrix(ortoedro.position);
			glm::mat4 ortoedroRotationMatrix = GenerateRotationMatrix(ortoedro.rotation, ortoedro.rotation.z);
			glm::mat4 ortoedroScaleMatrix = GenerateScaleMatrix(ortoedro.scale);

			glUniformMatrix4fv(glGetUniformLocation(ortoedroCompiledProgram, "translationMatrix"), 1, GL_FALSE, glm::value_ptr(ortoedroTranslationMatrix));
			glUniformMatrix4fv(glGetUniformLocation(ortoedroCompiledProgram, "rotationMatrix"), 1, GL_FALSE, glm::value_ptr(ortoedroRotationMatrix));
			glUniformMatrix4fv(glGetUniformLocation(ortoedroCompiledProgram, "scaleMatrix"), 1, GL_FALSE, glm::value_ptr(ortoedroScaleMatrix));

			glUniform2f(glGetUniformLocation(ortoedroCompiledProgram, "windowSize"), WINDOW_WIDTH, WINDOW_HEIGHT);

			if (ortoedroRenderizado)
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 14);


			//Piramide
			glBindVertexArray(vaoPiramide);
			glUseProgram(piramideCompiledProgram);

			//Matrices de transformacion

			//generar matrices

			piramide.position += piramide.forward * piramide.velocity;
			piramide.rotation += piramide.forwardRotation * piramide.angularVelocity;

			if (piramide.position.y >= 0.9f || piramide.position.y <= 0.9f)
				piramide.forward = -piramide.forward;

			glm::mat4 piramideTranslationMatrix = GenerateTranslationMatrix(piramide.position);
			glm::mat4 piramideRotationMatrix = GenerateRotationMatrix(piramide.rotation, piramide.rotation.y);
			glm::mat4 piramideScaleMatrix = GenerateScaleMatrix(piramide.scale);

			glUniformMatrix4fv(glGetUniformLocation(piramideCompiledProgram, "translationMatrix"), 1, GL_FALSE, glm::value_ptr(piramideTranslationMatrix));
			glUniformMatrix4fv(glGetUniformLocation(piramideCompiledProgram, "rotationMatrix"), 1, GL_FALSE, glm::value_ptr(piramideRotationMatrix));
			glUniformMatrix4fv(glGetUniformLocation(piramideCompiledProgram, "scaleMatrix"), 1, GL_FALSE, glm::value_ptr(piramideScaleMatrix));

			
			//Paso los uniforms
			
			glUniform2f(glGetUniformLocation(piramideCompiledProgram, "windowSize"), WINDOW_WIDTH, WINDOW_HEIGHT);


			//Dibujo piramide
			if(piramideRenderizada)
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 9);


			//Cambiamos buffers
			glFlush();
			glfwSwapBuffers(window);
		}

		//Desactivar y eliminar programa
		glUseProgram(0);
		glDeleteProgram(cuboCompiledProgram);
		glDeleteProgram(piramideCompiledProgram);


	}
	else {
		std::cout << "Ha petao." << std::endl;
		glfwTerminate();
	}

	//Finalizamos GLFW
	glfwTerminate();
}