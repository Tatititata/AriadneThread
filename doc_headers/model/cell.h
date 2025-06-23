#ifndef POINT_H_
#define POINT_H_

/**
 * @struct Cell
 * @brief Represents a cell or coordinate in a 2D grid.
 *
 * Contains row (`r`) and column (`c`) indices.
 */
struct Cell {
  int r;  ///< Row index.
  int c;  ///< Column index.

  /**
   * @brief Adds two cells coordinate-wise.
   * @param other Another cell to add.
   * @return New Cell with summed coordinates.
   */
  Cell operator+(const Cell& other) const { return {r + other.r, c + other.c}; }
};

/**
 * @brief Equality operator for Cell.
 * @param a First cell.
 * @param b Second cell.
 * @return true if both cells have the same coordinates.
 */
inline bool operator==(const Cell& a, const Cell& b) {
  return a.r == b.r && a.c == b.c;
}

/**
 * @brief Inequality operator for Cell.
 * @param a First cell.
 * @param b Second cell.
 * @return true if cells have different coordinates.
 */
inline bool operator!=(const Cell& a, const Cell& b) { return !(a == b); }

#endif  //
