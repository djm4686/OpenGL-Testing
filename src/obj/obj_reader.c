/*
 * obj_reader.c
 *
 *  Created on: Jul 27, 2017
 *      Author: dmadden
 */

#include <stdlib.h>
#include <stdio.h>
#include "obj_reader.h"



Obj read_object(FILE * obj){
	Obj model;
	char buffer[50];
	int verts = 0;
	int text_coords = 0;
	int normals = 0;
	int p_spaces = 0;
	int face_count = 0;
	while(fgets(buffer, 50, obj) != NULL){
		switch(buffer[0]){
			case 'v':
				if(buffer[1] == ' '){
					verts++;
				}
				else if(buffer[1] == 't'){
					text_coords++;
				}
				else if(buffer[1] == 'n'){
					normals++;
				}
				else if(buffer[1] == 'p'){
					p_spaces++;
				}
				break;
			case 'f':
				face_count++;
				break;
		}

	}
	fseek(obj, 0, SEEK_SET);
	Vertex verteces[verts];
	TextureCoord coords[text_coords];
	VertexNormal vnormals[normals];
	PSpaceVertex pverteces[p_spaces];
	Face faces[face_count];
	int v_i = 0;
	int t_i = 0;
	int n_i = 0;
	int p_i = 0;
	int f_i = 0;
	while(fgets(buffer, 50, obj) != NULL){
			switch(buffer[0]){
				case 'v':
					if(buffer[1] == ' '){
						verteces[v_i] = read_vertex(buffer);
						v_i++;
					}
					else if(buffer[1] == 't'){
						coords[t_i] = read_texture_coord(buffer);
						t_i++;
					}
					else if(buffer[1] == 'n'){
						vnormals[n_i] = read_vertex_normal(buffer);
						n_i++;
					}
					else if(buffer[1] == 'p'){
						pverteces[p_i] = read_pvertex(buffer);
						p_i++;
					}
					break;
				case 'f':
					faces[f_i] = read_face(buffer);
					f_i++;
					break;
			}
	}
	for(int i=0; i<face_count;i++){
		//fprintf(stdout, "%i\n", faces[i].fp[0].v);
	}
	fprintf(stdout, "Verteces: %i\n", verts);
	fprintf(stdout, "TextureCoords: %i\n", text_coords);
	fprintf(stdout, "VertexNormals: %i\n", normals);
	fprintf(stdout, "PSpaceVerteces: %i\n", p_spaces);
	fprintf(stdout, "Faces: %i\n", face_count);

	return model;
}

Vertex read_vertex(char line[]){
	char * val;
	Vertex v;
	val = strtok(line, " ");
	float vals[4];
	int i = 0;
	while(val != NULL){
		if(!strstr(val, "v")){
			vals[i] = atof(val);
			i++;
		}
		val = strtok(NULL, " ");

	}
	v.x = vals[0];
	v.y = vals[1];
	v.z = vals[2];
	if(i > 3){
		v.w = vals[3];
	}
	return v;
}

TextureCoord read_texture_coord(char line[]){
	char * val;
	TextureCoord t;
	val = strtok(line, " ");
	float vals[3];
	int i = 0;
	while(val != NULL){
		if(!strstr(val, "v")){
			vals[i] = atof(val);
			i++;
		}
		val = strtok(NULL, " ");
	}
	t.u = vals[0];
	t.v = vals[1];
	t.w = vals[2];
	return t;
}
VertexNormal read_vertex_normal(char line[]){
	char * val;
	VertexNormal n;
	val = strtok(line, " ");
	float vals[3];
	int i = 0;
	while(val != NULL){
		if(!strstr(val, "v")){
			vals[i] = atof(val);
			i++;
		}
		val = strtok(NULL, " ");
	}
	n.x = vals[0];
	n.y = vals[1];
	n.z = vals[2];
	return n;
}

PSpaceVertex read_pvertex(char line[]){
	char * val;
	PSpaceVertex p;
	val = strtok(line, " ");
	float vals[3];
	int i = 0;
	while(val != NULL){
		if(!strstr(val, "v")){
			vals[i] = atof(val);
			i++;
		}
		val = strtok(NULL, " ");
	}
	p.u = vals[0];
	p.v = vals[1];
	p.w = vals[2];
	return p;
}

Face read_face(char line[]){
	char * values;
	char temp_line[strlen(line)];
	strcpy(temp_line, line);
	puts(temp_line);
	Face f;
	values = strtok(line, " ");
	int total_params = 0;
	while(values != NULL){
		if(!strstr(values, "f")){
			total_params++;
		}
		values = strtok(NULL, " ");
	}

	FaceParam f_params[total_params];
	char *temp1 = temp_line;
	values = strtok_r(temp_line, " ", &temp1);
	int param_counter = 0;

	while(values != NULL){
		int v;
		int vt;
		int vn;
		int i = 0;
		fprintf(stdout, values);
		if(!strstr(values, "f")){
			FaceParam fp;
			char * sub_value;
			char *temp2 = values;
			sub_value = strtok_r(values, "/", temp2);
			while(sub_value != NULL){
				switch(i){
					case 0:
						v = atoi(sub_value);
						fprintf(stdout, "%i", v);
						break;
					case 1:
						vt = atoi(sub_value);
						break;
					case 2:
						vn = atoi(sub_value);
				}
				i++;
				sub_value = strtok_r(values, "/", temp2);
			}
			fp.v = v;
			fp.vt = vt;
			fp.vn = vn;
			f_params[param_counter] = fp;
			param_counter++;
		}

		values = strtok_r(temp_line, " ", temp1);
	}
	f.fp = f_params;
	return f;

}


