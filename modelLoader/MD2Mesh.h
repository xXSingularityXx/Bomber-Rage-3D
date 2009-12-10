#ifndef MD2_MESH_H
#define MD2_MESH_H

#include "MeshLoader.h"
#include <fstream>
#include <iomanip>
#include <map>
#include <vector>
#include <string>

namespace MD2
{
	//magic number IDP2
	const int MD2_IDENT = (('2'<<24) + ('P'<<16) + ('D'<<8) + 'I');

	//file versoin
	const int MD2_VERSION = 8;

	// The Vertex Struct
	typedef struct
	{
		unsigned char   v[3];                // compressed vertex (x, y, z) coordinates
		unsigned char   lightnormalindex;    // index to a normal vector for the lighting

	} VertexCoord;

	// The Frame Struct
	typedef struct
	{
		float			scale[3];       // scale values
		float			translate[3];   // translation vector
		char			name[16];       // frame name
		VertexCoord*	verts;			// vertex list
	} Frame;

	// triangle
	typedef struct
	{
		short   index_vert[3];    // indexes to triangle's vertices
		short   index_st[3];      // indexes to vertices' texture coorinates
	} Triangle;

	// Texture coords.
	struct TexCoord
	{
	  short s;
	  short t;
	};

	// animation state
	typedef struct
	{
		int     startframe;              // first frame
		int     endframe;                // last frame
		int     fps;                     // frame per second for this animation

		float   curr_time;               // current time
		float   old_time;                // old time
		float   interpol;                // percent of interpolation

		int     type;                    // animation type

		int     curr_frame;              // current frame
		int     next_frame;              // next frame

	} AnimState;

	typedef struct
	{
		int     ident;              // magic number. must be equal to "IDP2"
		int     version;            // md2 version. must be equal to 8

		int     skinwidth;          // width of the texture
		int     skinheight;         // height of the texture
		int     framesize;          // size of one frame in bytes

		int     num_skins;          // number of textures
		int     num_xyz;            // number of vertices
		int     num_st;             // number of texture coordinates
		int     num_tris;           // number of triangles
		int     num_glcmds;         // number of opengl commands
		int     num_frames;         // total number of frames

		int     ofs_skins;          // offset to skin names (64 bytes each)
		int     ofs_st;             // offset to s-t texture coordinates
		int     ofs_tris;           // offset to triangles
		int     ofs_frames;         // offset to frame data
		int     ofs_glcmds;         // offset to opengl commands
		int     ofs_end;            // offset to end of file
	} Header;

	struct AnimInfo
	{
		int startFrame;
		int endFrame;
	};



class MD2Mesh
{
public:
	MD2Mesh::MD2Mesh();
	~MD2Mesh();
	void loadMD2(std::string _path);


	typedef std::map<std::string, AnimInfo> AnimMap;

	Header		 m_header;
	Triangle*	 m_tris;
	TexCoord*	 m_texCoord;
	Frame*		 m_frames;
	int*	     m_glcmds;

	int m_currFrame;
	int m_nextFrame;
	float m_interp;
	float m_percent;
	float m_scale;
	float m_time; //will aways be between 0 and 1;
	AnimMap m_anims;
	AnimInfo m_frameInfo;

private:
	void setupAnimation();

};

}

#endif

