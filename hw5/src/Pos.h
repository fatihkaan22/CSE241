#ifndef HW5_POS_H
#define HW5_POS_H

namespace FatihKaanHex {
    class Pos {
    private:
        int x;
        int y;

    public:
        Pos(int x, int y);

        int getX() const;

        int getY() const;

        void set(const Pos &p);

        bool operator==(const Pos &p) const;

        Pos &operator=(const Pos &p);

        void setX(int x);

        void setY(int y);

        void print() const;

        Pos();

    };
}
#endif //HW5_POS_H
