﻿#include "AllRtmClasses.h"

///////////////////////
//  LOCAL FUNCTIONS  //
///////////////////////

void AddDrivewayToMatrix(rtm::CoatingMatrix& matrix, rtm::CoatingType type, int column, size_t colShift, size_t width,
    int row, size_t rowShift, size_t height, rtm::AngleType angle)
{
    using namespace rtm;

    CoatingMatrix addedMatrix{ DrivewayObject::DrivewayMatrix(type, column + colShift, row + rowShift, width, height, angle) };
    for (size_t i = 0; i < width; ++i) {
        for (size_t j = 0; j < height; ++j) {
            matrix[colShift + i][rowShift + j] = std::move(addedMatrix[i][j]);
        }
    }
}

///////////////////////
//  CLASS FUNCTIONS  //
///////////////////////

rtm::CrossroadObject::CrossroadObject()
    : CoatingUnion{}
    , linesCounts_{ 0, 0, 0, 0 }
    , nullDirection_{ NullAngle }
    , controlUnit_{}
{}

rtm::CrossroadObject::CrossroadObject(CoatingType type, int column, int row, LinesCounts linesCounts, size_t controlUnitType)
    : CoatingUnion{ CrossroadType, column, row, std::move(CrossroadObject::CrossroadMatrix(type, column, row, linesCounts)) }
    , linesCounts_{ linesCounts }
    , nullDirection_{ NullAngle }
    , controlUnit_{ std::make_shared<ControlUnit>(controlUnitType, column, row, linesCounts) }
{}

rtm::CrossroadObject::CrossroadObject(CoatingType type, int column, int row, LinesCounts linesCounts, AngleType nullDirection, size_t controlUnitType)
    : CoatingUnion{ TCrossroadType, column, row, std::move(CrossroadObject::TCrossroadMatrix(type, column, row, linesCounts, nullDirection)) }
    , linesCounts_{ linesCounts }
    , nullDirection_{ nullDirection }
    , controlUnit_{ std::make_shared<ControlUnit>(controlUnitType, column, row, linesCounts, nullDirection) }
{}

