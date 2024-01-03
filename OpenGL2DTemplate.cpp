#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <glut.h>
#include <cmath>
#include <ctime>
#include <iostream>
#include <random>

#define GLUT_KEY_ESCAPE 27
#define DEG2RAD(a) (a * 0.0174532925)
#define M_PI 3.14159265358979323846
float humanx = 0;
float humanz = 0;
float Speed = 0.1;
float RotY = 0;


GLUquadric* qobj;

float RotF = 80;

float TransK = 0;
bool FlagK = true;
bool FlagVisKey = true;

float RotB = 0;
bool FlagB = true;

float RotS = 0;
bool FlagS = true;

int Time = 1000;

bool Won = false;
bool Lost = false;

float TransR = 0;
bool FlagR = true;

bool AnimationOnOff = false;
bool Fencecolor = false;

float randx = 1;
float randy = 1;
float randz = 1;

bool GoalReached = false;


	void print(int x, int y, const char* string)
	{
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT));

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		glRasterPos2i(x, y);  // Set the position of the text in window coordinates
		int len = (int)strlen(string);

		for (int i = 0; i < len; i++)
		{
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
		}

		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	}

float GenerateRandomcolor() {
	std::random_device rd;
	std::default_random_engine engine(rd());
	std::uniform_real_distribution<float> distribution(0.0, 1.0);
	return distribution(engine);
}

void time(int val)//timer animation function, allows the user to pass an integer valu to the timer function.
{
	if (Time == 00) {
		if (FlagVisKey)
			Lost = true;
		else
		{
			Won = true;
		}

	}
	else {
		Time--;
	}

	glutPostRedisplay();
	glutTimerFunc(1000, time, 0);

	printf("time %d", Time);


}


void Timer(int value) {

	if (AnimationOnOff) {
		RotF += 0.1;

		if (RotF > 360)
			RotF -= 360;


		if (FlagK)
		{
			TransK += 0.004;
			if (TransK >= 0.1) {
				FlagK = false;
			}
		}
		else {
			TransK -= 0.004;
			if (TransK <= 0) {
				FlagK = true;
			}
		}

		if (FlagB)
		{
			RotB += 0.035;
			if (RotB >= 0.5) {
				FlagB = false;
			}
		}
		else {
			RotB -= 0.035;
			if (RotB <= -2) {
				FlagB = true;
			}
		}


		if (FlagS)
		{
			RotS += 0.1;
			if (RotS >= 10) {
				FlagS = false;
			}
		}
		else {
			RotS -= 0.1;
			if (RotS <= -10) {
				FlagS = true;
			}
		}

		if (FlagR)
		{
			TransR += 0.01;
			if (TransR >= 0.8) {
				FlagR = false;
			}
		}
		else {
			TransR -= 0.01;
			if (TransR <= 0) {
				FlagR = true;
			}
		}

	}
	glutPostRedisplay();
	glutTimerFunc(16, Timer, 0);

}

void Loc(int k, int x, int y)
{
	if (!GoalReached && !Lost) {
		if (humanx >= 5 || humanx <= -5 || humanz >= 2) {
			if (humanx >= 5) {
				humanx -= 0.1;
			}
			if (humanx <= -5) {
				humanx += 0.1;
			}
			if (humanz >= 2) {
				humanz -= 0.1;
			}
			if (humanz <= -18) {
				humanz += 0.1;
			}
		}
		else
		{
			if (k == GLUT_KEY_RIGHT) {
				humanx += Speed;
				RotY = -90;
			}
			if (k == GLUT_KEY_LEFT)
			{
				humanx -= Speed;
				RotY = 90;
			}
			if (k == GLUT_KEY_UP)
			{
				humanz -= Speed;
				RotY = 0;
			}
			if (k == GLUT_KEY_DOWN)
			{
				humanz += Speed;
				RotY = 180;
			}
		}

		if (humanx >= -0.2 && humanx <= 0.2 && humanz >= -1.1 && humanz <= -0.9) {
			FlagVisKey = false;
			AnimationOnOff = true;
		}
		if (humanz <= -18) {
			GoalReached = true;
			if (!FlagVisKey)
				Won = true;
		}
	}
	glutPostRedisplay();
}

class Vector3f {
public:
	float x, y, z;

	Vector3f(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) {
		x = _x;
		y = _y;
		z = _z;
	}

	Vector3f operator+(Vector3f& v) {
		return Vector3f(x + v.x, y + v.y, z + v.z);
	}

	Vector3f operator-(Vector3f& v) {
		return Vector3f(x - v.x, y - v.y, z - v.z);
	}

	Vector3f operator*(float n) {
		return Vector3f(x * n, y * n, z * n);
	}

	Vector3f operator/(float n) {
		return Vector3f(x / n, y / n, z / n);
	}

	Vector3f unit() {
		return *this / sqrt(x * x + y * y + z * z);
	}

