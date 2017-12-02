﻿#ifndef __CROSSROAD_OBJECT_INCLUDED__
#define __CROSSROAD_OBJECT_INCLUDED__

#include "CoatingUnion.h"

namespace rtm {
    /// Класс пересечения дорог
    class CrossroadObject
        : public CoatingUnion
    {
    public:
        /// Конструктор по умолчанию
        CrossroadObject();        
        /// Конструктор для обычного перекрестка
        /// @param type тип покрытия
        /// @param column левая колонка перекрестка
        /// @param row нижняя строка перекрестка
        /// @param linesCounts количество полос в каждом направлении
        /// @param controlUnitType номер типа управляющего блока
        CrossroadObject(CoatingType type, int column, int row, LinesCounts linesCounts,
            size_t controlUnitType = 0);        
        /// Конструктор для Т-образного перекрестка
        /// @param type тип покрытия
        /// @param column левая колонка перекрестка
        /// @param row нижняя строка перекрестка
        /// @param linesCounts количество полос в каждом направлении
        /// @param nullDirection сторона, в направлении которой нельзя двигаться
        /// @param controlUnitType номер типа управляющего блока
        CrossroadObject(CoatingType type, int column, int row, LinesCounts linesCounts, AngleType nullDirection,
            size_t controlUnitType = 0);
        /// Функция для получения матрицы покрытий перекрестка
        /// @param type тип покрытия
        /// @param column левая колонка перекрестка
        /// @param row нижняя строка перекрестка
        /// @param linesCounts количество полос в каждом направлении
        /// @return матрица покрытий
        static CoatingMatrix CrossroadMatrix(CoatingType type, int column, int row, LinesCounts linesCounts);        
        /// Функция для получения матрицы покрытий Т-образного перекрестка
        /// @param type тип покрытия
        /// @param column левая колонка перекрестка
        /// @param row нижняя строка перекрестка
        /// @param linesCounts количество полос в каждом направлении
        /// @param nullDirection сторона, в направлении которой нельзя двигаться
        /// @return матрица покрытий
        static CoatingMatrix TCrossroadMatrix(CoatingType type, int column, int row, LinesCounts linesCounts, AngleType nullDirection);

        /// Деструктор по умолчанию
        virtual ~CrossroadObject() = default;

        /// Функция для получения стороны, в направлении которой нельзя двигаться
        /// @return угол, соответствующий запрещенной стороне
        AngleType GetNullDirection() const;
        
        /// Функция для получения управляющего блока, привязанного к данному объекту
        /// @return умный указатель на управляющий блок
        ControlUnitShared GetControlUnit() const;
        /// Функция для добавления спрайтов на сцену
        /// @param layer слой, на который надо добавить спрайты управляющего блока
        virtual void ShowSprites(cocos2d::Layer* const layer) override;
        /// Функция для удаления спрайтов со сцены
        /// @param layer слой, с которого надо удалить спрайты управляющего блока
        virtual void ReleaseSprites(cocos2d::Layer* const layer) override;

    private:
        /// Количество полос в каждом направлении
        LinesCounts linesCounts_;
        /// Сторона, в направлении которой нельзя двигаться
        AngleType nullDirection_;
        /// Умный указатель на управляющий блок
        ControlUnitShared controlUnit_;
    };
}

#endif // __CROSSROAD_OBJECT_INCLUDED__