/* Adaptação do exemplo Hello Triangle para exibição de uma pokeball */

#include <iostream>
#include <string>
#include <assert.h>
#include <utility> // for std::tuple

#include <math.h>
#include <cmath>

using namespace std;

// GLAD
#include "../../deps/include/glad/glad.h"

// GLFW
#include <GLFW/glfw3.h>


// Protótipo da função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Protótipos das funções
int setupShader();
std::tuple<int, int, int, int> setupGeometry();

// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint WIDTH = 800, HEIGHT = 600;

// Código fonte do Vertex Shader (em GLSL): ainda hardcoded
const GLchar* vertexShaderSource = "#version 400\n"
"layout (location = 0) in vec3 position;\n"
"void main()\n"
"{\n"
//...pode ter mais linhas de código aqui!
"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
"}\0";

//Códifo fonte do Fragment Shader (em GLSL): ainda hardcoded
const GLchar* fragmentShaderSource = "#version 400\n"
"uniform vec4 inputColor;\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = inputColor;\n"
"}\n\0";

const int nPointsCirlces = 60;


// Função MAIN
int main()
{
	// Inicialização da GLFW
	glfwInit();

	//Muita atenção aqui: alguns ambientes não aceitam essas configurações
	//Você deve adaptar para a versão do OpenGL suportada por sua placa
	//Sugestão: comente essas linhas de código para desobrir a versão e
	//depois atualize (por exemplo: 4.5 com 4 e 5)
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

	//Essencial para computadores da Apple
//#ifdef __APPLE__
//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//#endif

	// Criação da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ola Triangulo Branco! -- Rafael S. G.", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da função de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros d funções da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	// Obtendo as informações de versão
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	// Definindo as dimensões da viewport com as mesmas dimensões da janela da aplicação
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);


	// Compilando e buildando o programa de shader
	GLuint shaderID = setupShader();

	// Gerando um buffer simples, com a geometria de um triângulo
	auto vaos = setupGeometry();
	GLuint VAO1 = std::get<0>(vaos);
	GLuint VAO2 = std::get<1>(vaos);
	GLuint VAO3 = std::get<2>(vaos);
	GLuint VAO4 = std::get<3>(vaos);
	// Enviando a cor desejada (vec4) para o fragment shader
	// Utilizamos a variáveis do tipo uniform em GLSL para armazenar esse tipo de info
	// que não está nos buffers
	GLint colorLoc = glGetUniformLocation(shaderID, "inputColor");
	
	glUseProgram(shaderID);
	
	// Loop da aplicação - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as funções de callback correspondentes
		glfwPollEvents();

		// Limpa o buffer de cor
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(20);

		// parte de cima
		glBindVertexArray(VAO1);
		glUniform4f(colorLoc, 1.0f, 0.0f, 0.0f, 0.0f);
		glDrawArrays(GL_TRIANGLE_FAN, 0, nPointsCirlces);

		// parte de baixo
		glBindVertexArray(VAO2);
		glUniform4f(colorLoc, 1.0f, 1.0f, 1.0f, 1.0f);
		glDrawArrays(GL_TRIANGLE_FAN, 0, nPointsCirlces);

		// linha horizintal
		glBindVertexArray(VAO3);
		glUniform4f(colorLoc, 0.0f, 0.0f, 0.0f, 0.0f);
		glDrawArrays(GL_LINE_LOOP, 0, 2);

		// circulo de dentro
		glBindVertexArray(VAO4);
		glDrawArrays(GL_TRIANGLE_FAN, 0, nPointsCirlces);

		// finalizar o loop
		glBindVertexArray(0); //Desconectando o buffer de geometria
		glfwSwapBuffers(window);
	}
	// Pede pra OpenGL desalocar os buffers
	glDeleteVertexArrays(1, &VAO1);
	glDeleteVertexArrays(1, &VAO2);
	// Finaliza a execução da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}

// Função de callback de teclado - só pode ter uma instância (deve ser estática se
// estiver dentro de uma classe) - É chamada sempre que uma tecla for pressionada
// ou solta via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