	Vector3f cross(Vector3f v) {
		return Vector3f(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
};

class Camera {
public:
	Vector3f eye, center, up;

	Camera(float eyeX = .0f, float eyeY = 1.0f, float eyeZ = 1.0f, float centerX = 0.0f, float centerY = 0.0f, float centerZ = 0.0f, float upX = 0.0f, float upY = 1.0f, float upZ = 0.0f) {
		eye = Vector3f(eyeX, eyeY, eyeZ);
		center = Vector3f(centerX, centerY, centerZ);
		up = Vector3f(upX, upY, upZ);
	}

	void moveX(float d) {
		Vector3f right = up.cross(center - eye).unit();
		eye = eye + right * d;
		center = center + right * d;
	}

	void moveY(float d) {
		eye = eye + up.unit() * d;
		center = center + up.unit() * d;
	}
	void setRightSideView() {
		eye = Vector3f(9, 1.85, -6.3);
		center = Vector3f(8, 1.83, -6.3);
		up = Vector3f(0.0f, 1.0f, 0.0f);
	}

	void PlayerView() {
		float eyeX = 0.0f; float eyeY = 1.0f; float eyeZ = 1.0f;
		float centerX = 0.0f; float centerY = 0.0f; float centerZ = 0.0f;
		float upX = 0.0f; float upY = 1.0f; float upZ = 0.0;

		eye = Vector3f(eyeX, eyeY, eyeZ);
		center = Vector3f(centerX, centerY, centerZ);
		up = Vector3f(upX, upY, upZ);
	}

	void setTopView() {
		eye = Vector3f(0.05, 7.41, -7.25);
		center = Vector3f(0.05, 6.41, -7.3);
		up = Vector3f(0.0f, 0.03, -1);
	}

	void setFrontView() {
		eye = Vector3f(0, 4.8, 5);
		center = Vector3f(0, 4.83, 4);
		up = Vector3f(0, 1, 0.03);
	}

	void moveZ(float d) {
		Vector3f view = (center - eye).unit();
		eye = eye + view * d;
		center = center + view * d;
	}

	void rotateX(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + up * sin(DEG2RAD(a));
		up = view.cross(right);
		center = eye + view;
	}

	void rotateY(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + right * sin(DEG2RAD(a));
		right = view.cross(up);
		center = eye + view;
	}

	void look() {
		gluLookAt(
			eye.x, eye.y, eye.z,
			center.x, center.y, center.z,
			up.x, up.y, up.z
		);
	}
};

Camera camera;

void setupLights() {

	GLfloat ambient[] = { 0.7f, 0.7f, 0.7, 1.0f };
	GLfloat diffuse[] = { 0.6f, 0.6f, 0.6, 1.0f };
	GLfloat specular[] = { 1.0f, 1.0f, 1.0, 1.0f };
	GLfloat shininess[] = { 50 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

	GLfloat lightIntensity[] = { 0.7f, 0.7f, 1, 1.0f };
	GLfloat lightPosition[] = { -7.0f, 6.0f, 3.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightIntensity);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);

}
void setupCamera() {

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(120, 1000 / 800, 0.001, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	camera.look();
}

void drawPost(float x, float y, float z, float height) {
	glPushMatrix();
	glTranslatef(x, y, z);
	glScalef(0.1, height, 0.1); // Adjust the scale for post thickness and height
	glutSolidCube(1.0);
	glPopMatrix();
}

void drawHorizontalBar(float x1, float y1, float z1, float x2, float y2, float z2, float thickness) {
	glPushMatrix();
	glTranslatef((x1 + x2) / 2, (y1 + y2) / 2, (z1 + z2) / 2);
	float length = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) + (z2 - z1) * (z2 - z1));
	glScalef(thickness, thickness, length); // Adjust the scale for bar thickness and length
	glutSolidCube(1.0);
	glPopMatrix();
}

void drawHorizontalBar2(float x1, float y1, float z1, float x2, float y2, float z2, float thickness) {
	glPushMatrix();
	glTranslatef((x1 + x2) / 2, (y1 + y2) / 2, (z1 + z2) / 2);
	float length = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) + (z2 - z1) * (z2 - z1));
	glScalef(length, thickness, thickness); // Adjust the scale for bar thickness and length
	glutSolidCube(1.0);
	glPopMatrix();
}




void drawFence() {
	// Draw posts along the line x = 1
	for (float z = -20.0; z <= 2.0; z += 1.0) {
		drawPost(5.0, 0.0, z, 3.5);
	}

	// Draw horizontal bars
	for (float y = 0.5; y <= 2.0; y += 1.0) {
		drawHorizontalBar(5.0, y, -20.0, 5.0, y, 2.0, 0.05);
	}

	for (float z = -20.0; z <= 2.0; z += 1.0) {
		drawPost(-5.0, 0.0, z, 3.5);
	}

	// Draw horizontal bars
	for (float y = 0.5; y <= 2.0; y += 1.0) {
		drawHorizontalBar(-5.0, y, -20.0, -5.0, y, 2.0, 0.05);
	}


	for (float x = -5.0; x <= 5.0; x += 1.0) {
		drawPost(x, 0.0, 2.0, 3.5);
	}

	// Draw horizontal bars
	for (float y = 0.5; y <= 2.0; y += 1.0) {
		drawHorizontalBar2(-5.0, y, 2.0, 5.0, y, 2.0, 0.05);
	}

}