rtm::CoatingMatrix rtm::CrossroadObject::CrossroadMatrix(CoatingType type, int column, int row, LinesCounts linesCounts)
{
    size_t width{ linesCounts[Upward] + linesCounts[Downward] + 2 };
    size_t height{ linesCounts[Rightward] + linesCounts[Leftward] + 2 };

    if (width == 2 || height == 2) {
        return CoatingMatrix{};
    }

    size_t lastCol{ width - 1 };
    size_t lastRow{ height - 1 };

    CoatingMatrix result{ width };
    for (size_t i = 0; i <= lastCol; ++i) {
        result[i] = CoatingVector{ height };
    }

    //////////////////////////////
    // Center of crossroad

    result[0][0] = std::make_unique<RoadCoating>(type, RoadTypeNo17, column, row, Right);
    result[0][lastRow] = std::make_unique<RoadCoating>(type, RoadTypeNo17, column, row + lastRow, Down);
    result[lastCol][0] = std::make_unique<RoadCoating>(type, RoadTypeNo17, column + lastCol, row, Up);
    result[lastCol][lastRow] = std::make_unique<RoadCoating>(type, RoadTypeNo17, column + lastCol, row + lastRow, Left);

    if (width == 3 && height == 3) {
        result[1][1] = std::make_unique<RoadCoating>(type, RoadTypeNo3, column + 1, row + 1, Up);
    }
    else if (width == 3) {
        result[1][1] = std::make_unique<RoadCoating>(type, RoadTypeNo4, column + 1, row + 1, Up);
        result[1][lastRow - 1] = std::make_unique<RoadCoating>(type, RoadTypeNo4, column + 1, row + lastRow - 1, Down);
    }
    else if (height == 3) {
        result[1][1] = std::make_unique<RoadCoating>(type, RoadTypeNo4, column + 1, row + 1, Right);
        result[lastCol - 1][1] = std::make_unique<RoadCoating>(type, RoadTypeNo4, column + lastCol - 1, row + 1, Left);
    }
    else {
        result[1][1] = std::make_unique<RoadCoating>(type, RoadTypeNo5, column + 1, row + 1, Up);
        result[lastCol - 1][1] = std::make_unique<RoadCoating>(type, RoadTypeNo5, column + lastCol - 1, row + 1, Left);
        result[1][lastRow - 1] = std::make_unique<RoadCoating>(type, RoadTypeNo5, column + 1, row + lastRow - 1, Right);
        result[lastCol - 1][lastRow - 1] = std::make_unique<RoadCoating>(type, RoadTypeNo5, column + lastCol - 1, row + lastRow - 1, Down);
    }

    // Block backward directions    
    for (size_t i = 1; i <= lastCol - 1; ++i) {
        for (size_t j = 1; j <= lastRow - 1; ++j) {
            // Add object if not init
            if (!result[i][j]) {
                result[i][j] = std::make_unique<RoadCoating>(type, RoadTypeNo6, column + i, row + j, Up);
            }
            // Left side
            if (1 <= i && i < 1 + linesCounts[Downward]) {
                // Bottom side
                if (1 <= j && j < 1 + linesCounts[Rightward]) {
                    result[i][j]->SetDirectionAvailability(Up, false);
                    result[i][j]->SetDirectionAvailability(Left, false);
                }
                // Top side
                else {
                    result[i][j]->SetDirectionAvailability(Up, false);
                    result[i][j]->SetDirectionAvailability(Right, false);
                }
            }
            // Right side
            else {
                // Bottom side
                if (1 <= j && j < 1 + linesCounts[Rightward]) {
                    result[i][j]->SetDirectionAvailability(Down, false);
                    result[i][j]->SetDirectionAvailability(Left, false);
                }
                // Top side
                else {
                    result[i][j]->SetDirectionAvailability(Down, false);
                    result[i][j]->SetDirectionAvailability(Right, false);
                }
            }
        }
    }

    //////////////////////////////
    // Entrances of crossroad

    AddDrivewayToMatrix(result, type, column, 1, linesCounts[Downward], row, 0, 1, Down);
    AddDrivewayToMatrix(result, type, column, 1 + linesCounts[Downward], linesCounts[Upward], row, 0, 1, Up);

    AddDrivewayToMatrix(result, type, column, 1, linesCounts[Downward], row, lastRow, 1, Down);
    AddDrivewayToMatrix(result, type, column, 1 + linesCounts[Downward], linesCounts[Upward], row, lastRow, 1, Up);

    AddDrivewayToMatrix(result, type, column, 0, 1, row, 1, linesCounts[Rightward], Right);
    AddDrivewayToMatrix(result, type, column, 0, 1, row, 1 + linesCounts[Rightward], linesCounts[Leftward], Left);

    AddDrivewayToMatrix(result, type, column, lastCol, 1, row, 1, linesCounts[Rightward], Right);
    AddDrivewayToMatrix(result, type, column, lastCol, 1, row, 1 + linesCounts[Rightward], linesCounts[Leftward], Left);

    return result;
}

