#include "AnimatedNode.h"

void AnimatedNode::setAnimation(std::string _animName)
{
	if (m_md2Mesh->m_anims.find(_animName.c_str()) != m_md2Mesh->m_anims.end())
		m_md2Mesh->m_frameInfo = m_md2Mesh->m_anims[_animName.c_str()];
	else {
		m_md2Mesh->m_frameInfo.startFrame = 0;
		m_md2Mesh->m_frameInfo.endFrame = 0;
		std::cout << "Animation not found.";
	}
}

void AnimatedNode::advanceFrame(float _dt)
{
	// if time is negative dont do anything
	if (_dt < 0) {
        return;
    }
	// increment the time by _dt
	m_md2Mesh->m_time += _dt;

	if ( m_md2Mesh->m_time < 1000000000) {
		//if time greater than 1, subtract it by 1
        m_md2Mesh->m_time -= (int)m_md2Mesh->m_time;
    }
    else {
        m_md2Mesh->m_time=0;
    }
}

void AnimatedNode::draw()
{
	glPushAttrib(GL_ENABLE_BIT);
	glPushMatrix();

	if (m_isTexturizedNode)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// We don't combine the color with the original surface color, use only the texture map.
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	}
	else
	{
		glDisable(GL_TEXTURE_2D);
	}

	float time = m_md2Mesh->m_time;
	int endFrame = m_md2Mesh->m_frameInfo.endFrame;
	int startFrame = m_md2Mesh->m_frameInfo.startFrame;

	glRotatef(-90, 1, 0, 0);
	glRotatef(-90, 0, 0, 1);

	glTranslatef(m_Position.x, m_Position.y, m_Position.z);
	glRotatef(m_AngleRotation, m_Rotation.x, m_Rotation.y, m_Rotation.z);
	glScalef(m_Scale.x, m_Scale.y, m_Scale.z);

	glColor3f(m_Color.r, m_Color.g , m_Color.b);

	//Figure out the two frames between which we are interpolating
	int frameIndex1 = (int)(time * (endFrame - startFrame + 1)) + startFrame;
	if (frameIndex1 > endFrame) {
		frameIndex1 = startFrame;
	}

	int frameIndex2;
	if (frameIndex1 < endFrame) {
		frameIndex2 = frameIndex1 + 1;
	}
	else {
		frameIndex2 = startFrame;
	}

	//Figure out the fraction between the two frames
	float frac =
		(time - (float)(frameIndex1 - startFrame) /
		(float)(endFrame - startFrame + 1)) * (endFrame - startFrame + 1);


	//Draw the model as an interpolation between the two frames
	glBegin(GL_TRIANGLES);
	  for (int i = 0; i < m_md2Mesh->m_header.num_tris; ++i)
      {
		  	// Draw each vertex of this triangle
			for (int j = 0; j < 3; ++j)
			{
				// point to the the frame at frameIndexes
				MD2::Frame *pFrameA = &m_md2Mesh->m_frames[frameIndex1];
				MD2::Frame *pFrameB = &m_md2Mesh->m_frames[frameIndex2];

				// point to the vertices we will interpolate
				MD2::VertexCoord *pVertA = &pFrameA->verts[m_md2Mesh->m_tris[i].index_vert[j]];
				MD2::VertexCoord *pVertB = &pFrameB->verts[m_md2Mesh->m_tris[i].index_vert[j]];

				// if we want load it with a texture
				if (m_isTexturizedNode)
				{
					MD2::TexCoord *pTexCoords = &m_md2Mesh->m_texCoord[m_md2Mesh->m_tris[i].index_st[j]];
					 // Compute final texture coords.
					GLfloat s = static_cast<GLfloat>(pTexCoords->s) / m_md2Mesh->m_header.skinwidth;
					GLfloat t = static_cast<GLfloat>(pTexCoords->t) / m_md2Mesh->m_header.skinheight;

					glTexCoord2f (s, 1.0f - t);
				}
				//MD2_NORMALS
				// Compute interpolated normal vector
				const GLfloat *normA = &MD2_NORMALS[pVertA->lightnormalindex];
				const GLfloat *normB = &MD2_NORMALS[pVertB->lightnormalindex];

				float n[3];
				n[0] = normA[0] + frac * (normB[0] - normA[0]);
				n[1] = normA[1] + frac * (normB[1] - normA[1]);
				n[2] = normA[2] + frac * (normB[2] - normA[2]);

				glNormal3fv (n);

				float vecA[3], vecB[3], v[3];
				// First, uncompress vertex positions
				vecA[0] = pFrameA->scale[0] * pVertA->v[0] + pFrameA->translate[0];
				vecA[1] = pFrameA->scale[1] * pVertA->v[1] + pFrameA->translate[1];
				vecA[2] = pFrameA->scale[2] * pVertA->v[2] + pFrameA->translate[2];

				vecB[0] = pFrameB->scale[0] * pVertB->v[0] + pFrameB->translate[0];
				vecB[1] = pFrameB->scale[1] * pVertB->v[1] + pFrameB->translate[1];
				vecB[2] = pFrameB->scale[2] * pVertB->v[2] + pFrameB->translate[2];

					    // Linear interpolation and scaling
				v[0] = (vecA[0] + frac * (vecB[0] - vecA[0]));
				v[1] = (vecA[1] + frac * (vecB[1] - vecA[1]));
				v[2] = (vecA[2] + frac * (vecB[2] - vecA[2]));

				glVertex3fv (v);
			 }
		}

	glEnd();
	glPopAttrib();
	glPopMatrix();
}
