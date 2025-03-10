#include <windows.h>
#include <gl/gl.h>
#include <stdbool.h>
#include <math.h>
#include "camera.h"

// Математические константы для работы с углами
#define M_PI 3.14159265358979323846    // Число пи
#define M_PI_2 1.57079632679489661923  // Пи/2 (90 градусов)
#define M_PI_4 0.78539816339744830962  // Пи/4 (45 градусов)

// Структура камеры с начальными значениями:
// x, y - позиция камеры в пространстве
// z - высота камеры
// Xrot - угол поворота вокруг оси X (наклон вверх/вниз)
// Zrot - угол поворота вокруг оси Z (поворот влево/вправо)
struct SCamera camera = {0.0f, 0.0f, 2.0f, 45, 0};

// Применяет преобразования камеры к сцене
void Camera_Apply() {
    glRotatef(-camera.Xrot, 1, 0, 0);  // Поворот вокруг оси X (наклон)
    glRotatef(-camera.Zrot, 0, 0, 1);  // Поворот вокруг оси Z
    glTranslatef(-camera.x, -camera.y, -camera.z);  // Перемещение камеры
}

// Обрабатывает вращение камеры
// xAngle - угол наклона (вверх/вниз)
// zAngle - угол поворота (влево/вправо)
void Camera_Rotation(float xAngle, float zAngle) {
    camera.Zrot += zAngle;
    // Нормализация угла Zrot в пределах 0-360 градусов
    if (camera.Zrot < 0) camera.Zrot += 360;
    if (camera.Zrot > 360) camera.Zrot -= 360;

    camera.Xrot += xAngle;
    // Ограничение угла наклона от 0 до 180 градусов
    if (camera.Xrot < 0) camera.Xrot = 0;
    if (camera.Xrot > 180) camera.Xrot = 180;
}

// Автоматическое управление камерой с помощью мыши
// centerX, centerY - координаты центра экрана
// speed - скорость движения камеры
void Camera_AutoMoveByMouse(int centerX, int centerY, float speed) {
    POINT cur;
    POINT base = {centerX, centerY};
    GetCursorPos(&cur);  // Получаем текущую позицию курсора
    // Вращаем камеру на основе отклонения курсора от центра
    Camera_Rotation((base.y - cur.y) / 5, (base.x - cur.x) / 5);
    SetCursorPos(base.x, base.y);  // Возвращаем курсор в центр экрана
}

// Перемещение камеры в заданном направлении
// forwardMove: 1 - вперед, -1 - назад, 0 - нет движения
// rightMove: 1 - вправо, -1 - влево, 0 - нет движения
// speed - скорость перемещения
void Camera_MoveDirectional(int forwardMove, int rightMove, float speed) {
    // Вычисляем угол движения на основе поворота камеры
    float angle = -camera.Zrot / 180 * M_PI;

    // Корректируем угол в зависимости от направления движения
    if (forwardMove > 0)  // Движение вперед
        angle += rightMove > 0 ? M_PI_4 : (rightMove < 0 ? -M_PI_4 : 0);
    if (forwardMove < 0)  // Движение назад
        angle += M_PI + (rightMove > 0 ? -M_PI_4 : (rightMove < 0 ? M_PI_4 : 0));
    if (forwardMove == 0) {  // Движение вбок
        angle += rightMove > 0 ? M_PI_2 : -M_PI_2;
        if (rightMove == 0) speed = 0;  // Нет движения
    }

    // Обновляем позицию камеры
    bool coordinatesChanged = false;
    if (speed != 0) {
        float prev_x = camera.x;
        float prev_y = camera.y;
        float prev_z = camera.z;

        // Вычисляем новые координаты с учетом угла и скорости
        camera.x += sin(angle) * speed;
        camera.y += cos(angle) * speed;

        // Проверяем, изменились ли координаты
        if (camera.x != prev_x || camera.y != prev_y || camera.z != prev_z) {
            coordinatesChanged = true;
        }
    }
}