rtm::CoatingMatrix rtm::CrossroadObject::TCrossroadMatrix(CoatingType type, int column, int row, LinesCounts linesCounts, AngleType nullDirection)
{
    size_t topBorder{ 1 };
    size_t rightBorder{ 1 };
    size_t bottomBorder{ 1 };
    size_t leftBorder{ 1 };
    if (nullDirection == Up) {
        topBorder = 0;
    }
    else if (nullDirection == Right) {
        rightBorder = 0;
    }
    else if (nullDirection == Down) {
        bottomBorder = 0;
    }
    else if (nullDirection == Left) {
        leftBorder = 0;
    }

    size_t crossroadWidth{ linesCounts[Upward] + linesCounts[Downward] };
    size_t crossroadHeight{ linesCounts[Rightward] + linesCounts[Leftward] };

    if (crossroadWidth == 0 || crossroadHeight == 0) {
        return CoatingMatrix{};
    }

    size_t width{ leftBorder + crossroadWidth + rightBorder };
    size_t height{ bottomBorder + crossroadHeight + topBorder };
    size_t lastCol{ width - 1 };
    size_t lastRow{ height - 1 };

    CoatingMatrix result{ width };
    for (size_t i = 0; i <= lastCol; ++i) {
        result[i] = CoatingVector{ height };
    }

    if (nullDirection == Up) {
        result[0][0] = std::make_unique<RoadCoating>(type, RoadTypeNo17, column, row, Right);
        result[lastCol][0] = std::make_unique<RoadCoating>(type, RoadTypeNo17, column + lastCol, row, Up);
    }
    else if (nullDirection == Right) {
        result[0][0] = std::make_unique<RoadCoating>(type, RoadTypeNo17, column, row, Right);
        result[0][lastRow] = std::make_unique<RoadCoating>(type, RoadTypeNo17, column, row + lastRow, Down);
    }
    else if (nullDirection == Down) {
        result[0][lastRow] = std::make_unique<RoadCoating>(type, RoadTypeNo17, column, row + lastRow, Down);
        result[lastCol][lastRow] = std::make_unique<RoadCoating>(type, RoadTypeNo17, column + lastCol, row + lastRow, Left);
    }
    else if (nullDirection == Left) {
        result[lastCol][0] = std::make_unique<RoadCoating>(type, RoadTypeNo17, column + lastCol, row, Up);
        result[lastCol][lastRow] = std::make_unique<RoadCoating>(type, RoadTypeNo17, column + lastCol, row + lastRow, Left);
    }

    if (crossroadWidth == 1 && crossroadHeight == 1) {
        size_t i{ leftBorder };
        size_t j{ bottomBorder };
        result[i][j] = std::make_unique<RoadCoating>(type, RoadTypeNo7, column + i, row + j, nullDirection);
    }
    else if (crossroadWidth == 1) {
        if (nullDirection == Up) {
            size_t i{ leftBorder };
            size_t j{ bottomBorder };
            result[i][j] = std::make_unique<RoadCoating>(type, RoadTypeNo4, column + i, row + j, nullDirection);
        }
        else if (nullDirection == Down) {
            size_t i{ leftBorder };
            size_t j{ lastRow - topBorder };
            result[i][j] = std::make_unique<RoadCoating>(type, RoadTypeNo4, column + i, row + j, nullDirection);
        }
        else if (nullDirection == Right) {
            size_t i{ leftBorder };
            size_t j{ bottomBorder };
            result[i][j] = std::make_unique<RoadCoating>(type, RoadTypeNo10, column + i, row + j, nullDirection);

            i = leftBorder;
            j = lastRow - topBorder;
            result[i][j] = std::make_unique<RoadCoating>(type, RoadTypeNo8, column + i, row + j, nullDirection);
        }
        else if (nullDirection == Left) {
            size_t i{ leftBorder };
            size_t j{ bottomBorder };
            result[i][j] = std::make_unique<RoadCoating>(type, RoadTypeNo8, column + i, row + j, nullDirection);

            i = leftBorder;
            j = lastRow - topBorder;
            result[i][j] = std::make_unique<RoadCoating>(type, RoadTypeNo10, column + i, row + j, nullDirection);
        }
    }
    else if (crossroadHeight == 1) {
        if (nullDirection == Up) {
            size_t i{ leftBorder };
            size_t j{ bottomBorder };
            result[i][j] = std::make_unique<RoadCoating>(type, RoadTypeNo8, column + i, row + j, nullDirection);

            i = lastCol - rightBorder;
            j = bottomBorder;
            result[i][j] = std::make_unique<RoadCoating>(type, RoadTypeNo10, column + i, row + j, nullDirection);
        }
        else if (nullDirection == Down) {
            size_t i{ leftBorder };
            size_t j{ bottomBorder };
            result[i][j] = std::make_unique<RoadCoating>(type, RoadTypeNo10, column + i, row + j, nullDirection);

            i = lastCol - rightBorder;
            j = bottomBorder;
            result[i][j] = std::make_unique<RoadCoating>(type, RoadTypeNo8, column + i, row + j, nullDirection);
        }
        else if (nullDirection == Right) {
            size_t i{ leftBorder };
            size_t j{ bottomBorder };
            result[i][j] = std::make_unique<RoadCoating>(type, RoadTypeNo4, column + i, row + j, nullDirection);
        }
        else if (nullDirection == Left) {
            size_t i{ lastCol - rightBorder };
            size_t j{ bottomBorder };
            result[i][j] = std::make_unique<RoadCoating>(type, RoadTypeNo4, column + i, row + j, nullDirection);
        }
    }
    else {
        if (nullDirection == Up) {
            size_t i{ leftBorder };
            size_t j{ bottomBorder };
            result[i][j] = std::make_unique<RoadCoating>(type, RoadTypeNo5, column + i, row + j, Up);

            i = lastCol - rightBorder;
            j = bottomBorder;
            result[i][j] = std::make_unique<RoadCoating>(type, RoadTypeNo5, column + i, row + j, Left);
        }
        else if (nullDirection == Down) {
            size_t i{ leftBorder };
            size_t j{ lastRow - topBorder };
            result[i][j] = std::make_unique<RoadCoating>(type, RoadTypeNo5, column + i, row + j, Right);

            i = lastCol - rightBorder;
            j = lastRow - topBorder;
            result[i][j] = std::make_unique<RoadCoating>(type, RoadTypeNo5, column + i, row + j, Down);
        }
        else if (nullDirection == Right) {
            size_t i{ leftBorder };
            size_t j{ bottomBorder };
            result[i][j] = std::make_unique<RoadCoating>(type, RoadTypeNo5, column + i, row + j, Up);

            i = leftBorder;
            j = lastRow - topBorder;
            result[i][j] = std::make_unique<RoadCoating>(type, RoadTypeNo5, column + i, row + j, Right);
        }
        else if (nullDirection == Left) {
            size_t i{ lastCol - rightBorder };
            size_t j{ bottomBorder };
            result[i][j] = std::make_unique<RoadCoating>(type, RoadTypeNo5, column + i, row + j, Left);

            i = lastCol - rightBorder;
            j = lastRow - topBorder;
            result[i][j] = std::make_unique<RoadCoating>(type, RoadTypeNo5, column + i, row + j, Down);
        }
    }

    // Block backward directions
    for (size_t i = leftBorder; i <= lastCol - rightBorder; ++i) {
        for (size_t j = bottomBorder; j <= lastRow - topBorder; ++j) {
            if (!result[i][j]) {
                if (i == 0 || j == 0 || i == lastCol || j == lastRow) {
                    result[i][j] = std::make_unique<RoadCoating>(type, RoadTypeNo9, column + i, row + j, nullDirection);
                }
                else {
                    result[i][j] = std::make_unique<RoadCoating>(type, RoadTypeNo6, column + i, row + j, Up);
                }
            }
            // Left side
            if (leftBorder <= i && i < leftBorder + linesCounts[Downward]) {
                // Bottom side
                if (bottomBorder <= j && j < bottomBorder + linesCounts[Rightward]) {
                    result[i][j]->SetDirectionAvailability(Up, false);
                    result[i][j]->SetDirectionAvailability(Left, false);
                }
                // Top side
                else {
                    result[i][j]->SetDirectionAvailability(Up, false);
                    result[i][j]->SetDirectionAvailability(Right, false);
                }
            }
            // Right side
            else {
                // Bottom side
                if (bottomBorder <= j && j < bottomBorder + linesCounts[Rightward]) {
                    result[i][j]->SetDirectionAvailability(Down, false);
                    result[i][j]->SetDirectionAvailability(Left, false);
                }
                // Top side
                else {
                    result[i][j]->SetDirectionAvailability(Down, false);
                    result[i][j]->SetDirectionAvailability(Right, false);
                }
            }
        }
    }

    //////////////////////////////
    // Entrances of crossroad

    if (nullDirection != Up) {
        AddDrivewayToMatrix(result, type, column, leftBorder, linesCounts[Downward], row, lastRow, 1, Down);
        AddDrivewayToMatrix(result, type, column, leftBorder + linesCounts[Downward], linesCounts[Upward], row, lastRow, 1, Up);
    }

    if (nullDirection != Right) {
        AddDrivewayToMatrix(result, type, column, lastCol, 1, row, bottomBorder, linesCounts[Rightward], Right);
        AddDrivewayToMatrix(result, type, column, lastCol, 1, row, bottomBorder + linesCounts[Rightward], linesCounts[Leftward], Left);
    }

    if (nullDirection != Down) {
        AddDrivewayToMatrix(result, type, column, leftBorder, linesCounts[Downward], row, 0, 1, Down);
        AddDrivewayToMatrix(result, type, column, leftBorder + linesCounts[Downward], linesCounts[Upward], row, 0, 1, Up);
    }

    if (nullDirection != Left) {
        AddDrivewayToMatrix(result, type, column, 0, 1, row, bottomBorder, linesCounts[Rightward], Right);
        AddDrivewayToMatrix(result, type, column, 0, 1, row, bottomBorder + linesCounts[Rightward], linesCounts[Leftward], Left);
    }

    return result;
}

rtm::AngleType rtm::CrossroadObject::GetNullDirection() const
{
    return nullDirection_;
}

rtm::ControlUnitShared rtm::CrossroadObject::GetControlUnit() const
{
    return controlUnit_;
}

void rtm::CrossroadObject::ShowSprites(cocos2d::Layer* const layer)
{
    CoatingUnion::ShowSprites(layer);
    if (controlUnit_) {
        controlUnit_->ShowSprites(layer);
    }
}

void rtm::CrossroadObject::ReleaseSprites(cocos2d::Layer* const layer)
{
    CoatingUnion::ReleaseSprites(layer);
    if (controlUnit_) {
        controlUnit_->ReleaseSprites(layer);
    }
}
