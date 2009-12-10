
#include "StaticNode.h"

/*
	draw the note at the screen
*/

void StaticNode::draw()
{
	glPushMatrix();
	glPushAttrib(GL_ENABLE_BIT);

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
	
	glTranslatef(m_Position.x, m_Position.y, m_Position.z);
	glRotatef(m_AngleRotation, m_Rotation.x, m_Rotation.y, m_Rotation.z);
	glScalef(m_Scale.x, m_Scale.y, m_Scale.z);

	glColor3f(m_Color.r, m_Color.g , m_Color.b);

	glBegin(GL_TRIANGLES); // glBegin and glEnd delimit the vertices that define a primitive (in our case triangles)
	for (int l_index=0; l_index < m_ObjMesh->getPolygonsQty(); l_index++)
    {
        //----------------- FIRST VERTEX -----------------

		// Texture coordinates of the first vertex	
		if (m_isTexturizedNode)
		glTexCoord2f( m_ObjMesh->getTextureFromIndex(m_ObjMesh->getPolygonFromIndex(l_index).a).u,
                      m_ObjMesh->getTextureFromIndex(m_ObjMesh->getPolygonFromIndex(l_index).a).v);
       
		// Coordinates of the first vertex		
		glVertex3f( m_ObjMesh->getVertexFromIndex(m_ObjMesh->getPolygonFromIndex(l_index).a).x,
                    m_ObjMesh->getVertexFromIndex(m_ObjMesh->getPolygonFromIndex(l_index).a).y,
                    m_ObjMesh->getVertexFromIndex(m_ObjMesh->getPolygonFromIndex(l_index).a).z); 


        //----------------- SECOND VERTEX -----------------

        // Texture coordinates of the second vertex		
		if (m_isTexturizedNode)
        glTexCoord2f( m_ObjMesh->getTextureFromIndex(m_ObjMesh->getPolygonFromIndex(l_index).b).u,
                      m_ObjMesh->getTextureFromIndex(m_ObjMesh->getPolygonFromIndex(l_index).b).v);

        // Coordinates of the second vertex
		glVertex3f( m_ObjMesh->getVertexFromIndex(m_ObjMesh->getPolygonFromIndex(l_index).b).x,
                    m_ObjMesh->getVertexFromIndex(m_ObjMesh->getPolygonFromIndex(l_index).b).y,
                    m_ObjMesh->getVertexFromIndex(m_ObjMesh->getPolygonFromIndex(l_index).b).z); 
        

        //----------------- THIRD VERTEX -----------------

        // Texture coordinates of the third vertex
		if (m_isTexturizedNode)
		glTexCoord2f( m_ObjMesh->getTextureFromIndex(m_ObjMesh->getPolygonFromIndex(l_index).c).u,
                      m_ObjMesh->getTextureFromIndex(m_ObjMesh->getPolygonFromIndex(l_index).c).v);
        
		// Coordinates of the Third vertex		
		glVertex3f( m_ObjMesh->getVertexFromIndex(m_ObjMesh->getPolygonFromIndex(l_index).c).x,
                    m_ObjMesh->getVertexFromIndex(m_ObjMesh->getPolygonFromIndex(l_index).c).y,
                    m_ObjMesh->getVertexFromIndex(m_ObjMesh->getPolygonFromIndex(l_index).c).z); 
    }
    glEnd();
	glPopMatrix();
	glPopAttrib();
}