void drawGround() {
	glColor3f(0.4, 0.5, 0.2); // Green color
	glBegin(GL_QUADS);
	glScalef(1, 1.1, 1);
	glVertex3f(-5.0, 0.0, -20.0);
	glVertex3f(5.0, 0.0, -20.0);
	glVertex3f(5.0, 0.0, 2.0);
	glVertex3f(-5.0, 0.0, 2.0);
	glEnd();
}


void drawSphere(double radius, int slices, int stacks) {
	glutSolidSphere(radius, slices, stacks);
}

void drawSolidCylinder(GLdouble radius, GLdouble height, GLint slices, GLint stacks) {
	GLUquadricObj* quadObj = gluNewQuadric();
	gluQuadricDrawStyle(quadObj, GLU_FILL);

	glPushMatrix();
	gluCylinder(quadObj, radius, radius, height, slices, stacks);
	glTranslatef(0.0, 0.0, height);
	glutSolidSphere(radius, slices, stacks);
	glPopMatrix();

	gluDeleteQuadric(quadObj);
}
void drawCylinder(double baseRadius, double topRadius, double height, int slices, int stacks) {
	drawSolidCylinder(baseRadius, height, slices, stacks);
}


void drawBalloon(float x, float y, float z, float r, float g, float b) {
	glColor3f(r, g, b); // Set the color

	// Draw balloon body
	glPushMatrix();
	glTranslatef(x, y, z);
	glutSolidSphere(0.1, 20, 20); // Balloon body
	glPopMatrix();

	// Draw balloon string
	glColor3f(0.5, 0.5, 0.5); // Gray color for the string
	glPushMatrix();
	glBegin(GL_LINES);
	glVertex3f(x, y, z);
	glVertex3f(x, y - 0.4, z); // Adjust the length of the string as needed
	glEnd();
	glPopMatrix();
}

void drawHuman() {
	// Head
	glPushMatrix();
	glColor3f(1.0, 0.8, 0.6); // Skin color
	glTranslatef(0.0 + humanx, 0.8, 0.0 + humanz);
	drawSphere(0.1, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0); // Eye color
	glTranslatef(0.04 + humanx, 0.83, -0.08 + humanz);
	drawSphere(0.015, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0); // Eye color
	glTranslatef(-0.04 + humanx, 0.83, -0.08 + humanz);
	drawSphere(0.015, 20, 20);
	glPopMatrix();

	// Body
	glPushMatrix();
	glColor3f(0.0, 0.4, 1.0); // Blue shirt
	glTranslatef(0.0 + humanx, 0.7, 0.0 + humanz);
	glRotatef(90, 1.0, 0.0, 0.0);
	drawCylinder(0.1, 0.1, 0.3, 20, 1);
	glPopMatrix();

	// Right Leg
	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0); // Black pants
	glTranslatef(0.05 + humanx, 0.4, 0.0 + humanz);
	glRotatef(90, 1.0, 0.0, 0.0);
	drawCylinder(0.05, 0.05, 0.4, 20, 1);
	glPopMatrix();

	// Left Leg
	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0); // Black pants
	glTranslatef(-0.05 + humanx, 0.4, 0.0 + humanz);
	glRotatef(90, 1.0, 0.0, 0.0);
	drawCylinder(0.05, 0.05, 0.4, 20, 1);
	glPopMatrix();

	// Right Arm
	glPushMatrix();
	glColor3f(1.0, 0.8, 0.6); // Skin color
	glTranslatef(0.15 + humanx, 0.7, 0.0 + humanz);
	glRotatef(90, 1.0, 0.0, 0.0);
	drawCylinder(0.05, 0.05, 0.2, 20, 1);
	glPopMatrix();

	// Left Arm
	glPushMatrix();
	glColor3f(1.0, 0.8, 0.6); // Skin color
	glTranslatef(-0.15 + humanx, 0.7, 0.0 + humanz);
	glRotatef(90, 1.0, 0.0, 0.0);
	drawCylinder(0.05, 0.05, 0.2, 20, 1);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(humanx, 0, humanz);
	glRotatef(RotB, 0.0, 0.0, 1.0);
	glRotatef(-10, 1, 0, 0);
	glTranslatef(0.175, 0.9, 0.07);
	drawBalloon(0, 0, 0, 1, 0, 0);
	glPopMatrix();
}

void wheelSupport() {
	//wheel support
	glPushMatrix();
	glTranslatef(-2, 0.8, -7.1);
	glBegin(GL_LINES);
	glVertex2f(-0.7071, 1.7071);
	glVertex2f(0.7071, 0.2929);
	glEnd();

	glPushMatrix();
	glBegin(GL_LINES);
	glVertex2f(0.7071, 1.7071);
	glVertex2f(-0.7071, 0.2929);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glBegin(GL_LINES);
	glVertex2f(-1, 1);
	glVertex2f(1, 1);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glBegin(GL_LINES);
	glVertex2f(0, 2);
	glVertex2f(0, 0);
	glEnd();
	glPopMatrix();
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-2, 0.8, -7.4);
	glBegin(GL_LINES);
	glVertex2f(-0.7071, 1.7071);
	glVertex2f(0.7071, 0.2929);
	glEnd();

	glPushMatrix();
	glBegin(GL_LINES);
	glVertex2f(0.7071, 1.7071);
	glVertex2f(-0.7071, 0.2929);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glBegin(GL_LINES);
	glVertex2f(-1, 1);
	glVertex2f(1, 1);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glBegin(GL_LINES);
	glVertex2f(0, 2);
	glVertex2f(0, 0);
	glEnd();
	glPopMatrix();
}


