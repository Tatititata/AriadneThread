#ifndef POINT_H_
#define POINT_H_

struct Cell {
  int r;
  int c;

  Cell operator+(const Cell& other) const { return {r + other.r, c + other.c}; }
};

inline bool operator==(const Cell& a, const Cell& b) {
  return a.r == b.r && a.c == b.c;
}

inline bool operator!=(const Cell& a, const Cell& b) { return !(a == b); }

#endif