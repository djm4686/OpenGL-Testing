/*
 ============================================================================
 Name        : GraphicsTesting.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <GLUT/glut.h>
#include <GLFW/glfw3.h>
#include "lib/linmath.h"


static const struct {
	float x, y, z;
	float r, g, b;
} vertices[4] = {
		{ -0.6f, -0.4f, 0.f, 1.f, 0.f, 0.f},
		{  0.6f, -0.4f, 0.f, 0.f, 1.f, 0.f},
		{  0.6f,  0.4f, 0.f, 0.f, 0.f, 1.f},
		{ -0.6f,  0.4f, 0.f, 0.f, 0.f, 0.f}
};

char* read_shader(const char* shader_path){
	FILE *f = fopen(shader_path, "r");
	if(!f){
		perror("No file or something");
		fprintf(stderr, shader_path);
		exit(1);
	}
	fseek(f, 0L, SEEK_END);
	long lSize = ftell(f);
	rewind(f);

	/* allocate mem */
	char* buffer = calloc(1, lSize+1);
	if (!buffer) fclose(f),fputs("memory alloc failed", stderr),exit(1);
	if( 1 != fread(buffer, lSize, 1, f) )
		fclose(f),free(buffer),fputs("read failed", stderr),exit(1);
	fclose(f);
	return buffer;

}

void error_callback(int error, const char* description){
	fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
	fprintf(stdout, "hit key: %i", key);
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

int main(void) {

	GLuint vertex_buffer, vertex_shader, fragment_shader, program;
	GLint mvp_location, vpos_location, vcol_location;

	if( !glfwInit() ){
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}
	glfwSetErrorCallback(error_callback);
	int width = 640;
	int height = 480;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	GLFWwindow* window = glfwCreateWindow(width, height, "My Title", NULL, NULL);
	if(!window){
		fprintf(stderr, "Oh no, no window\n");
		return -2;
	}


	glfwSetKeyCallback(window, key_callback);

	glfwMakeContextCurrent(window);

	double time  = glfwGetTime();

	glfwSwapInterval(1);

	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	char* vert_text = read_shader("src/vertex_shader.sdr");
	glShaderSource(vertex_shader, 1, &vert_text, NULL);
	glCompileShader(vertex_shader);

	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	char* frag_text = read_shader("src/fragment_shader.sdr");
	glShaderSource(fragment_shader, 1, &frag_text, NULL);
	glCompileShader(fragment_shader);

	program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);

	glLinkProgram(program);

	mvp_location = glGetUniformLocation(program, "MVP");
	vpos_location = glGetAttribLocation(program, "vPos");
	vcol_location = glGetAttribLocation(program, "vCol");

	glEnableVertexAttribArray(vpos_location);
	glVertexAttribPointer(vpos_location, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*) 0);
	glEnableVertexAttribArray(vcol_location);
	glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*) (sizeof(float) * 3));

	while(!glfwWindowShouldClose(window)){
		float ratio;
		int width, height;
		mat4x4 m, p, mvp;
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float) height;
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);
		mat4x4_identity(m);
		mat4x4_rotate_Z(m, m, (float) glfwGetTime());
		mat4x4_rotate_X(m, m, (float) glfwGetTime());
		mat4x4_rotate_Y(m, m, (float) glfwGetTime());
		mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
		mat4x4_mul(mvp, p, m);
		glUseProgram(program);
		glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
		glDrawArrays(GL_QUADS, 0, 4);
		glfwSwapBuffers(window);
		glfwPollEvents();

	}


	glfwDestroyWindow(window);
	glfwTerminate();
	return EXIT_SUCCESS;
}
