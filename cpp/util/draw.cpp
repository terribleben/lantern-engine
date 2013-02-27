
// draw.cpp

#include "draw.h"

void lookAt(double eye_x, double eye_y, double eye_z, double at_x, double at_y, double at_z, double up_x, double up_y, double up_z) {
    GLfloat m[16];
    GLfloat x[3], y[3], z[3];
    GLfloat mag;
    
    /* Make rotation matrix */
    
    /* Z vector */
    z[0] = eye_x - at_x;
    z[1] = eye_y - at_y;
    z[2] = eye_z - at_z;
    
    mag = sqrt(z[0] * z[0] + z[1] * z[1] + z[2] * z[2]);
    if (mag) {   /* mpichler, 19950515 */
        z[0] /= mag;
        z[1] /= mag;
        z[2] /= mag;
    }
    
    /* Y vector */
    y[0] = up_x;
    y[1] = up_y;
    y[2] = up_z;
    
    /* X vector = Y cross Z */
    x[0] = y[1] * z[2] - y[2] * z[1];
    x[1] = -y[0] * z[2] + y[2] * z[0];
    x[2] = y[0] * z[1] - y[1] * z[0];
    
    /* Recompute Y = Z cross X */
    y[0] = z[1] * x[2] - z[2] * x[1];
    y[1] = -z[0] * x[2] + z[2] * x[0];
    y[2] = z[0] * x[1] - z[1] * x[0];
    
    /* mpichler, 19950515 */
    /* cross product gives area of parallelogram, which is < 1.0 for
     * non-perpendicular unit-length vectors; so normalize x, y here
     */
    mag = sqrt(x[0] * x[0] + x[1] * x[1] + x[2] * x[2]);
    if (mag) {
        x[0] /= mag;
        x[1] /= mag;
        x[2] /= mag;
    }
    mag = sqrt(y[0] * y[0] + y[1] * y[1] + y[2] * y[2]);
    if (mag) {
        y[0] /= mag;
        y[1] /= mag;
        y[2] /= mag;
    }
    
#define M(row,col)  m[col*4+row]
    M(0,0) = x[0];
    M(0,1) = x[1];
    M(0,2) = x[2];
    M(0,3) = 0.0;
    M(1,0) = y[0];
    M(1,1) = y[1];
    M(1,2) = y[2];
    M(1,3) = 0.0;
    M(2,0) = z[0];
    M(2,1) = z[1];
    M(2,2) = z[2];
    M(2,3) = 0.0;
    M(3,0) = 0.0;
    M(3,1) = 0.0;
    M(3,2) = 0.0;
    M(3,3) = 1.0;    
#undef M
    
    // multiply into m
    glMultMatrixf(m);
    
    // translate eye to origin
    glTranslatef(-eye_x, -eye_y, -eye_z);
}

void drawPolygon(Point3f* vertices, unsigned int numVertices, bool filled) {
	GLfloat glVertices[numVertices * 2];
	for (unsigned int i = 0; i < numVertices; i++) {
		glVertices[i * 2] = vertices[i].x;
		glVertices[i * 2 + 1] = vertices[i].y;
	}
	
	glVertexPointer(2, GL_FLOAT, 0, glVertices);
	glDrawArrays((filled) ? GL_TRIANGLE_FAN : GL_LINE_LOOP, 0, numVertices);
}

void drawRectangle(Point3f topLeft, Point3f bottomRight, bool filled) {
	GLfloat vertices[4 * 2];
	
	vertices[0] = topLeft.x; vertices[1] = topLeft.y; // top left
	vertices[2] = bottomRight.x; vertices[3] = topLeft.y; // top right
	if (filled) {
		vertices[4] = topLeft.x; vertices[5] = bottomRight.y; // bottom left
		vertices[6] = bottomRight.x; vertices[7] = bottomRight.y; // bottom right
	} else {
		vertices[4] = bottomRight.x; vertices[5] = bottomRight.y; // bottom right
		vertices[6] = topLeft.x; vertices[7] = bottomRight.y; // bottom left
	}
	
	glVertexPointer(2, GL_FLOAT, 0, vertices);
	glDrawArrays((filled) ? GL_TRIANGLE_STRIP : GL_LINE_LOOP, 0, 4);
}

void drawQuadrilateral(Point3f p1, Point3f p2, Point3f p3, Point3f p4, bool filled) {
	GLfloat vertices[4 * 2];
	
	vertices[0] = p1.x; vertices[1] = p1.y;
	vertices[2] = p2.x; vertices[3] = p2.y;
	vertices[4] = p3.x; vertices[5] = p3.y;
	vertices[6] = p4.x; vertices[7] = p4.y;
	
	glVertexPointer(2, GL_FLOAT, 0, vertices);
	glDrawArrays((filled) ? GL_TRIANGLE_STRIP : GL_LINE_LOOP, 0, 4);
}