//Esta função está basntante hardcoded - objetivo é compilar e "buildar" um programa de
// shader simples e único neste exemplo de código
// O código fonte do vertex e fragment shader está nos arrays vertexShaderSource e
// fragmentShader source no iniçio deste arquivo
// A função retorna o identificador do programa de shader
int setupShader()
{
	// Vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// Checando erros de compilação (exibição via log no terminal)
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// Checando erros de compilação (exibição via log no terminal)
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Linkando os shaders e criando o identificador do programa de shader
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// Checando por erros de linkagem
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

// Esta função está bastante harcoded - objetivo é criar os buffers que armazenam a 
// geometria de um triângulo
// Apenas atributo coordenada nos vértices
// 1 VBO com as coordenadas, VAO com apenas 1 ponteiro para atributo
// A função retorna o identificador do VAO
std::tuple<int, int, int, int> setupGeometry()
{
	int nPoints = nPointsCirlces;
	float angle = 0.0;
	float slice = 2 * M_PI / nPoints;
	float radius = 0.5;

	GLfloat topVertices[3 * ((nPoints/2) + 1)]; // vertices da parte de cima do circulo (vermelho)
	GLfloat bottomVertices[3 * ((nPoints/2) + 1)]; // vertices da parte de baixo do circulo (branco)
	GLfloat verticesInner[3 * nPoints]; // vertices da do circulo de dentro (botao preto)

	for (int i = 0; i < (nPoints / 2) + 1; i++) {
		topVertices[3 * i] = radius * cos(angle);
		topVertices[3 * i + 1] = radius * sin(angle);
		topVertices[3 * i + 2] = 0.0;

		// Vertices de baixo sao iguais aos de cima, exceto por y
		bottomVertices[3 * i] = topVertices[3 * i];
		bottomVertices[3 * i + 1] = -topVertices[3 * i + 1];
		bottomVertices[3 * i + 2] = 0.0;

		angle += slice;
	}

	// O círculo de dentro será completo e menor
	angle = 0.0;
	radius = 0.1;
	for (int i = 0; i < nPoints; i++) {
		verticesInner[3 * i] = radius * cos(angle);
		verticesInner[3 * i + 1] = radius * sin(angle);
		verticesInner[3 * i + 2] = 0.0;
		
		angle += slice;
	}

	// Linha horizontal no centro do círculo, dividindo a parte de baixo e de cima
	// hardcoded
	GLfloat verticesLine[2 * 3] = { 
		0.5, 0.0, 0.0,
		-0.5, 0.0, 0.0,
	};

	// VBO e VAO da parte de cima
	GLuint topVBO, topVAO;
	glGenBuffers(1, &topVBO);
	glBindBuffer(GL_ARRAY_BUFFER, topVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(topVertices), topVertices, GL_STATIC_DRAW);
	glGenVertexArrays(1, &topVAO);
	glBindVertexArray(topVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// VBO e VAO da parte de baixo
	GLuint bottomVBO, bottomVAO;
	glGenBuffers(1, &bottomVBO);
	glBindBuffer(GL_ARRAY_BUFFER, bottomVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bottomVertices), bottomVertices, GL_STATIC_DRAW);
	glGenVertexArrays(1, &bottomVAO);
	glBindVertexArray(bottomVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// VBO e VAO da linha horizontal
	GLuint lineVBO, lineVAO;
	glGenBuffers(1, &lineVBO);
	glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesLine), verticesLine, GL_STATIC_DRAW);
	glGenVertexArrays(1, &lineVAO);
	glBindVertexArray(lineVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// VBO e VAO do círculo de dentro
	GLuint innerVBO, innerVAO;
	glGenBuffers(1, &innerVBO);
	glBindBuffer(GL_ARRAY_BUFFER, innerVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesInner), verticesInner, GL_STATIC_DRAW);
	glGenVertexArrays(1, &innerVAO);
	glBindVertexArray(innerVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	return std::make_tuple(topVAO, bottomVAO, lineVAO, innerVAO);
}