void DrawCube() {
	// Draw cubes at the beginning of the wheel support lines
	for (int i = 0; i < 2; ++i) {
		glPushMatrix();

		glTranslatef(-2, 0.8, -7.25);


		if (i % 2 == 0) {
			glTranslatef(-0.7071, 1.7071, 0);
		}
		else {
			glTranslatef(0.7071, 1.7071, 0);
		}

		glutSolidCube(0.31);
		glPopMatrix();
	}

	// Draw cubes at the end of the wheel support lines
	for (int i = 0; i < 2; ++i) {
		glPushMatrix();
		glTranslatef(-2, 0.8, -7.25);

		if (i % 2 == 0) {
			glTranslatef(-0.7071, 0.2929, 0);
		}
		else {
			glTranslatef(0.7071, 0.2929, 0);
		}
		glutSolidCube(0.31);
		glPopMatrix();
	}

	glPushMatrix();
	glTranslatef(-2, 0.8, -7.25);
	glTranslatef(0, 2, 0);
	glutSolidCube(0.31);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2, 0.8, -7.25);
	glTranslatef(0, 0, 0);
	glutSolidCube(0.31);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2, 0.8, -7.25);
	glTranslatef(1, 1, 0);
	glutSolidCube(0.31);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2, 0.8, -7.25);
	glTranslatef(-1, 1, 0);
	glutSolidCube(0.31);
	glPopMatrix();
}

void wheelsSupportGroundBase() {

	glColor3f(0.6f, 0.31f, 0.11f);
	glTranslatef(0.0f, 7.5f, 0.0f);
	glScalef(1.0f, 15.0f, 1.0f);
	glutSolidCube(1.0);
}

void wheelsSupportGround() {
	glPushMatrix();
	glTranslatef(9.0f, 0.0f, 3.0f);
	glRotatef(35.0, 0.00, 0.00, 0.10);
	wheelsSupportGroundBase();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-9.0f, 0.0f, 3.0f);
	glRotatef(-35.0, 0.00, 0.00, 0.10);
	wheelsSupportGroundBase();
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-9.0f, 0.0f, 7.0f);
	glRotatef(-35.0, 0.00, 0.00, 0.10);
	wheelsSupportGroundBase();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(9.0f, 0.0f, 7.0f);
	glRotatef(35.0, 0.00, 0.00, 0.10);
	wheelsSupportGroundBase();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 12, 1.96);
	glRotatef(90, 0, 0, 1);
	drawCylinder(1, 1, 5, 25, 20);
	glPopMatrix();

}




void drawFilledDisk() {
	const int numSegments = 100;
	const double radius = 1.0;
	const double height = 0.1;

	glColor3f(0.6f, 0.31f, 0.11f);

	// Draw bottom disc
	GLUquadricObj* bottomQuadObj = gluNewQuadric();
	gluQuadricDrawStyle(bottomQuadObj, GLU_FILL);
	gluDisk(bottomQuadObj, 0.0, radius, numSegments, 1);

	drawCylinder(radius, radius, height, numSegments, 1);

	drawCylinder(0.15, 0.15, 1, numSegments, 1);

	// Draw top disc
	glPushMatrix();
	glTranslatef(0.0, 0.0, height);
	GLUquadricObj* topQuadObj = gluNewQuadric();
	gluQuadricDrawStyle(topQuadObj, GLU_FILL);
	gluDisk(topQuadObj, 0.0, radius, numSegments, 1);
	glPopMatrix();


	gluDeleteQuadric(bottomQuadObj);
	gluDeleteQuadric(topQuadObj);
}

void drawFilledDisktop() {

	const int numSegments = 100;
	const double radius = 1.0;
	const double height = 0.05;

	glColor3f(0.6f, 0.31f, 0.11f);

	// Draw bottom disc
	GLUquadricObj* bottomQuadObj = gluNewQuadric();
	gluQuadricDrawStyle(bottomQuadObj, GLU_FILL);
	gluDisk(bottomQuadObj, 0.0, radius, numSegments, 1);

	drawCylinder(radius, radius, height, numSegments, 1);


	// Draw top disc
	glPushMatrix();
	glTranslatef(0.0, 0.0, height);
	GLUquadricObj* topQuadObj = gluNewQuadric();
	gluQuadricDrawStyle(topQuadObj, GLU_FILL);
	gluDisk(topQuadObj, 0.0, radius, numSegments, 1);
	glPopMatrix();


	gluDeleteQuadric(bottomQuadObj);
	gluDeleteQuadric(topQuadObj);
}