void drawTriangle(Point3f p1, Point3f p2, Point3f p3, bool filled) {
	GLfloat vertices[3 * 2];
	
	vertices[0] = p1.x; vertices[1] = p1.y;
	vertices[2] = p2.x; vertices[3] = p2.y;
	vertices[4] = p3.x; vertices[5] = p3.y;
		
	glVertexPointer(2, GL_FLOAT, 0, vertices);
	glDrawArrays((filled) ? GL_TRIANGLE_STRIP : GL_LINE_LOOP, 0, 3);
}

void drawLine(Point3f p1, Point3f p2) {
	GLfloat vertices[2 * 2];

	vertices[0] = p1.x; vertices[1] = p1.y;
	vertices[2] = p2.x; vertices[3] = p2.y;

	glVertexPointer(2, GL_FLOAT, 0, vertices);
	glDrawArrays(GL_LINES, 0, 2);
}

void drawLineWithColor(Point3f p1, Point3f p2, Point3f col1, Point3f col2, float alpha1, float alpha2) {
	glEnableClientState(GL_COLOR_ARRAY);
	GLfloat colors[2 * 4];
	
	colors[0] = col1.x; colors[1] = col1.y; colors[2] = col1.z; colors[3] = alpha1;
	colors[4] = col1.x; colors[5] = col1.y; colors[6] = col1.z; colors[7] = alpha2;
	glColorPointer(4, GL_FLOAT, 0, colors);
	
	drawLine(p1, p2);
	glDisableClientState(GL_COLOR_ARRAY);
}

void drawTexture(Point3f position, float width, float height) {
	float halfWidth = width / 2.0f;
	float halfHeight = height / 2.0f;
    
    GLfloat vertices[] = {
        position.x - halfWidth, position.y + halfHeight, 0,
        position.x + halfWidth, position.y + halfHeight, 0,
        position.x - halfWidth, position.y - halfHeight, 0,
        position.x + halfWidth, position.y - halfHeight, 0
    };
    
    GLfloat texCoords[] = {
        0, 0,
        1, 0,
        0, 1,
        1, 1
    };
	drawTexture(vertices, texCoords);
}

void drawTexture(Point3f p1, Point3f p2, Point3f p3, Point3f p4) {
    drawTexture(p1, p2, p3, p4, false, false);
}

void drawTexture(Point3f p1, Point3f p2, Point3f p3, Point3f p4, bool xReversed, bool yReversed) {
    GLfloat vertices[] = {
        p1.x, p1.y, p1.z,
        p2.x, p2.y, p2.z,
        p3.x, p3.y, p3.z,
        p4.x, p4.y, p4.z
    };
    
    int xRev = (xReversed) ? 1 : 0;
    int yRev = (yReversed) ? 1 : 0;
    
    GLfloat texCoords[] = {
        xRev, 1 - yRev,
        1 - xRev, 1 - yRev,
        xRev, yRev,
        1 - xRev, yRev
    };
	drawTexture(vertices, texCoords);
}

void drawTexture(GLfloat* vertices, GLfloat* texCoords) {
	GLfloat normals[] = {
        0, 0, 1,
        0, 0, 1,
        0, 0, 1,
        0, 0, 1
    };
	
	glEnable(GL_TEXTURE_2D);
    glPushMatrix();
	
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glNormalPointer(GL_FLOAT, 0, normals);
    glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    
    glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

void transformVertices(GLfloat* vertices, unsigned int numVertices, Point3f &position, float orientation) {
	for (unsigned int i = 0; i < numVertices; i++) {
		vertices[i * 2] = (vertices[i * 2] * cos(orientation)) + position.x;
		vertices[i * 2 + 1] = (vertices[i * 2 + 1] * sin(orientation)) + position.y;
	}
}

void drawQuadrilateral(Point3f p1, Point3f p2, Point3f p3, Point3f p4, bool filled, Point3f &position, float orientation) {
	GLfloat vertices[4 * 2];
	
	vertices[0] = p1.x; vertices[1] = p1.y;
	vertices[2] = p2.x; vertices[3] = p2.y;
	vertices[4] = p3.x; vertices[5] = p3.y;
	vertices[6] = p4.x; vertices[7] = p4.y;
	
	transformVertices(vertices, 4, position, orientation);
	
	glVertexPointer(2, GL_FLOAT, 0, vertices);
	glDrawArrays((filled) ? GL_TRIANGLE_STRIP : GL_LINE_LOOP, 0, 4);
}

void drawTriangle(Point3f p1, Point3f p2, Point3f p3, bool filled, Point3f &position, float orientation) {
	GLfloat vertices[3 * 2];
	
	vertices[0] = p1.x; vertices[1] = p1.y;
	vertices[2] = p2.x; vertices[3] = p2.y;
	vertices[4] = p3.x; vertices[5] = p3.y;
	
	transformVertices(vertices, 3, position, orientation);
	
	glVertexPointer(2, GL_FLOAT, 0, vertices);
	glDrawArrays((filled) ? GL_TRIANGLE_STRIP : GL_LINE_LOOP, 0, 3);
}