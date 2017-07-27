typedef struct {
	float x, y, z, w;
} Vertex;

typedef struct{
	float u, v, w;
} TextureCoord;

typedef struct{
	float x, y, z;
} VertexNormal;

typedef struct {
	float u, v, w;
} PSpaceVertex;

typedef struct {
	int v, vt, vn;
} FaceParam;

typedef struct {
	FaceParam *fp;
} Face;



typedef struct{
	Face *f;
	Vertex *v;
	TextureCoord *t;
	VertexNormal *n;
} Obj;

Obj read_object(FILE * obj);

Vertex read_vertex(char line[]);

TextureCoord read_texture_coord(char line[]);

VertexNormal read_vertex_normal(char line[]);

Face read_face(char line[]);

PSpaceVertex read_pvertex(char line[]);