void drawHalfSphere(double radius) {
	int stacks = 10;
	int  slices = 10;

	for (int i = 0; i <= stacks / 2; ++i) {
		double lat0 = M_PI * (-0.5 + (double)(i - 1) / stacks);
		double z0 = sin(lat0) * radius;
		double zr0 = cos(lat0) * radius;

		double lat1 = M_PI * (-0.5 + (double)i / stacks);
		double z1 = sin(lat1) * radius;
		double zr1 = cos(lat1) * radius;

		glBegin(GL_QUAD_STRIP);
		for (int j = 0; j <= slices; ++j) {
			double lng = 2 * M_PI * (double)(j - 1) / slices;
			double x = cos(lng) * radius;
			double y = sin(lng) * radius;

			glNormal3f(x / radius, y / radius, 0.0);
			glVertex3f(x * zr0, y * zr0, z0);
			glVertex3f(x * zr1, y * zr1, z1);
		}
		glEnd();
	}
}

void drawCubes() {

	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslated(0.75, 0.4, 0);
	glRotated(-90, 1, 0, 0);
	drawHalfSphere(0.5);
	glPopMatrix();


	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslated(-0.75, 0.4, 0);
	glRotated(-90, 1, 0, 0);
	drawHalfSphere(0.5);
	glPopMatrix();


	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslated(0, 0.4, 0.75);
	glRotated(-90, 1, 0, 0);
	drawHalfSphere(0.5);
	glPopMatrix();


	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslated(0, 0.4, -0.75);
	glRotated(-90, 1, 0, 0);
	drawHalfSphere(0.5);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslated(0.53, 0.4, 0.53);
	glRotated(-90, 1, 0, 0);
	drawHalfSphere(0.5);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslated(-0.53, 0.4, -0.53);
	glRotated(-90, 1, 0, 0);
	drawHalfSphere(0.5);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslated(-0.53, 0.4, 0.53);
	glRotated(-90, 1, 0, 0);
	drawHalfSphere(0.5);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslated(0.53, 0.4, -0.53);
	glRotated(-90, 1, 0, 0);
	drawHalfSphere(0.5);
	glPopMatrix();
}


void cupsGame() {
	glPushMatrix();
	glRotatef(RotF, 0, 1, 0);
	drawCubes();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 1, 0);
	glRotatef(270, 1, 0, 0);
	drawFilledDisktop();
	glPopMatrix();

	glPushMatrix();
	glRotatef(270, 1, 0, 0);
	drawFilledDisk();
	glPopMatrix();
}

void key() {


	glColor3f(0.588, 0.294, 0);

	glPushMatrix();
	glTranslatef(0, 1, 0);
	glutSolidTorus(0.025, 0.045, 50, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0.84, 0);
	glScalef(0.83, 4.5, 0.83);
	glutSolidCube(0.05);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0155, 0.74, 0);
	glRotatef(90, 0, 0, 1);
	glScalef(0.8, 1.35, 0.8);
	glutSolidCube(0.05);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0155, 0.79, 0);
	glRotatef(90, 0, 0, 1);
	glScalef(0.8, 1.35, 0.8);
	glutSolidCube(0.05);
	glPopMatrix();


}

void drawTree() {
	glPushMatrix();
	glScalef(1.7, 1.7, 1.7);
	glTranslatef(0, 0.6, 0);
	// Tree trunk
	glColor3f(0.5, 0.3, 0.1); // Brown color
	glPushMatrix();
	glScalef(0.2, 1.5, 0.2);
	glutSolidCube(1.0);
	glPopMatrix();

	// Tree leaves
	glColor3f(0.4, 0.5, 0.2); // Green color
	glPushMatrix();
	glTranslatef(0.0, 0.4, 0.0);
	glutSolidSphere(0.35, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 0.7, 0.0);
	glutSolidSphere(0.25, 20, 20);
	glPopMatrix();
	glPopMatrix();

}


void drawFlower() {

	glScalef(0.2, 0.2, 0.2);
	glTranslatef(0, 0.8, 0);

	// Draw stem
	glColor3f(0.0, 0.5, 0.0); // Green color for the stem
	glPushMatrix();
	glScalef(0.1, 1.2, 0.1);
	glutSolidCube(1.0);
	glPopMatrix();

	// Draw two stems forming a V shape
	glColor3f(0.0, 0.5, 0.0); // Green color for the stems

	// Left stem
	glPushMatrix();
	glRotatef(-350, 0, 0, 1);
	glTranslatef(-0.1, 0.0, 0.0);
	glScalef(0.05, 1.2, 0.05);
	glutSolidCube(1.0);
	glPopMatrix();

	// Right stem
	glPushMatrix();
	glRotatef(350, 0, 0, 1);
	glTranslatef(0.1, 0.0, 0.0);
	glScalef(0.05, 1.2, 0.05);
	glutSolidCube(1.0);
	glPopMatrix();

	// Draw central sphere
	glColor3f(1.0, 1.0, 0.0); // Yellow color for the center
	glPushMatrix();
	glTranslatef(0.0, 0.5, 0.0);
	glutSolidSphere(0.1, 20, 20);
	glPopMatrix();

	// Draw flower petals
	glColor3f(1.0, 0.0, 0.0); // Red color for the petals

	for (int i = 0; i < 6; ++i) {
		glPushMatrix();
		float petalAngle = static_cast<float>(i) * 2.0f * M_PI / 6.0f;
		float x = 0.3 * cos(petalAngle);
		float z = 0.3 * sin(petalAngle);
		glTranslatef(x, 0.5, z);
		glutSolidSphere(0.2, 20, 20);
		glPopMatrix();
	}
}


