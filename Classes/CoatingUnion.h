#ifndef __COATING_UNION_INCLUDED__
#define __COATING_UNION_INCLUDED__

#include "General.h"

namespace rtm {
    class CoatingUnion abstract
    {
    public:
        CoatingUnion();
        CoatingUnion(CoatingType type, int column, int row, CoatingMatrix&& objects);

        virtual ~CoatingUnion() = default;

        CoatingType GetType() const;
        size_t GetWidth() const;
        size_t GetHeight() const;
        CoatingUnique const& GetCoatingObject(int column, int row) const;
        virtual float GetLength() const;

        bool IsCorrectColumn(int column) const;
        bool IsCorrectRow(int row) const;

        virtual void ShowSprites(WorldScene* const scene);
        virtual void ReleaseSprites(WorldScene* const scene);

    private:
        CoatingType type_;
        int column_;
        int row_;
        size_t width_;
        size_t height_;
        CoatingMatrix objects_;
    };
}

#endif // __COATING_UNION_INCLUDED__