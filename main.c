#include <windows.h>
#include <gl/gl.h>
#include "camera.h"
#include <math.h>

#define M_PI 3.14159265358979323846    // ����� ��
#define M_PI_2 1.57079632679489661923  // ��/2 (90 ��������)

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);

void WndResize(int x, int y){
    glViewport(0,0,x,y); //������������� ������� ����
    float k=x/(float)y; //����������� ������
    float sz = 0.1; //������� �������
    glLoadIdentity(); //�������� ��������� �������
    glFrustum(-k*sz, k*sz, -sz, sz, sz*2, 100); //��������� ������������� ��������
}

float theta = 0.0f;

void MoveCamera(){
    Camera_MoveDirectional(
        GetKeyState('W') < 0 ? 1 : GetKeyState('S') < 0 ? -1 : 0,
        GetKeyState('D') < 0 ? 1 : GetKeyState('A') < 0 ? -1 : 0,
    0.1);
    Camera_AutoMoveByMouse(400,320,0.1);
}

void Init_Light()
{
    // �������� ��������� � OpenGL
    glEnable(GL_LIGHTING);
    // �������� ������� ����������� ��������� �� ������������
    glShadeModel(GL_SMOOTH);

    // ����������� ��������� ����� ��� ����� ������ �������
    // ����������� ��� � ��������� ���������
    GLfloat light_ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat light_diffuse[] = { 1.2f, 1.2f, 1.2f, 1.0f };
    // ���������� ��������� ��������� ����������
    GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    // ��������� ��������� � ��������� ����� GL_LIGHT0
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    // ��������� ���������, ����� ���� �� ��������� ��� ������ � �����������
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.3f);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.05f);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.005f);

    // �������� ������ �������� ����� (GL_LIGHT0)
    glEnable(GL_LIGHT0);
    // �������� �������������� ������������ ��������
    glEnable(GL_NORMALIZE);
}

void Init_Material()
{
    glEnable(GL_COLOR_MATERIAL); //���������� ������������� ���������
    glShadeModel(GL_SMOOTH); // ���������� �������
    GLfloat material_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat material_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat material_specular[] = { 1.0f, 1.0f, 1.0f, 32.0f };
    GLfloat material_shininess[] = { 50.0f }; //����� ���������
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, material_shininess);
}

void Draw_Cube(float r, float g, float b) {
    glEnable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);

    // ����������� �������� ����
    // ������� ��������� ���������
    GLfloat mat_ambient[] = { r*0.3f, g*0.3f, b*0.3f, 1.0f };
    // ��������� ��������� - �������� ���� ���������
    GLfloat mat_diffuse[] = { r, g, b, 1.0f };
    // ���������� ��������� - ���������� �����
    GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    // ������� ����������� ��������� - ������ � ������� ������
    GLfloat mat_shininess[] = { 64.0f };

    // ��������� ��������� ���������
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    GLfloat vertices[] = {
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f
    };

    GLuint indices[] = {
        0, 1, 2, 2, 3, 0,  // �������� �����
        1, 5, 6, 6, 2, 1,  // ������ �����
        7, 6, 5, 5, 4, 7,  // ������ �����
        4, 0, 3, 3, 7, 4,  // ����� �����
        4, 5, 1, 1, 0, 4,  // ������ �����
        3, 2, 6, 6, 7, 3   // ������� �����
    };

    GLfloat normals[] = {
        0.0f, 0.0f, -1.0f,  // �������� �����
        1.0f, 0.0f, 0.0f,   // ������ �����
        0.0f, 0.0f, 1.0f,   // ������ �����
        -1.0f, 0.0f, 0.0f,  // ����� �����
        0.0f, -1.0f, 0.0f,  // ������ �����
        0.0f, 1.0f, 0.0f    // ������� �����
    };

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glNormalPointer(GL_FLOAT, 0, normals);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, indices);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

    glEnable(GL_COLOR_MATERIAL);
}

void drawChessBoard(int n) {
    float size = 2.0f;
    float start = -(n * size) / 2;

    glEnable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);

    glBegin(GL_QUADS);
    for(int x = 0; x < n; x++) {
        for(int z = 0; z < n; z++) {
            if((x + z) % 2 == 0) {
                // ������� ������
                GLfloat white_diffuse[] = { 0.9f, 0.9f, 0.9f, 1.0f };
                GLfloat white_ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
                GLfloat white_specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
                GLfloat white_shininess[] = { 32.0f };

                glMaterialfv(GL_FRONT, GL_AMBIENT, white_ambient);
                glMaterialfv(GL_FRONT, GL_DIFFUSE, white_diffuse);
                glMaterialfv(GL_FRONT, GL_SPECULAR, white_specular);
                glMaterialfv(GL_FRONT, GL_SHININESS, white_shininess);
            } else {
                // ������ ������
                GLfloat black_diffuse[] = { 0.1f, 0.1f, 0.1f, 1.0f };
                GLfloat black_ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
                GLfloat black_specular[] = { 0.3f, 0.3f, 0.3f, 1.0f };
                GLfloat black_shininess[] = { 32.0f };

                glMaterialfv(GL_FRONT, GL_AMBIENT, black_ambient);
                glMaterialfv(GL_FRONT, GL_DIFFUSE, black_diffuse);
                glMaterialfv(GL_FRONT, GL_SPECULAR, black_specular);
                glMaterialfv(GL_FRONT, GL_SHININESS, black_shininess);
            }

            float x1 = start + x * size;
            float z1 = start + z * size;
            float x2 = x1 + size;
            float z2 = z1 + size;

            glNormal3f(0.0f, -1.0f, 0.0f);

            glVertex3f(x1, 0.0f, z1);
            glVertex3f(x2, 0.0f, z1);
            glVertex3f(x2, 0.0f, z2);
            glVertex3f(x1, 0.0f, z2);
        }
    }
    glEnd();

    glEnable(GL_COLOR_MATERIAL);
    glDisable(GL_LIGHTING);
}

