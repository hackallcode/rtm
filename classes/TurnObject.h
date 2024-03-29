﻿#ifndef __TURN_OBJECTS_INCLUDED__
#define __TURN_OBJECTS_INCLUDED__

#include "CoatingUnion.h"

namespace rtm {
    /// @brief Класс поворота дороги
    class TurnObject
        : public CoatingUnion
    {
    public:
        /// @brief Конструктор по умолчанию
        TurnObject();
        /// @brief Конструктор по размерам
        /// @param isRight тип поворота (false - левый, true - правый)
        /// @param type тип покрытия
        /// @param column левая колонка объекта
        /// @param row нижняя строка объекта
        /// @param linesCount количество полос
        /// @param angle угол поворота объекта
        TurnObject(bool isRight, CoatingType type, int column, int row, size_t linesCount, AngleType angle);
        /// @brief Функция для получения матрицы правого поворота
        /// @param type тип покрытия
        /// @param column левая колонка объекта
        /// @param row нижняя строка объекта
        /// @param linesCount количество полос
        /// @param angle угол поворота объекта
        /// @return матрица покрытий
        static CoatingMatrix RightTurnMatrix(CoatingType type, int column, int row, size_t linesCount, AngleType angle);
        /// @brief Функция для получения матрицы левого поворота
        /// @param type тип покрытия
        /// @param column левая колонка объекта
        /// @param row нижняя строка объекта
        /// @param linesCount количество полос
        /// @param angle угол поворота объекта
        /// @return матрица покрытий
        static CoatingMatrix LeftTurnMatrix(CoatingType type, int column, int row, size_t linesCount, AngleType angle);

        /// @brief Деструктор по умолчанию
        virtual ~TurnObject() = default;

        /// @brief Функция для получения типа поворота
        /// @return true, если правый поворот, false, если левый
        bool IsRight() const;
        /// @brief Функция для получения угла поворота объекта
        /// @return угол поворота объекта
        AngleType GetAngle() const;

    private:
        /// @brief Тип поворота
        bool isRight_;
        /// @brief Угол поворота объекта
        AngleType angle_;
    };
}

#endif // __TURN_OBJECTS_INCLUDED__