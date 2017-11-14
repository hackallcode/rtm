#include "CoatingUnion.h"
#include "CoatingObject.h"
#include "WorldScene.h"

rtm::CoatingUnion::CoatingUnion()
    : type_{ CoatingUnionType }
    , column_{ 0 }
    , row_{ 0 }
    , width_{ 0 }
    , height_{ 0 }
    , objects_{}
{}

rtm::CoatingUnion::CoatingUnion(CoatingType type, int column, int row, CoatingMatrix&& objects)
    : type_{ type }
    , column_{ column }
    , row_{ row }
    , width_{ 0 }
    , height_{ 0 }
    , objects_{ std::move(objects) }
{
    if (!objects_.empty()) {
        if (!objects_[0].empty()) {
            width_ = objects_.size();
            height_ = objects_[0].size();
        }
    }
}

rtm::CoatingType rtm::CoatingUnion::GetType() const
{
    return type_;
}

rtm::CoatingUnique const& rtm::CoatingUnion::GetCoatingObject(int column, int row) const
{
    return objects_[column - column_][row - row_];
}

float rtm::CoatingUnion::GetLength() const
{
    return 1.0f;
}

bool rtm::CoatingUnion::IsCorrectColumn(int column) const
{
    return column >= column_ && column < static_cast<int>(column_ + width_);
}

bool rtm::CoatingUnion::IsCorrectRow(int row) const
{
    return row >= row_ && row < static_cast<int>(row_ + height_);
}

void rtm::CoatingUnion::ShowCoatingObjects(WorldScene* const scene)
{
    for (auto& col : objects_) {
        for (auto& elem : col) {
            if (elem) {
                if (elem->GetSprite() != nullptr) {
                    scene->addChild(elem->GetSprite(), COATING_OBJECT_Z_ORDER);
                }
            }
        }
    }
}

void rtm::CoatingUnion::ReleaseCoatingObjects(WorldScene* const scene)
{
    for (auto& col : objects_) {
        for (auto& elem : col) {
            if (elem) {
                if (elem->GetSprite() != nullptr) {
                    scene->removeChild(elem->GetSprite());
                }
                elem.reset();
            }
        }
    }
}

size_t rtm::CoatingUnion::GetWidth() const
{
    return width_;
}

size_t rtm::CoatingUnion::GetHeight() const
{
    return height_;
}