void Draw_Prism(float alpha) {
    glEnable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);  // ������������ �������� ��� ���������������

    // �������� blending ��� ������������
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // ����������� �������� ������ � �������������
    GLfloat mat_ambient[]  = { 0.2f, 0.2f, 0.5f, alpha };
    GLfloat mat_diffuse[]  = { 0.3f, 0.3f, 0.8f, alpha };
    GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, alpha };
    GLfloat mat_shininess[] = { 64.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    const int verticesPerBase = 16;    // 16 ������ ��� ������� ��������� (32 ������ �����)
    const float radius = 1.0f;          // ������ ���������
    const float height = 1.5f;          // ������ ������
    float angle = 2.0f * M_PI / verticesPerBase;  // ���� ����� ���������

    // ��������� ���������� ������ ������� � �������� ���������
    float bottom_vertices[verticesPerBase][3];
    float top_vertices[verticesPerBase][3];
    for (int i = 0; i < verticesPerBase; i++) {
        bottom_vertices[i][0] = radius * cos(angle * i);
        bottom_vertices[i][1] = -height / 2;
        bottom_vertices[i][2] = radius * sin(angle * i);

        top_vertices[i][0] = radius * cos(angle * i);
        top_vertices[i][1] = height / 2;
        top_vertices[i][2] = radius * sin(angle * i);
    }

    // ������ ������� ����� � ����������� ���������
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= verticesPerBase; i++) {
        int idx = i % verticesPerBase;
        // ������� � ������ �� ������ ��������� � ����� �� ����������
        float nx = cos(angle * idx);
        float nz = sin(angle * idx);
        glNormal3f(nx, 0.0f, nz);
        glVertex3f(top_vertices[idx][0], top_vertices[idx][1], top_vertices[idx][2]);
        glVertex3f(bottom_vertices[idx][0], bottom_vertices[idx][1], bottom_vertices[idx][2]);
    }
    glEnd();

    // ������ ������ ���������
    glBegin(GL_POLYGON);
        glNormal3f(0.0f, -1.0f, 0.0f);
        for (int i = verticesPerBase - 1; i >= 0; i--) {
            glVertex3f(bottom_vertices[i][0], bottom_vertices[i][1], bottom_vertices[i][2]);
        }
    glEnd();

    // ������ ������� ���������
    glBegin(GL_POLYGON);
        glNormal3f(0.0f, 1.0f, 0.0f);
        for (int i = 0; i < verticesPerBase; i++) {
            glVertex3f(top_vertices[i][0], top_vertices[i][1], top_vertices[i][2]);
        }
    glEnd();

    // ������������ ����� ������: ��������� ��������
    glDisable(GL_LIGHTING);  // ��������� ��������� ��� ��������� ��������
    glColor3f(1.0f, 1.0f, 0.0f);  // Ƹ���� ���� ��� ��������
    glLineWidth(2.0f);

    // ������ ���������
    glBegin(GL_LINE_LOOP);
        for (int i = 0; i < verticesPerBase; i++) {
            glVertex3f(bottom_vertices[i][0], bottom_vertices[i][1], bottom_vertices[i][2]);
        }
    glEnd();

    // ������� ���������
    glBegin(GL_LINE_LOOP);
        for (int i = 0; i < verticesPerBase; i++) {
            glVertex3f(top_vertices[i][0], top_vertices[i][1], top_vertices[i][2]);
        }
    glEnd();

    // ������������ �����
    glBegin(GL_LINES);
        for (int i = 0; i < verticesPerBase; i++) {
            glVertex3f(bottom_vertices[i][0], bottom_vertices[i][1], bottom_vertices[i][2]);
            glVertex3f(top_vertices[i][0], top_vertices[i][1], top_vertices[i][2]);
        }
    glEnd();

    glLineWidth(1.0f);
    glEnable(GL_LIGHTING);

    glDisable(GL_BLEND);
    glEnable(GL_COLOR_MATERIAL);
}

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    WNDCLASSEX wcex;
    HWND hwnd;
    HDC hDC;
    HGLRC hRC;
    MSG msg;
    BOOL bQuit = FALSE;

    /* register window class */
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "GLSample";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);;


    if (!RegisterClassEx(&wcex))
        return 0;

    /* create main window */
    hwnd = CreateWindowEx(0,
                          "GLSample",
                          "OpenGL Sample",
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          800,
                          640,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hwnd, nCmdShow);

    /* enable OpenGL for the window */
    EnableOpenGL(hwnd, &hDC, &hRC);
    RECT rct;
    GetClientRect(hwnd,&rct);
    WndResize(rct.right,rct.bottom);
    glEnable(GL_DEPTH_TEST);

    // �������������� ��������� � ��������� �����
    Init_Light();
    Init_Material();
    /* program main loop */
    while (!bQuit)
    {
        /* check for messages */
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


            if (GetForegroundWindow()==hwnd) MoveCamera();

            // ����������� ���� �� 10 ������ ���� �����, ������ ������ ����
            GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
            glPushMatrix();
                glLoadIdentity();
                glLightfv(GL_LIGHT0, GL_POSITION, light_position);
            glPopMatrix();
            // ��������� ������� �������
            glPushMatrix();
            // ���������� ��� ������������� ��� ��������� ����� � ���������� �����������
            glLoadIdentity();
            // ������������� ������� �����
            glLightfv(GL_LIGHT0, GL_POSITION, light_position);
            // ��������������� �������
            glPopMatrix();

            glPushMatrix();
            Camera_Apply();


            // ������ ��������� ��������
            glPushMatrix();
            glRotatef(-90, 1, 0, 0);
            drawChessBoard(16);
            glPopMatrix();

            // ������������� ������� ����� ��� ���� ���������
            glLineWidth(3.0f); // ������ ����� ����� (����� ������������ ��������)

            // ������ ��� ���������
            glBegin(GL_LINES);
                // ��� X - �������
                glColor3f(1.0f, 0.0f, 0.0f);
                glVertex3f(0.0f, 0.0f, 0.0f);
                glVertex3f(5.0f, 0.0f, 0.0f);

                // ��� Y - �������
                glColor3f(0.0f, 1.0f, 0.0f);
                glVertex3f(0.0f, 0.0f, 0.0f);
                glVertex3f(0.0f, 5.0f, 0.0f);

                // ��� Z - �����
                glColor3f(0.0f, 0.0f, 1.0f);
                glVertex3f(0.0f, 0.0f, 0.0f);
                glVertex3f(0.0f, 0.0f, 5.0f);
            glEnd();

            // ���������� ����������� ������� �����
            glLineWidth(1.0f);

            glTranslatef(0.0f, 5.0f, 0.0f);

            glBegin(GL_TRIANGLES);
                glColor3f(1.0f, 0.0f, 0.0f);   glVertex3f(0.0f,   1.0f,  0.0f);
                glColor3f(0.0f, 1.0f, 0.0f);   glVertex3f(0.87f,  -0.5f, 0.0f);
                glColor3f(0.0f, 0.0f, 1.0f);   glVertex3f(-0.87f, -0.5f, 0.0f);
            glEnd();

            // ������ ��������� �����, ������������� �� �����, � ������������� �� 0 �� 0.9
            {
                const int numPrisms = 8;
                float circleRadius = 5.0f;
                for (int i = 0; i < numPrisms; i++) {
                    float angleDeg = i * (360.0f / numPrisms);
                    float angleRad = angleDeg * M_PI / 180.0f;
                    float x = circleRadius * cos(angleRad);
                    float y = circleRadius * sin(angleRad);
                    float alpha = (numPrisms > 1 ? (i / (float)(numPrisms - 1)) : 0.9f) * 0.9f;

                    glPushMatrix();
                        glTranslatef(x, y, 1.0f);
                        glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
                        Draw_Prism(alpha);
                    glPopMatrix();
                }
            }

            glPopMatrix();

            SwapBuffers(hDC);
            Sleep(1);
            theta += 1.0f;
        }
    }

    /* shutdown OpenGL */
    DisableOpenGL(hwnd, hDC, hRC);

    /* destroy the window explicitly */
    DestroyWindow(hwnd);

    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
            PostQuitMessage(0);
        break;

        case WM_DESTROY:
            return 0;

        case WM_KEYDOWN:
        {
            switch (wParam)
            {
                case VK_ESCAPE:
                    PostQuitMessage(0);
                break;
            }
        }
        break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{
    PIXELFORMATDESCRIPTOR pfd;

    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC(hwnd);

    /* set the pixel format for the DC */
    ZeroMemory(&pfd, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
                  PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    iFormat = ChoosePixelFormat(*hDC, &pfd);

    SetPixelFormat(*hDC, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hRC = wglCreateContext(*hDC);

    wglMakeCurrent(*hDC, *hRC);
}

void DisableOpenGL (HWND hwnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}