void drawTreeandFlowers() {

	glPushMatrix();
	glTranslated(-0.19, 0, 0.1);
	glRotated(-10, 1, 0, 0);
	glRotated(20, 0, 0, 1);
	drawFlower();
	glPopMatrix();
	glPushMatrix();
	glTranslated(-0.19, 0, 0.08);
	glRotated(20, 1, 0, 0);
	glRotated(20, 0, 0, 1);
	drawFlower();
	glPopMatrix();
	glPushMatrix();
	glTranslated(0.19, 0, -0.1);
	glRotated(10, 1, 0, 0);
	glRotated(-20, 0, 0, 1);
	drawFlower();
	glPopMatrix();
	glPushMatrix();
	glTranslated(0.19, 0, -0.08);
	glRotated(-20, 1, 0, 0);
	glRotated(-20, 0, 0, 1);
	drawFlower();
	glPopMatrix();

	drawTree();

}

void drawHalfCircleDisk() {
	glColor3f(1, 0, 0); // Gray color for the disk
	const GLfloat radius = 1.0;
	const GLint slices = 100;

	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0.0, 0.0, 0.0); // Center of the half-disk

	for (int i = 0; i <= slices; ++i) {
		GLfloat theta = static_cast<GLfloat>(i) / static_cast<GLfloat>(slices) * M_PI;
		GLfloat x = radius * cos(theta);
		GLfloat y = radius * sin(theta);
		glVertex3f(x, y, 0.0);
	}

	glEnd();
}


void seeSawBase() {

	glPushMatrix();
	glColor3f(1, 0, 0);
	glRotatef(-90, 1, 0, 0);
	glutSolidCone(0.3, 0.6, 20, 20);
	glPopMatrix();

}



void Seat() {

	glPushMatrix();
	glColor3f(1, 0, 0);
	glRotatef(-90, 0, 1, 0);
	glTranslated(0, 0.5, 0);
	glScalef(0.25, 0.03, 0.3);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslatef(0, 0.5, -0.12);
	glScalef(0.16, 0.25, 1.02);
	drawHalfCircleDisk();
	glPopMatrix();


}

void drawSeesaw() {


	glPushMatrix();
	glColor3f(1, 1, 1);
	glRotatef(-90, 0, 1, 0);
	glTranslated(0, 0.65, 0);
	glScalef(0.35, 0.1, 3);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.2, 0.2, 0);
	glRotatef(-90, 0, 1, 0);
	Seat();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.2, 0.2, 0);
	glRotatef(90, 0, 1, 0);
	Seat();
	glPopMatrix();


}

void SwingSeat() {

	glPushMatrix();
	glTranslatef(-4, 0, -5);
	glTranslatef(0, 1.5, 0);
	glRotatef(RotS, 1, 0, 0);
	glTranslatef(0, -1.5, 0);

	glColor3f(0.5, 0.5, 0.5); // Gray color for the string
	glPushMatrix();
	glBegin(GL_LINES);
	glVertex3f(-0.15, 1.5, -.13);
	glVertex3f(-0.15, 0.5, -.13); // Adjust the length of the string as needed
	glEnd();
	glPopMatrix();

	glColor3f(0.5, 0.5, 0.5); // Gray color for the string
	glPushMatrix();
	glBegin(GL_LINES);
	glVertex3f(0.15, 1.5, -.13);
	glVertex3f(0.15, 0.5, -.13); // Adjust the length of the string as needed
	glEnd();
	glPopMatrix();

	glPushMatrix();
	Seat();
	glPopMatrix();

	glPopMatrix();

}

void swingSupport() {

	glPushMatrix();
	glTranslatef(-4, 0, -5);
	glTranslatef(-0.65, 0, 0);
	glRotatef(90, 0, 1, 0);
	glScaled(0.08, 0.13, 0.13);
	wheelsSupportGround();
	glPopMatrix();
}

void TimeText() {
	glDisable(GL_LIGHTING);
	char* p0s[20];
	sprintf((char*)p0s, "Timer %d", Time);
	glPushMatrix();
	glColor3f(0, 0, 0);
	print(1, 1, (char*)p0s);
	glPopMatrix();
	glEnable(GL_LIGHTING);
}

void Rocket() {

	glPushMatrix();
	glColor3f(0.6f, 0.31f, 0.11f);
	glScalef(1.2, 0.1, 1.2);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 1, 1);
	glRotatef(-90, 1, 0, 0);
	drawSolidCylinder(0.25, 1.4, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslatef(0, 1.55, 0);
	glRotatef(-90, 1, 0, 0);
	glutSolidCone(0.21, 0.2, 20, 20);
	glPopMatrix();
}

void RocketSeat() {
	glPushMatrix();
	glTranslatef(0, TransR, 0.4);
	Seat();
	glPopMatrix();
}

void LoseText() {
	glDisable(GL_LIGHTING);
	char* p0s[20];
	sprintf((char*)p0s, "YOU LOSE");
	glPushMatrix();
	glColor3f(1, 0, 0);
	glScalef(2, 2, 2);
	print(450, 350, (char*)p0s);
	glPopMatrix();
	glEnable(GL_LIGHTING);
}

