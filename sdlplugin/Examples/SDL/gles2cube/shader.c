/*
 * This proprietary software may be used only as
 * authorised by a licensing agreement from ARM Limited
 * (C) COPYRIGHT 2009 - 2011 ARM Limited
 * ALL RIGHTS RESERVED
 * The entire notice above must be reproduced on all authorised
 * copies and copies may only be made to the extent permitted
 * by a licensing agreement from ARM Limited.
 */

/*
 * shader.c
 * Functions for loading and process shaders.
 */

#include <stdio.h>
#include <malloc.h>
#include "shader.h"

char* load_shader(char *fileName) {
    char *text = NULL;
    FILE *fin = NULL;
    long len = 0;

    fin = fopen(fileName, "r");

    if (!fin) {
        fprintf(stderr, "Error: Cannot read file '%s'\n", fileName);
	return NULL;
    }

    fseek(fin, 0, SEEK_END); /* Seek end of file */
    len = ftell(fin);
    fseek(fin, 0, SEEK_SET); /* Seek start of file again */
    text = calloc(len+1, sizeof(char));
    fread(text, sizeof(char), len, fin);
    text[len] = '\0';
    fclose(fin);

    return text;
}

int process_shader(GLuint *shader, char *fileName, GLint shaderType) {
    GLint iStatus;
    const char *texts[1] = { NULL };

    /* Create shader and load into GL. */
    *shader = glCreateShader(shaderType);

    texts[0] = load_shader(fileName);
    if (!texts[0]) {
	return 1;
    }

    glShaderSource(*shader, 1, texts, NULL);

    /* Clean up shader source. */
    free((void *)texts[0]);
    texts[0] = NULL;

    /* Try compiling the shader. */
    glCompileShader(*shader);
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &iStatus);

    // Dump debug info (source and log) if compilation failed.
    if(iStatus != GL_TRUE) {
#if 1
	GLint len;
	char *debugSource = NULL;
	char *errorLog = NULL;

	/* Get shader source. */
	glGetShaderiv(*shader, GL_SHADER_SOURCE_LENGTH, &len);

	debugSource = malloc(len);

	glGetShaderSource(*shader, len, NULL, debugSource);

	printf("Debug source START:\n%s\nDebug source END\n\n", debugSource);
	free(debugSource);

	/* Now get the info log. */
	glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &len);

	errorLog = malloc(len);

	glGetShaderInfoLog(*shader, len, NULL, errorLog);

	printf("Log START:\n%s\nLog END\n\n", errorLog);
	free(errorLog);
#endif
	return 1;
    }
    return 0;
}

