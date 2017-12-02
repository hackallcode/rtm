﻿#ifndef __DYNAMIC_OBJECT_INCLUDED__
#define __DYNAMIC_OBJECT_INCLUDED__

#include "WorldObject.h"

namespace rtm {
    /// Класс динамического объекта (который двигается, обновляется)
    class DynamicObject 
        : public WorldObject
    {
    public:
        /// Конструктор по умочанию
        DynamicObject();
        /// Конструктор с использованием уже готового спрайта
        /// @param sprite указатель на готовый спрайт
        /// @param x абсцисса будущего объекта
        /// @param y ордината будущего объекта
        /// @param angle угол поворота строения
        /// @param speed первоначальная скорость
        DynamicObject(cocos2d::Sprite* sprite, float x, float y, float angle, float speed);
        /// Конструктор из файла
        /// @param filename путь к файлу инициализации
        /// @param x абсцисса будущего объекта
        /// @param y ордината будущего объекта
        /// @param angle угол поворота строения
        /// @param speed первоначальная скорость
        DynamicObject(std::string const& filename, float x, float y, float angle, float speed);
        
        /// Деструктор по умолчанию
        virtual ~DynamicObject() = default;

        /// Функция для получения скорости
        /// @return скорость объекта
        float GetSpeed() const;
        /// Функция для получения последнего приращения положения
        /// @return длина последнего смещения
        float GetLastDelta() const;
        /// Функция для проверки наличия столкновений у данного объекта
        /// @return true, если после последней проверки была столкновение, иначе false
        bool HasCollision() const;

        /// Функция обновления
        /// @param world контроллер мира, в котором находится объект
        virtual void Update(WorldController* const world);

        /// Функция для поиска объектов неподалеку
        /// @param world контроллер мира, в котором находится объект
        /// @return true, если какой-нибудь объект находится рядом, иначе false
        bool IsNearOthers(WorldController* const world);

        friend void CheckCollisions(WorldController* const world);

    protected:
        /// Функция для установки скорости
        /// @param speed новая скорость
        void SetSpeed_(float speed);
        /// Функция для сохранения информации о столкновениях
        /// @param flag есть ли столкновение
        void SetCollisionFlag_(bool flag);
        
        /// Функция для проверки попадания объекта в зону видимости
        /// @param other указатель на второй объект
        /// @param radius радиус видимости
        /// @param angle угол видимости (в каждую из сторон)
        /// @param angleShift сдвиг области видимости
        /// @return true, если other находится в области видимости данного объекта, иначе false
        bool IsBeholding_(WorldObject const* const other, float radius = VIEW_RADIUS, 
            float angle = VIEW_ANGLE, float angleShift = VIEW_ANGLE_SHIFT) const;
        /// Функция для проверки наличия столкновения с other
        /// @param other указатель на второй объект
        /// @return true, если объект пересекается с other, иначе false
        bool IsIntersecting_(WorldObject const* const other) const;

    private:
        /// Текущая скорость
        float speed_;
        /// Длина последнего смещения
        float lastDelta_;
        /// Наличие столкновений у данного объекта
        bool hasCollision_;

        /// Функция для проверки, находится ли other рядом с данным объектом
        /// @param other указатель на второй объект
        /// @return true, если other рядом, иначе false
        bool IsNear_(WorldObject const* const other) const;
    };

    /// Функция для вычисления столкновений в мире
    /// @param world контроллер мира, в котором будут происходить вычисления
    void CheckCollisions(WorldController* const world);
}

#endif // __DYNAMIC_OBJECT_INCLUDED__