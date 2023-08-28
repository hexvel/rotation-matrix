#include <math.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>

float a, b, c;

float cubeWidth = 10;
int width = 160, height = 44;
float zBuffer[160 * 44];
char buffer[160 * 44];
int backgroundASCIICode = ' ';
int distaceFromCam = 60;
float K1 = 40;
float incrementSpeed = 0.6;

float x, y, z;
float ooz;
int xp, yp;
int indx;

float calcX(int i, int j, int k) {
	return j * sin(a) * sin(b) * cos(c) - k * cos(a) * sin(b) * cos(c) +
		j * cos(a) * sin(c) + k * sin(a) * sin(c) + i * cos(b) * cos(c);
}

float calcY(int i, int j, int k) {
	return j * cos(a) * cos(c) + k * sin(a) * cos(c) -
		j * sin(a) * sin(b) * sin(c) + k * cos(a) * sin(b) * sin(c) -
		i * cos(b) * sin(c);
}

float calcZ(int i, int j, int k) {
	return k * cos(a) * cos(b) - j * sin(a) * cos(b) + i * sin(b);
}

void usleep(__int64 usec) 
{ 
    HANDLE timer; 
    LARGE_INTEGER ft; 

    ft.QuadPart = -(10*usec);

    timer = CreateWaitableTimer(NULL, TRUE, NULL); 
    SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0); 
    WaitForSingleObject(timer, INFINITE); 
    CloseHandle(timer); 
}

void calcForSurface(float cubeX, float cubeY, float cubeZ, int ch) {
	x = calcX(cubeX, cubeY, cubeZ);
	y = calcY(cubeX, cubeY, cubeZ);
	z = calcZ(cubeX, cubeY, cubeZ) + distaceFromCam;

	ooz = 1 / z;
	xp = (int)(width / 2 + K1 * ooz * x * 2);
	yp = (int)(height / 2 + K1 * ooz * y);

	indx = xp + yp * width;
	if (indx >= 0 && indx < width * height) {
		if (ooz > zBuffer[indx]) {
			zBuffer[indx] = ooz;
			buffer[indx] = ch;
		}
	}
}

int main() {
	printf("\x1b[2J");
	while (1) {
		memset(buffer, backgroundASCIICode, width * height);
		memset(zBuffer, 0, width * height * 4);
		for (float cubeX = -cubeWidth; cubeX < cubeWidth; cubeX += incrementSpeed) {
			for (float cubeY = -cubeWidth; cubeY < cubeWidth; cubeY += incrementSpeed) {
				calcForSurface(cubeX, cubeY, -cubeWidth, '.');
				calcForSurface(cubeWidth, cubeY, cubeX, '$');
				calcForSurface(-cubeWidth, cubeY, -cubeX, 'g');
				calcForSurface(-cubeX, cubeY, cubeWidth, '%');
				calcForSurface(cubeX, -cubeWidth, cubeY, ';');
				calcForSurface(cubeX, cubeWidth, cubeY, '~');
			}
		}
		printf("\x1b[H");
		for (int k = 0; k < width * height; k++) {
			putchar(k % width ? buffer[k] : 10);
		}

		a += 0.2;
		b += 0.2;
	}
	return 0;
}