void WinText() {
	glDisable(GL_LIGHTING);
	char* p0s[20];
	sprintf((char*)p0s, "YOU WIN");
	glPushMatrix();
	glColor3f(0, 1, 0);
	glScalef(2, 2, 2);
	print(450, 350, (char*)p0s);
	glPopMatrix();
	glEnable(GL_LIGHTING);
}

void Display() {
	setupCamera();
	setupLights();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (!GoalReached && !Lost) {
		TimeText();
	}

	glPushMatrix();
	glScalef(2, 2, 2);
	glTranslatef(1.8, 0, -9);
	RocketSeat();
	Rocket();
	glPopMatrix();


	if (FlagVisKey) {
		glPushMatrix();
		glTranslatef(0, TransK, 0);
		glTranslatef(0, -0.2, -1);
		key();
		glPopMatrix();
	}


	swingSupport();
	SwingSeat();


	glPushMatrix();
	glTranslatef(-3, 0, -5);
	glRotatef(110, 0, 1, 0);
	seeSawBase();
	glRotatef(RotS, 0, 0, 1);
	drawSeesaw();
	glPopMatrix();


	glPushMatrix();
	glRotatef(RotB, 0.0, 0.0, 1.0);
	glRotatef(-10, 1, 0, 0);
	glTranslatef(0.175, 0.9, 0.07);
	drawBalloon(2, 0, 0, 1, 1, 0);
	glPopMatrix();


	glPushMatrix();
	glRotatef(RotB, 0.0, 0.0, 1.0);
	glRotatef(-10, 1, 0, 0);
	glTranslatef(0.175, 0.9, 0.07);
	drawBalloon(2, 0.5, -3, 0, 1, 1);
	glPopMatrix();

	glPushMatrix();
	glRotatef(RotB, 0.0, 0.0, 1.0);
	glRotatef(-10, 1, 0, 0);
	glTranslatef(0.175, 0.9, 0.07);
	drawBalloon(3, 1, -4, 1, 0, 0);
	glPopMatrix();

	glPushMatrix();
	glRotatef(RotB, 0.0, 0.0, 1.0);
	glRotatef(-10, 1, 0, 0);
	glTranslatef(0.175, 0.9, 0.07);
	drawBalloon(3, 3, -19, 1, 0, 0);
	glPopMatrix();

	glPushMatrix();
	glRotatef(RotB, 0.0, 0.0, 1.0);
	glRotatef(-10, 1, 0, 0);
	glTranslatef(0.175, 0.9, 0.07);
	drawBalloon(-3, 3, -17, 1, 1, 0);
	glPopMatrix();

	glPushMatrix();
	glRotatef(RotB, 0.0, 0.0, 1.0);
	glRotatef(-10, 1, 0, 0);
	glTranslatef(0.175, 0.9, 0.07);
	drawBalloon(-2, 2, -16, 1, 1, 1);
	glPopMatrix();

	glPushMatrix();
	glRotatef(RotB, 0.0, 0.0, 1.0);
	glRotatef(-10, 1, 0, 0);
	glTranslatef(0.175, 0.9, 0.07);
	drawBalloon(2, 2, -11, 0, 1, 1);
	glPopMatrix();

	glPushMatrix();
	glRotatef(RotB, 0.0, 0.0, 1.0);
	glRotatef(-10, 1, 0, 0);
	glTranslatef(0.175, 0.9, 0.07);
	drawBalloon(3, 2, -12, 1, 1, 1);
	glPopMatrix();

	glPushMatrix();
	glRotatef(RotB, 0.0, 0.0, 1.0);
	glRotatef(-10, 1, 0, 0);
	glTranslatef(0.175, 0.9, 0.07);
	drawBalloon(-3, 3, -15, 1, 0, 1);
	glPopMatrix();

	glPushMatrix();
	glRotatef(RotB, 0.0, 0.0, 1.0);
	glRotatef(-10, 1, 0, 0);
	glTranslatef(0.175, 0.9, 0.07);
	drawBalloon(3, 2, -8, 1, 0, 1);
	glPopMatrix();


	glPushMatrix();
	glRotatef(RotB, 0.0, 0.0, 1.0);
	glRotatef(-10, 1, 0, 0);
	glTranslatef(0.175, 0.9, 0.07);
	drawBalloon(4, 2, -6, 1, 1, 0);
	glPopMatrix();

	glPushMatrix();
	glRotatef(RotB, 0.0, 0.0, 1.0);
	glRotatef(-10, 1, 0, 0);
	glTranslatef(0.175, 0.9, 0.07);
	drawBalloon(-4, 1, -3, 1, 1, 1);
	glPopMatrix();


	glPushMatrix();
	glRotatef(RotB, 0.0, 0.0, 1.0);
	glRotatef(-10, 1, 0, 0);
	glTranslatef(0.175, 0.9, 0.07);
	drawBalloon(-3, 1, -8, 0, 1, 1);
	glPopMatrix();

	glPushMatrix();
	glRotatef(RotB, 0.0, 0.0, 1.0);
	glRotatef(-10, 1, 0, 0);
	glTranslatef(0.175, 0.9, 0.07);
	drawBalloon(-2, 1, -6, 0, 0, 1);
	glPopMatrix();

	glPushMatrix();
	glRotatef(RotB, 0.0, 0.0, 1.0);
	glRotatef(-10, 1, 0, 0);
	glTranslatef(0.175, 0.9, 0.07);
	drawBalloon(1, 0, 1, 0, 1, 0);
	glPopMatrix();


	glPushMatrix();
	glTranslated(3, 0, -12);
	drawTreeandFlowers();
	glPopMatrix();

	glPushMatrix();
	glTranslated(3, 0, 0.6);
	drawTreeandFlowers();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-3, 0, -15);
	drawTreeandFlowers();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-4, 0, -1);
	drawTreeandFlowers();
	glPopMatrix();


	glPushMatrix();
	glScaled(2, 2, 2);
	glTranslated(1, 0, -3);
	cupsGame();
	glPopMatrix();



	glPushMatrix();


	glPushMatrix();
	glScaled(1.5, 1.5, 1.5);
	glTranslatef(-2, 0, -8);
	glScaled(0.15, 0.15, 0.15);
	wheelsSupportGround();
	glPopMatrix();

	glPushMatrix();
	glScaled(1.5, 1.5, 1.5);
	glColor3f(1, 0, 0);
	glTranslatef(-2, 1.8, 0);
	glRotatef(RotF, 0.0, 0.0, 1.0);
	glTranslatef(2, -1.8, 0);
	DrawCube();
	glPopMatrix();

	glPushMatrix();
	glScaled(1.5, 1.5, 1.5);
	glLineWidth(3);
	glColor3f(1, 1, 1);
	glTranslatef(-2, 1.8, 0);
	glRotatef(RotF, 0.0, 0.0, 1.0);
	glTranslatef(2, -1.8, 0);
	wheelSupport();
	glPopMatrix();

	glPopMatrix();

	glPushMatrix();
	glTranslatef(humanx, 0, humanz);
	glRotatef(RotY, 0.0, 1.0, 0.0);
	glTranslatef(-humanx, 0, -humanz);
	drawHuman();
	glPopMatrix();

	glPushMatrix();
	glColor3f(randx, randy, randz);
	drawFence();
	glColor3f(0.4, 0.5, 0.2); // Green color
	drawGround();
	glPopMatrix();

	if ((GoalReached && Won) || Won) {
		WinText();
	}
	if ((GoalReached && !Won) || Lost) {
		LoseText();
	}

	glFlush();
}


