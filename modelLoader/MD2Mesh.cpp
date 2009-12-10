
#include "MD2Mesh.h"


MD2::MD2Mesh::MD2Mesh()
{
	m_time=0;
	//m_frameInfo.startFrame = 0;
	//m_frameInfo.endFrame = m_header.num_frames-1;
}

MD2::MD2Mesh::~MD2Mesh()
{
	delete [] m_tris;
	delete [] m_texCoord;
	delete [] m_frames;
	delete [] m_glcmds;
}

void MD2::MD2Mesh::loadMD2(std::string _path)
{
	//open the MD2 file
	std::ifstream file;
	file.open(_path.c_str(), std::ios::binary);

	if (file.fail())
		return;

	//read the header of the file
	file.read (reinterpret_cast<char *>(&m_header), sizeof (MD2::Header));

	// Check if identification and version are valid
	if (m_header.ident != MD2::MD2_IDENT)
	{
		file.close();
		return;
	}

	if (m_header.version != MD2::MD2_VERSION)
	{
		file.close();
		return;
	}

	// Memory allocation for model data
	//_skins = new Md2Skin_t[_header.num_skins];
	m_texCoord = new MD2::TexCoord[m_header.num_st];
	m_tris = new MD2::Triangle[m_header.num_tris];
	m_frames = new MD2::Frame[m_header.num_frames];
	m_glcmds = new int[m_header.num_glcmds];

	// Read texture coords.
	file.seekg (m_header.ofs_st, std::ios::beg);
	file.read (reinterpret_cast<char *>(m_texCoord), sizeof (MD2::TexCoord) * m_header.num_st);

	// Read triangle data
	file.seekg (m_header.ofs_tris, std::ios::beg);
	file.read (reinterpret_cast<char *>(m_tris), sizeof(MD2::Triangle) * m_header.num_tris);

	// Read frames
	file.seekg (m_header.ofs_frames, std::ios::beg);
	for (int i = 0; i < m_header.num_frames; ++i)
    {
        // Memory allocation for the vertices of this frame
		m_frames[i].verts = new MD2::VertexCoord[m_header.num_xyz];

		// Read frame data: scale, translate, name of animation and array of vertices
		file.read (reinterpret_cast<char *>(&m_frames[i].scale), sizeof (float)*3);
		file.read (reinterpret_cast<char *>(&m_frames[i].translate), sizeof (float)*3);
		file.read (reinterpret_cast<char *>(&m_frames[i].name), sizeof (char) * 16);
		file.read (reinterpret_cast<char *>(m_frames[i].verts), sizeof (MD2::VertexCoord) * m_header.num_xyz);
    }

	// Read OpenGL commands
	file.seekg (m_header.ofs_glcmds, std::ios::beg);
	file.read (reinterpret_cast<char *>(m_glcmds), sizeof (int) * m_header.num_glcmds);

	file.close();

	setupAnimation();
}

void MD2::MD2Mesh::setupAnimation()
{
	std::string currentAnim;
	MD2::AnimInfo animInfo = { 0, 0 };

	for (int i = 0; i < m_header.num_frames; ++i)
    {
		std::string frameName = m_frames[i].name;
		std::string frameAnim;

		// Extract animation name from frame name
		std::string::size_type len = frameName.find_first_of ("0123456789");
		if ((len == frameName.length () - 3) &&	(frameName[len] != '0'))
			len++;
			frameAnim.assign (frameName, 0, len);

			if (!currentAnim.compare(frameAnim) == 0)
			{
				if (i > 0)
				{
					// Previous animation is finished, insert
					// it and start new animation.
					m_anims.insert (AnimMap::value_type
					 (currentAnim, animInfo));
				}

				// Initialize new anim info
				animInfo.startFrame = i;
				animInfo.endFrame = i;

				currentAnim = frameAnim;
			}
			else
			{
				animInfo.endFrame = i;
			}
    }

	// Insert last animation
	m_anims.insert (AnimMap::value_type (currentAnim, animInfo));
}