void TimerColor(int value) {

	if (Fencecolor) {
		randx = GenerateRandomcolor();
		randy = GenerateRandomcolor();
		randz = GenerateRandomcolor();
	}
	else
	{
		randx = 0.6;
		randy = 0.3;
		randz = 0;
	}
	glutPostRedisplay();
	glutTimerFunc(1000, TimerColor, 0);

}


void Keyboard(unsigned char key, int x, int y) {
	float d = 0.05;

	switch (key) {
	case 'w':
		camera.moveY(d);
		break;
	case 's':
		camera.moveY(-d);
		break;
	case 'a':
		camera.moveX(d);
		break;
	case 'd':
		camera.moveX(-d);
		break;
	case 'q':
		camera.moveZ(d);
		break;
	case 'e':
		camera.moveZ(-d);
		break;
	case 'W':
		camera.setTopView();
		break;
	case 'S':
		camera.setFrontView();
		break;
	case 'D':
		camera.setRightSideView();
		break;

	case 'A':
		camera.PlayerView();
		break;

	case 'f':
		if (AnimationOnOff == true) {
			AnimationOnOff = false;
		}
		else {
			AnimationOnOff = true;
		}
		break;

	case 'F':
		if (Fencecolor == true) {
			Fencecolor = false;
		}
		else {
			Fencecolor = true;
		}
		break;

	case GLUT_KEY_ESCAPE:
		exit(EXIT_SUCCESS);
	}

	float a = 2.0;

	switch (key) {
	case 'z':
		camera.rotateX(a);
		break;
	case 'x':
		camera.rotateX(-a);
		break;
	case 'c':
		camera.rotateY(a);
		break;
	case 'v':
		camera.rotateY(-a);
		break;
	}

	printf(" centerx:  % f", camera.center.x);
	printf(" centery:  % f", camera.center.y);
	printf(" centerz:  % f\n", camera.center.z);
	printf(" eyex:  % f", camera.eye.x);
	printf(" eyey:  % f", camera.eye.y);
	printf(" eyez:  % f\n", camera.eye.z);
	printf(" upx:  % f", camera.up.x);
	printf(" upy:  % f", camera.up.y);
	printf(" upz:  % f\n", camera.up.z);

	glutPostRedisplay();
}


void main(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitWindowSize(1000, 800);
	glutInitWindowPosition(50, 50);

	glutCreateWindow(" Amusement Park");
	glutDisplayFunc(Display);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(Loc);
	glutTimerFunc(25, TimerColor, 0);
	glutTimerFunc(25, Timer, 0);
	glutTimerFunc(0, time, 0);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);


	glutMainLoop();


}